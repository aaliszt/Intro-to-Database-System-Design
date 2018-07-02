# Aaron Liszt 
# 3 / 16 / 2017 
# Makefile for vlr project
output: main.o member.o admin.o
	g++ main.o member.o admin.o -Wall -I/usr/include/cppconn -o vlr -L/usr/lib -lmysqlcppconn -std=c++11 -w

main.o: main.cpp
	g++ main.cpp -Wall -I/usr/include/cppconn -L/usr/lib -lmysqlcppconn -std=c++11 -c main.cpp

member.o: member.cpp member.h
	g++ -Wall -I/usr/include/cppconn -L/usr/lib -lmysqlcppconn -std=c++11 -c member.cpp

admin.o: admin.cpp admin.h
	g++ -Wall -I/usr/include/cppconn -L/usr/lib -lmysqlcppconn -std=c++11 -w -c admin.cpp

clean:
	rm *.o vlr
