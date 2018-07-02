/* Copyright 2008, 2010, Oracle and/or its affiliates. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

There are special exceptions to the terms and conditions of the GPL
as it is applied to this software. View the full text of the
exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
software distribution.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* @README A
    - Connect to dbdev.cs.kent.edu server using your flashline username and password
    - place this file (CompleteExample2.cpp) on your home directory or 
      anywhere, this is only for testing purposes
*/

#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main(void)
{
cout << endl;
cout << "Let's have MySQL count from 10 to 1..." << endl;

try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  sql::PreparedStatement *pstmt;

  /* Create a connection */
  driver = get_driver_instance();
  
  /*@README B
        - Collect your DB password (YOURDATABASEPASSWORD)
          from here https://webdev.cs.kent.edu/FLASHLINEUSERNAME/DB/DB_Access.html
        - To connect to the Database,
        - Replace FLASHLINEUSERNAME with your flash user name on connect
        - Replace YOURDATABASEPASSWORD with your database password, collected from the 
          above link
  */
  con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
  /* Connect to the MySQL test database */
  
  /*@README C
        - Replace FLASHLINEUSERNAME with your flash user name on setSchema.
  */  
  con->setSchema("aliszt1");

  stmt = con->createStatement();
  stmt->execute("DROP TABLE IF EXISTS test");
  stmt->execute("CREATE TABLE test(id INT)");
  delete stmt;

  /* '?' is the supported placeholder syntax */
  pstmt = con->prepareStatement("INSERT INTO test(id) VALUES (?)");
  for (int i = 1; i <= 10; i++) {
    pstmt->setInt(1, i);
    pstmt->executeUpdate();
  }
  delete pstmt;

  /* Select in ascending order */
  pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
  res = pstmt->executeQuery();

  /* Fetch in reverse = descending order! */
  res->afterLast();
  while (res->previous())
    cout << "\t... MySQL counts: " << res->getInt("id") << endl;
  delete res;

  delete pstmt;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

cout << endl;

return EXIT_SUCCESS;
}
/* @README D
    Compile your code:
    $ g++ -Wall -I/usr/include/cppconn -o CE2 CompleteExample2.cpp -L/usr/lib -lmysqlcppconn

    Run your code:
    $ ./CE2 

    Result should look:
    $ ./CE2 

    Let's have MySQL count from 10 to 1...
            ... MySQL counts: 10
            ... MySQL counts: 9
            ... MySQL counts: 8
            ... MySQL counts: 7
            ... MySQL counts: 6
            ... MySQL counts: 5
            ... MySQL counts: 4
            ... MySQL counts: 3
            ... MySQL counts: 2
            ... MySQL counts: 1
*/

/* @README E
    To access and use your database, using shell, connect to MySQL 
    using these commands:
    $ mysql -u FLASHLINEUSERNAME -p
    You will be asked to Enter DB password, YOURDATABASEPASSWORD from @README B
    >use FLASHLINEUSERNAME;
    >show tables;
    >select * from test;
    +------+
    | id   |
    +------+
    |    1 |
    |    2 |
    |    3 |
    |    4 |
    |    5 |
    |    6 |
    |    7 |
    |    8 |
    |    9 |
    |   10 |
    +------+
*/