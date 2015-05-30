#include "legrep.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [OPTION]... PATTERN [FILE]\n";
        return 1;
    }

    vector<string> arguments;

    int i;
    for (i = 1; i < argc; i++) {
        string temp = string(argv[i]);
        if (temp[0] != '-') break;

        arguments.push_back(temp);
    }

    if (i == argc) {
        std::cerr << "#ERROR# No PATTERN specified\n";
        return 2;
    }

    string PATTERN = string(argv[i]);
    string FILE = "(null)";
    if (argc > ++i) FILE = argv[i];

    std::cout << "Arguments:";
    for (unsigned int j = 0; j < arguments.size(); j++)
        std::cout << " " << arguments[j];
    std::cout << "\nPattern: " << PATTERN;
    std::cout << "\nFILE: " << (FILE == "(null)" ? "No file" : FILE);
    std::cout << "\n";

    LeGrep *grep = NULL;
    vector<string> *args;
    if (arguments.empty())
        args = NULL;
    else args = &arguments;

    try{
        grep = new LeGrep(FILE, PATTERN, args);
    }
    catch (string s) {
        std::cerr << "#ERROR# File not found '" << s << "'\n";
        delete(grep);
        return 1;
    }

    int occurrences = 0;

    if (FILE == "(null)") goto NOFILE;

    // grep->printFile();
    // std::cout << "\n";

    occurrences = grep->searchFile();

    std::cout << "\n" << occurrences << " occurrences of '" << PATTERN << "'\n\n";

    delete(grep);
    return 0;

NOFILE:

    string line;
    while(getline(cin, line))
        grep->searchLine(line);

    delete(grep);

    return 0;
}
