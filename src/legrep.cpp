#include "legrep.h"

#include <iostream>
#include <sstream>

LeGrep::LeGrep(int argc, char *argv[])
{
    try {
        this->args = new LeGrepArguments(argc, argv);
    }
    catch (LeGrepException &e) {
        throw e;
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
                (found == 0 && args->invertMatch)) {
                string line = args->files[i].getLine(j);

                // Lines before
                if (args->beforeContext.first) {
                    int bContext = args->beforeContext.second;
                    for (; bContext > 0; --bContext) {
                        if (j-bContext < 0) continue;
                        std::cout << j-bContext + 1 << ": " << args->files[i].getLine(j-bContext) << "\n";
                    }
                }

                // Show line
                std::cout << j+1 << ": " << line << "\n";

                // Show position
                if (!args->invertMatch) {
                    std::stringstream helper;
                    helper << j+1 << ": ";
                    string s(line.length() + helper.str().length(), ' ');
                    for(unsigned int k = 0; k < indexes.size(); k++)
                        s[indexes[k]+helper.str().length()] = '^';
                    std::cout << s << "\n";
                }

                // Lines after
                if (args->afterContext.first) {
                    int aContext = 1;
                    for (; aContext <= args->afterContext.second; ++aContext) {
                        if (j+aContext >= args->files[i].size()) break;
                        std::cout << j+aContext + 1 << ": " << args->files[i].getLine(j+aContext) << "\n";
                    }
                }

            }

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
