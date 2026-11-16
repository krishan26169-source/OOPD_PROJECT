// Project: OOPD_PROJECT
// Submitted by: KRISHAN KANT KUMAR
// Roll Number: MT26169
// Group Number: 119

#include "erp.h"

// Constructor
Course::Course() {
    code = "";
    type = REGULAR;
    credits = 0;
    category = CORE;
    prereq = -1;
}

// Store course information
void Course::setCourse(const string& c, CourseType t,
                       int cr, CourseCategory cat, int p) {

    code = c;
    type = t;
    credits = cr;
    category = cat;
    prereq = p;
}

// Return course code
string Course::getCode() const {
    return code;
}

// Return course type
CourseType Course::getType() const {
    return type;
}

// Return course credits
int Course::getCredits() const {
    return credits;
}

// Return course category
CourseCategory Course::getCategory() const {
    return category;
}

// Return prerequisite index
int Course::getPrereq() const {
    return prereq;
}
