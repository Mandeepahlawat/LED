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
        bool invalid; // if unwanted symbols are being used as commands
    
        bool hasComma();
        void checkValidityAndParseAttributes();
        void assignAddressesAndSymbol();
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
        void parse(std::string, int, int);
};
#endif
