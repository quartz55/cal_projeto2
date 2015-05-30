#ifndef LEGREP_H
#define LEGREP_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "lefile.h"

using namespace std;

class LeGrep
{
private:
    pair<bool, int> afterContext;
    pair<bool, int> beforeContext;
    bool ignoreCase;
    bool invertMatch;

    LeFile *file;
    string pattern;

    vector<int> KMP(string S, string K);
    void setArguments(vector<string> arguments);
public:
    LeGrep(string filename, string pattern, vector<string> *arguments);
    ~LeGrep();

    void printFile() const {file->print();};
    int searchFile();
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
