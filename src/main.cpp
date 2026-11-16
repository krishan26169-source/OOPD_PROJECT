// Project: OOPD_PROJECT
// Submitted by: KRISHAN KANT KUMAR
// Roll Number: MT26169
// Group Number: 119

#include "erp.h"

int main() {

    ERP erp;

    string line;
    int lineNumber = 0;

    while (getline(cin, line)) {

        lineNumber++;

        ErrorCode status = erp.processLine(line, lineNumber);

        if (status != OK) {
            cout << "ERROR " << lineNumber << " "
                 << errorCodeName(status) << endl;
        }
    }

    return 0;
}
