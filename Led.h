#ifndef LED_H
#define LED_H
#include <iostream>
#include <string>
#include <vector>
#include <list>

class Led{
    private:
    std::string fileName;
    std::list<std::string> buffer;
    std::vector<std::string> clipboard;
    int currentLineNumber;
    int lastLineNumber;
    bool filePresent;
    bool unsavedData;
    int linesReadFromFileCount;
    void parseCommand();
    void setLastLineNumber();
    void setUnSavedData();
    
    // LED operations
    void insertData(int);
    void appendData(int);
    void pasteData(int);
    void pasteDataAbove(int);
    void deleteLineRange(int, int);
    void cutLineRange(int, int);
    void replaceLineRange(int, int);
    void joinLineRange(int, int);
    void printLineRange(int, int);
    void changeLineRange(int, int);
    void moveCurrLineUp(int);
    void moveCurrLineDown(int);
    void setCurrentLineAndPrint(int);
    void writeFile();
    void readFileContent(std::ifstream);
    void quit();
    
    public:
    
    Led(std::string="");
    void run();

    void fileExists();
    
    void performCommand(int, int, char);
    
    
};
#endif
