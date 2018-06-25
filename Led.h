//-----------------------------------------------
/**
* The is the header file for LED class which takes a file name as input
* from the user and reads the content from that file into the buffer

* If no file name is specified then it initializes an empty buffer and enters
* into the command mode, where user enters a command

* This class directly interacts with the command class to parse the command entered
* by the user, to transform it into a generic form and to validate it
*
*
* @author  Mandeep Ahlawat
* @version 1.0
* @since   2018-06-22 
*/
//-----------------------------------------------
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
    void parseCommand(); // continously looks for a command symbol and perform commands based on the input
    void setLastLineNumber(); // sets the last line number of the buffer.
    void setUnSavedData(); // sets the flag that the content is changed.

    void fileExists(); // checks if the filename entered exists or not
    void performCommand(int, int, char); // this function checks for the input char symbol and performs the operations based on it
    
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
    
    Led(std::string=""); // ctor
    void run();    // method to run the operations of the LED
};
#endif
