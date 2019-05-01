#include "MancalaBoard.h"
#include <iostream>

CMancalaBoard::CMancalaBoard(){
    DTurn = 0;
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = 4;   
    }
    for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
        DStores[Index] = 0;
    }
}

CMancalaBoard::CMancalaBoard(int turn, const int pits[MANCALA_TOTAL_PITS], const int stores[MANCALA_PLAYERS]){
    DTurn = turn;
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = pits[Index];   
    }
    for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
        DStores[Index] = stores[Index];
    }
}

void CMancalaBoard::ResetBoard(){
    for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
        DPits[Index] = 4;   
    }
}

int CMancalaBoard::PlayerTurn() const{
    return DTurn;
}

int CMancalaBoard::PlayerScore(int player) const{
    return DStores[player];
}

int CMancalaBoard::PitStoneCount(int player, int pit){
    return DPits[player * MANCALA_PIT_SLOTS + pit]; 
}

bool CMancalaBoard::GameOver() const{
    for(int Index = 0; MANCALA_TOTAL_PITS; Index++){
        if(DPits[Index]){
            return false;   
        }
    }
    return true;
}

std::string CMancalaBoard::ToString() const{
    std::string ReturnString  = "P1          PITS\n";
    ReturnString += "      5   4   3   2   1\n";
    ReturnString += "/---------------------------\\\n";
    ReturnString += "|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[Index] >= 10){
            ReturnString += std::to_string(DPits[Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";    
        }
    }
    ReturnString += "   |\n|";
    
    if(DStores[0] > 10){
        ReturnString += std::to_string(DStores[0]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[0]);
    }
    ReturnString += " |-------------------|";
    if(DStores[1] >= 10){
        ReturnString += std::to_string(DStores[1]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[1]);
    }
    ReturnString += " |\n|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
            ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";    
        }
    }
    ReturnString += "   |\n";
    ReturnString += "\\---------------------------/\n";
    ReturnString += "      1   2   3   4   5\n";
    ReturnString += "             PITS          P2\n";
                
    return ReturnString;
}

CMancalaBoard::operator std::string() const{
    std::string ReturnString  = "P1          PITS\n";
    ReturnString += "      5   4   3   2   1\n";
    ReturnString += "/---------------------------\\\n";
    ReturnString += "|   |";
    for(int Index = MANCALA_PIT_SLOTS-1; Index >= 0; Index--){
        if(DPits[Index] >= 10){
            ReturnString += std::to_string(DPits[Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";    
        }
    }
    ReturnString += "   |\n|";
    
    if(DStores[0] >= 10){
        ReturnString += std::to_string(DStores[0]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[0]);
    }
    ReturnString += " |-------------------|";
    if(DStores[1] > 10){
        ReturnString += std::to_string(DStores[1]);
    }
    else{
        ReturnString += std::string(" ") + std::to_string(DStores[1]);
    }
    ReturnString += " |\n|   |";
    for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
        if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
            ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
        }
        else{
            ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";    
        }
    }
    ReturnString += "   |\n";
    ReturnString += "\\---------------------------/\n";
    ReturnString += "      1   2   3   4   5\n";
    ReturnString += "             PITS          P2\n";
                
    return ReturnString;
}

bool CMancalaBoard::Move(int player, int pit){
    int PitIndex = player * MANCALA_PIT_SLOTS + pit;
    
    if((PitIndex < 0) or (PitIndex > MANCALA_TOTAL_PITS)){
        return false;    
    }
    int Stones = DPits[PitIndex];
    int LastPitDrop = PitIndex;
    DPits[PitIndex] = 0;
    //std::cout<<"@ line "<<__LINE__<<" "<<Stones<<std::endl;
    while(Stones > 1){
        PitIndex++;
        if((PitIndex % MANCALA_PIT_SLOTS) == 0){
            if(player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
                DStores[player]++;
                Stones--;
            }
        }
        PitIndex %= MANCALA_TOTAL_PITS;
        if(Stones){
            DPits[PitIndex]++;
            Stones--;
            LastPitDrop = PitIndex;
        }
    }
    PitIndex++;
    PitIndex %= MANCALA_TOTAL_PITS;
    if(DPits[PitIndex] == 0){
        //std::cout<<"@ line "<<__LINE__<<std::endl;
        int OppositeSide = MANCALA_TOTAL_PITS - 1  - PitIndex;
        DStores[player] += DPits[OppositeSide] + 1;
        DPits[OppositeSide] = 0;
    }
    else if(Stones == 1){
        if((PitIndex % MANCALA_PIT_SLOTS) == 0){
            if(player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
                DStores[player]++;
                Stones--;
            }
        }
        PitIndex %= MANCALA_TOTAL_PITS;
        if(Stones){
            DPits[PitIndex]++;
            Stones--;
            LastPitDrop = PitIndex;
        }
    }
    if((LastPitDrop == MANCALA_PIT_SLOTS) or (PitIndex != MANCALA_PIT_SLOTS)){
        DTurn = 1 - DTurn;
    }
    return true;
}
