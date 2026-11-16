# Project: OOPD_PROJECT
# Submitted by: KRISHAN KANT KUMAR
# Roll Number: MT26169
# Group Number: 119

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -fno-exceptions

TARGET = acaderp

SOURCES = src/main.cpp \
          src/ERP.cpp \
          src/Course.cpp \
          src/Student.cpp \
          src/Registration.cpp \
          src/Utilities.cpp

all: $(TARGET)

$(TARGET): $(SOURCES) src/erp.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	./run_tests.sh

clean:
	rm -f $(TARGET)

.PHONY: all test clean
