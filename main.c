#include "raylib.h"

int main()
{
    int option;
    
    option = menu(); // function call and return depending on user's choice
    
    if(option == PLAY)
        // call game function / loop
        
    if(option == LEADERBOARD)
        // call leaderboard function (with binary file display)
        
    if(option == EXIT)
        return 0; // not sure if this call is right
}