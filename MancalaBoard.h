#ifndef MANCALABOARD_H
#define MANCALABOARD_H

#include <string>

#define MANCALA_PIT_SLOTS   5
#define MANCALA_PLAYERS     2
#define MANCALA_TOTAL_PITS  (MANCALA_PLAYERS * MANCALA_PIT_SLOTS)

class CMancalaBoard{
    protected:  
        int DTurn;
        int DPits[MANCALA_TOTAL_PITS];
        int DStores[MANCALA_PLAYERS];
        
    public:
        CMancalaBoard();
        CMancalaBoard(int turn, const int pits[MANCALA_TOTAL_PITS], const int stores[MANCALA_PLAYERS]);
        
        void ResetBoard();
        
        int PlayerTurn() const;
        int PlayerScore(int player) const;
        int PitStoneCount(int player, int pit);
        bool GameOver() const;
        
        std::string ToString() const;
        operator std::string() const;
        
        bool Move(int player, int pit);
};


#endif
