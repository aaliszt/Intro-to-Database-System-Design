// Aaron Liszt
// user.h
// Header definition for user
// User objects will be used by multiple classes in the game

#ifndef USER
#define USER

#include <string>

class User{
private:
    static User* player_user;
    User() { username = ""; loggedIn = false; } 			//private constructor
	std::string username;
	bool loggedIn;
public:
    static User* getInstance();
    bool isLoggedIn() { return loggedIn; }
	void Login() { loggedIn = true; }
    void setUsername(std::string uname) { username = uname; }
    std::string getUsername() { return username; }
};

#endif
