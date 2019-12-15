// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
using namespace std;

class Boggle {
public:
    Boggle(int sideLength, Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col) const;
    bool checkWord(string word) const;
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getHumanScore() const;
    int getComputerScore() const;
    int getDimension() const;
    HashSet<string> getFoundWords() const;
    string toString();
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    // TODO: add any other member functions/variables necessary
    // NOTE: Should create a field representing the current state
    // of the board, which would be most simply implemented by a 1D-array,
    // Though for functions like getLetter the meaning of row and column
    // is lost, so for the sake of readable code you could use a grid objects
    bool humanWordSearch(const string &word, Grid<bool> &isUsed, int row, int col, int p1, int p2);
    void computerWordSearch(Set<string> &result, string word, Grid<bool> &isUsed, int row, int col);

    int boggleDim;
    int humanScore;
    int computerScore;
    Lexicon dictionary;
    Grid<char> gameBoard; //want to dynamically allocate memory for internal rep
    HashSet<string> foundWords;
};

//Free function which allows user to enter the board text manually

#endif // _boggle_h
