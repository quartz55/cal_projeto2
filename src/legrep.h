#ifndef LEGREP_H
#define LEGREP_H

#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>

#include "lefile.h"

using namespace std;

class LeGrepException
{
    string arg;
    string message;

   public:
    LeGrepException(string arg, string message) : arg(arg), message(message) {}
    void print()
    {
        std::cerr << "\n#LE_GREP_EXCEPTION# " << arg << " " << message << "\n";
    }
};

struct LeGrepArguments {
    pair<bool, int> afterContext;
    pair<bool, int> beforeContext;
    bool ignoreCase;
    bool invertMatch;
    string pattern;
    vector<LeFile> files;

    LeGrepArguments(int argc, char *argv[]);
};

class LeGrep
{
   private:
    LeGrepArguments *args;

    vector<int> KMP(string S, string K);

   public:
    LeGrep(int argc, char *argv[]);
    ~LeGrep();

    void readInput();
    int searchFiles();
    int searchLine(string line);

    static string stringToUpper(string str);
};

#endif /* LEGREP_H */
