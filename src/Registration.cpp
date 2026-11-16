// Project: OOPD_PROJECT
// Submitted by: KRISHAN KANT KUMAR
// Roll Number: MT26169
// Group Number: 119

#include "erp.h"

// Constructor
Registration::Registration() {
    course = -1;
    term = 0;
    grade = "";
    thesis = NOT_THESIS;
}

// Store registration information
void Registration::setRegistration(int c, int t) {
    course = c;
    term = t;
    grade = "";
    thesis = REGISTERED;
}

// Return course index
int Registration::getCourse() const {
    return course;
}

// Return semester
int Registration::getTerm() const {
    return term;
}

// Return grade
string Registration::getGrade() const {
    return grade;
}

// Return thesis state
ThesisState Registration::getThesisState() const {
    return thesis;
}

// Check whether registration has a grade
bool Registration::isGraded() const {
    return grade != "";
}

// Store grade
void Registration::setGrade(const string& g) {
    grade = g;
}

// Update thesis state
void Registration::setThesisState(ThesisState s) {
    thesis = s;
}
