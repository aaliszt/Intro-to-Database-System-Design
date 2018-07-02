// Aaron Liszt
// 3 / 13 / 17
// main.cpp
// Cointains main loops and functionaility for logging into and accessing the aliszt1 database

#include <iostream>
#include <string>
#include <stdlib.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "admin.h"
#include "member.h"

// Check if the password entered by the user matches their username
// returns false if password does not match username
bool check_pass(std::string uname, std::string pass){
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

                if(res->next()) // Check if username and password was found (res->next() should not exist if the username wasnt found)
                        valid = true;

                delete res;
                delete pstmt;
                delete con;
        } catch (sql::SQLException &e) {
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
bool check_user(std::string uname){
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
                if(res->getString("found") == "1") // If user exists found should be 1
                        valid = false;

  		delete res;
  		delete stmt;
  		delete con;
	} catch (sql::SQLException &e) {
  		std::cout << "# ERR: SQLException in " << __FILE__;
  		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
  		std::cout << "# ERR: " << e.what();
  		std::cout << " (MySQL error code: " << e.getErrorCode();
  		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return valid;
}

// Gets the usertype of a user given their username
std::string get_usertype(std::string uname){
	std::string UT = "";

	try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::ResultSet *res;
                sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");
		
		stmt = con->createStatement();
                res = stmt->executeQuery("SELECT usertype FROM user_type WHERE username = '" + uname + "'");
		res->next();
                UT = res->getString("usertype");

                delete res;
                delete stmt;
                delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
	return UT;
}

// Check if a user has voted for a candidate before
// Returns true if the user has not voted for candidate before, and false if user has
bool check_vote(std::string uname, std::string cname){
	bool validVote = false;

	try {
 	       sql::Driver *driver;
               sql::Connection *con;
               sql::ResultSet *res;
               sql::Statement *stmt;

               driver = get_driver_instance();
               con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
               con->setSchema("aliszt1");

               stmt = con->createStatement();
               stmt->execute("CALL check_checkvote('" + cname + "', @count, '" + uname + "')");
	       res = stmt->executeQuery("SELECT @count AS voted");               
 
	       res->next();
	       if(res->getString("voted") == "0") // If there is no result from the check vote_table then this is a valid vote (user has not voted for candidate before)
			validVote = true;
        
               delete res;
               delete stmt;
               delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
        return validVote;
}

// Checks if a specified candidate exists in the vote table
// Returns true if the candidate was found false otherwise
bool check_candidate(std::string cname){
	bool validCandidate = false;

        try {
               sql::Driver *driver;
               sql::Connection *con;
               sql::ResultSet *res;
               sql::Statement *stmt;

               driver = get_driver_instance();
               con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
               con->setSchema("aliszt1");

               stmt = con->createStatement();
               stmt->execute("CALL check_candidate('" + cname + "', @count)");
               res = stmt->executeQuery("SELECT @count AS found");

               res->next();
               if(res->getString("found") == "1") // If candiate exists found should be 1
                        validCandidate = true;

               delete res;
               delete stmt;
               delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
        return validCandidate;
}

int main(){
	int option = -1; 		// Menu choice, selected by user
	bool validUser = false; 	// Used in case 1 & 2 to check if username exists in database
	bool correctPass = false; 	// Used in case 1 to check if user entered a passwrod matching their username
	std::string name; 		// Used in case 2 to add store name of new user
	std::string email; 		// Used in case 2 to store email of new user
	std::string username; 		// Used in case 1 & 2 to keep users username
	std::string password; 		// Used in case 1 & 2 to keep users password
	std::string usertype; 		// Used in case 1 to keep the usertype of the logged in user
	admin adder; 			// Used in case 2 to add new user to database

	///////////////////////
	// MAIN LOOPING MENU //
	//////////////////////
	while(option != 3){
		std::cout << std::endl << "What would you like to do: " << std::endl;
		std::cout << "1) Login as existing user" << std::endl;
		std::cout << "2) Create new user and login" << std::endl;
		std::cout << "3) Exit" << std::endl;
		std::cout << "Enter your selection number '1' '2' or '3' (without ''): ";
		std::cin >> option;
		std::cout << std::endl;
		
		switch(option){

			///////////////////////////
			//LOGIN AS EXISTING USER//
			/////////////////////////

			case 1:
				validUser = true;
				correctPass = false;
				while(validUser){    // Run until user enters a username that exists in the database
                                        std::cout << "Enter your username: ";
                                        std::cin >> username;

                                        validUser = check_user(username);
                                        if(validUser)
                                                std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
                                }
				while(!correctPass){
					std::cout << "Enter the password for " + username + "(not case-sensitive):";
					if(username == "admin"){ // Admin uses whitespace password, must read diffrently
						std::cin.ignore(1000, '\n');
						getline(std::cin, password);
					}
					else
						std::cin >> password;
					
					correctPass = check_pass(username, password);
					if(!correctPass)
						std::cout << "Password incorect! Try again." << std::endl;
				}
				
				usertype = get_usertype(username);
				//////////////////////////////////
				//LOGGED IN AS MEMBER/CANDIDATE//
				////////////////////////////////

				if(usertype == "member" || usertype == "candidate"){
					std::cout << std::endl << "Logged in as user " + username + "." << std::endl;
					
					int op = -1;
					member user(username);
					std::string candidate;
					
					while(op != 2){
						std::cout << "What would " + username + " like to do: " << std::endl;
                				std::cout << "1) Vote for a candidate" << std::endl;
                				std::cout << "2) Logout" << std::endl;
                				std::cout << "Enter your selection: ";
                				std::cin >> op;
                				std::cout << std::endl;
						
						switch(op){
							case 1:
								validUser = true;
								while(validUser){    // Run until user enters a username that exists in the database
                                   					std::cout << "Enter username of the candidate you want to vote for: ";
                                                           		std::cin >> candidate;

                                                                        validUser = check_user(candidate);
                                                                        if(validUser)
                                                                                std::cout << "User - " + candidate + " does not exist! Please enter a username." << std::endl;
                                                                }
								if(check_vote(username, candidate)){ // If check_vote is true the vote is valid
									user.vote(candidate, username);
									std::cout << "Voted for " + candidate + " sucessfully!" << std::endl;
								}
								else
									std::cout << "You have already voted for this candidate!" << std::endl;
							break;
							case 2:
								std::cout << "Good-bye " + username + "!" << std::endl;
							break;
							default:
								std::cout << "Please choose a valid option!" << std::endl;
							break;
						}

					}
				}

				///////////////////////
				//LOGGED IN AS ADMIN//
				/////////////////////

				else{
					std::cout << std::endl << "Logged in as administrator." << std::endl;
			
					int adop = -1;
                                        admin adminUser;
					
					while(adop != 7){
						std::cout << "What would you like to do: " << std::endl;
                				std::cout << "1) Add a user to the database" << std::endl;
                				std::cout << "2) Remove a user from the database" << std::endl;
						std::cout << "3) Assign a candidate" << std::endl;
						std::cout << "4) Un-assign a candidate" << std::endl;
						std::cout << "5) Display votes for all candidates" << std::endl;
						std::cout << "6) Display all users info" << std::endl;
                				std::cout << "7) Logout" << std::endl;
                				std::cout << "Enter your selection: ";
                				std::cin >> adop;
                				std::cout << std::endl;
						
						switch(adop){
                                                        case 1: // Add user
								validUser = false;
								username = "";
								password = "";
								name = "";
								email = "";
								while(!(validUser)){    // Run until user enters valid username
                                        				std::cout << "Enter a unique username: ";
                                        				std::cin >> username;

                                        				validUser = check_user(username);
                                        				if(!(validUser))
                                                				std::cout << "Username is already in use!" << std::endl;
                                				}
                                				std::cout << "Enter a name: ";
                                				std::cin >> name;
                                				std::cout << "Enter a password(not case-sensitive): ";
                                				std::cin >> password;
                                				std::cout << "Enter an email: ";
                                				std::cin >> email;
                            
								adminUser.add_user(username, password, name, email);
								std::cout << "User added!" << std::endl;
                                                        break;
                                                        case 2: // Remove user
								username = "";
								validUser = true;
                                				while(validUser){    // Run until user enters a username that exists in the database
                                        				std::cout << "Enter username of user to be removed: ";
                                        				std::cin >> username;

                                        				validUser = check_user(username);
                                        				if(validUser)
                                                				std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
                                				}
								adminUser.remove_user(username);
								std::cout << "User removed!" << std::endl;

                                                        break;
                                                        case 3: // Assign Candidate
								username = "";
								validUser = true;
                                                                while(validUser){    // Run until user enters a candidate that exists in the database
                                                                        std::cout << "Enter username of candidate to be assigned: ";
                                                                        std::cin >> username;

                                                                        validUser = check_user(username);
                                                                        if(validUser)
                                                                                std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
                                                                }
								if(!check_candidate(username)){// Check if user is already a candidate
                                                                	adminUser.assign_candidate(username);
									std::cout << "Candidate added!" << std::endl;
								}
								else
									std::cout << "User is already a candidate!" << std::endl;
                                                        break;
							case 4: // Un-assign Candidate
								username = "";
                                                                validUser = true;
                                                                while(validUser){    // Run until user enters a candidate that exists in the database
                                                                        std::cout << "Enter username of candidate to be un-assigned: ";
                                                                        std::cin >> username;

                                                                        validUser = check_user(username);
                                                                        if(validUser)
                                                                                std::cout << "User - " + username + " does not exist! Please enter a username." << std::endl;
                                                                }
								if(check_candidate(username)){ // Check if the user is a candidate
                                                               		adminUser.unassign_candidate(username);
                                                                	std::cout << "Candidate removed!" << std::endl;
								}
								else
									std::cout << "User - " + username + " is not a candidate!" << std::endl;
							break;
							case 5: // Vote Stats
								adminUser.display_votes();
							break;
							case 6: // User Info
								adminUser.display_members();
							break;
							case 7: // Exit
								std::cout << "Good-bye!" << std::endl;
							break;
                                                        default:
                                                                std::cout << "Please choose a valid option!" << std::endl;
                                                        break;
                                                }
					}
				}		
			break;

			/////////////////////////////
			//CREATE NEW USER TO LOGIN//
			///////////////////////////

			case 2:
				
				validUser = false;	
				
				std::cout << "Creating a new user..." << std::endl;
				while(!(validUser)){	// Run until user enters valid username
					std::cout << "Enter a unique username: ";
					std::cin >> username;

					validUser = check_user(username);
					if(!(validUser))
						std::cout << "Username is already in use!" << std::endl;
				}
				std::cout << "Enter your first name: ";
				std::cin >> name;
				std::cout << "Enter a password(not case-sensitive): ";
				std::cin >> password;
				std::cout << "Enter your email: ";
				std::cin >> email;
				
				adder.add_user(username, password, name, email);
				std::cout << "New user added sucessfully! You can now login as an existing user." << std::endl;
				username = ""; // Reset username and password and validUser
				password = "";
				validUser = false;
			break;

			/////////
			//EXIT//
			///////

			case 3: // Exit
				std::cout << "Good-Bye!" << std::endl;
			break;
			default:
				std::cout << "Please choose a valid option!" << std::endl;
			break;
		}
	}
}
