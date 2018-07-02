// Aaron Liszt
// user.h
// Header definition for user
// User objects will be used by multiple classes in the game

#include "user.h"

User* User::getInstance()
{
    if(player_user == NULL)
        player_user = new User(); // lazy singleton
    return player_user;

    //player_user is will be available throughout the lifetime 
    // of the program. 
}
