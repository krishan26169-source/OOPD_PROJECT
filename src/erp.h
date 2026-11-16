#ifndef ERP_H
#define ERP_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

using namespace std;

// Maximum limits
const int MAX_STUDENTS = 100;
const int MAX_COURSES = 50;
const int MAX_REGS = 40;
const int MAX_WORDS = 10;

// Error codes
enum ErrorCode {
    OK,
    E_PARSE,
    E_FULL,
    E_DUPLICATE,
    E_UNKNOWN_STUDENT,
    E_UNKNOWN_COURSE,
    E_BAD_COURSE,
    E_BAD_GRADE,
    E_GRADE_TYPE,
    E_NOT_REGISTERED,
    E_ALREADY_GRADED,
    E_THESIS_STATE,
    E_ALREADY_PASSED,
    E_ALREADY_REGISTERED,
    E_PREREQ,
    E_CREDIT_LIMIT,
    E_THESIS_NOT_ALLOWED,
    E_NO_GPA
};

// Course types
enum CourseType {
    REGULAR,
    SU,
    THESIS
};

// Course categories
enum CourseCategory {
    CORE,
    ELECTIVE
};

// Thesis states
enum ThesisState {
    NOT_THESIS,
    REGISTERED,
    IN_PROGRESS,
    FINAL,
    FAILED
};


// --------------------------------------------------
// COURSE CLASS
// --------------------------------------------------

class Course {
private:
    string code;
    CourseType type;
    int credits;
    CourseCategory category;
    int prereq;

public:
    Course();

    void setCourse(const string& c, CourseType t,
                   int cr, CourseCategory cat, int p);

    string getCode() const;
    CourseType getType() const;
    int getCredits() const;
    CourseCategory getCategory() const;
    int getPrereq() const;
};


// --------------------------------------------------
// REGISTRATION CLASS
// --------------------------------------------------

class Registration {
private:
    int course;
    int term;
    string grade;
    ThesisState thesis;

public:
    Registration();

    void setRegistration(int c, int t);

    int getCourse() const;
    int getTerm() const;
    string getGrade() const;
    ThesisState getThesisState() const;

    bool isGraded() const;

    void setGrade(const string& g);
    void setThesisState(ThesisState s);
};


// --------------------------------------------------
// STUDENT CLASS
// --------------------------------------------------

class Student {
private:
    string roll;
    string name;

    Registration* regs;
    int numRegs;

public:
    Student();
    ~Student();

    Student(const Student& other);
    Student& operator=(const Student& other);

    void setStudent(const string& r, const string& n);

    string getRoll() const;
    string getName() const;

    int getNumRegs() const;

    Registration* getRegistration(int index);
    const Registration* getRegistration(int index) const;

    ErrorCode addRegistration(int course, int term);

    int findRegistration(int course, int term) const;
};


// --------------------------------------------------
// ERP CLASS
// --------------------------------------------------

class ERP {
private:

    // Main database
    Student students[MAX_STUDENTS];
    Course courses[MAX_COURSES];

    int numStudents;
    int numCourses;
    bool programSet;
    bool dataStarted;

    // Program requirements
    int totalCredits;
    int electiveCredits;
    int minGPA;
    int maxLoad;
    int thesisCredits;

    // Search functions
    int findStudent(const string& roll) const;
    int findCourse(const string& code) const;

    // Command handlers
    ErrorCode setProgram(const string* words, int count);
    ErrorCode addCourse(const string* words, int count);
    ErrorCode addStudent(const string* words, int count);

    ErrorCode registerCourse(int s, int c, int term);

    ErrorCode submitGrade(int s, int c, int term,
                          const string& symbol);

    ErrorCode convertThesis(int s, int c,
                            const string& letter);

    // Academic calculations
    ErrorCode gpaHundredths(int s, int* result) const;

    int gradePoints(const string& grade) const;

    bool isPassed(int studentIndex, int courseIndex) const;

    int checkEligibility(int s, int c, int term) const;

    ErrorCode calculateAverage(int s, string& result) const;

    ErrorCode checkGraduation(int s, string& result) const;

public:

    ERP();

    // Main command processor
    ErrorCode processLine(const string& line, int lineNumber);

};


// --------------------------------------------------
// UTILITY FUNCTIONS
// --------------------------------------------------

int splitWords(const string& line, string* words, int maxWords);

const char* errorCodeName(ErrorCode error);

bool isValidLetterGrade(const string& grade);

#endif
