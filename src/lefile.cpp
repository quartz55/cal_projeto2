#include "lefile.h"

#include <fstream>

LeFile::LeFile(string FILENAME)
{
    std::ifstream myfile(FILENAME.c_str());
    if(myfile.is_open()) {
        string line;
        while(getline(myfile, line))
            lines.push_back(line);
    }
    else throw FILENAME;
}
