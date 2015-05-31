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

    try {
        LeGrep grep(argc, argv);
    }
    catch (LeGrepException &e)
    {
        e.print();
        return 1;
    }

    return 0;
}
