#include <Game/CGame.h>
  
#include <Game/CGameLayout.h>
#include <Engine/CInputManager.h>
#include <Game/CBoardUtils.h>
#include <Engine/CDialog.h>

#include <stdlib.h>
#include <sstream>
#include <fstream>

#include "user.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


template<typename T> void safe_delete(T*& a) 
	{
	  delete a;
	  a = NULL;
	}


CGame::CGame ():
	game_over ( true ),
	paused (false),
	show_pause_menu (false),
	player (NULL),
	board (NULL),
	layout (NULL), 
	ghost ( NULL ),
	pause_menu ( NULL ),
	bool_exit ( false ),
	bool_menu ( false)
	{
		//load_high_score ();
	}
CGame::CGame ( std::string map, int difficulty ):
	game_over ( true ),
	paused (false),
	show_pause_menu (false),
	player (NULL),
	board (NULL),
	layout (NULL), 
	ghost ( NULL ),
	pause_menu ( NULL ),
	bool_exit ( false ),
	bool_menu ( false),
	map ( map),
	difficulty ( difficulty ),
	stage ( 1 ),
	high_score ( 0 ),
	points ( 0 )
	{
		//load_high_score ();
	}

CGame::~CGame ()
	{
		safe_delete ( board );
		safe_delete ( player );
		safe_delete ( layout );
		safe_delete ( ghost );
		safe_delete ( pause_menu );

	}
void CGame::start (  )
	{
		safe_delete ( board );
		safe_delete ( player );
		safe_delete ( layout );
		safe_delete ( ghost );
		safe_delete ( pause_menu );
		
		if(stage == 1)		
			get_timeDate();

		if(game_over){
			if (high_score >= points)
                		save_high_score ();

			points = 0;
			lives  = 3;
			stage  = 1;
		}
		load_high_score ();

		game_over = false;
		show_pause_menu  = false;
		paused 	= false;
		

		if ( ! map . empty ())
			board = CBoardUtils::load_file ( map );
		else
			board = CBoardUtils::load_file ( "default" );

		player = new CPlayer ( board -> get_start_pos () );

		ghost = new CGhostManager ( board -> get_ghost_x (),
									board -> get_ghost_y (),
									difficulty, points );

		int cur_h, cur_w;
		getmaxyx ( stdscr, cur_h, cur_w );

		if ( cur_h - 2 < board -> get_height () || 
			 cur_w - 2 < board -> get_width () )
			{
				std::vector<std::string> v = {  "Map is bigger than your actual",
				"window. Please resize it ", "and run game again" };
				CDialog::show ( v, "ERROR" );
				bool_exit = true;
			}

		layout = new CGameLayout (this, cur_w, cur_h);

		pause_menu = new CMenu ( 1, 1, layout -> pause_menu -> get_width () - 2,
								 layout -> pause_menu -> get_height () - 2 );

		CMenuItem * item;

		item = new CMenuItem ( "Resume Game", RESUME);
		pause_menu -> add ( item );

		item = new CMenuItem ( "Restart Game", RESTART);
		pause_menu -> add ( item );

		pause_menu -> add ( NULL );

		item = new CMenuItem ( "Main Menu", MAIN_MENU);
		pause_menu -> add ( item );

		item = new CMenuItem ( "Exit Game", EXIT_GAME);
		pause_menu -> add ( item );

		timer_player . start();
		timer_ghost  . start();
		timer . start ();

	}
void CGame::restart ()
	{
		safe_delete ( player );
		safe_delete ( layout );
		safe_delete ( ghost );

		player = new CPlayer ( board -> get_start_pos () );
		ghost = new CGhostManager ( 32, 9, difficulty, points );
		int cur_h, cur_w;
		getmaxyx ( stdscr, cur_h, cur_w );

		layout = new CGameLayout (this, cur_w, cur_h);

		timer_player . start();
		timer_ghost  . start();
		timer . start ();

	}

bool CGame::is_paused () const
	{
		return paused;
	}
bool CGame::is_over () const
	{
		return game_over;
	}
bool CGame::will_exit () const
	{
		return bool_exit;
	}
bool CGame::will_go_to_menu () const
	{
		return bool_menu;
	}
void CGame::handle_input ()
	{
		if ( ! CInputManager::any_key_pressed () )
			return;

		if ( CInputManager::is_pressed ( 'p' ))
			{
				paused = ( is_paused () ) ? false : true;
				return;
			}
		if ( is_paused () )
			{
				pause_menu -> HandleInput ();
				return;
			}
		if ( CInputManager::is_pressed (	KEY_RIGHT  ))
			this -> player -> move ( CPlayer::RIGHT );
		if ( CInputManager::is_pressed (	KEY_LEFT  )) 
			this -> player -> move ( CPlayer::LEFT );
		if ( CInputManager::is_pressed (	KEY_DOWN  ))
			this -> player -> move ( CPlayer::DOWN );
		if ( CInputManager::is_pressed (	KEY_UP  ))
			this -> player -> move ( CPlayer::UP );



	}
bool CGame::is_food_eaten ( )
	{
		if ( board -> is_dot ( player -> getX (), player -> getY () ))
			{
				board -> eat_dot ( player -> getX (), player -> getY () );
				points += 10;
				return true;
			}
		return false;
	}
bool CGame::is_star_eaten ( )
	{
		if ( board -> is_star ( player -> getX (), player -> getY () ))
			{
				board -> eat_dot ( player -> getX (), player -> getY () );
				points += 50;
				ghost -> set_frightened_mode ( true );
				return true;
			}
		return false;
	}
void CGame::update ()
	{
		timer_player . pause ();
		timer_ghost  . pause ();

		int delta = this -> get_delay ( stage );
		int delta_ghost;

		if ( points > high_score )
			high_score = points;

		if ( board -> all_dots_eaten () )
			{
				std::vector<std::string> v = {  "Your score is " +  points };
				CDialog::show ( v, "LEVEL PASSED" );
				stage++;
				start ();
			}

		if ( is_paused () )
			{
				if ( pause_menu -> WillExit () )
					{
						int option = pause_menu -> CurrentID ();

						switch ( option )
							{
								case RESUME:
									paused = false;
									break;
								case RESTART:
									game_over = true;
									start ();
									return;
								case MAIN_MENU:
									bool_menu = true;
									return;
								case EXIT_GAME:
									bool_exit = true;
									return;
							}
						pause_menu -> reset ();
					}

				return;
			}

		if ( timer_player . d_ms () >= delta )
			{
				if ( ! player -> is_alive ())
					{
						if ( lives > 0 )
							{
								save_life_info();
								lives--;
								std::vector<std::string> v = {  "You have " + 
																std::to_string(lives) +
																" lives left." };
								CDialog::show ( v, "You Died" );
								get_timeDate();
								restart ();
								
							}	
						else 
							{
								save_life_info();
								game_over = true;
								std::vector<std::string> v = {  "Your score is: " +
																 std::to_string(points) + "." };
								if ( high_score <= points )
									v . push_back ( "NEW HIGHSCORE!!!");
								CDialog::show ( v , "GAME OVER");
								paused = true;
								pause_menu -> RemoveByID ( RESUME );
							}
					}
				else
					{
						player -> update ( board );
						ghost -> check_collisions ( player );
						is_food_eaten ();
						is_star_eaten ();

						
					}

				timer_player . start ();
			}
		else
			{
				timer_player . unpause ();
			}
		if ( ghost -> are_frightened () )
			delta_ghost = 1.6 * delta;
		else delta_ghost = delta;

		if ( timer_ghost . d_ms () >= delta_ghost )
			{
				ghost ->  update ( board, player );
				ghost -> check_collisions ( player );
				timer_ghost . start ();
			}
		else timer_ghost . unpause ();
	}
void CGame::draw ()
	{
		layout -> draw ( pause_menu );	
	}
int  CGame::get_delay ( int speed ) const
	{
		switch ( speed )
			{
				case 1:
					return 120;
				case 2:
					return 100;
				case 3:
					return 90;
				case 4:
					return 80;
				case 5:
					return 60;
				default:
					return 50;
			}
	}

//Loads the lowest of the top 10 highscore from the score table in the game database
void CGame::load_high_score(){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL get_lowest_highscore(@p0)");
		res = stmt->executeQuery("SELECT @p0 AS low_score");
		
		res->next();
		high_score = res->getInt("low_score");

		delete stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		high_score = 0;
	}
	//std::ofstream file("test.txt");
	//file << "Highscore loaded! Score is: " << high_score << std::endl;
	//file.close();
}

void CGame::save_high_score(){

	std::string uname = User::getInstance()->getUsername();
	int sc = high_score;
	
	//std::ofstream file("test.txt");
        //file << "\nNew High Score is: " << std::to_string(sc) << std::endl;
	//file << "User: " << uname << std::endl;
	//file.close();
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		driver = get_driver_instance();
		con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
		con->setSchema("aliszt1");

		stmt = con->createStatement();
		stmt->execute("CALL add_new_highscore('" + uname + "', '" + std::to_string(sc) + "')");

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

void CGame::save_life_info(){
	std::string uname = User::getInstance()->getUsername();	
	std::string diff = "";
	std::string s_date = start_date;
	std::string e_date = "";
	std::string s_time = start_time;
	std::string e_time = "";
	
	int lvl_pass = stage - start_lvl;
	
	//if(lvl_pass < 0)
	//	lvl_pass = 0;	

	switch(difficulty){
	case 0:
		diff = "Easy";
		break;
	case 1:
		diff = "Medium";
		break;
	case 2:
		diff = "Hard";
		break;
	default:
		diff = "Error";
		break;
	}	
        try {
                sql::Driver *driver;
                sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;
		sql::Statement *stmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");
		
		pstmt = con->prepareStatement("SELECT CURDATE() AS date");
		res = pstmt->executeQuery();

		res->next();
		e_date = res->getString("date");

		delete res;
		delete pstmt;
				
                pstmt = con->prepareStatement("SELECT CURTIME() AS time");
                res = pstmt->executeQuery();

                res->next();
                e_time = res->getString("time");

		stmt = con->createStatement();
		stmt->execute("CALL store_life('" + uname + "', '" + std::to_string(start_lvl) + "', '" + s_date + "', '" + e_date + "', '" + s_time + "', '" + e_time + "', '" 
		+ std::to_string(lvl_pass) + "', '" + diff + "')");

                delete pstmt;
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
	start_lvl = stage;
}

void CGame::get_timeDate(){
        try {
                sql::Driver *driver;
                sql::Connection *con;
                sql::ResultSet *res;
                sql::PreparedStatement *pstmt;

                driver = get_driver_instance();
                con = driver->connect("dbdev.cs.kent.edu", "aliszt1", "bAPuyc00");
                con->setSchema("aliszt1");

                pstmt = con->prepareStatement("SELECT CURTIME() AS time");
                res = pstmt->executeQuery();

                res->next();
                start_time = res->getString("time");
		
		delete pstmt;
		delete res;

		pstmt = con->prepareStatement("SELECT CURDATE() AS date");
                res = pstmt->executeQuery();

                res->next();
                start_date = res->getString("date");

                delete pstmt;
                delete con;
		delete res;
        }
        catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
}

