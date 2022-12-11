#include "Function.h"

// Default constructor for the Function object
Function::Function()
{

    format = "";
    nvars = 0;
    vars = nullptr;
    minterms = nullptr;
    dcares = nullptr;
    eq = "";
}

// Overloaded constructor for the Function object that builds the function from an ifstream
Function::Function(ifstream &datIn)
{
    
    string read;
    getline(datIn, read);
    setFormat(read);
    eq = "";
}

// Destructor for the Function object
Function::~Function()
{

    delete[] minterms;
    delete[] vars;
    delete[] dcares;
}

// Accessor method for the nVars function
int Function::getNvars() const
{ return nvars; }

// Accessor method for the minterms pointer
int* Function::getMinterms() const
{ return minterms; }

// Accessor method for the dont-care values pointer
int* Function::getDcares() const
{ return dcares; }

// Takes in a string representing the Function and reads the function data from it, assigning it to the member variables
void Function::setFormat(string form)
{

    // Defining the variables necessary for formatting the data
    string split, dat, check;
    vector<char> alph;
    format = form;
    size_t loc = format.find_first_of("(");
    size_t loc2 = format.find_first_of(")");
    check = format.substr(0, loc);

    // Checking the label to make sure it's a alphabetical character
    if(isalpha(check[0]))
    { setLabel(check[0]); } else { cout << "ERROR! Invalid input file format.\nThe function label must be an alphabetical character!"; exit(-1); }
    dat = format.substr(loc + 1);
    size_t loc3 = format.find_first_of(",");
    int counter = 0;
    
    // Loop through the string, finding each comma separation and extracting the variable
    while(loc3 < loc2)
    {

        dat = format.substr(loc + 1, loc3);
        split = format.substr(loc3);

        if(isalpha(dat[0]))
        { alph.push_back(dat[0]); } else { cout << "ERROR! Invalid input file format.\nVariable " << counter + 1 << " must be an alphabetical character, not " << dat[0]; exit(-1); }
        counter++;
        loc3 = format.find_first_of(",", loc3);
        loc = format.find_first_of(" ", loc + 1);
        if(dat[1] == ')') break;
    }
    // cout << "Final count is " << counter << "characters\n"; DEBUG CODE

    // Reserving the memory for the arrays
    nvars = counter;
    vars = new char[nvars];
    minterms = new int[((int) pow(2, nvars))];
    dcares = new int[((int) pow(2, nvars))];

    for(int i = 0; i < nvars; i++)
    {
        /*
        auto it = alph.begin();
        vars[i] = *(it++);
        */
        vars[i] = toupper(alph[i]);
        // cout << "Pushed variable " << vars[i] << " from value " << alph[i] << endl; DEBUG CODE
    } 

    loc = format.find_first_of("(", loc3);
    loc2 = format.find_first_of(")", loc);
    check = format.substr(loc - 1, loc);
    if(check[0] != 'm')
    {  cout << "ERROR! Invalid input file format. No minterms were defined. " << check[0]; exit(-1); }
    counter = 0;
    
    loc3 = format.find_first_of(",", loc);

    for(int i = 0; i < pow(2, nvars); i++)
    {
        minterms[i] = 0;
    }
    // Repeating the above process but for the minterms
    while(true)
    {

        dat = format.substr(loc + 1, loc3);
        if(!(isdigit(dat[0]))) break;
        // cout << "CHECKING DATA " << dat << endl; DEBUG CODE
        cout << "Minterm " << stoi(dat) << " is true!" << endl;
        minterms[stoi(dat)] = 1;
        loc = format.find_first_of(" ", loc + 1);
        loc3 = format.find_first_of(",", loc);
    }

    // cout << "Finished loop 2!\n"; DEBUG CODE
    

    for(int i = 0; i < pow(2, nvars); i++)
    {
        dcares[i] = 0;
    }
    // Checking for dont care conditions
    size_t loc4 = format.find_first_of("d", loc);
    if(loc4 != string::npos)
    {
        cout << "Found dcares! at Loc4: " << loc4 << " being " << format.substr(loc4) << endl;
        loc = format.find_first_of("(", loc4);
        loc2 = format.find_first_of(")", loc4);
        loc3 = format.find_first_of(",", loc4);
        while(loc3 < loc2)
        {

            if(loc3 == string::npos) break;
            dat = format.substr(loc + 1, loc3);
            cout << "Checking loop substr " << dat << endl;
            dcares[stoi(dat)] = 1;
            cout << "DC " << stoi(dat) << " is true!" << endl;
            loc3 = format.find_first_of(",", loc3 + 1);
            loc = format.find_first_of(" ", loc + 1);
            if(loc3 == string::npos) break;

        }
        dat = format.substr(loc + 1, loc2);
        cout << "Checking substr " << dat << endl;
        cout << "DC " << stoi(dat) << " is true!" << endl;
        dcares[stoi(dat)] = 1;


    } else { cout << "Couldn't find any dont care conditions!\nContinuing with no d conditions!\n"; }

    for(int i = 0; i < pow(2, nvars); i++)
            if(dcares[i] != 1) dcares[i] = 0;



}

// Sets the function label
void Function::setLabel(char lab)
{ label = tolower(lab); }

// Prints the function data to a file
void Function::printData(ofstream &datOut)
{

    datOut << format << endl;
    datOut << "Variables are: ";
    for(int i = 0; i < nvars; i++)
        datOut << vars[i] << ", ";
    datOut << endl;

    datOut << "Minterms are: ";
    for(int i = 0; i < pow(2, nvars); i++)
        datOut << minterms[i] << ", ";
    datOut << endl;

    datOut << "DCAres are: ";
    for(int i = 0; i < pow(2, nvars); i++)
        datOut << dcares[i] << ", ";
    datOut << endl;
    datOut << "Final Equation: ";
}

// Returns the array of variable names
char* Function::getVarNames() const
{ return vars; }

// Turns the final array of variables into a string representation of the equation
void Function::prepareEq(vector<int*> fvars)
{

    eq = "f(";
    for(int i = 0; i < nvars - 1; i++)
    {

        eq += "" + to_string(vars[i]) + ", ";
    }
    eq += "" + to_string(vars[nvars - 1]) + ") = ";

    int sz =  fvars.size();
    for(int i = 0; i < sz; i++)
    {

        for(int j = 0; j < nvars; j++)
        {

            if(fvars[i][j] != 3)
            {

                eq += vars[j];
                if(fvars[i][j] == 0)
                    eq += "'";
            }
        }
        if(i == sz - 1)
            eq += " + ";
    }
}
