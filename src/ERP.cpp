#include "erp.h"

// Constructor
ERP::ERP() {

    numStudents = 0;
    numCourses = 0;

    // Default program requirements
    totalCredits = 160;
    electiveCredits = 24;
    minGPA = 500;
    maxLoad = 24;
    thesisCredits = 100;

    programSet = false;
    dataStarted = false;
}


// Find student using roll number
int ERP::findStudent(const string& roll) const {

    for (int i = 0; i < numStudents; i++) {

        if (students[i].getRoll() == roll) {
            return i;
        }
    }

    return -1;
}


// Find course using course code
int ERP::findCourse(const string& code) const {

    for (int i = 0; i < numCourses; i++) {

        if (courses[i].getCode() == code) {
            return i;
        }
    }

    return -1;
}
// Convert string to a non-negative integer
static bool parseInteger(const string& text, int& value) {

    if (text.empty()) {
        return false;
    }

    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        if (!isdigit(static_cast<unsigned char>(text[i]))) {
            return false;
        }
    }

    long result = strtol(text.c_str(), NULL, 10);

    if (result > 2147483647L) {
        return false;
    }

    value = static_cast<int>(result);
    return true;
}
// Set academic program requirements
ErrorCode ERP::setProgram(const string* words, int count) {

    if (count != 6 || programSet || dataStarted) {
        return E_PARSE;
    }

    int total, elective, load, thesis;

    if (!parseInteger(words[1], total) ||
        !parseInteger(words[2], elective) ||
        !parseInteger(words[4], load) ||
        !parseInteger(words[5], thesis)) {
        return E_PARSE;
    }

    // Validate GPA format
    string gpa = words[3];

    if (gpa.length() < 4 || gpa[gpa.length() - 3] != '.') {
        return E_PARSE;
    }

    if (!isdigit(static_cast<unsigned char>(gpa[gpa.length() - 2])) ||
        !isdigit(static_cast<unsigned char>(gpa[gpa.length() - 1]))) {
        return E_PARSE;
    }

    string whole = gpa.substr(0, gpa.length() - 3);
    int gpaWhole;

    if (!parseInteger(whole, gpaWhole)) {
        return E_PARSE;
    }

    if (gpaWhole > 10) {
        return E_PARSE;
    }

    int fraction =
        (gpa[gpa.length() - 2] - '0') * 10 +
        (gpa[gpa.length() - 1] - '0');

    if (gpaWhole == 10 && fraction != 0) {
        return E_PARSE;
    }

    totalCredits = total;
    electiveCredits = elective;
    minGPA = gpaWhole * 100 + fraction;
    maxLoad = load;
    thesisCredits = thesis;

    programSet = true;

    return OK;
}
// Add a new course to the catalogue
ErrorCode ERP::addCourse(const string* words, int count) {

    if (count != 6) {
        return E_PARSE;
    }

    string code = words[1];

    if (code.empty() || code.length() > 10) {
        return E_PARSE;
    }

    if (findCourse(code) != -1) {
        return E_DUPLICATE;
    }

    if (numCourses >= MAX_COURSES) {
        return E_FULL;
    }

    CourseType type;

    if (words[2] == "REGULAR") {
        type = REGULAR;
    }
    else if (words[2] == "SU") {
        type = SU;
    }
    else if (words[2] == "THESIS") {
        type = THESIS;
    }
    else {
        return E_BAD_COURSE;
    }

    int credits;

    if (!parseInteger(words[3], credits)) {
        return E_BAD_COURSE;
    }

    if (credits > 24 || (credits == 0 && type != SU)) {
        return E_BAD_COURSE;
    }

    CourseCategory category;

    if (words[4] == "CORE") {
        category = CORE;
    }
    else if (words[4] == "ELECTIVE") {
        category = ELECTIVE;
    }
    else {
        return E_BAD_COURSE;
    }

    int prereq = -1;

    if (words[5] != "-") {

        prereq = findCourse(words[5]);

        if (prereq == -1) {
            return E_BAD_COURSE;
        }

        if (words[5] == code) {
            return E_BAD_COURSE;
        }
    }

    courses[numCourses].setCourse(
        code, type, credits, category, prereq
    );

    numCourses++;

    return OK;
}
// Add a new student
ErrorCode ERP::addStudent(const string* words, int count) {

    if (count != 3) {
        return E_PARSE;
    }

    string roll = words[1];
    string name = words[2];

    if (roll.empty() || roll.length() > 10 ||
        name.empty() || name.length() > 30) {
        return E_PARSE;
    }

    if (findStudent(roll) != -1) {
        return E_DUPLICATE;
    }

    if (numStudents >= MAX_STUDENTS) {
        return E_FULL;
    }

    students[numStudents].setStudent(roll, name);

    numStudents++;

    return OK;
}
// Check whether a student has passed a particular course
bool ERP::isPassed(int studentIndex, int courseIndex) const {

    if (studentIndex < 0 || studentIndex >= numStudents ||
        courseIndex < 0 || courseIndex >= numCourses) {
        return false;
    }

    const Student& student = students[studentIndex];

    int latestIndex = -1;

    // Find the latest graded attempt in registration order
    for (int i = 0; i < student.getNumRegs(); i++) {

        const Registration* reg = student.getRegistration(i);

        if (reg != NULL &&
            reg->getCourse() == courseIndex &&
            reg->isGraded()) {

            latestIndex = i;
        }
    }

    if (latestIndex == -1) {
        return false;
    }

    const Registration* latest =
        student.getRegistration(latestIndex);

    const Course& course = courses[courseIndex];

    string grade = latest->getGrade();

    if (course.getType() == REGULAR) {
        return grade != "F";
    }

    if (course.getType() == SU) {
        return grade == "S";
    }

    if (course.getType() == THESIS) {

        if (latest->getThesisState() != FINAL) {
            return false;
        }

        return grade != "F";
    }

    return false;
}
// Check whether a student is eligible to register for a course
int ERP::checkEligibility(int s, int c, int term) const {

    // E1: Student and course must exist
    if (s < 0 || s >= numStudents ||
        c < 0 || c >= numCourses) {
        return E_UNKNOWN_STUDENT;
    }

    const Student& student = students[s];
    const Course& course = courses[c];

    // E2: Student must not have already passed this course
    if (isPassed(s, c)) {
        return E_ALREADY_PASSED;
    }

    // E3: No active or ungraded registration may exist
    for (int i = 0; i < student.getNumRegs(); i++) {

        const Registration* reg = student.getRegistration(i);

        if (reg == NULL || reg->getCourse() != c) {
            continue;
        }

        if (course.getType() == THESIS) {
            ThesisState state = reg->getThesisState();

            if (state == REGISTERED || state == IN_PROGRESS) {
                return E_ALREADY_REGISTERED;
            }
        }
        else if (!reg->isGraded()) {
            return E_ALREADY_REGISTERED;
        }
    }

    // E4: Prerequisite must have been passed
    int prerequisite = course.getPrereq();

    if (prerequisite != -1 &&
        !isPassed(s, prerequisite)) {
        return E_PREREQ;
    }

    // E5: Check the credit limit for this term
    int termCredits = 0;

    for (int i = 0; i < student.getNumRegs(); i++) {

        const Registration* reg = student.getRegistration(i);

        if (reg != NULL && reg->getTerm() == term) {
            int registeredCourse = reg->getCourse();

            if (registeredCourse >= 0 &&
                registeredCourse < numCourses) {
                termCredits += courses[registeredCourse].getCredits();
            }
        }
    }

    if (termCredits + course.getCredits() > maxLoad) {
        return E_CREDIT_LIMIT;
    }

    // E6: Thesis registration requires sufficient earned credits
    if (course.getType() == THESIS) {

        int earnedCredits = 0;

        for (int i = 0; i < numCourses; i++) {

            if (!isPassed(s, i)) {
                continue;
            }

            earnedCredits += courses[i].getCredits();
        }

        if (earnedCredits < thesisCredits) {
            return E_THESIS_NOT_ALLOWED;
        }
    }

    return OK;
}
// Register a student for a course
ErrorCode ERP::registerCourse(int s, int c, int term) {

    // Validate student and course indices
    if (s < 0 || s >= numStudents) {
        return E_UNKNOWN_STUDENT;
    }

    if (c < 0 || c >= numCourses) {
        return E_UNKNOWN_COURSE;
    }

    // Validate academic term
    if (term < 1 || term > 12) {
        return E_PARSE;
    }

    // Check all registration eligibility conditions
    int eligibility = checkEligibility(s, c, term);

    if (eligibility != OK) {
        return static_cast<ErrorCode>(eligibility);
    }

    // Add registration only after all checks pass
    return students[s].addRegistration(c, term);
}
// Submit a grade for a registered course
ErrorCode ERP::submitGrade(int s, int c, int term,
                           const string& symbol) {

    // Validate student and course indices
    if (s < 0 || s >= numStudents) {
        return E_UNKNOWN_STUDENT;
    }

    if (c < 0 || c >= numCourses) {
        return E_UNKNOWN_COURSE;
    }

    // Validate term
    if (term < 1 || term > 12) {
        return E_PARSE;
    }

    Student& student = students[s];
    Course& course = courses[c];

    // Find the registration
    int regIndex = student.findRegistration(c, term);

    if (regIndex == -1) {
        return E_NOT_REGISTERED;
    }

    Registration* reg = student.getRegistration(regIndex);

    if (reg == NULL) {
        return E_NOT_REGISTERED;
    }

    // A grade cannot be entered twice
    if (reg->isGraded()) {
        return E_ALREADY_GRADED;
    }

    // Validate the grade symbol
    bool validLetter = isValidLetterGrade(symbol);
    bool validPassFail = (symbol == "S" || symbol == "X");

    if (!validLetter && !validPassFail) {
        return E_BAD_GRADE;
    }

    // REGULAR courses accept only letter grades
    if (course.getType() == REGULAR) {

        if (!validLetter) {
            return E_GRADE_TYPE;
        }

        reg->setGrade(symbol);
    }

    // SU courses accept only S or X
    else if (course.getType() == SU) {

        if (!validPassFail) {
            return E_GRADE_TYPE;
        }

        reg->setGrade(symbol);
    }

    // THESIS courses accept S or X
    else if (course.getType() == THESIS) {

        if (!validPassFail) {
            return E_GRADE_TYPE;
        }

        reg->setGrade(symbol);

        if (symbol == "S") {
            reg->setThesisState(IN_PROGRESS);
        }
        else {
            reg->setThesisState(FAILED);
        }
    }

    return OK;
}
// Convert an in-progress thesis to a final letter grade
ErrorCode ERP::convertThesis(int s, int c,
                             const string& letter) {

    if (s < 0 || s >= numStudents) {
        return E_UNKNOWN_STUDENT;
    }

    if (c < 0 || c >= numCourses) {
        return E_UNKNOWN_COURSE;
    }

    // CONVERT is allowed only for thesis courses
    if (courses[c].getType() != THESIS) {
        return E_GRADE_TYPE;
    }

    // Check whether the supplied letter is valid
    if (!isValidLetterGrade(letter)) {
        return E_BAD_GRADE;
    }

    Student& student = students[s];

    // Examine the latest registration for this thesis
    for (int i = student.getNumRegs() - 1; i >= 0; i--) {

        Registration* reg = student.getRegistration(i);

        if (reg == NULL || reg->getCourse() != c) {
            continue;
        }

        // A thesis already converted cannot be converted again
        if (reg->getThesisState() == FINAL) {
            return E_ALREADY_GRADED;
        }

        // Only an IN_PROGRESS thesis can be converted
        if (reg->getThesisState() != IN_PROGRESS) {
            return E_THESIS_STATE;
        }

        reg->setGrade(letter);
        reg->setThesisState(FINAL);

        return OK;
    }

    // No registration exists for this thesis
    return E_THESIS_STATE;
}

// Convert a letter grade into grade points
int ERP::gradePoints(const string& grade) const {

    if (grade == "A")  return 10;
    if (grade == "A-") return 9;
    if (grade == "B")  return 8;
    if (grade == "B-") return 7;
    if (grade == "C")  return 6;
    if (grade == "C-") return 5;
    if (grade == "D")  return 4;
    if (grade == "F")  return 0;

    return -1;
}
// Calculate GPA in hundredths using integer arithmetic
ErrorCode ERP::gpaHundredths(int s, int* result) const {

    if (s < 0 || s >= numStudents || result == NULL) {
        return E_PARSE;
    }

    const Student& student = students[s];

    int numerator = 0;
    int denominator = 0;

    // Process every course in the catalogue
    for (int c = 0; c < numCourses; c++) {

        const Course& course = courses[c];

        // SU courses never contribute to GPA
        if (course.getType() == SU) {
            continue;
        }

        int latestIndex = -1;

        // Find the latest graded attempt in registration order
        for (int i = 0; i < student.getNumRegs(); i++) {

            const Registration* reg = student.getRegistration(i);

            if (reg != NULL &&
                reg->getCourse() == c &&
                reg->isGraded()) {

                latestIndex = i;
            }
        }

        // No graded attempt exists
        if (latestIndex == -1) {
            continue;
        }

        const Registration* latest =
            student.getRegistration(latestIndex);

        // Thesis contributes only after final conversion
        if (course.getType() == THESIS &&
            latest->getThesisState() != FINAL) {
            continue;
        }

        int points = gradePoints(latest->getGrade());

        if (points < 0) {
            continue;
        }

        numerator += course.getCredits() * points;
        denominator += course.getCredits();
    }

    // No GPA-contributing courses
    if (denominator == 0) {
        return E_NO_GPA;
    }

    // Round half-up to two decimal places
    *result = (200 * numerator + denominator) /
              (2 * denominator);

    return OK;
}
// Calculate the average grade based on rounded GPA
ErrorCode ERP::calculateAverage(int s, string& result) const {

    int gpa;

    // Calculate GPA in hundredths
    ErrorCode status = gpaHundredths(s, &gpa);

    if (status != OK) {
        return status;
    }

    // Convert GPA into the corresponding grade band
    if (gpa >= 950) {
        result = "A";
    }
    else if (gpa >= 850) {
        result = "A-";
    }
    else if (gpa >= 750) {
        result = "B";
    }
    else if (gpa >= 650) {
        result = "B-";
    }
    else if (gpa >= 550) {
        result = "C";
    }
    else if (gpa >= 450) {
        result = "C-";
    }
    else if (gpa >= 200) {
        result = "D";
    }
    else {
        result = "F";
    }

    return OK;
}
// Check all five graduation requirements
ErrorCode ERP::checkGraduation(int s, string& result) const {

    // Validate student
    if (s < 0 || s >= numStudents) {
        return E_UNKNOWN_STUDENT;
    }

    int earnedCredits = 0;
    int passedElectiveCredits = 0;

    bool r2Failed = false;
    bool r4Passed = false;

    // Examine every course in the catalogue
    for (int c = 0; c < numCourses; c++) {

        const Course& course = courses[c];

        bool passed = isPassed(s, c);

        // Count credits from passed courses only
        if (passed) {

            earnedCredits += course.getCredits();

            if (course.getCategory() == ELECTIVE) {
                passedElectiveCredits += course.getCredits();
            }
        }

        // R2: Every CORE course must be passed
        if (course.getCategory() == CORE && !passed) {
            r2Failed = true;
        }

        // R4: At least one FINAL thesis with C or better
        if (course.getType() == THESIS) {

            const Student& student = students[s];
            int latestIndex = -1;

            for (int i = 0; i < student.getNumRegs(); i++) {

                const Registration* reg = student.getRegistration(i);

                if (reg != NULL &&
                    reg->getCourse() == c &&
                    reg->isGraded()) {

                    latestIndex = i;
                }
            }

            if (latestIndex != -1) {

                const Registration* latest =
                    student.getRegistration(latestIndex);

                if (latest->getThesisState() == FINAL &&
                    gradePoints(latest->getGrade()) >= 6) {

                    r4Passed = true;
                }
            }
        }
    }

    // Collect all failed rules in the required order
    result = "";

    if (earnedCredits < totalCredits) {
        result += "R1";
    }

    if (r2Failed) {
        if (!result.empty()) result += ",";
        result += "R2";
    }

    if (passedElectiveCredits < electiveCredits) {
        if (!result.empty()) result += ",";
        result += "R3";
    }

    if (!r4Passed) {
        if (!result.empty()) result += ",";
        result += "R4";
    }

    // R5: Minimum rounded GPA
    int gpa;
    ErrorCode status = gpaHundredths(s, &gpa);

    if (status != OK || gpa < minGPA) {
        if (!result.empty()) result += ",";
        result += "R5";
    }

    // No failed rules means graduation requirements are satisfied
    if (result.empty()) {
        result = "YES";
    }
    else {
        result = "NO " + result;
    }

    return OK;
}

// --------------------------------------------------
// Process one input command
// --------------------------------------------------

ErrorCode ERP::processLine(const string& line, int lineNumber) {

    (void)lineNumber;

    // Ignore blank lines and lines whose first non-space character is #
    size_t first = line.find_first_not_of(" \t\r\n");

    if (first == string::npos || line[first] == '#') {
        return OK;
    }

    string words[MAX_WORDS];
    int count = splitWords(line, words, MAX_WORDS);

    // Ignore blank lines and comments
    if (count == 0) {
        return OK;
    }

    if (count < 0) {
        return E_PARSE;
    }

    string command = words[0];

    // PROGRAM must be the first command
    if (command == "PROGRAM") {
        return setProgram(words, count);
    }

    // Any non-PROGRAM command starts the data section
    dataStarted = true;

    // COURSE command
    if (command == "COURSE") {
        return addCourse(words, count);
    }

    // STUDENT command
    if (command == "STUDENT") {
        return addStudent(words, count);
    }

    // REGISTER command
    if (command == "REGISTER") {

        if (count != 4) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        int c = findCourse(words[2]);

        if (c == -1) {
            return E_UNKNOWN_COURSE;
        }

        int term;

        if (!parseInteger(words[3], term)) {
            return E_PARSE;
        }

        return registerCourse(s, c, term);
    }

    // GRADE command
    if (command == "GRADE") {

        if (count != 5) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        int c = findCourse(words[2]);

        if (c == -1) {
            return E_UNKNOWN_COURSE;
        }

        int term;

        if (!parseInteger(words[3], term)) {
            return E_PARSE;
        }

        return submitGrade(s, c, term, words[4]);
    }

    // CONVERT command
    if (command == "CONVERT") {

        if (count != 4) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        int c = findCourse(words[2]);

        if (c == -1) {
            return E_UNKNOWN_COURSE;
        }

        return convertThesis(s, c, words[3]);
    }

    // GPA command
    if (command == "GPA") {

        if (count != 2) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        int gpa;
        ErrorCode status = gpaHundredths(s, &gpa);

        if (status != OK) {
            return status;
        }

        cout << "GPA " << words[1] << " "
             << gpa / 100 << ".";

        if (gpa % 100 < 10) {
            cout << "0";
        }

        cout << gpa % 100 << endl;

        return OK;
    }

    // AVG command
    if (command == "AVG") {

        if (count != 2) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        string average;
        ErrorCode status = calculateAverage(s, average);

        if (status != OK) {
            return status;
        }

        cout << "AVG " << words[1] << " "
             << average << endl;

        return OK;
    }

    // GRAD command
    if (command == "GRAD") {

        if (count != 2) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            return E_UNKNOWN_STUDENT;
        }

        string graduation;
        ErrorCode status = checkGraduation(s, graduation);

        if (status != OK) {
            return status;
        }

        cout << "GRAD " << words[1] << " "
             << graduation << endl;

        return OK;
    }

    // ELIGIBLE command
    if (command == "ELIGIBLE") {

        if (count != 4) {
            return E_PARSE;
        }

        int s = findStudent(words[1]);

        if (s == -1) {
            cout << "ELIGIBLE " << words[1]
                 << " " << words[2] << " NO E1" << endl;
            return OK;
        }

        int c = findCourse(words[2]);

        if (c == -1) {
            cout << "ELIGIBLE " << words[1]
                 << " " << words[2] << " NO E1" << endl;
            return OK;
        }

        int term;

        if (!parseInteger(words[3], term) ||
            term < 1 || term > 12) {
            return E_PARSE;
        }

        int eligibility = checkEligibility(s, c, term);

        if (eligibility == OK) {
            cout << "ELIGIBLE " << words[1]
                 << " " << words[2] << " YES" << endl;
        }
        else {

            const char* check = "E1";

            if (eligibility == E_ALREADY_PASSED) {
                check = "E2";
            }
            else if (eligibility == E_ALREADY_REGISTERED) {
                check = "E3";
            }
            else if (eligibility == E_PREREQ) {
                check = "E4";
            }
            else if (eligibility == E_CREDIT_LIMIT) {
                check = "E5";
            }
            else if (eligibility == E_THESIS_NOT_ALLOWED) {
                check = "E6";
            }

            cout << "ELIGIBLE " << words[1]
                 << " " << words[2]
                 << " NO " << check << endl;
        }

        return OK;
    }

    // Unknown command
    return E_PARSE;
}
