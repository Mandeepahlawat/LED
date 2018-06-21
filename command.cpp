#include <string>
#include <iostream>
#include "command.h"

using namespace std;

Command::Command(){
    inputString = "";
    lineAddress1 = "";
    lineAddress2 = "";
    symbol = '\0';
    checkValidity = true;
    invalid = false;
}

void Command::trimAndSetInputString(const string &str){
    const auto strBegin = str.find_first_not_of(" \t");
    if(strBegin == string::npos){
        inputString = "";
        return;
    }
    const auto strEnd = str.find_last_not_of(" \t");
    const auto strRange = strEnd - strBegin + 1;
    inputString = str.substr(strBegin, strRange);
}

bool Command::hasComma(){
    if(inputString.find(',') != string::npos)
        return true;
    return false;
}

bool Command::onlyDigits(const string &lineNumber){
    for(char a : lineNumber){
        if(!isdigit(a)){
            return false;
        }
    }
    return true;
}

bool Command::validAlpha(const string &lineNumber){
    if(isalpha(lineNumber[0]) && lineNumber.size() == 1){
        return true;
    }
    return false;
}

bool Command::validSymbol(const char &sym){
    string str = "aivudxrjpc-+gwq*";
    if(str.find_first_of(sym) != string::npos){
        return true;
    }
    return false;
}

bool Command::isValid(int currentLine, int lastLine){
    if(checkValidity){
        if(invalid){
            return false;
        }
        if(lineAddress1 == "" || lineAddress2 == "" || symbol == ' ' || symbol == '\0'){
            // if either of them is not set then return false
            return false;
        }
        else{
            
            if(onlyDigits(lineAddress1) && onlyDigits(lineAddress2)){
                // if both lineaddress1 and lineaddress2 are numbers
                int l1 = stoi(lineAddress1);
                int l2 = stoi(lineAddress2);
                return (1 <= l1 &&  l1 <= l2 && l2 <= lastLine);
            }
            else if((validAlpha(lineAddress1) || lineAddress1 == "." || lineAddress1 == "$") && onlyDigits(lineAddress2)){
                // if lineaddress1 is character and lineaddress2 is number
                int l2 = stoi(lineAddress2);
                return (1 <= currentLine && currentLine <= l2 && l2 <= lastLine);
            }
            else if (onlyDigits(lineAddress1) && (validAlpha(lineAddress2) || lineAddress2 == "." || lineAddress2 == "$")){
                // if lineaddress1 is number and lineaddress2 is character
                int l1 = stoi(lineAddress1);
                if(lineAddress1 == "1" && lineAddress2 == "$" && symbol == 'p'){
                    return true;
                }
                return (1 <= l1 && l1 <= currentLine && currentLine <= lastLine);
            }
            else if(validAlpha(lineAddress1) && validAlpha(lineAddress2)){
                // if both are characters
                return (1 <= currentLine <= lastLine);
            }
            
        }
        return false;
    }
    return true;
}

void Command::parse(string strCommand, int currentLine, int lastLine){
    trimAndSetInputString(strCommand);
//    inputString = strCommand;
    string stringCurrentLine = to_string(currentLine);
    string stringLastLine = to_string(lastLine);
    if(hasComma()){
        // command can contain lineaddress1 or lineaddress2 or a command or a combination of them
        int temp = 0;
        while(inputString[temp] != ','){
            if(!isspace(inputString[temp])){
                lineAddress1 = lineAddress1 + inputString[temp];
            }
            temp++;
        }
        
        temp = temp + 1;
        
        while(temp != inputString.size()){
            if(isdigit(inputString[temp]) || inputString[temp] == '$' || inputString[temp] == '.'){
                if(!isspace(inputString[temp])){
                    lineAddress2 = lineAddress2 + inputString[temp];
                }
            }
            else{
                if(!isspace(inputString[temp])){
                    if(validSymbol(inputString[temp])){
                        symbol = symbol + inputString[temp];
                    }
                    else{
                        invalid = true;
                    }
                }
            }
            temp++;
        }
        
        if(lineAddress1 == ""){
            lineAddress1 = stringCurrentLine;
        }
        if(lineAddress2 == ""){
            lineAddress2 = stringCurrentLine;
        }
        if(symbol == ' ' || symbol == '\0'){
            symbol = 'p';
        }
    }
    else{
        // if a command doesn't contain a comma then it can either be lineadress1 or a command but can't be both
        int temp = 0;
        if(isdigit(inputString[temp]) || inputString[temp] == '$' || inputString[temp] == '.'){
            while(temp != inputString.size()){
                if(isdigit(inputString[temp]) || inputString[temp] == '$' || inputString[temp] == '.'){
                    if(!isspace(inputString[temp])){
                        lineAddress1 = lineAddress1 + inputString[temp];
                        
                        if(lineAddress1.size() == 1 && (inputString[temp] == '$' || inputString[temp] == '.')){
                            // don't check validity if $ or . are used to print empty out put buffer
                            checkValidity = false;
                        }
                    }
                }
                else{
                    if(!isspace(inputString[temp])){
                        if(validSymbol(inputString[temp])){
                            symbol = symbol + inputString[temp];
                        }
                        else{
                            invalid = true;
                        }
                    }
                }
                temp++;
            }
        }
        else if(inputString == ""){
            // enter is pressed, simulate 1+ command
            lineAddress1 = "1";
            symbol = '+';
            checkValidity = false;
        }
        else{
            // only command symbol is present
            if(validSymbol(inputString[temp])){
                symbol = symbol + inputString[temp];
            }
            else{
                invalid = true;
            }
            
            // set l1 and l2 to current line if it's just a command and command is not '+' or '-'
            if(symbol != '+' && symbol != '-'){
                lineAddress1 = stringCurrentLine;
                lineAddress2 = stringCurrentLine;
            }
            
            if(symbol == '+' || symbol == '-'){
                checkValidity = false;
            }
            else if(lastLine != 0){
                if(validSymbol(symbol)){
                    checkValidity = false;
                }
            }
            else{
                if(symbol == 'i' || symbol == 'a' || symbol == 'p'){
                    checkValidity = false;
                }
            }
        }
        
        if(lineAddress1 == "" && lineAddress2 == "" && symbol == 'p'){
            lineAddress1 = stringCurrentLine;
            lineAddress2 = stringCurrentLine;
        }
        
        if(symbol == '*'){
            lineAddress1 = "1";
            lineAddress2 = stringLastLine;
            symbol = 'p';
        }
        if(lineAddress1 == ""){
            lineAddress1 = "1";
            lineAddress2 = "1";
        }
        if(lineAddress1 != "" && lineAddress2 == ""){
            lineAddress2 = lineAddress1;
        }
        if(symbol == ' ' || symbol == '\0'){
            lineAddress2 = lineAddress1;
            symbol = 'g';
        }
    }
    
    // transform character symbols to line numbers for lineAddress1
    if(lineAddress1 == "."){
        lineAddress1 = stringCurrentLine;
    }
    else if (lineAddress1 == "$"){
        lineAddress1 = stringLastLine;
    }
    
    // transform character symbols to line numbers for lineAddress2
    if(lineAddress2 == "."){
        lineAddress2 = stringCurrentLine;
    }
    else if (lineAddress2 == "$"){
        lineAddress2 = stringLastLine;
    }
    
//    cout << lineAddress1 << "," << lineAddress2 << symbol << endl;
}
