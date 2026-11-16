# OOPD_PROJECT
Academic Records and Graduation Audit System
**Submitted by:** KRISHAN KANT KUMAR  
**Roll Number:** MT26169  
**Group Number:** 119


## 1. Project Overview

OOPD_PROJECT is a command-driven academic records management system
implemented in C++11.

It manages students, courses, registrations and grades. It supports
three course types:

- REGULAR: Letter grades A, A-, B, B-, C, C-, D and F.
- SU: Satisfactory/Unsatisfactory grades S and X.
- THESIS: Progress grades S and X, followed by conversion to a
  final letter grade.

The system provides GPA calculation, average-grade calculation,
graduation auditing and course eligibility checking.

## 2. Requirements

- Linux environment
- g++ compiler supporting C++11
- make
- unzip utility (to extract the submission archive)

## 3. Project Structure

    OOPD_PROJECT/
    |
    |-- src/
    |   |-- erp.h
    |   |-- main.cpp
    |   |-- ERP.cpp
    |   |-- Course.cpp
    |   |-- Student.cpp
    |   |-- Registration.cpp
    |   |-- Utilities.cpp
    |
    |-- tests/
    |   |-- T01.in
    |   |-- T01.out
    |   |-- ...
    |   |-- T26.in
    |   |-- T26.out
    |
    |-- Makefile
    |-- README.md
    |-- run_tests.sh

## 4. Setup and Execution

Extract the submission archive:

    unzip OOPD_PROJECT.zip

Enter the project directory:

    cd OOPD_PROJECT

Compile the project:

    make

Run the executable with an input file:

    ./acaderp < tests/T01.in

Run the complete automated test suite:

    ./run_tests.sh

Alternatively, run the tests using:

    make test

Remove the generated executable:

    make clean

## 5. Build Instructions

Compile the project using:

    make

This produces the executable:

    acaderp

The project uses the following compiler options:

    -std=c++11 -Wall -Wextra -fno-exceptions

To remove the generated executable:

    make clean

## 6. Running the Program

The program reads commands from standard input.

Example:

    ./acaderp < sample.in

Commands that change stored data do not print output when successful.
Queries print their results. Errors are printed in the format:

    ERROR <line_number> <error_code>

The program continues processing subsequent commands after an error.

## 7. Automated Testing

Run the complete test suite using:

    ./run_tests.sh

Alternatively:

    make test

The test suite contains 26 input/output pairs, T01 through T26.
Each test runs the program with its corresponding input file and
compares the actual output against the expected output.

The test script reports the number of passed and failed tests.

## 8. Program Architecture

### ERP

ERP is the main management class. It maintains the course catalogue,
student records and academic program requirements.

It processes commands and performs operations such as registration,
grade submission, GPA calculation, eligibility checking and
graduation auditing.

### Course

Course stores the course code, course type, credit value,
category and prerequisite information.

### Student

Student stores the roll number, name and registration records.

Each student has a fixed maximum capacity of 40 registrations.

### Registration

Registration stores the course index, term, grade and thesis state
for an individual course registration.

### Utilities

Utilities contains common helper functions for splitting input
commands, identifying grade symbols and converting error codes
into their output names.

### Main

The main function reads input one line at a time, sends each line
to ERP for processing and prints an error message whenever a
command returns an error code.

## 9. Memory Management

The project uses fixed-size arrays for the student and course
catalogues.

Each Student object dynamically allocates an array of 40
Registration objects using:

    new Registration[MAX_REGS]

The Student destructor releases this memory using:

    delete[] regs

The Student copy constructor and assignment operator perform
deep copies of the registration array to avoid sharing dynamically
allocated memory between Student objects.

The ERP object is created in main. Its Student objects are
destroyed automatically when the ERP object goes out of scope.
Their destructors release the dynamically allocated registration
arrays.

## 10. Error Handling

The project uses an ErrorCode enumeration to report errors.

Fallible operations return an ErrorCode instead of using
exceptions.

The main input loop checks the returned status and prints the
corresponding error code with the input line number.

A failed command must not partially modify the stored academic data.

## 11. Assumptions and Implementation Decisions

- If PROGRAM is not supplied, the default requirements are:
  total credits = 160, elective credits = 24, minimum GPA = 5.00,
  maximum term load = 24 and thesis eligibility credits = 100.
- Course prerequisites must already exist when a course is created.
- Student names are single words, as required by the command format.
- Terms are restricted to integers from 1 through 12.
- Only the latest graded attempt of a course is used for GPA and
  earned-credit calculations.
- A failed attempt can be repeated. A course cannot be registered
  again while its latest attempt is ungraded.
- SU courses do not contribute to GPA.
- Thesis progress grades do not contribute to GPA. A thesis
  contributes only after conversion to a final letter grade.
- GPA is calculated using integer arithmetic and rounded half-up
  to two decimal places.
- Blank lines and comment lines beginning with # are ignored,
  but they still count toward input line numbers.
- The program processes commands sequentially and reports errors
  without terminating the input loop.

## 12. Assignment Compliance

The implementation uses C++11 and the permitted headers and
data structures specified in the assignment.

It does not use STL containers, smart pointers or exceptions.

## 13. Verification

The project was verified using the automated test suite.
All 26 test cases passed in the normal test run.

The same 26 test cases also passed when compiled and run with
AddressSanitizer and UndefinedBehaviorSanitizer.
