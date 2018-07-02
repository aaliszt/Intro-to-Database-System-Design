// Aaron Liszt
// admin.cpp
// Defines methods for the admin class

#include <iostream>
#include <string>
#include <iomanip>
#include "admin.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

// Adds a user to the database given a username, password, name, and email
void admin::add_user(std::string un, std::string pass, std::string n, std::string em){
	try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL add_user('" + un + "', '" + n + "', '" + pass + "', '" + em + "')");

                delete stmt;
                delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
} 

// Removes a user from the database given their username
void admin::remove_user(std::string un){
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
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}

// Assigns a user as a candidate given their username
void admin::assign_candidate(std::string un){
	try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL assign_candidate('" + un + "')");

                delete stmt;
                delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }	
}

// Unassigns a user as a candidate given their username
void admin::unassign_candidate(std::string un){
	try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL unassign_candidate('" + un + "')");

                delete stmt;
                delete con;
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}

// Displays all users usernames, names, passwords, emails, and usertypes
void admin::display_members(){
	 try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;
                std::auto_ptr<sql::ResultSet> res;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL get_users()");

                do{
                        res.reset(stmt->getResultSet());
                        while(res->next()){
                                std::cout << "User: " << std::setw(15) << std::left <<res->getString("username") << "\t Name: " << std::setw(15) << std::left << res->getString("name") 
				<< "\t Password: " << std::setw(15) << std::left << res->getString("password") << "\t Email: " << std::setw(25) << std::left << res->getString("email") 
				<< "\t Usertype: " << std::setw(15) << std::left <<res->getString("usertype") << std::endl;
			}
                }while(stmt->getMoreResults());

                delete stmt;
                delete con;
		res.release();
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }		
}

// Displays the race status with each candidates username and the number of votes they currently have
void admin::display_votes(){
	try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;
		std::auto_ptr<sql::ResultSet> res;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                stmt = con->createStatement();
                stmt->execute("CALL get_votes()");
		
		do{
			res.reset(stmt->getResultSet());
			while(res->next())
				std::cout << "Candidate: " << res->getString("username") << "\t Votes: " << res->getInt("num_votes") << std::endl;

		}while(stmt->getMoreResults());

                delete stmt;
                delete con;
		res.release();
        } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}
