#include "Game.h"
#include "GameController.h"
#include "LinkedList.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>

#define EXIT_SUCCESS 0

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::istringstream;

string promptUser();
void startNewGame(bool colourEnabled, bool symbolsEnabled);
bool loadGame(bool colourEnabled, bool symbolsEnabled);
void showCredits();
void terminateGame();

// Split inputString into tokens based on the locations of delimiter and return
// them as a vector
std::vector<string> splitString(string inputString, string delimiter);

// Validate that a colour and shape are valid Tile colours and shapes
bool validateTile(char colour, int shape);

// Returns user input as std::string which can be parsed as int, where required,
// using string stream (as demonstrated in menu)
string promptUser() {
    string input = "";
    cout << "> ";
    // cin >> input;
    std::getline(std::cin, input);
    return input;
}

void startNewGame(bool colourEnabled, bool symbolsEnabled) {
    cout << "Starting a New Game" << endl << endl;
    
    // TODO prompt for how many players there should be

    GameController* theGame = new GameController(2, colourEnabled,
                                                 symbolsEnabled);
    cout << "Let's Play!" << endl;
    theGame->gameStart();
    theGame->gameLoop();
    delete theGame;
}

bool loadGame(bool colourEnabled, bool symbolsEnabled) {
    // Get the filename from the user
    cout << "Enter the filename from which to load a game" << endl;
    string filename = promptUser();

    bool success = false;
    std::vector<string> lines;

    // Attempt to read the file
    try {
        std::ifstream inFile;
        inFile.open(filename);

        // Check that the file exists and store the contents of the file
        if (inFile.good()) {
            // Store each line of the file in the vector
            while (!inFile.eof()) {
            string currLine;
            std::getline(inFile, currLine);
            lines.push_back(currLine);
            }
        }

        inFile.close();
    } catch (...) {
        // Error while reading file, success remains false
    }

    // Check that the format of the file is correct
    const int linesPerPlayer = 3;
    const int gameStateLines = 4;

    // Verify the file had enough lines in it as a sanity check
    // TODO make this handle multiple players
    int noPlayers = 2;
    if (lines.size() >= (linesPerPlayer
                         * (unsigned int) noPlayers) + gameStateLines) {
        try {
            bool formatIsValid = true;

            Player* players[noPlayers];
            for (int i = 0; i < noPlayers; ++i) {
                players[i] = nullptr;
            }

            // Create players
            for (int i = 0; i < noPlayers; ++i) {
                string name = lines.at((i * linesPerPlayer));
                players[i] = new Player(name);

                // Set player's score
                int score = std::stoi(lines.at((i * linesPerPlayer) + 1));
                players[i]->setScore(score);

                // Fill player's hand
                std::vector<string> tileStrings =
                    splitString(lines.at((i * linesPerPlayer) + 2), ",");
                
                for (unsigned int j = 0; j < tileStrings.size(); ++j) {
                    char colour = tileStrings.at(j).at(0);
                    int shape = std::stoi(tileStrings.at(j).substr(1, 1));

                    // If the colour/shape is invalid, the tile will still be
                    // added to the player's hand, however the format of the
                    // file will be considered invalid
                    formatIsValid = formatIsValid &&
                                    validateTile(colour, shape);

                    Tile* t = new Tile(colour, shape);
                    players[i]->addToHand(t);
                    delete t;
                }
            }

            // Create board
            Board board;
            bool firstTurn = false;

            // Add tiles to board
            int boardTilesLine = (linesPerPlayer * noPlayers) + 1;
            std::vector<string> placedTiles =
                splitString(lines.at(boardTilesLine), ", ");

            for (unsigned int i = 0; i < placedTiles.size(); ++i) {
                std::vector<string> tileAndPos = splitString(placedTiles.at(i),
                                                             "@");
                if (tileAndPos.size() != 2) {
                    // Format is invalid, unless the board is empty in which
                    // case there is only an empty string in the board-tile line
                    if (!(placedTiles.size() == 1 && placedTiles.at(0) == "")) {
                        formatIsValid = false;
                    } else {
                        // Board is empty, so the next move is the first one
                        firstTurn = true;
                    }
                } else {
                    char tileColour = tileAndPos.at(0).at(0);
                    int tileShape = std::stoi(tileAndPos.at(0).substr(1, 1));
                    Tile* t = new Tile(tileColour, tileShape);

                    // Mark file format as invalid if tile is invalid
                    formatIsValid = formatIsValid &&
                                    validateTile(tileColour, tileShape);

                    // Add tile to board
                    char tileRow = tileAndPos.at(1).at(0);
                    int tileCol = std::stoi(tileAndPos.at(1).substr(1));

                    board.placeTile(*t, tileRow, tileCol);
                    delete t;
                }
            }

            // Create tile bag
            std::vector<string> bagTiles =
                splitString(lines.at((linesPerPlayer * noPlayers) + 2), ",");

            LinkedList tileList;
            for (unsigned int i = 0; i < bagTiles.size(); ++i) {
                string tileString = bagTiles.at(i);

                if (tileString.size() != 2) {
                    // An empty tile bag is considered valid
                    if (!(bagTiles.size() == 1 && bagTiles.at(0) == "")) {
                        formatIsValid = false;
                    }
                } else {
                    char tileColour = tileString.at(0);
                    int tileShape = std::stoi(tileString.substr(1, 1));
                    Tile* t = new Tile(tileColour, tileShape);

                    // Mark file format as invalid if tile is invalid
                    formatIsValid = formatIsValid &&
                                    validateTile(tileColour, tileShape);

                    tileList.addBack(t);
                    delete t;
                }
            }

            // Store current player
            string currPlayerName =
                lines.at((linesPerPlayer * noPlayers) + 3);

            // Determine which player is the current player
            int currPlayerNo = -1;
            for (int i = 0; i < noPlayers; ++i) {
                Player* p = players[i];
                if (p != nullptr && p->getName() == currPlayerName) {
                    currPlayerNo = i;
                }
            }

            if (currPlayerNo < 0 || currPlayerNo >= noPlayers) {
                formatIsValid = false;
            }

            // Create gameController & game, then begin the game if the file
            // was a valid save
            if (formatIsValid) {
                success = true;

                cout << "Qwirkle game successfully loaded" << endl;
                GameController* theGame = new GameController(players,
                                                             noPlayers,
                                                             board,
                                                             tileList,
                                                             currPlayerNo,
                                                             firstTurn,
                                                             colourEnabled,
                                                             symbolsEnabled);
                theGame->gameLoop();
                delete theGame;
            } else {
                cout << "The file loaded is not a valid Qwirkle save." << endl;
            }

            // Clean up memory
            for (int i = 0; i < noPlayers; ++i) {
                if (players[i] != nullptr) {
                    delete players[i];
                }
            }

        } catch (...) {
            // Error occurred while constructing game, success remains false
            cout << "An error occurred while loading the game." << endl;
        }
    }

    return success;
}

void terminationMessage() {
    cout << "Goodbye" << endl;
}

// bit ugly but who cares its just credits ¯\_(ツ)_/¯
void showCredits() {
    cout << "-------------------------------------" << endl;
    cout << "Name: Ahmad Seiam Farighi" << endl << "Student ID: s3842662"
         << endl << "Email: s3842662@student.rmit.edu.au" << endl << endl;

    cout << "Name: Daniel Marmion" << endl << "Student ID: s3842912" << endl
         << "Email: s3842912@student.rmit.edu.au" << endl << endl;

    cout << "Name: Richard Forsey" << endl << "Student ID: s3857811" << endl
         << "Email: s3857811@student.rmit.edu.au" << endl << endl;

    cout << "Name: Aaron Fisher" << endl << "Student ID: s3840619" << endl
         << "Email: s3840619@student.rmit.edu.au" << endl;
    cout << "-------------------------------------" << endl << endl;
}

int main(void) {
    cout << "Welcome to Quirkle!" << endl << "-------------------" << endl;
    atexit(terminationMessage);

    // Enhancement toggles
    bool colourEnabled = true;
    bool symbolsEnabled = true;

    bool shouldDisplayMenu = true;
    do {
        cout << "Menu" << endl << "----" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Load Game" << endl;
        cout << "3. Credits (Show student information)" << endl;
        cout << "4. Quit" << endl;
        cout << "5. Enable/disable coloured printing" << endl;
        cout << "6. Enable/disable unicode tile symbols" << endl << endl;

        
        istringstream iss (promptUser());
        int selection = 0;
        iss >> selection;

        cout << endl;

        if (iss.fail()) {
            if (iss.eof()) {
                shouldDisplayMenu = false;
            } else {
                cout << "Invalid Input. Please enter a number from 1-4."
                     << endl << endl;
            }
        } else {
            if (selection == 1) {
                startNewGame(colourEnabled, symbolsEnabled);

                // assumes menu should not repeat after game is completed
                shouldDisplayMenu = false;       
            } else if (selection == 2) {
                loadGame(colourEnabled, symbolsEnabled);

                // assumes menu should not repeat after game is completed
                shouldDisplayMenu = false;
            } else if (selection == 3) {
                showCredits();
            } else if (selection == 4) {
                shouldDisplayMenu = false;
            } else if (selection == 5) {
                colourEnabled = !colourEnabled;

                cout << "Coloured printing "
                     << (colourEnabled ? "enabled" : "disabled")
                     << "." << endl << endl;
            } else if (selection == 6) {
                symbolsEnabled = !symbolsEnabled;

                cout << "Unicode tile symbols "
                     << (symbolsEnabled ? "enabled" : "disabled")
                     << "." << endl << endl;
            } else {
                cout << "Sorry, that isn't an option. "
                     << "Please enter a number from 1-4." << endl;
            }
        }
    } while (shouldDisplayMenu);

    LinkedList* list = new LinkedList();
    delete list;

    return EXIT_SUCCESS;
}

std::vector<string> splitString(string inputString, string delimiter) {
    std::vector<string> tokens;

    // Test whether or not there is at least one occurence of delimiter in the
    // string
    if (inputString.find(delimiter) != string::npos) {
        int startpos = 0;
        while (inputString.find(delimiter, startpos) != string::npos) {
            int delimiterLoc = inputString.find(delimiter, startpos);
            int tokenLength = delimiterLoc - startpos;

            tokens.push_back(inputString.substr(startpos, tokenLength));

            startpos = delimiterLoc + delimiter.size();
        }

        // Account for the remaining substring after the last delimiter
        tokens.push_back(inputString.substr(startpos));
    } else {
        // Delimiter is not in string, so add only the original string to the
        // vector
        tokens.push_back(inputString);
    }

    return tokens;
}

bool validateTile(char colour, int shape) {
    char colorsArray[NUM_COLOURS] {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

    bool colourIsValid = false;
    for (char c : colorsArray) {
        if (colour == c) {
            colourIsValid = true;
        }
    }

    bool shapeIsValid = shape >= CIRCLE && shape <= CLOVER;

    return colourIsValid && shapeIsValid;
}