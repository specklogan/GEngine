#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

class Logging {
public:
    void writeConfig(string stringToLog, fstream logFile) {
        logFile << stringToLog << std::endl;
        logFile.close();
    }

    fstream openLogFile() {
        fstream myFile;
        myFile.open("./log/latestlog.txt", ios::app);
        return myFile;
    }
};