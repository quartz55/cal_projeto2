#ifndef LEGREP_H
#define LEGREP_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <getopt.h>

#include "lefile.h"

using namespace std;

class LeGrepException
{
    string arg;
    string message;

public:
    LeGrepException(string arg, string message){
        this->arg = arg;
        this->message = message;
    }
    void print(){
        std::cerr << "#LE_GREP_EXCEPTION# " << arg << " " << message << "\n";
    }

};

struct LeGrepArguments
{
    pair<bool, int> afterContext;
    pair<bool, int> beforeContext;
    bool ignoreCase;
    bool invertMatch;
    string pattern;
    vector<LeFile> files;

    LeGrepArguments(int argc, char *argv[]);
};

inline LeGrepArguments::LeGrepArguments(int argc, char **argv)
{
    int c;

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"ignore-case", no_argument, 0, 'i'},
            {"invert-match", no_argument, 0, 'v'},
            {"after-context", required_argument, 0, 'A'},
            {"before-context", required_argument, 0, 'B'},
            {0, 0, 0, 0}};

        c = getopt_long(argc, argv, "ivA:B:", long_options, &option_index);
        if (c == -1) break;

        switch (c) {
            case 'i':
                this->ignoreCase = true;
                std::cout << "Ignore case\n";
                break;

            case 'v':
                this->invertMatch = true;
                std::cout << "Invert match\n";
                break;

            case 'A':
                this->afterContext.first = true;
                this->afterContext.second = atoi(optarg);
                std::cout << "After context with " << optarg << " lines\n";
                break;

            case 'B':
                this->beforeContext.first = true;
                this->beforeContext.second = atoi(optarg);
                std::cout << "Before context with " << optarg << " lines\n";
                break;

            case '?':
                break;
        }
    }

    if (optind < argc) {
        this->pattern = string(argv[optind++]);
        std::cout << "Pattern: " << this->pattern << "\n";
        while (optind < argc){
            std::cout << argv[optind] << " ";
            try{
                this->files.push_back(LeFile(argv[optind]));
            }
            catch (string &s) {
                throw LeGrepException("[FILE]", s);
            }
            ++optind;
        }
        std::cout << "\n";
    }
    else throw LeGrepException("[PATTERN]", "cannot be null");
}

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

inline string LeGrep::stringToUpper(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline vector<int> LeGrep::KMP(string S, string K)
{
    vector<int> T(K.size() + 1, -1);
    vector<int> matches;

    if (K.size() == 0) {
        matches.push_back(0);
        return matches;
    }
    for (size_t i = 1; i <= K.size(); i++) {
        int pos = T[i - 1];
        while (pos != -1 && K[pos] != K[i - 1]) pos = T[pos];
        T[i] = pos + 1;
    }

    int sp = 0;
    int kp = 0;
    while (sp < (int)S.size()) {
        while (kp != -1 && (kp == (int)K.size() || K[kp] != S[sp])) kp = T[kp];
        kp++;
        sp++;
        if (kp == (int)K.size()) matches.push_back(sp - K.size());
    }

    return matches;
}


#endif /* LEGREP_H */
