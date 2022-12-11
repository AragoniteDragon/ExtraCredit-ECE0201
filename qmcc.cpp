#include <iostream>
#include <cmath>
#include <fstream>
#include <limits.h>
#include "Function.cpp"

void convertBinary(int, int*);
vector<int> padBinary(int, int, int*);
string constructString(int, int*);
string qmca(Function*);
int compareBytes(int*, int*, int, int);
int convertValue(int*);
vector<int*> prepareMinterms(int**);

using namespace std;

int varSize;

int main()
{
    
    ifstream datIn("function.txt");
    ofstream datOut("Function Info.txt");
    Function fx(datIn);
    cout << "Finished defining functions\n";
    varSize = fx.getNvars();
    cout << "Set global variable to " << varSize << endl;
    cout << "Prepared global variable\n";
    string eq = qmca(&fx);
    cout << "Organized pointers\n";
    fx.printData(datOut);
    datOut << eq;
    datIn.close();
    datOut.close();
    return 0;
}

// Takes in a given integer value and returns a pointer to an array of integers that represent the value in binary form
void convertBinary(int val, int* result)
{

    int size = 1 + floor(log2(val));
    int tracker = val;
    int prev;
    for (int i = 0; i < size; i++)
    {

        prev = tracker;
        tracker /= 2;
        if(tracker * 2 == prev)
        { result[size - i - 1] = 0; } else { result[size - i - 1] = 1; }
    }
}

// Overloaded convertBinary function that pads the result array with the given number of zeros
vector<int> padBinary(int val, int pad, int* result)
{
    // convertBinary(val, result);
    // cout << "Done mini binary conversion!\n";
    int size = 1;
    if(val != 0) 
        size = 1 + floor(log2(val));
    if(size > pad)
    { cout << "ERROR! Padding is less than the binary size!"; exit(-1); }
    vector<int> converted;
    int converter[size];
    int tracker = val;
    for(int i = 0; i < size; i++)
    {
        converter[i] = tracker % 2;
        tracker /= 2;
    }
    for(int i = 0; i < pad; i++)
    {
        
        if(i < size)
        { converted.push_back(converter[i]); }
        else
        { converted.push_back(0); }
    }
    return converted;
}

// Takes in the 2d-array of possible minterms and outputs a vector containing only the minterms and dont-cares
vector<int*> prepareMinterms(int** terms)
{

    vector<int*> minterms;
    for(int i = 0; i < pow(2, varSize); i++)
    {
        
        if(terms[i][0] == 1 || terms[i][0] == 2)
        { minterms.push_back(terms[i]); cout << "\nMinterm " << i << " is important!\n"; for(int j = 0; j < varSize + 1; j++) cout << minterms[i][j]; }
    }
    return minterms;
}

// Takes in a pointer to an array of integers that represent a binary number and returns a string representation of it
string constructString(int size, int* dat)
{

    string str = "";
    for (int i = size - 1; i >= 0; i--)
        str += to_string(*(dat + i));
    return str;
}

// Takes in a Function object and returns an array of pointers to arrays representing the binary possible minterms of the given Function
string qmca(Function *fx)
{
    vector<int*> minterms;
    int needed[((int) pow(2, varSize))];
    int** result = new int*[((int) pow(2, varSize))];
    const int* const locmterms = fx->getMinterms();
    const int* const locdcar = fx->getDcares();
    cout << "Preparing the organize pointers loop\n";
    int mterms[((int) pow(2, varSize))];
    int dcar[((int) pow(2, varSize))];
    for (int i = 0; i < ((int) pow(2, varSize)); i++) 
        mterms[i] = *(locmterms + i);
    for (int i = 0; i < ((int) pow(2, varSize)); i++) 
        dcar[i] = *(locdcar + i);

    for(int i = 0; i < pow(2, varSize); i++)
    {
        int* inf = new int[varSize];
        //cout << "Preparing binary conversion\n";
        //cout << "Padding binary with i = " << i << " padding = " << size << endl;
        vector<int> vect = padBinary(i, varSize, inf);
        int* compare = new int[vect.size()];
        copy(vect.begin(), vect.end(), compare);
        //cout << "Converted " << constructString(size, compare) << endl;
        int* value = new int[varSize + 1];
        for(int j = 1; j <= varSize; j++)
            value[j] = compare[j - 1];
        if(mterms[i] == 1) { value[0] = 1; } else if(dcar[i] == 1) { value[0] = 2; } else value[0] = 0;
        result[i] = value;
        //cout << "Final export value is ";
        //for(int i = 1; i <= size + 1; i++) cout << value[size + 1 - i];
        //cout << endl;
        delete[] compare;
        if(value[0] == 1 || value[0] == 2)
        {
            if(value[0] == 2)
                value[0] = -i;
            else
            { value[0] = i; needed[i] = 1; } 
            minterms.push_back(value);
             
            cout << "Minterm " << i << " is important!\n"; 
            for(int j = 1; j <= varSize + 1; j++) 
                cout << value[varSize + 1 - j]; 
            cout << endl; 
            
        }
        
    }

    cout << "Finished organizing\n";


    // STEP 1: Create a finite array copy of the vector to keep track of combined minterms
    // When you remove values from a vector, the index of the rest of the values changes, so an array is created instead
    // This can also be used to sort the arrays by the number of '1' values in them
    int size = minterms.size();
    int index[size][varSize + 1];
    int terms[varSize + 1][size][varSize + 1];
    int cter;
    int track[((int) pow(2, varSize))];
    
    vector<vector<vector<int>>> fullGroup;
    vector<vector<int>> finalTerms;
    cout << "Obtained " << size << " minterms!\n";
    for(int i = 0; i < varSize + 1; i++)
    { vector<vector<int>> ngroup; fullGroup.push_back(ngroup); }

    for(int i = 0; i < size; i++)
    {
        cter = 0;
        vector<int> indiv;
        cout << "Pushed back ";
        for(int j = 0; j < varSize + 1; j++)
        {

            indiv.push_back(minterms[i][j]);
            if(minterms[i][j] == 1)
                cter++;
            cout << minterms[i][j];
        }
        if(minterms[i][0] == 1)
        { cter--; track[i] = 0; }
        else
            track[i] = 2;
        fullGroup[cter].push_back(indiv);
        cout << " into group " << cter << endl;
        
            
    }
    cout << "Finalized sizes:\n";
    for(int j = 0; j < fullGroup.size(); j++)
        cout << "Group " << j << " has a size of " << fullGroup[j].size() << endl;


    // STEP 2: Begin comparing the values against eachother
    cout << "Starting step 2\n";
    bool done = false;
    vector<vector<vector<int>>> fullCopy;
    for(int i = 0; i < varSize + 1; i++)
    { vector<vector<int>> ngroup; fullCopy.push_back(ngroup); }
    bool atAll = true;
    int loop = 0;
    while(atAll)
    {

        cout << "Beginning loop" << loop << endl;
        atAll = false;
        if(loop != 0)
        {

            // Reset the trackers
            for(int b = 0; b < pow(2, varSize); b++)
            { track[b] = 0; }
            cout << "Finished resetting trackers" << endl;

            // At the start of each loop, clear the variables used previously and move in the combined variables created
            // previously. Doing so requires us to also clear the combined variables created previously to make room for the new variables 
            for(int x = 0; x < varSize; x++)
            {
                if(fullGroup[x].size() > 0)
                {

                    cout << "Clearing variables " << x << endl;
                    fullGroup[x].clear();
                    cout << "Clearing variables completed " << x << endl;
                    if(fullCopy[x].size() > 0)
                    {

                        for(int y = 0; y < fullCopy[x].size(); y++)
                        { cout << "Pushing back x = " << x << " y = " << y << " is " << constructString(fullCopy[x][y].size(), fullCopy[x][y].data()) << endl; fullGroup[x].push_back(fullCopy[x][y]); }
                    }
                    
                    cout << "Full copy also cleared\n";
                    fullCopy[x].clear();
                }
            }
        }

        for(int i = 0; i < fullGroup.size(); i++)
        {

            
            int csize = fullGroup[i].size();
            int nsize = fullGroup[i + 1].size();
            int ind = pow(2, loop);
            if(csize > 0)
            {
                
                if(nsize > 0)
                {

                    for(int j = 0; j < csize; j++)
                    {

                        bool matched = false;
                        for(int n = 0; n < nsize; n++)
                        {
                            cout << "Beginning comparison of i = " << i << " j = " << j << endl;
                            int comp = compareBytes(fullGroup[i][j].data(), fullGroup[i + 1][n].data(), ind, ind);
                            if(comp > 0)
                            {
                                
                                cout << "Found something at comp = " << comp << endl;
                                matched = true;
                                atAll = true;
                                vector<int> cop;
                                for(int k = 0; k < fullGroup[i][j].size(); k++)
                                    cop.push_back(fullGroup[i][j][k]);
                                cop[comp] = 3;
                                for(int m = 0; m < ind; m++)
                                { cout << "Beginning for loop\n"; cop.insert(cop.begin() + ind, fullGroup[i + 1][n][m]); }
                                cout << "Compared " << constructString(fullGroup[i][j].size(), fullGroup[i][j].data()) << " with " << constructString(fullGroup[i + 1][n].size(), fullGroup[i + 1][n].data()) << " and got " << comp << endl;
                                cout << "New value is " << constructString(cop.size(), cop.data()) << endl;
                                int ct = 0;
                                int found = -1;
                                for(int t = 0; t < fullCopy[i].size(); t++)
                                {
                                    int loc = 0;
                                    for(int s = 0; s < 4; s++)
                                    { cout << "Comparing " << fullCopy[i][t][fullCopy[i][t].size() - 1 - s] << " and " << cop[cop.size() - 1 - s] << endl; if(fullCopy[i][t][fullCopy[i][t].size() - 1 - s] == cop[cop.size() - 1 - s]) loc++; }
                                    //int com = compareBytes(cop.data(), fullCopy[i][t].data(), cop.size() - varSize, fullCopy[i][t].size() - varSize);
                                    if(loc == 4)
                                    { ct++; found = t; }
                                }
                                if(ct == 0)
                                    fullCopy[i].push_back(cop);
                                else cout << "The term is a duplicate and not necessary! " << constructString(cop.size(), cop.data()) << " and " << constructString(fullCopy[i][found].size(), fullCopy[i][found].data()) << endl;
                                for(int m = 0; m < ind * 2; m++)
                                { track[abs(cop[m])] = cop[m]; cout << "Minterm " << cop[m] << " has been used\n"; }                                
                            }
                            // Hey future me... This is where we need to continue working on. We just changed the LSB of each minterm into an
                            // integer representation of which minterm it is, with it being negative if the minterm is a dont care condition,
                            // I'm thinking we should be able to just track how many times we have looped to use a pow(2, loop) to calculate
                            // how many variables we are working with replacing, and just modify each vector by making them larger as we go
                            // just appending all the minterms onto the beginning of the vector. This way, we can match the minterms without
                            // having to go through the headache of figuring out which minterms have been matched and which haven't

                        }
                        if(matched == false)
                        { 
                            int sum = 0;
                            for(int m = 0; m < ind; m++)
                            {
                                cout << "Looking at " << track[abs(fullGroup[i][j][m])] << endl;
                                if((track[abs(fullGroup[i][j][m])] == 0) && (fullGroup[i][j][m] > 0))
                                    sum++;
                            }
                            if(sum > 0)
                            {

                                finalTerms.push_back(fullGroup[i][j]); cout << "Term i = " << i << " j = " << j << " could not be matched and is essential " << constructString(fullGroup[i][j].size(), fullGroup[i][j].data()) << endl; 
                            } else { cout << "Term i = " << i << " j = " << j << " could not be matched but isn't essential" << endl; }
                            
                            
                        }

                    }
                } else
                {
                    
                    int fsize = fullGroup[varSize - loop].size();
                    if(fsize > 0)
                    {
                        cout << "Running last minute check on last group " << varSize - loop << endl;
                        for(int z = 0; z < fsize; z++)
                        {
                            int sum = 0;
                            for(int m = 0; m < ind; m++)
                            {
                                cout << "Looking at " << track[abs(fullGroup[varSize - loop][z][m])] << endl;
                                if((track[abs(fullGroup[varSize - loop][z][m])] == 0) && (fullGroup[varSize - loop][z][m] > 0))
                                    sum++;
                            }
                            if(sum > 0)
                            {
                                
                                int ct = 0;
                                int found = -1;
                                for(int t = 0; t < finalTerms.size(); t++)
                                {
                                    int loc = 0;
                                    for(int s = 0; s < 4; s++)
                                    { cout << "Comparing " << finalTerms[t][finalTerms[t].size() - 1 - s] << " and " << fullGroup[varSize - loop][z][fullGroup[varSize - loop][z].size() - 1 - s] << endl; if(finalTerms[t][finalTerms[t].size() - 1 - s] == fullGroup[varSize - loop][z][fullGroup[varSize - loop][z].size() - 1 - s]) loc++; }
                                    //int com = compareBytes(cop.data(), fullCopy[i][t].data(), cop.size() - varSize, fullCopy[i][t].size() - varSize);
                                    if(loc == 4)
                                    { ct++; found = t; }
                                }
                                if(ct == 0)
                                { finalTerms.push_back(fullGroup[varSize - loop][z]); cout << "Term i = " << varSize - loop << " j = " << z << " could not be matched and is essential " << constructString(fullGroup[varSize - loop][z].size(), fullGroup[varSize - loop][z].data()) << endl; }
                                else cout << "The term is a duplicate and not necessary! " << constructString(fullGroup[varSize - loop][z].size(), fullGroup[varSize - loop][z].data()) << endl;
                                
                                
                            } else { cout << "Term i = " << varSize - loop << " j = " << z << " could not be matched but isn't essential" << constructString(fsize, fullGroup[varSize - loop][z].data()) << endl; }
                        }
                    }
                    break;
                }
                
                
            }
                
                
        }
        cout << "It is currently atAll = " << atAll << endl;
        loop++;
    }
    
    for(int j = 0; j < finalTerms.size(); j++)
    {
        cout << "Final term " << constructString(finalTerms[j].size(), finalTerms[j].data()) << " remains\n";
    }
    
    // Final step, extract the final terms
    vector<vector<int>> implicants;

    // Extract the essential prime implicants
    cout << "Preparing to extract the essential prime implicants\n";
    for(int i = 0; i < pow(2, varSize); i++)
    {
        int count = 0;
        int spot = -1;
        if(needed[i] == 1)
        {

            cout << i << " is needed!\n";
            for(int j = 0; j < finalTerms.size(); j++)
            {

                //cout << "looking through " << constructString(finalTerms[j].size(), finalTerms[j].data()) << endl;
                for(int k = 0; k < finalTerms[j].size() - varSize; k++)
                {
                    //cout << "Checking at j = " << j << " and k = " << k << " of " << finalTerms[j][k] << endl;
                    if(finalTerms[j][k] == i)
                    { count++; spot = j; }
                }
            }

            if(count == 1)
            {
                implicants.push_back(finalTerms[spot]);
                for(int z = 0; z < finalTerms[spot].size() - varSize; z++)
                { needed[finalTerms[spot][z]] = 0; cout << "Minterm " << finalTerms[spot][z] << " is checked off by " << constructString(finalTerms[spot].size(), finalTerms[spot].data()) << endl; }
                finalTerms.erase(finalTerms.begin() + spot);
                cout << "Value " << i << " is an essential prime implicant" << endl;
            }

        }
    }

    // Obtain the remaining minterms in the most efficient way possible
    bool fin = false;
    while(!(fin))
    {
        int max = INT_MIN;
        int loc = -1;
        int sum = 0;
        if(finalTerms.size() > 0)
        {

            for(int i = 0; i < finalTerms.size(); i++)
            {

                sum = 0;
                for(int k = 0; k < finalTerms[i].size() - varSize; k++)
                {

                    if(needed[finalTerms[i][k]] == 1)
                        sum++;
                }
                if(sum > max)
                {
                    max = sum;
                    loc = i;
                }
            }
        }
        
        if(max == 0)
        {

            fin = true;
        } else
        {

            implicants.push_back(finalTerms[loc]);
            for(int z = 0; z < finalTerms[loc].size() - varSize; z++)
            { needed[finalTerms[loc][z]] = 0; cout << "Minterm " << finalTerms[loc][z] << " is checked off by " << constructString(finalTerms[loc].size(), finalTerms[loc].data()) << endl; }
            finalTerms.erase(finalTerms.begin() + loc);

        }
    }

    for(vector<int> thing : implicants)
        cout << "Implicant: " << constructString(thing.size(), thing.data()) << endl;

    // Convert the final implicants and print them out
    string eq = "f(";
    int nvars = varSize;
    char *vars = fx->getVarNames();
    for(int i = 0; i < nvars - 1; i++)
    {
        eq.push_back(vars[i]);
        eq += ", ";
    }
    eq.push_back(vars[nvars - 1]);
    eq += ") = ";

    int sz = implicants.size();
    for(int i = 0; i < sz; i++)
    {

        int which = 0;
        for(int j = implicants[i].size() - 1; j > implicants[i].size() - varSize - 1; j--)
        {
            
            if(implicants[i][j] != 3)
            {

                eq.push_back(vars[which]);
                if(implicants[i][j] == 0)
                    eq += "'";
            }
            which++;    
        }
        if(i != sz - 1)
            eq += " + ";
    }
    cout << eq << endl;
    return eq;
}

// Takes in two pointers to arrays representing binary minterms, and returns the index of the negligible term (in the first minterm), 
// returning -1 if there isn't one and -2 if there is more than one
int compareBytes(int* A, int* B, int indexA, int indexB)
{

    int found = -1;
    for(int i = 0; i < varSize; i++)
    {

        if(A[i + indexA] != B[i + indexB])
        {
            if(found == -1)
            { found = i + indexA; } else { found = -2; break; }
        }
    }
    return found;
}


// Take in an array representing a binary value and returns its decimal representation
int convertValue(int* val)
{

    int size = varSize;
    int sum = 0;
    for(int i = 0; i < size; i++)
    {

        sum += pow(2, i) * val[i + 1];
    }
    return sum;
}
