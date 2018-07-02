// Aaron Liszt
// admin.h
// Header definiton for admin class

#ifndef ADMIN
#define ADMIN

#include <string>

class admin{
public:
	void add_user(std::string un, std::string pass, std::string n, std::string em);
	void remove_user(std::string un);
	void assign_candidate(std::string un);
	void unassign_candidate(std::string un);
	void display_members();
	void display_votes();	
};

#endif

