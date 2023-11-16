#include <iostream>
#include <string>

#include "tree.hpp"

using namespace ilrd;

int main(int argc, char *argv[])
{
    if(1 < argc)
    {
        PrintTree(argv[1]);
    }
    else
    {
        PrintTree();
    }

    return 0;
}