#include "linkload.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Symbols::Symbols() {}

         /********************************************************************
    *** FUNCTION passOne***
    *********************************************************************
    *** DESCRIPTION : does pass one with processing of text records ***
    *** INPUT ARGS : filepath ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/
void Symbols::passOne(string& filePath, int i) 
{
    ifstream inFile;

    char charLine [1000];

    inFile.open(filePath);

    if (!inFile)
    {
        //cout << "Error opening " << filePath << endl;
        return;
    }

    while (inFile.getline(charLine, 1000, '\n'))
    {
        string line (charLine);

        if (line[0] == 'H')
        {
            processHeader(line, i);

            continue;
        }

        else if (line[0] == 'D')
        {
            processD (line, i);

            continue;
        }

        else if (line[0] == 'T')
        {
            processText (line, i);

            i++;

            continue;
        }
    }

    inFile.close();
}

    /********************************************************************
    *** FUNCTION passTwo***
    *********************************************************************
    *** DESCRIPTION : performs pass two  ***
    *** INPUT ARGS : filepath ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::passTwo (string& filePath)
{
    ifstream inFile;

    char charLine [1000];

    inFile.open(filePath);

    int i;

    if (!inFile)
    {
        //cout << "Error opening " << filePath << endl;
        return;
    }

    while (inFile.getline(charLine, 1000, '\n'))
    {
        string line (charLine);

        if (line[0] == 'M')
        {
            processModif(line, i);

            i++;

            continue;
        }

        else if (line[0] == 'E')
        {
            processE (line, i);

            continue;
        }
    }

    inFile.close();
}

    /********************************************************************
    *** FUNCTION processHeader***
    *********************************************************************
    *** DESCRIPTION : processes header records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::processE(string& line, int i)
{
    if (line.length() > 1)
    {
        line = line.substr(1, line.length());

        firstInstr = stoi(line, nullptr, 16);
    }
}

void Symbols::processHeader(string& line, int i)
{
    char charProg [30];

    char charNums [13];

    int csaddr = 0x00;

    sscanf(line.c_str(), "H%s %s", charProg, charNums);

    string strProg (charProg);

    string strNums (charNums);

    string strEnd = strNums.substr (6, strNums.length());

    int end = stoi(strEnd, nullptr, 16);

    string empty = "";

    if (lengthVect.size() == 0)
    {
        csaddr = startingPos;
    }
    else
    {
        csaddr = prevCsaddr + prevLength;
    }

    insert(strProg, empty, csaddr, end, i);

    prevCsaddr = csaddr;

    csaddrs.push_back(csaddr);

    prevLength = end;
}

    /********************************************************************
    *** FUNCTION processD***
    *********************************************************************
    *** DESCRIPTION : processes D records ***
    *** INPUT ARGS : string& line, int i) ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::processD(string& line, int i)
{
    char one [12], two [12], three[12], four [12], five [12], six[12];

    string empty = "";

    string symbol, tempVal;

    int counter = 1, val = 0x00; 

    while (counter < line.size())
    {
        symbol = line.substr(counter, 6);

        tempVal = line.substr(counter + 6, 6);

        counter += 12;

        val = stoi (tempVal, nullptr, 16);

        val += prevCsaddr;

        symbol.erase(remove_if(symbol.begin(), symbol.end(), ::isspace), symbol.end());

        insert (empty, symbol, val, 0, i);
    }   
}

    /********************************************************************
    *** FUNCTION processText***
    *********************************************************************
    *** DESCRIPTION : processes text records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::processText(string& line, int i)
{
    line = line.substr(1, line.length());

    int textRecCounter = 0x00;

    string startAdd = line.substr(0, 6);

    //string strLen = line.substr(6, 2);

    string textRecord = line.substr (8, line.length());

    int start = stoi (startAdd, nullptr, 16);

    //int length = stoi (strLen, nullptr, 16);

    start += prevCsaddr;

    storeText(start, textRecord);
}

    /********************************************************************
    *** FUNCTION processModif***
    *********************************************************************
    *** DESCRIPTION : processes modification records ***
    *** INPUT ARGS : string& line, int i ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::processModif (string& line, int i)
{
    int position = 0;

    int modifNums = 0;

    string oldBytes;

    line = line.substr (1, line.length());

    string startAdd = line.substr (0, 6);

    string length = line.substr (6, 2);

    string modifExpr = line.substr(8, line.length());

    int start = stoi (startAdd, nullptr, 16);

    position = getSymbPos (modifExpr);

    start += prevCsaddr;

    int row = (start - 0x2200) / 0x10;

    int col = (start - 0x2200) % 0x10;

    for (int i = 0; i < 3; i++)
    {
        oldBytes += memoryMap[row] [col];

        if (col == 0xF)
        {
            row++;

            col = 0;
        }
        else
        {
            col++;
        }
    }

    replace(oldBytes.begin(), oldBytes.end(), '?', '0');

    int oldBytesNum = stoi (oldBytes, nullptr, 16);

    if (modifExpr[0] == '+')
    {
        oldBytesNum += addrVect [position];
    }
    else if (modifExpr[0] == '-')
    {
        oldBytesNum -= addrVect [position];
    }

    ostringstream stream;

    stream << hex << uppercase << oldBytesNum;

    string newBytes = stream.str();

    if (newBytes.length() == 4)
    {
        newBytes = "00" + newBytes;
    }
    else if (newBytes.length() == 1)
    {
        newBytes = "00000" + newBytes;
    }
    else if (newBytes.length() == 2)
    {
        newBytes = "0000" + newBytes;
    }
    else if (newBytes.length() == 8)
    {
        newBytes = newBytes.substr (2, newBytes.length());
    }
    else if (newBytes.length() == 7)
    {
        newBytes = newBytes.substr(1, newBytes.length());
    }

    storeText (start, newBytes);
}

    /********************************************************************
    *** FUNCTION getSymbPos***
    *********************************************************************
    *** DESCRIPTION : returns position of the symbol in the table ***
    *** INPUT ARGS : expression ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : int ***
    ********************************************************************/

int Symbols::getSymbPos(string& expression) 
{
    char sign;

    sign = expression[0];

    expression = expression.substr(1, expression.length());
    
    int position = 0;

    for (int i = 0; i < allSymbols.size(); i++)
    {
        if (allSymbols[i] == expression)
        {
            position = i;

            break;
        }
    }

    expression = sign + expression;

    return position;
}

    /********************************************************************
    *** FUNCTION storeText***
    *********************************************************************
    *** DESCRIPTION : stores text into memory ***
    *** INPUT ARGS : addr, bytes ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::storeText(int addr, string& bytes)
{
    for (size_t i = 0; i < bytes.length(); i += 2)
    {
        string byte = bytes.substr(i, 2);

        int row = (addr - 0x2200) / 0x10;

        int col = (addr - 0x2200) % 0x10;

        memoryMap[row][col] = byte;

        addr++;
    }
}

    /********************************************************************
    *** FUNCTION initializeArray***
    *********************************************************************
    *** DESCRIPTION : initialize array to ?? ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::initializeArray()
{
    for (int r = 0; r < 50; r++)
    {
        for (int c = 0; c < 16; c++)
        {
            memoryMap[r][c] = "??";
        }
    }
}

    /********************************************************************
    *** FUNCTION displayArray***
    *********************************************************************
    *** DESCRIPTION : prints array to monitor and MEMORY ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::displayArray()
{
    int location = 0x2200;

    ofstream outFile;

    outFile.open("MEMORY");

    if (!outFile)
    {
        cout << "ERROR OPENING MEMORY" << endl;
        return;
    }

    cout << "       ";
    outFile << "       ";

    for (int i = 0x0; i < 0x10; i++)
    {
        cout << hex << uppercase << i << "  ";
        outFile << hex << uppercase << i << "  ";
    }

    cout << endl;
    outFile << endl;

    for (int r = 0; r < 50; r++)
    {
        cout << hex << uppercase << location << ": ";
        outFile << hex << uppercase << location << ": ";

        for (int c = 0; c < 16; c++)
        {
            if (memoryMap [r][c].length() == 1)
            {
                memoryMap[r][c] = "0" + memoryMap[r][c];
            }

            cout << uppercase << memoryMap [r][c] << " ";
            outFile << uppercase << memoryMap [r][c] << " ";
        }

        location += 0x10;

        cout << endl;
        outFile << endl;
    }
}

void Symbols::insert(string& csect, string& symbol, int addr, int length, int i) 
{
    csectVect.push_back(csect);

    symbolVect.push_back(symbol);

    addrVect.push_back(addr);

    lengthVect.push_back(length);

    if (csect == "")
    {
        allSymbols.push_back(symbol);
    }
    else if (symbol == "")
    {
        allSymbols.push_back(csect);
    }
}

    /********************************************************************
    *** FUNCTION printVects***
    *********************************************************************
    *** DESCRIPTION : prints the external symbol table ***
    *** INPUT ARGS : none ***
    *** OUTPUT ARGS : none ***
    *** IN/OUT ARGS :  none***
    *** RETURN : void ***
    ********************************************************************/

void Symbols::printVects() 
{
    for (int i = 0; i < csectVect.size(); i++)
    {
        if (addrVect[i] == 0)
        {
            cout << setw(7) << left << csectVect[i] << setw(8) << right << symbolVect[i] << setw(6) << right << hex << uppercase << "" << setw(8) << right << hex << uppercase << lengthVect[i] <<endl;

        }

        else if (lengthVect[i] == 0)
        {
            cout << setw(7) << left << csectVect[i] << setw(8) << right << symbolVect[i] << setw(6) << right << hex << uppercase << addrVect[i] << setw(8) << right << hex << uppercase << "" <<endl;
        }
        else 
        {
            cout << setw(7) << left << csectVect[i] << setw(8) << right << symbolVect[i] << setw(6) << right << hex << uppercase << addrVect[i] << setw(8) << right << hex << uppercase << lengthVect[i] <<endl;
        }
    }
}