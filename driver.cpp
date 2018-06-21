#include "Led.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int main(int argc, char * argv[]){
    string filename;
    switch (argc){
        case 1:
            // no file name
            break;
        case 2:
            filename = argv[1];
            break;
        default:
            cout << "Too many arguments -- all discarded" << endl;
            break;
    }
    Led editor(filename);
    editor.run();
    return 0;
}
