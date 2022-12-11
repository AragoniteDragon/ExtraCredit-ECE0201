#ifndef FUN_H
#define FUN_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;


class Function
{

    private:
        string format;
        string eq;
        char label;
        char *vars;
        int nvars;
        int *minterms;
        int *dcares;

    public:
        Function();
        Function(ifstream&);
        ~Function();
        int getNvars() const;
        int* getMinterms() const;
        int* getDcares() const;
        char* getVarNames() const;
        void setFormat(string);
        void setLabel(char);
        void printData(ofstream&);
        void prepareEq(vector<int*>);


};



#endif // FUN_H