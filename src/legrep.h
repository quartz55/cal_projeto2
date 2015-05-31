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
    /**
     * Constructor
     * Reads arguments provided and if there are any files specified
     * it will 'grep' the files, if not it will read input from STDIN
     *
     * @param argc Number of arguments
     * @param argv Arguments
     */
    LeGrep(int argc, char *argv[]);
    ~LeGrep();

    /**
     * Reads lines from input and 'greps' them until EOF (CTRL+D) is reached
     */
    void readInput();
    /**
     * 'Greps' each file (in args->files) outputting the line number and
     *matching
     * location in STDOUT
     *
     * @return Number of occurrences
     */
    int searchFiles();
    /**
     * 'Greps' a line (provided from STDIN) and outputs the same line to STDOUT
     * if the 'grep' was successful
     *
     * @param line Line to 'grep'
     *
     * @return Number of occurrences
     */
    int searchLine(string line);

    /**
     * Converts a string to uppercase (for ignore-case argument)
     *
     * @param str String to convert to uppercase
     *
     * @return Converted string
     */
    static string stringToUpper(string str);
};

#endif /* LEGREP_H */
