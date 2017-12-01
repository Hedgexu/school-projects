#include "client_listen.h"

#include <thread>

using namespace std;

void client_listen::LoadCursorPosition ( )
{
	wmove(win_write, cursorX, cursorY);
}

void client_listen::storeCursorPosition ( )
{
	getyx(win_write, cursorX, cursorY);
}

client_listen::client_listen ( int fd, struct sockaddr_in serverAddr, WINDOW * win, WINDOW * win2, bool * stop )
: fd ( fd ), serverAddr ( serverAddr ), win_listen ( win ), win_write(win2), stop(stop)
{
	serverAddrLen = sizeof ( serverAddr );
	lines = getmaxy ( win_listen ) - 1;
	
	thread second ( & client_listen::init, this );
	second.detach();
}


void client_listen::print ( )
{
	storeCursorPosition();
		
	for ( int i = 0; i < ( int ) messages.size(); i++ )
		mvwprintw ( win_listen, i, 0, messages[i].c_str() );
		
	wrefresh ( win_listen );
	LoadCursorPosition();
	wrefresh ( win_write );
}

void client_listen::init ( )
{
	char buffer[100];

	while ( true )
	{
		int recvlen = recvfrom ( fd, buffer, 100, 0, (struct sockaddr *) & serverAddr, & serverAddrLen );
		buffer[recvlen] = '\0';
		if ( recvlen > 0 )
		{
			string radek = ( string ) strtok ( buffer, "\0" );
			if ( radek == "QUIT" )
			{
				*stop = true;
				break;
			}
				
			addMess ( radek );
			print ( );
		}
	}
}

void client_listen::addMess ( string &  mess )
{
	if ( ( int ) messages.size()  == lines )
		messages.erase ( messages.begin() );
		
	messages.push_back ( mess );
}
