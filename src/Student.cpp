// Project: OOPD_PROJECT
// Submitted by: KRISHAN KANT KUMAR
// Roll Number: MT26169
// Group Number: 119

#include "erp.h"

// Constructor
Student::Student() {
    roll = "";
    name = "";
    numRegs = 0;

    regs = new Registration[MAX_REGS];
}

// Destructor
Student::~Student() {
    delete[] regs;
}

// Copy constructor
Student::Student(const Student& other) {
    roll = other.roll;
    name = other.name;
    numRegs = other.numRegs;

    regs = new Registration[MAX_REGS];

    for (int i = 0; i < numRegs; i++) {
        regs[i] = other.regs[i];
    }
}

// Assignment operator
Student& Student::operator=(const Student& other) {

    if (this != &other) {

        roll = other.roll;
        name = other.name;
        numRegs = other.numRegs;

        for (int i = 0; i < numRegs; i++) {
            regs[i] = other.regs[i];
        }
    }

    return *this;
}

// Store student information
void Student::setStudent(const string& r, const string& n) {
    roll = r;
    name = n;
}

// Return roll number
string Student::getRoll() const {
    return roll;
}

// Return student name
string Student::getName() const {
    return name;
}

// Return number of registrations
int Student::getNumRegs() const {
    return numRegs;
}

// Return registration at a particular index
Registration* Student::getRegistration(int index) {

    if (index < 0 || index >= numRegs) {
        return NULL;
    }

    return &regs[index];
}

// Constant version of registration getter
const Registration* Student::getRegistration(int index) const {

    if (index < 0 || index >= numRegs) {
        return NULL;
    }

    return &regs[index];
}

// Add a new registration
ErrorCode Student::addRegistration(int course, int term) {

    if (numRegs >= MAX_REGS) {
        return E_FULL;
    }

    regs[numRegs].setRegistration(course, term);
    numRegs++;

    return OK;
}

// Find the latest registration using course index and term
int Student::findRegistration(int course, int term) const {

    for (int i = numRegs - 1; i >= 0; i--) {

        if (regs[i].getCourse() == course &&
            regs[i].getTerm() == term) {

            return i;
        }
    }

    return -1;
}
