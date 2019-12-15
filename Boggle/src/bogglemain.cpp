/*
 * CS 106B Boggle
 * This file contains the main program and user interface for running your
 * Boggle game.  We provide you a skeleton of this file that contains a shell
 * of the overall logic, but you must complete the playOneGame function.
 *
 * The playOneGame function talks to the Boggle class that you will write.
 * This file should contain all user interaction (cout / cin), while the Boggle
 * class should contain ALL game state such as the 16 letter cubes, the set of
 * words that have been formed, the algorithms for searching for words, etc.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Version: Sat 2014/02/05
 * Author : Marty Stepp
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "bogglegui.h"
using namespace std;

static const string DICTIONARY_FILE = "dictionary.txt";

// function prototype declarations
int boggleSizePrompt();
void playOneGame(Lexicon& dictionary, int rowDim);   // written by you, in boggleplay.cpp

int main() {
    cout << "Welcome to CS 106B Boggle!" << endl;
    cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
    cout << "The good news is that you might improve your vocabulary a bit." << endl;
    cout << "The bad news is that you're probably going to lose miserably to" << endl;
    cout << "this little dictionary-toting hunk of silicon." << endl;
    cout << "If only YOU had a gig of RAM!" << endl;
    cout << endl;
    Lexicon dictionary(DICTIONARY_FILE);
    getLine("Press Enter to begin the game ... ");
    // NOTE: obtain the dimensions of board outside multiple game loop because
    // 1.) BoggleGUI::close() function does not work as stated and details for
    // fixing are most likely in Java backend which is well beyond my current
    // expertise. The simple fix to ensure user doesn't break the game is to
    // obtain an initial and final value for the board dimensions for all
    // subsequent games within a single program execution. Thus, only way to
    // change gameboard dimensions is to terminate the program and restart.
    int rowDim = boggleSizePrompt();
    // play games repeatedly until user decides to quit
    while (true) {
        playOneGame(dictionary, rowDim);
        cout << endl;
        if (!getYesOrNo("Play again (Y/N)? ")) {
            break;
        }
    }

    cout << "Have a nice day." << endl;
    BoggleGUI::shutdown();
    return 0;
}

int boggleSizePrompt() {
    int rowDim = 0;
    while (true) {
        rowDim = getInteger("Enter the dimension of the Boggle game board (4 or 5): ");
        if (rowDim != 4 && rowDim != 5) {
            cerr << "Not a valid size for the game board. Did you actually read the prompt?" << endl;
        } else return rowDim;
    }
}
