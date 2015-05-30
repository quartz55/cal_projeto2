#include "legrep.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [OPTION]... PATTERN [FILE]\n";
        return 1;
    }

    LeGrep grep(argc, argv);

    return 0;
}
