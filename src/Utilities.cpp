// Project: OOPD_PROJECT
// Submitted by: KRISHAN KANT KUMAR
// Roll Number: MT26169
// Group Number: 119

#include "erp.h"

// Split input line into individual words
int splitWords(const string& line, string* words, int maxWords) {

    int count = 0;
    int i = 0;
    int length = static_cast<int>(line.length());

    while (i < length) {

        // Skip whitespace
        while (i < length && isspace(static_cast<unsigned char>(line[i]))) {
            i++;
        }

        if (i >= length) {
            break;
        }

        // Check maximum word limit
        if (count >= maxWords) {
            return -1;
        }

        words[count] = "";

        // Extract current word
        while (i < length && !isspace(static_cast<unsigned char>(line[i]))) {
            words[count] += line[i];
            i++;
        }

        count++;
    }

    return count;
}


// Convert ErrorCode into printable string
const char* errorCodeName(ErrorCode error) {

    switch (error) {

        case OK: return "OK";
        case E_PARSE: return "E_PARSE";
        case E_FULL: return "E_FULL";
        case E_DUPLICATE: return "E_DUPLICATE";
        case E_UNKNOWN_STUDENT: return "E_UNKNOWN_STUDENT";
        case E_UNKNOWN_COURSE: return "E_UNKNOWN_COURSE";
        case E_BAD_COURSE: return "E_BAD_COURSE";
        case E_BAD_GRADE: return "E_BAD_GRADE";
        case E_GRADE_TYPE: return "E_GRADE_TYPE";
        case E_NOT_REGISTERED: return "E_NOT_REGISTERED";
        case E_ALREADY_GRADED: return "E_ALREADY_GRADED";
        case E_THESIS_STATE: return "E_THESIS_STATE";
        case E_ALREADY_PASSED: return "E_ALREADY_PASSED";
        case E_ALREADY_REGISTERED: return "E_ALREADY_REGISTERED";
        case E_PREREQ: return "E_PREREQ";
        case E_CREDIT_LIMIT: return "E_CREDIT_LIMIT";
        case E_THESIS_NOT_ALLOWED: return "E_THESIS_NOT_ALLOWED";
        case E_NO_GPA: return "E_NO_GPA";

        default: return "E_PARSE";
    }
}


// Check whether a symbol is a valid letter grade
bool isValidLetterGrade(const string& grade) {

    if (grade == "A"  ||
        grade == "A-" ||
        grade == "B"  ||
        grade == "B-" ||
        grade == "C"  ||
        grade == "C-" ||
        grade == "D"  ||
        grade == "F") {

        return true;
    }

    return false;
}
