#ifndef LEFILE_H
#define LEFILE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::vector;
using std::string;

class LeFile
{
   private:
    string name;
    vector<string> lines;

   public:
    LeFile() : name("") {}
    LeFile(string FILENAME);
    string getName() const { return name; }
    void print() const
    {
        for (unsigned int i = 0; i < lines.size(); i++)
            std::cout << lines[i] << "\n";
    }
    string getLine(int i) const { return lines[i]; }
    int size() const { return lines.size(); }
    bool empty() const { return lines.empty() && name != ""; }
};

inline LeFile::LeFile(string FILENAME)
{
    std::ifstream myfile(FILENAME.c_str());
    if (myfile.is_open()) {
        this->name = FILENAME;
        string line;
        while (getline(myfile, line)) lines.push_back(line);
    } else
        throw FILENAME;
}

#endif /* LEFILE_H */
