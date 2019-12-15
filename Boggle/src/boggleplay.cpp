/*
 * boggleplay.cpp
 *
 *
 */

#include <string>
#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"

//int boggleSizePrompt();
string getBoardText(int rowDim);
void printHumanData(Boggle& game, string message);
void playHumanTurn(Boggle& game);
void printComputerData(Boggle& game, string message);
void playComputerTurn(Boggle& game);
void setUpGUI(int boardDim);
void fillGameBoardGUI(Boggle &game, string &boardText, int boardDim);
void announceWinner(Boggle &game);

void playOneGame(Lexicon& dictionary, int rowDim) {
    //int rowDim = boggleSizePrompt();
    setUpGUI(rowDim); //used to initialize AND reset GUI
    string boardText = getBoardText(rowDim);
    Boggle gameBoard = Boggle(rowDim, dictionary, boardText);
    fillGameBoardGUI(gameBoard, boardText, rowDim);
    playHumanTurn(gameBoard);
    playComputerTurn(gameBoard);
    announceWinner(gameBoard);
}

//int boggleSizePrompt() {
//    int rowDim = 0;
//    while (true) {
//        rowDim = getInteger("Enter the dimension of the Boggle game board (4 or 5): ");
//        if (rowDim != 4 && rowDim != 5) {
//            cerr << "Not a valid size for the game board. Did you actually read the prompt?" << endl;
//        } else return rowDim;
//    }
//}

//TODO: Must make compatible with either normal boggle or big boggle!
string getBoardText(int rowDim) {
    if(getYesOrNo("Do you want to generate a random board? ")) {
        return "";
    } else {
        int boardSize = (int)rowDim*rowDim;
        string trialInput = "";
        while (true) {
            trialInput = getLine("Type the " + integerToString(boardSize) + " letters to appear on the board\n(NOTE: 'Q' is not allowed in this version of Boggle): ");
            if (trialInput.length() != boardSize) {
                std::cerr << "That is not a valid " + integerToString(boardSize) + "-letter board string. Try again." << std::endl;
            } else {
                bool good = true;
                int i = 0;
                while (good && i < boardSize) {
                    if (!isalpha(trialInput[i]) || toupper(trialInput[i]) == 'Q') {
                        good = false;
                    }
                    i++;
                }
                if (good) {
                    break;
                } else {
                    std::cerr << "That is not a valid " + integerToString(boardSize) + "-letter board string. Try again." << std::endl;
                }
            }
        }
        return toUpperCase(trialInput);
    }
}

void printHumanData(Boggle& game, string message) {
    clearConsole();
    cout << message << endl << game << endl
         << "Your words (" << game.getFoundWords().size() << "): " << game.getFoundWords()
         << endl << "Your score: " << game.getHumanScore() << endl;
}

void playHumanTurn(Boggle& game) {
    printHumanData(game, "It's your turn!");
    BoggleGUI::setStatusMessage("It's your turn!");
    string trialWord = toLowerCase(getLine("Type a word (or Enter to stop): "));
    while (trialWord != "") {
        if (!game.checkWord(trialWord)) {
            printHumanData(game, "You must enter an unfound 4+ letter word from the dictionary.");
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        } else if (game.humanWordSearch(trialWord)) {
            printHumanData(game, "You found a new word! \"" + toUpperCase(trialWord) + "\"");
            BoggleGUI::setStatusMessage("You found a new word! \"" + toUpperCase(trialWord) + "\"");
            BoggleGUI::recordWord(toLowerCase(trialWord), BoggleGUI::HUMAN);
            BoggleGUI::setScore(game.getHumanScore(), BoggleGUI::HUMAN);
        } else if (trialWord != "") {
            printHumanData(game, "That word can't be formed on this board");
            BoggleGUI::setStatusMessage("That word can't be formed on this board");
        }
        trialWord = toLowerCase(getLine("Type a word (or Enter to stop): "));
    }
}

void printComputerData(Boggle& game, string message) {
    BoggleGUI::clearHighlighting();
    Set<string> computerWords = game.computerWordSearch();
    cout << message << endl << "My words (" << computerWords.size() << "): "
         << computerWords << endl << "My score: " << game.getComputerScore()
         << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    for (string word : computerWords) {
        BoggleGUI::recordWord(toLowerCase(word), BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(game.getComputerScore(), BoggleGUI::COMPUTER);
}

void playComputerTurn(Boggle& game) {
    cout << endl;
    printComputerData(game, "It's my turn!");
}

void setUpGUI(int boardDim) {
    if (!BoggleGUI::isInitialized()) {
        BoggleGUI::initialize(boardDim, boardDim);
    } else {
        //BoggleGUI::close(); <-- doesn't work like graphics lib says??
        BoggleGUI::reset();
        //BoggleGUI::initialize(boardDim, boardDim);

    }
}

// FIXME: This is the part of the code that breaks when we try to chnge
// board size while running (perhaps define a REDRAWGRID method to help
// facillitate user's ability to choose gameboard size on the fly
void fillGameBoardGUI(Boggle &game, string &boardText, int boardDim) {
    if (boardText != "") {
        BoggleGUI::labelAllCubes(toUpperCase(boardText));
    } else {
        for (int i = 0; i < boardDim; i++) {
            for (int j = 0; j < boardDim; j++) {
                BoggleGUI::labelCube(i, j, game.getLetter(i, j), false);
            }
        }
    }
}

void announceWinner(Boggle &game) {
    if (game.getComputerScore() > game.getHumanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!\n";
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations! (I guess)\n";
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations! (I guess)");
    }
}
