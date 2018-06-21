#include "Led.h"
#include "command.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <iomanip>

using namespace std;

Led::Led(string filename) : currentLineNumber{0}, lastLineNumber{0} {
    fileName = filename;
    fileExists();
    unsavedData = false;
}

void Led::run(){
    if(filePresent){
        cout << "\"" << fileName << "\" " << linesReadFromFileCount << " lines" << "\n" << "Entering command mode." << endl;
    }
    else{
        if(fileName != ""){
            cout << "\"" << fileName << "\" " << "[New File]" << "\n" << "Entering command mode." << endl;
        }
        else{
            cout << "\"?\" " << "[New File]" << "\n" << "Entering command mode." << endl;
        }
    }
    parseCommand();
}

void Led::parseCommand(){
    cout <<"? ";
    string inputCommand;
    
    while(getline(cin, inputCommand)){
        Command cmd;
        cmd.parse(inputCommand, currentLineNumber, lastLineNumber);
        if(cmd.isValid(currentLineNumber, lastLineNumber)){
            performCommand(cmd.getLineAddress1(), cmd.getLineAddress2(), cmd.getSymbol());
        }
        else{
            cout << "Invalid range" << endl;
        }
        cout <<"? ";
    }
}

void Led::fileExists(){
    ifstream ifs(fileName);
    if(ifs.good()){
        // read file into the buffer
        filePresent = true;
        string line;
        linesReadFromFileCount = 0;
        while (std::getline(ifs, line)){
            buffer.push_back(line);
            linesReadFromFileCount++;
        }
        setLastLineNumber();
        currentLineNumber = lastLineNumber;
        ifs.close();
        return;
    }
    filePresent = false;
    ifs.close();
}

//void Led::readFileContent(ifstream ifs){
//
//}

void Led::performCommand(int l1, int l2, char cmd){
    switch (cmd) {
        case 'i':
            insertData(l1);
            break;
        case 'a':
            appendData(l1);
            break;
        case 'v':
            pasteData(l1);
            break;
        case 'u':
            pasteDataAbove(l1);
            break;
        case 'd':
            deleteLineRange(l1, l2);
            break;
        case 'x':
            cutLineRange(l1, l2);
            break;
        case 'r':
            replaceLineRange(l1, l2);
            break;
        case 'j':
            joinLineRange(l1, l2);
            break;
        case 'p':
            printLineRange(l1, l2);
            break;
        case 'c':
            changeLineRange(l1, l2);
            break;
        case '-':
            moveCurrLineUp(l1);
            break;
        case '+':
            moveCurrLineDown(l1);
            break;
        case 'g':
            setCurrentLineAndPrint(l1);
            break;
        case 'w':
            writeFile();
            break;
        case 'q':
            quit();
            break;
        default:
            break;
    }
}

void Led::quit(){
    if(unsavedData){
        cout << "Save changes to file (y/n)? ";
        string choice;
        getline(cin, choice);
        if(choice == "y"){
            writeFile();
            cout << "Bye" << endl;
            exit(1);
        }
        else if(choice == "n"){
            cout << "Bye" << endl;
            exit(1);
        }
        else {
            cout << "bad answer: " << choice << endl;
            cout << "Enter y for yes and n for a no." << endl;
            quit();
        }
    }
    else{
        cout << "Bye" << endl;
        exit(1);
    }
}

void Led::writeFile(){
    if(unsavedData){
        if(fileName == ""){
            cout << "Enter a file name: ";
            getline(cin, fileName);
        }
        ofstream out(fileName);
        int temp = 0;
        for(auto s : buffer){
            out << s;
            temp++;
            if(temp != buffer.size()){
                out << endl;
            }
        }
        cout << lastLineNumber << " lines written to file: " << "\"" << fileName << "\"" <<endl;
        out.close();
        unsavedData = false;
    }
    else{
        cout << "You haven't made any changes to the file yet, so no data is saved." << endl;
    }
}

void Led::setCurrentLineAndPrint(int l1){
    currentLineNumber = l1;
    printLineRange(l1, l1);
}

void Led::moveCurrLineUp(int l1){
    currentLineNumber = currentLineNumber - l1;
    if(currentLineNumber <= 0){
        cout << "top of file reached" << endl;
        currentLineNumber = 1;
    }
}

void Led::moveCurrLineDown(int l1){
    currentLineNumber = currentLineNumber + l1;
    if(currentLineNumber > lastLineNumber){
        cout << "end of file reached" << endl;
        currentLineNumber = lastLineNumber;
    }
}

void Led::changeLineRange(int l1, int l2){
    cout << "Change What? ";
    string text;
    getline(cin, text);
    cout << setw(6) << "To" << " What? ";
    string replacementText;
    getline(cin, replacementText);
    
    int currLine = 1;
    int changeOccurenceCount = 0;
    for( auto &str : buffer){
        if(currLine >= l1 && currLine <= l2){
            size_t startPos = 0;
            while((startPos = str.find(text, startPos)) != string::npos){
                str.replace(startPos, text.size(), replacementText);
                changeOccurenceCount++;
                startPos = startPos + text.size();
                currentLineNumber = currLine;
            }
        }
        currLine++;
    }
    setUnSavedData();
    cout << "Changed " << changeOccurenceCount << " occurence(s)" << endl;
}

void Led::printLineRange(int l1, int l2){
    if(buffer.size() == 0){
        cout << "empty buffer" << endl;
    }
    else{
        int temp = 1;
        for(auto element : buffer){
            if(temp >= l1 && temp <= l2){
                cout << temp << (temp == currentLineNumber ? ">" : ":") << ' ' << element << endl;
            }
            temp++;
        }
    }
}

void Led::joinLineRange(int l1, int l2){
    list<string>::iterator it1;
    it1 = buffer.begin();
    advance(it1, l1-1);
    
    int temp = 1;
    string joinedString;
    for(auto element : buffer){
        if(temp >= l1 && temp <= l2){
            if(joinedString == ""){
                joinedString = element;
            }
            else{
                joinedString = joinedString + " " + element;
            }
        }
        temp++;
    }
    setUnSavedData();
    buffer.insert(it1, joinedString);
    deleteLineRange(l1+1, l2+1); // delete lines; added 1 to each l1 and l2 because new item is added before l1 so each element goes by 1
    currentLineNumber = l1;
    setLastLineNumber();
}

void Led::replaceLineRange(int l1, int l2){
    deleteLineRange(l1, l2);
    Led::insertData(l1);
}

void Led::cutLineRange(int l1, int l2){
    list<string>::iterator it1, it2;
    it1 = it2 = buffer.begin();
    advance(it1, l1-1); // index starts from 0 so need to subtract
    advance(it2, l2);
    
    clipboard.insert(clipboard.end(), it1, it2);
    deleteLineRange(l1, l2);
}

void Led::deleteLineRange(int l1, int l2){
    // deletes elements in range l1 and l2, both included, need to confirm if both are included?
    list<string>::iterator it1, it2;
    it1 = it2 = buffer.begin();
    advance(it1, l1-1); // index starts from 0 so need to subtract
    advance(it2, l2);
    
    buffer.erase(it1, it2);
    
    // if buffer size is greater than l2 and number of elements deleted
    if(buffer.size() >= l2 - (l2 - l1)){
        currentLineNumber = l1; // set to line number after deleted range
    }
    else{
        currentLineNumber = l1-1; // set to line number before deleted range as it has index 0 so need to subtract 1
    }
    setUnSavedData();
    setLastLineNumber();
}

void Led::pasteDataAbove(int l1){
    list<string>::iterator position = buffer.begin();
    advance(position, l1-1);
    
    for(auto const &a : clipboard){
        buffer.insert(position, a);
    }
    
    setUnSavedData();
    currentLineNumber = (int) distance(buffer.begin(), position); // advance counts 0 as well so need to add 1 here
    setLastLineNumber();
}

void Led::pasteData(int l1){
    list<string>::iterator position = buffer.begin();
    advance(position, l1-1);
    
    for(auto const &a : clipboard){
        buffer.insert(position, a);
    }
    setUnSavedData();
    currentLineNumber = (int) distance(buffer.begin(), position); // advance counts 0 as well so need to add 1 here
    setLastLineNumber();
}

void Led::insertData(int l1){
    string line;
    list<string>::iterator position = buffer.begin();
    advance(position, l1-1); // need to subtract 1 here becuase list index starts from 0 and we want to insert data before l2
    
    while(getline(cin, line)){
        if(buffer.size() > 0 || line.size() > 0){
            if(line == ";"){
                break;
            }
            buffer.insert(position, line);
            setUnSavedData();
        }
    }
    
    currentLineNumber = (int) distance(buffer.begin(), position); // advance counts 0 as well so need to add 1 here
    setLastLineNumber();
}

void Led::appendData(int l1){
    string line;
    list<string>::iterator position = buffer.begin();
    
    advance(position, l1); // no need to subtract 1 here because index starts from 0 and we want to append data after this number
    
    while(getline(cin, line)){
        if(buffer.size() > 0 || line.size() > 0){
            if(line == ";"){
                break;
            }
            buffer.insert(position, line);
            setUnSavedData();
        }
    }
    currentLineNumber = (int) distance(buffer.begin(), position);
    setLastLineNumber();
}

void Led::setLastLineNumber(){
    int size = int(buffer.size());
    if(size >= 1){
        lastLineNumber = int(buffer.size());
    }
    else{
        lastLineNumber = 0;
    }
}

void Led::setUnSavedData(){
    if(!unsavedData){
        unsavedData = true;
    }
}
