/********************************************************************
*** NAME : Carter Hanson
*** CLASS : CSc 354 ***
*** ASSIGNMENT : Assignment 1
*** DUE DATE : 9-6-23
*** INSTRUCTOR : HAMER ***
*********************************************************************
*** DESCRIPTION : Building a symbol table from symbols.dat of valid symbols and running tests from test.dat to compare against entries to the table
********************************************************************/
#include <iostream>
#include <iomanip>
#include "linkload.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    Symbols tree;

    if (argc > 1)
    {
        string names [10];

        int counter = 0;

        while (argv[counter] != nullptr)
        {
            counter++;
        }

        for (int i = 1; i < counter; i++)
        {
            names[i - 1] = argv[i];
        }

        tree.initializeArray();

        for (int i = 0; i < counter; i++)
        {
            string file = names[i];

            tree.passOne(file, i);
        }

        for (int i = 0; i < counter; i++)
        {
            string file = names[i];

            tree.prevCsaddr = tree.csaddrs[i];

            tree.passTwo(file);
        }
    }
    else
    {
        cout << "Error: not enough arguments." << endl;
    }

    cout << setw(6) << left << "CSECT" << setw(10) << right << "SYMBOL" << setw(5) << right << "ADDR" << setw(8) << right << "LENGTH" << endl << endl;

    tree.printVects();

    cout << endl << endl;

    tree.displayArray();

    cout << endl;

    cout << "Execution starts at address: " << tree.firstInstr + 0x2200 << endl;

    return 0;
}