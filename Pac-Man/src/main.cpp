#include <Engine/CNCurses.h>
#include <Engine/CStateManager.h>
#include <Game/CStateMenu.h>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include "user.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

User *User::player_user = NULL; // Used to strong username/status of current player

// Check if the password entered by the user matches their username
// returns false if password does not match username
bool check_pass(std::string uname, std::string pass) {
	bool valid = false;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		pstmt = con->prepareStatement("SELECT username, password FROM user WHERE username = '" + uname + "' AND password = '" + pass + "'"); // See if username exists in user
		res = pstmt->executeQuery();

		if (res->next()) // Check if username and password was found (res->next() should not exist if the username wasnt found)
			valid = true;

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return valid;
}

// Check if the username entered is unique/valid
// returns false if username is not valid(exists in user table already)
bool check_user(std::string uname) {
	bool valid = true;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::Statement *stmt;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL check_user('" + uname + "', @count)");
		res = stmt->executeQuery("SELECT @count AS found");

		res->next();
		if (res->getString("found") == "1") // If user exists found should be 1
			valid = false;

		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return valid;
}

// Adds a user to the database given a username, password, name, and email
void add_user(std::string un, std::string pass) {
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL add_user('" + un + "', '" + pass + "')");

		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

// Removes a user from the database given their username
void remove_user(std::string un) {
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL remove_user('" + un + "')");

		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

// Set a users password to a new password
void set_pass(std::string un, std::string pass) {
	 try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL set_password('" + un + "', '" + pass + "')");

                delete stmt;
                delete con;
        }
        catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}

// Display top 10 highsocres from database 
void display_highscores() {
         try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;
		std::auto_ptr<sql::ResultSet> res;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL get_highscores()");
		
		do{
			res.reset(stmt->getResultSet());
			while(res->next()){
				std::cout << "User: " << std::setw(15) << std::left << res->getString("username") 
				<< "\t Score: " << std::setw(15) << std::left << res->getInt("score") << std::endl;
			}
		}while(stmt->getMoreResults());
	
                delete stmt;
                delete con;
		res.release();
        }
        catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}

int main(int argc, char const *argv[])
{
	int option = -1; 			// Menu choice, selected by user
	bool validUser = false; 	// Used in case 1 & 2 to check if username exists in database
	bool correctPass = false; 	// Used in case 1 to check if user entered a passwrod matching their username
	std::string username; 		// Used in case 1 & 2 to keep users username
	std::string password; 		// Used in case 1 & 2 to keep users password

	while (option != 3) {
		std::cout << std::endl << "Welcome to Pacman, what would you like to do: " << std::endl;
		std::cout << "1) Login as an existing user" << std::endl;
		std::cout << "2) Register a new user" << std::endl;
		std::cout << "3) Exit" << std::endl;
		std::cout << "Enter your selection number '1' '2' or '3' (without ''): ";
		std::cin >> option;
		std::cout << std::endl;

		switch (option) {
		// LOGIN AS EXISTING USER
		case 1:
			validUser = true;
			correctPass = false;
			while (validUser) {    // Run until user enters a username that exists in the database
				std::cout << "Enter your username: ";
				std::cin >> username;

				validUser = check_user(username);
				if (validUser)
					std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
			}
			while (!correctPass) {
				std::cout << "Enter the password for " + username + "(not case-sensitive):";
				if (username == "admin") { // Admin uses whitespace password, must read diffrently
					std::cin.ignore(1000, '\n');
					getline(std::cin, password);
				}
				else
					std::cin >> password;

				correctPass = check_pass(username, password);
				if (!correctPass)
					std::cout << "Password incorect! Try again." << std::endl;
			}
			User::getInstance()->setUsername(username);
			User::getInstance()->Login();
			//std::cout << User::getInstance()->getUsername() << std::endl;		

			// LOGGED IN AS NON-ADMIN
			if (username != "admin") {
				std::cout << std::endl << "Logged in as user " + username + "." << std::endl;
				int op = -1;
				CStateManager states;

				while (op != 3) {
					std::cout << "\nWhat would " + username + " like to do: " << std::endl;
					std::cout << "1) Play the game" << std::endl;
					std::cout << "2) Display highscores" << std::endl;
					std::cout << "3) Logout" << std::endl;
					std::cout << "Enter your selection: ";
					std::cin >> op;
					std::cout << std::endl;

					switch (op) {
					case 1: // Start the game
						CNCurses::init();

						states.run(new CStateMenu);

						CNCurses::exit();
						break;
					case 2:	// Show High-scores
						display_highscores();
						break;
					case 3: // Exit
						std::cout << "Good-bye!" << std::endl;
						break;
					default:
						std::cout << "Please choose a valid option!" << std::endl;
						break;
					}
				}
			}

			// LOGGED IN AS ADMIN
			else {
				std::cout << std::endl << "Logged in as administrator." << std::endl;
				int adop = -1;

				while (adop != 3) {
					std::cout << "What would admin like to do: " << std::endl;
					std::cout << "1) Reset a users password" << std::endl;
					std::cout << "2) Remove a user from the game database" << std::endl;
					std::cout << "3) Logout" << std::endl;
					std::cout << "Enter your selection: ";
					std::cin >> adop;
					std::cout << std::endl;

					switch (adop) {
					case 1: // Change a user's password
						validUser = true;
						username = "";
						password = "";
						while (validUser) {    // Run until user enters a username that exists in the database
							std::cout << "Enter user to change password for: ";
							std::cin >> username;

							validUser = check_user(username);
							if (validUser)
								std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
						}
						std::cout << "Enter a new password for " + username + ":";
						std::cin >> password;

						set_pass(username, password);
						std::cout << "User - " + username + "'s password was changed!\n\n";						

						break;
					case 2: // Remove user
						username = "";
						validUser = true;
						while (validUser) {    // Run until user enters a username that exists in the database
							std::cout << "Enter username of user to be removed: ";
							std::cin >> username;

							validUser = check_user(username);
							if (validUser)
								std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
						}
						remove_user(username);
						std::cout << "User removed!" << std::endl;

						break;
					case 3: // Exit
						std::cout << "Good-bye!" << std::endl;
						break;
					default:
						std::cout << "Please choose a valid option!" << std::endl;
						break;
					}
				}
			}
			break;

		// CREATE NEW USER TO LOGIN
		case 2:
			validUser = false;

			std::cout << "Creating a new user..." << std::endl;
			while (!(validUser)) {	// Run until user enters valid username
				std::cout << "Enter a unique username: ";
				std::cin >> username;

				validUser = check_user(username);
				if (!(validUser))
					std::cout << "Username is already in use!" << std::endl;
			}
			std::cout << "Enter a password(not case-sensitive): ";
			std::cin >> password;

			add_user(username, password);
			std::cout << "New user added sucessfully!" << std::endl;
			username = ""; // Reset username and password and validUser
			password = "";
			validUser = false;
			break;

		// Exit
		case 3: 
			std::cout << "Good-Bye!" << std::endl;
			break;
		default:
			std::cout << "Please choose a valid option!" << std::endl;
			break;
		}
	}	
	return 0;
}
