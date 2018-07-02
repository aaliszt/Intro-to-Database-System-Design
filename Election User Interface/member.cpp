// Aaron Liszt
// member.cpp
// Defines functions for member.h

#include <iostream>
#include <string>
#include "member.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

// Vote for an existing candidate given the candidates username and the voters username
void member::vote(std::string can, std::string voter){
	try {
  		sql::Driver *driver;
  		sql::Connection *con;
		sql::Statement *stmt;
  		sql::PreparedStatement *pstmt;
		sql::ResultSet  *res;

  		driver = get_driver_instance();  
  		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
  		con->setSchema("aliszt1");
		
		pstmt = con->prepareStatement("SELECT username FROM vote WHERE username = '" + can + "'"); // See if user already exists as a candidate in vote
		res = pstmt->executeQuery();
		
		if(res->next()){ // Check if user was found (res->next() should not exist if the username wasnt found)
			stmt = con->createStatement();
			stmt->execute("CALL vote('" + voter + "', '" + can + "')");
			std::cout << "Voted for " + can + "!" << std::endl;
		}
		else
			std::cout << "User - " + can + "is not a candidate! Please vote for a valid candidate.";

		delete stmt;
  		delete pstmt;
  		delete con;
		delete res;
	} catch (sql::SQLException &e) {
  		std::cout << "# ERR: SQLException in " << __FILE__;
  		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
  		std::cout << "# ERR: " << e.what();
  		std::cout << " (MySQL error code: " << e.getErrorCode();
  		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	std::cout << std::endl;	
}
