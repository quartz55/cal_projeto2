#include "legrep.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void printHelp(char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [OPTION]... PATTERN [FILE]...\n";
        std::cout << "Try '" << argv[0] << " --help' for more information.\n";
        return 1;
    }

    // Show help screen
    if (string(argv[1]) == "--help") {
        printHelp(argv);
        return 1;
    }

    try {
        LeGrep grep(argc, argv);
    } catch (LeGrepException &e) {
        e.print();
        return 1;
    }

    return 0;
}

void printHelp(char *argv[])
{
    std::cout << "Usage: " << argv[0] << " [OPTION]... PATTERN [FILE]...\n";
    std::cout << "Search for PATTERN in each FILE or standard input.\n";
    std::cout << "PATTERN is, by default, a basic regular expression (BRE).\n";
    std::cout << "Example: " << argv[0] << " -i 'estrutura' text1.txt\n";

    std::cout << "\n";
    std::cout << "Regexp selection and interpretation:\n";
    std::cout << "  -i, --ignore-case"
              << "\t\tignore case distinctions"
              << "\n";

    std::cout << "\n";
    std::cout << "Miscellaneous:\n";
    std::cout << "  -v, --invert-match"
              << "\t\tselect non-matching lines"
              << "\n";

    std::cout << "\n";
    std::cout << "Context control:\n";
    std::cout << "  -B NUM, --before-context=NUM"
              << "\tprint NUM lines of leading context"
              << "\n"
              << "  -A NUM, --after-context=NUM"
              << "\tprint NUM lines of trailing context"
              << "\n";
}
