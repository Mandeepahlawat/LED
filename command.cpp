//-----------------------------------------------
/**
* The is the implementation file for command class which parses the
* command and returns line Address1, line Address2 and symbol
* to the LED object. It has various helper methods like check
* validity, hasComma and trim to validate the command.
*
* Irrespective of input string format, it always returns a string in the format of x,yz
* where x,y are line address 1 and line address 2 and z
* is the command
*
* For example:
* input : '1'        output : '1,1g'
* input : '1,1'      output : '1,1g'
* input : '1,1g'     output : '1,1g'
*
* @author  Mandeep Ahlawat
* @version 1.0
* @since   2018-06-22 
*/
//-----------------------------------------------
#include <string>
#include <iostream>
#include "command.h"

using namespace std;

//-----------------------------------------------
/*
* Constructor for the class which initializes all the
* variables to initial values
*/
//-----------------------------------------------
Command::Command(){
    inputString = "";
    lineAddress1 = "";
    lineAddress2 = "";
    symbol = '\0';
    checkValidity = true;
    invalid = false;
}


//-----------------------------------------------
/*
* Removes leading and trailing white spaces and tabs
* Searches the string for the first occurrence of non empty
* spaces and then search for the last non empty space character
* and sets the inputString member variable to new string

* @param  str the string input to the parse method
*/
//-----------------------------------------------
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

//-----------------------------------------------
/*
* Searches string for the ','

* @return True or false based on if the inputString has
*         a comma or not
*/
//-----------------------------------------------
bool Command::hasComma(){
    if(inputString.find(',') != string::npos)
        return true;
    return false;
}

//-----------------------------------------------
/*
* Searches lineNumber to see if it only has digits

* @param  lineNumber a constant string

* @return True or false based on if the lineNumber
*         has only digits or does it also has characters
*/
//-----------------------------------------------
bool Command::onlyDigits(const string &lineNumber){
    for(char a : lineNumber){
        if(!isdigit(a)){
            return false;
        }
    }
    return true;
}

//-----------------------------------------------
/*
* Searches lineNumber to see if it has valid symbols
* A lineNumber can have valid symbol only if its size
* is 1 and it's a character

* @param  lineNumber a constant string

* @return True or false based on if the lineNumber
*         has a valid alphabet character
*/
//-----------------------------------------------
bool Command::validAlpha(const string &lineNumber){
    if(isalpha(lineNumber[0]) && lineNumber.size() == 1){
        return true;
    }
    return false;
}

//-----------------------------------------------
/*
* Checks if the symbol entered is valid. All the valid
* symbols are assigned in a string and that string is
* searched for the input symbol to find if it's valid

* @param  input constant char symbol

* @return True or false based on if the input symbol
*         is valid or not
*/
//-----------------------------------------------
bool Command::validSymbol(const char &sym){
    string str = "aivudxrjpc-+gwq*";
    if(str.find_first_of(sym) != string::npos){
        return true;
    }
    return false;
}

//-----------------------------------------------
/*
* Checks if the string parsed by the parse method id valid
* This function also checks for some flags like checkValidity
* and invalid if they are set then it either skips the validation
* or just return false and invalidate the command

* @param currentLine, the current line in LED
* @param lastLine, the last line in buffer of LED

* @return True or false based on if the parsed command
*         is valid or not
*/
//-----------------------------------------------
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

//-----------------------------------------------
/*
* Parsed the input string and transform it into a generic format
* of the command, handles all the edge cases and strip all the symbols('.', '$')
* from the lineAddress1 and lineAddress2 and assign the respective line numbers.

* @param currentLine, the current line in LED
* @param lastLine, the last line in buffer of LED
* @param strCommand, the input command as a string
*/
void Command::parse(string strCommand, int currentLine, int lastLine){
    trimAndSetInputString(strCommand);
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
                    if(validSymbol(inputString[temp]) && symbol == '\0'){
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
            if(validSymbol(inputString[temp]) && inputString.size() == 1){
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
    
    // don't validate if the command symbol is 'q', i.e if user wants to quit
    if(symbol == 'q'){
        checkValidity = false;
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
}
