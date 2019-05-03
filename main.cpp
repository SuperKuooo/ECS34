#include <iostream>
#include <cctype>
#include "MancalaBoard.h"

int main(int argc, char *argv[]){
    CMancalaBoard MancalaBoard;
    std::string InputLine;
    int PitNumber;
    bool Done = false;
    
    while(!Done){
        MancalaBoard.ResetBoard();
        while(!MancalaBoard.GameOver()){
            std::cout<<std::string(MancalaBoard)<<std::endl;
            std::cout<<"-------------------"<<std::endl;
            std::cout<<"Player "<<(MancalaBoard.PlayerTurn() + 1)<<" select a pit> ";
            std::cin>>PitNumber;
            while(!MancalaBoard.Move(MancalaBoard.PlayerTurn(), PitNumber-1)){
                std::cout<<"Invalid move player "<<(MancalaBoard.PlayerTurn() + 1)<<" please select another pit> ";
                std::cin>>PitNumber;
            }
        }
        std::cout<<"Game over, ";
        if(MancalaBoard.PlayerScore(0) < MancalaBoard.PlayerScore(1)){
            std::cout<<"player 2 wins "<<MancalaBoard.PlayerScore(1)<<" to "<<MancalaBoard.PlayerScore(0)<<std::endl;
        }
        else if(MancalaBoard.PlayerScore(0) > MancalaBoard.PlayerScore(1)){
            std::cout<<"player 1 wins "<<MancalaBoard.PlayerScore(0)<<" to "<<MancalaBoard.PlayerScore(1)<<std::endl;
        }
        else{
            std::cout<<"tie game "<<MancalaBoard.PlayerScore(0)<<" to "<<MancalaBoard.PlayerScore(1)<<std::endl;
        }

        InputLine.clear();
        while(InputLine.empty()){
            std::cout<<"Play again? ";
            std::getline(std::cin, InputLine);
            if(!InputLine.empty() and ((tolower(InputLine[0]) == 'y') or (tolower(InputLine[0]) == 'n'))){
                Done = tolower(InputLine[0]) == 'n';
                break;
            }
        }
        
    }
    
    
    return EXIT_SUCCESS;
}
