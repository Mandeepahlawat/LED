//-----------------------------------------------
/**
* The is the header file for command class which parses the
* command and returns line Address1, line Address2 and symbol
* to the LED object. It has various helper methods like check
* validity, hasComma and trim to validate the command.
*
* Irrespective of input string format, it always returns a string in the format of x,yz
* where x,y are line address 1 and line address 2 and z
* is the command
*
*	For example:
* input : '1'				 output : '1,1g'
* input : '1,1'			 output : '1,1g'
* input :	'1,1g'		 output	: '1,1g'
*
* @author  Mandeep Ahlawat
* @version 1.0
* @since   2018-06-22 
*/
//-----------------------------------------------
#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <iostream>
class Command{
	private:
    std::string inputString;
		std::string lineAddress1;
		std::string lineAddress2;
		char symbol;
    bool checkValidity; // if command parses and needs to check for line range
    bool invalid; // if unwanted symbols are being used as commands then this variable is set to true

    bool hasComma(); // checks if there's a comma in the input string
    void trimAndSetInputString(const std::string &);  // remove leading and trailing white spaces and sets inputString
    bool onlyDigits(const std::string &); // checks if line numbers only has digits and no alphanumeric characters else it will be invalid
    bool validAlpha(const std::string &); // checks if line numbers with alphabet is valid
    bool validSymbol(const char &); // validate if the symbol is included in the list
    
	public:
    Command();
    bool isValid(int, int);
    inline int getLineAddress1() const {return stoi(lineAddress1);}
    inline int getLineAddress2() const {return stoi(lineAddress2);}
    inline char getSymbol() const {return symbol;}

    void parse(std::string, int, int); // parses the input string and returns it into the generic format
};
#endif
