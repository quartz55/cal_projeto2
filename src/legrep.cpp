#include "legrep.h"

#include <iostream>
#include <unistd.h>

LeGrep::LeGrep(int argc, char *argv[])
{
    try {
        this->args = new LeGrepArguments(argc, argv);
    }
    catch (LeGrepException &e) {
        e.print();
    }

    if (this->args->files.empty())
        readInput();
    else {
        searchFiles();
    }
}

int LeGrep::searchFiles()
{
    int total = 0;
    for (unsigned int i = 0; i < args->files.size(); i++) {
        std::cout << "FILE: " << args->files[i].getName() << "\n";
        string str = args->pattern;
        if (args->ignoreCase) str = LeGrep::stringToUpper(str);

        for (int j = 0; j < args->files[i].size(); j++) {
            string line = args->files[i].getLine(j);
            if (args->ignoreCase) line = LeGrep::stringToUpper(line);

            vector<int> indexes = KMP(line, str);
            int found = indexes.size();

            if ((found > 0 && !args->invertMatch) ||
                (found == 0 && args->invertMatch))
                std::cout << j+1 << ":" << args->files[i].getLine(j) << "\n";

            total += found;
        }
    }

    return total;
}

void LeGrep::readInput()
{
    string line;
    while(getline(cin, line))
        searchLine(line);
}

int LeGrep::searchLine(string line)
{
    string str = args->pattern;
    if (args->ignoreCase) str = LeGrep::stringToUpper(str);

    string temp_line = line;
    if (args->ignoreCase) temp_line = LeGrep::stringToUpper(temp_line);

    vector<int> indexes = KMP(temp_line, str);
    int found = indexes.size();

    if ((found > 0 && !args->invertMatch) ||
        (found == 0 && args->invertMatch))
        std::cout << line << "\n";

    return found;
}

LeGrep::~LeGrep()
{
    delete(args);
}
