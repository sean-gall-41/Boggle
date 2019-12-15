#include "Boggle.h"
#include "bogglegui.h"
#include "random.h"
#include "shuffle.h"
#include "simpio.h"
#include <cmath>

const int BOGGLE_SIZE = 16;
const int BIG_BOGGLE_SIZE = 25;
const int ANIMATION_DELAY = 100;


// letters on all 6 sides of every cube
static string CUBES[BOGGLE_SIZE] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[BIG_BOGGLE_SIZE] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

//NOTE: For now, implement 4x4 regular Boggle :(
//TODO: Implement dynamica memory management
Boggle::Boggle(int sideLength, Lexicon& dictionary, string boardText) {
    boggleDim = (int)sideLength*sideLength;
    humanScore = 0;
    computerScore = 0;
    this->dictionary = dictionary;
    if (boardText.length() == 0) {
        for (int i = 0; i < boggleDim; i++) {
            if (boggleDim == BOGGLE_SIZE) {
                boardText += CUBES[i][randomInteger(0, 5)];
            } else if (boggleDim == BIG_BOGGLE_SIZE) {
                boardText += BIG_BOGGLE_CUBES[i][randomInteger(0, 5)];
            }
        }
        shuffle(boardText); //Second component of cube random init
    }
    gameBoard.resize(sideLength, sideLength); //no memory management :(
    // Rather than enter values into gameBoard in quadratic time this little
    // optimization does it in linear time :) Does not make a huge difference
    for (int i = 0; i < boggleDim; i++) {
        gameBoard[i / sideLength][i % sideLength] = boardText[i];
    }
}

char Boggle::getLetter(int row, int col) const {
    if (gameBoard.inBounds(row, col)) {
        return gameBoard[row][col];
    } else return 1; //Or throw a runtime error, whatever
}

bool Boggle::checkWord(string word) const {
    if (dictionary.contains(toLowerCase(word)) && word.length() >= 4
            && !foundWords.contains(toUpperCase(word))) {
        return true;
    } else return false;
}

bool Boggle::humanWordSearch(const string &word, Grid<bool> &isUsed, int row,
                             int col, int p1, int p2) {
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
    BoggleGUI::setHighlighted(row, col, true);
    if (gameBoard[row][col] == word[p1] && !isUsed[row][col]) {
        if (p1 >= p2) return true;
        isUsed[row][col] = true; //mark this position
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (gameBoard.inBounds(i, j) && !isUsed[i][j]
                        && humanWordSearch(word, isUsed, i, j, p1 + 1, p2)) {
                    return true;
                }
            }
        }
        isUsed[row][col] = false; //unmark this position
    }
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    Grid<bool> isUsed(gameBoard.numRows(), gameBoard.numCols(), false);
    if (checkWord(toLowerCase(word))) {
        for (int i = 0; i < gameBoard.numRows(); i++) {
            for (int j = 0; j < gameBoard.numCols(); j++) {
                if (humanWordSearch(toUpperCase(word), isUsed, i, j, 0,
                                    word.length() - 1)) {
                    foundWords.add(toUpperCase(word));
                    humanScore += word.length() - 3;
                    return true;
                }
            }
        }
    }
    return false;
}

int Boggle::getHumanScore() const {
    return humanScore;
}

void Boggle::computerWordSearch(Set<string> &result, string word,
                                Grid<bool> &isUsed, int row, int col) {
    if (checkWord(word)) {
        result.add(toUpperCase(word));
        foundWords.add(toUpperCase(word));
        computerScore += word.length() - 3;
    }
    isUsed[row][col] = true;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (gameBoard.inBounds(i, j) && !isUsed[i][j]
                    && dictionary.containsPrefix(word + gameBoard[i][j])) {
                computerWordSearch(result, word + gameBoard[i][j],
                                   isUsed, i, j);
                //cout << '\n' << isUsed.toString2D() << '\n' << endl;
            }
        }
    }
    isUsed[row][col] = false;
}

// return a set of all valid words which can be found by computer.
// a word is valid if it passes all the usual checks AND it is NOT
// a human word
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Grid<bool> isUsed(gameBoard.numRows(), gameBoard.numCols(), false);
    for (int i = 0; i < gameBoard.numRows(); i++) {
        for (int j = 0; j < gameBoard.numCols(); j++) {
            computerWordSearch(result, charToString(gameBoard[i][j]),
                               isUsed, i, j);
        }
    }
    return result;        // remove this
}

int Boggle::getComputerScore() const {
    return computerScore;
}

int Boggle::getDimension() const {
    return boggleDim;
}

HashSet<string> Boggle::getFoundWords() const {
    return foundWords;
}

string Boggle::toString() {
    string thisGameBoard = "";
    for (int i = 0; i < boggleDim; i++) {
        thisGameBoard += gameBoard[i / gameBoard.numRows()]
                                  [i % gameBoard.numCols()];
        if (i % gameBoard.numCols() == gameBoard.numCols() - 1) {
            thisGameBoard += '\n';
        }
    }
    return thisGameBoard;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    out << boggle.toString();
    return out;
}

