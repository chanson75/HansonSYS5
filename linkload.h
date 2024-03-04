/********************************************************************
*** NAME : Carter Hanson    
*** CLASS : CSc 354 ***
*** ASSIGNMENT : Assignment 1
*** DUE DATE : 9-6-23
*** INSTRUCTOR : HAMER ***
*********************************************************************
*** DESCRIPTION : This header file contains the struct Node for the BST and the necessary functions to perform the desired operations for the symbol table
********************************************************************/
#ifndef SYMBOLS_H
#define SYMBOLS_H
#include<iostream>
#include <vector>

using namespace std;

class Symbols 
{
    public:

    /********************************************************************
    *** FUNCTION Symbols***
    *********************************************************************
    *** DESCRIPTION : Default constructor: initializes root to nullptr ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : None ***
    ********************************************************************/
        Symbols();
     
         /********************************************************************
    *** FUNCTION passOne***
    *********************************************************************
    *** DESCRIPTION : does pass one with processing of text records ***
    *** INPUT ARGS : filepath ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void passOne(string& filePath, int i);

    /********************************************************************
    *** FUNCTION passTwo***
    *********************************************************************
    *** DESCRIPTION : performs pass two  ***
    *** INPUT ARGS : filepath ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void passTwo (string& filePath);

    /********************************************************************
    *** FUNCTION initializeArray***
    *********************************************************************
    *** DESCRIPTION : initialize array to ?? ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void initializeArray();

    /********************************************************************
    *** FUNCTION displayArray***
    *********************************************************************
    *** DESCRIPTION : prints array to monitor and MEMORY ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void displayArray();

    /********************************************************************
    *** FUNCTION processHeader***
    *********************************************************************
    *** DESCRIPTION : processes header records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void processHeader(string& line, int i);

    /********************************************************************
    *** FUNCTION processD***
    *********************************************************************
    *** DESCRIPTION : processes D records ***
    *** INPUT ARGS : string& line, int i) ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void processD(string& line, int i);

    /********************************************************************
    *** FUNCTION processText***
    *********************************************************************
    *** DESCRIPTION : processes text records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void processText (string& line, int i);

    /********************************************************************
    *** FUNCTION processModif***
    *********************************************************************
    *** DESCRIPTION : processes modification records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void processModif (string& line, int i);

    /********************************************************************
    *** FUNCTION printVects***
    *********************************************************************
    *** DESCRIPTION : prints the external symbol table ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void printVects();

    /********************************************************************
    *** FUNCTION getSymbPos***
    *********************************************************************
    *** DESCRIPTION : returns position of the symbol in the table ***
    *** INPUT ARGS : expression ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : int ***
    ********************************************************************/
        int getSymbPos (string& expression);

    /********************************************************************
    *** FUNCTION storeText***
    *********************************************************************
    *** DESCRIPTION : stores text into memory ***
    *** INPUT ARGS : addr, bytes ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
        void storeText (int addr, string& bytes);

        void processE (string& line, int i);

        string memoryMap[50] [16];

        int startingPos = 0x2200;

        int firstInstr = 0x0;

        vector <string> csectVect;

        vector <string> symbolVect;

        vector <string> allSymbols;

        vector <int> addrVect;

        vector <int> lengthVect;

        vector <int> csaddrs;

        int prevLength = 0x00;

        int prevCsaddr = 0x00;

    private:
        void insert(string& csect, string& symbol, int addr, int length, int i);
};

#endif