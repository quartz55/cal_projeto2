#include "legrep.h"

#include <iostream>

LeGrep::LeGrep(string filename, string pattern, vector<string> *arguments)
{
    ignoreCase = false;
    invertMatch = false;
    afterContext.first = false;
    afterContext.second = 0;
    beforeContext.first = false;
    beforeContext.second = 0;
    file = NULL;

    if (filename != "(null)") {
        file = new LeFile(filename);
    }

    this->pattern = pattern;

    if (arguments != NULL) {
        setArguments(*arguments);
    }
}

int LeGrep::searchFile()
{
    if (file != NULL) {
        int total = 0;
        string str = this->pattern;
        if (this->ignoreCase) str = LeGrep::stringToUpper(str);

        for (int i = 0; i < file->size(); i++) {
            string line = file->getLine(i);
            if (this->ignoreCase) line = LeGrep::stringToUpper(line);

            vector<int> indexes = KMP(line, str);
            int found = indexes.size();

            if ((found > 0 && !this->invertMatch) ||
                (found == 0 && this->invertMatch))
                std::cout << i+1 << ":" << file->getLine(i) << "\n";

            total += found;
        }
        return total;
    }

    return -1;
}

int LeGrep::searchLine(string line)
{
    string str = this->pattern;
    if (this->ignoreCase) str = LeGrep::stringToUpper(str);

    string temp_line = line;
    if (this->ignoreCase) temp_line = LeGrep::stringToUpper(temp_line);

    vector<int> indexes = KMP(temp_line, str);
    int found = indexes.size();

    if ((found > 0 && !this->invertMatch) ||
        (found == 0 && this->invertMatch))
        std::cout << line << "\n";

    return found;
}

LeGrep::~LeGrep()
{
    delete(file);
}

void LeGrep::setArguments(vector<string> arguments)
{
    for (unsigned int i = 0; i < arguments.size(); i++) {
        if (arguments[i] == "-i" || arguments[i] == "--ignore-case")
            this->ignoreCase = true;
        if (arguments[i] == "-v" || arguments[i] == "--invert-match")
            this->invertMatch = true;
    }
}
