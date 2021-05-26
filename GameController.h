#ifndef ASSIGN2_GAMECONTROLLER_H
#define ASSIGN2_GAMECONTROLLER_H

#include "Game.h"

class GameController {
private:
    Game* game;
    int pCount;
    bool keepGoing;

    // Enhancement toggles
    bool colourEnabled;
    bool symbolsEnabled;

public:
    GameController(int playerCount, bool colourEnabled, bool symbolsEnabled);
    GameController(Player** players, int playerCount, Board& board, LinkedList& tileBag,
                   int currentPlayerNo, bool firstTurn, bool colourEnabled,
                   bool symbolsEnabled);
    ~GameController();

    void addPlayer();
    void gameStart();
    void gameLoop();
    void skipFirstTurn();

    string askForPlayerMove();
    bool validateAndExecute(string input);

    bool makeAMove(string tileSTR, string moveSTR);
    bool replaceATile(string tileSTR);

    bool validate_Place(string input);
    bool validate_Replace(string input);
    bool validate_PlayerName(string input);
    bool validate_save(std::vector<std::string>& input);
    void printScoreBoardHand();

};

#endif // ASSIGN2_GAMECONTROLLER_H