// Aaron Liszt
// member.h
// Header definition for member
// Member objects will be used by both candidates and members from the database since members/candidates have the same privileges

#ifndef MEMBER
#define MEMBER

#include <string>

class member{
public:
	member() { username = ""; }
	member(std::string& u) { username = u; }
	void vote(std::string can, std::string voter);
	std::string username;
};

#endif
