#ifndef LEFILE_H
#define LEFILE_H

#include <string>
#include <vector>
#include <iostream>

using std::vector; using std::string;

class LeFile
{
private:
    vector<string> lines;

public:
    LeFile(string FILENAME);
    void print() const {
        for (unsigned int i = 0; i < lines.size(); i++)
            std::cout << lines[i] << "\n";
    }
    string getLine(int i) const {return lines[i];}
    int size() const {return lines.size();}
};

#endif /* LEFILE_H */
