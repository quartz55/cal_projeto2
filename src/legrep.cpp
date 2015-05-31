#include "legrep.h"

#include <iostream>
#include <sstream>

LeGrep::LeGrep(int argc, char *argv[])
{
    try {
        this->args = new LeGrepArguments(argc, argv);
    } catch (LeGrepException &e) {
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
                        if (j - bContext < 0) continue;
                        std::cout << j - bContext + 1 << ": "
                                  << args->files[i].getLine(j - bContext)
                                  << "\n";
                    }
                }

                // Show line
                std::cout << j + 1 << ": " << line << "\n";

                // Show position
                if (!args->invertMatch) {
                    std::stringstream helper;
                    helper << j + 1 << ": ";
                    string s(line.length() + helper.str().length(), ' ');
                    for (unsigned int k = 0; k < indexes.size(); k++)
                        s[indexes[k] + helper.str().length()] = '^';
                    std::cout << s << "\n";
                }

                // Lines after
                if (args->afterContext.first) {
                    int aContext = 1;
                    for (; aContext <= args->afterContext.second; ++aContext) {
                        if (j + aContext >= args->files[i].size()) break;
                        std::cout << j + aContext + 1 << ": "
                                  << args->files[i].getLine(j + aContext)
                                  << "\n";
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
    while (getline(cin, line)) searchLine(line);
}

int LeGrep::searchLine(string line)
{
    string str = args->pattern;
    if (args->ignoreCase) str = LeGrep::stringToUpper(str);

    string temp_line = line;
    if (args->ignoreCase) temp_line = LeGrep::stringToUpper(temp_line);

    vector<int> indexes = KMP(temp_line, str);
    int found = indexes.size();

    if ((found > 0 && !args->invertMatch) || (found == 0 && args->invertMatch))
        std::cout << line << "\n";

    return found;
}

string LeGrep::stringToUpper(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

vector<int> LeGrep::KMP(string S, string K)
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

LeGrep::~LeGrep() { delete (args); }

LeGrepArguments::LeGrepArguments(int argc, char **argv)
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
                break;

            case 'v':
                this->invertMatch = true;
                break;

            case 'A':
                this->afterContext.first = true;
                this->afterContext.second = atoi(optarg);
                break;

            case 'B':
                this->beforeContext.first = true;
                this->beforeContext.second = atoi(optarg);
                break;

            case '?':
                break;
        }
    }

    if (optind < argc) {
        this->pattern = string(argv[optind++]);
        while (optind < argc) {
            LeFile temp;
            try {
                temp = LeFile(argv[optind]);
            } catch (string &s) {
                throw LeGrepException("'" + s + "'", "file NOT found");
            }
            if (!temp.empty()) this->files.push_back(temp);

            ++optind;
        }
    } else
        throw LeGrepException("[PATTERN]", "cannot be null");
}
