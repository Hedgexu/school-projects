#include "client_write.h"
#include "client_listen.h"

#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void client_write::sendMess ( char * str )
{
	char text [100];
	snprintf ( text, sizeof ( text ), "%i :%s", userid, str );
	sendto ( fd, text, (int) strlen ( text ), 0, (struct sockaddr *) & serverAddr, serverAddrLen );
}
void client_write::sendSTOP ( )
{
	char text [] = "STOP";
	sendto ( fd, text, strlen ( text ), 0, (struct sockaddr *) & serverAddr, serverAddrLen );
}
void client_write::sendQUIT ( )
{
	char text [20];
	snprintf ( text, sizeof ( text ), "QUIT %i", userid );
	sendto ( fd, text, strlen ( text ), 0, (struct sockaddr *) & serverAddr, serverAddrLen );
}

void client_write::run ( )
{
	while ( true )
	{
		char str[80];
		wclear ( win_write );
		mvwprintw ( win_write, 0, 0, "Write a message ( special messages QUIT a STOP ) " );
		wmove ( win_write, 1, 0 );
		wgetstr ( win_write, str );
		
		if (stop)
			break;
		
		if (strlen(str) < 1)
			continue;
			 
		str[ strlen(str) ] = '\0';
		if ( parse ( str ) )
		{
			usleep(500 * 1000);
			break;
		}
	}

}

bool client_write::parse ( char * str )
{
	string slovo = ( string ) strtok ( str, "\0" );
	if ( slovo == "QUIT" )
	{
		sendQUIT ( );
		return true;
	}
	else if ( slovo == "STOP" )
		sendSTOP ( );
	else
		sendMess( str );
		
	return false;
}


int client_write::openCliSocket ( const char * name, int port )
{
	struct addrinfo * ai;
	char portStr[10];

	snprintf ( portStr, sizeof ( portStr ), "%d", port );

	if ( getaddrinfo ( name, portStr, NULL, &ai ) )
	{
		cout << "getaddrinfo failed" << endl;
		return -1;
	}

	int fd = socket ( ai -> ai_family, SOCK_DGRAM, 0 );
	if ( fd == -1 )
	{
		freeaddrinfo ( ai );
		cout << "socked failed" << endl;
		return -1;
	}

	if ( connect ( fd, ai -> ai_addr, ai -> ai_addrlen ) == - 1 )
	{
		close ( fd );
		freeaddrinfo ( ai );
		cout << "connect failed" << endl;
		return -1;
	}

	freeaddrinfo ( ai );
	return fd;
}

// --------------------------------------------------------------------------------------------------------


int client_write::init ( const string & userName, const string & roomName )
{

	stop = false;
	fd = openCliSocket ( "127.0.0.1", 12345 );
	
	if ( fd < 0 ) return 1;
	
	cout << fd << endl;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons ( 12345 );
	serverAddrLen = ( socklen_t ) sizeof ( serverAddr );
	if ( inet_aton ( "127.0.0.1", & serverAddr.sin_addr ) == 0)
	{
		printf ( "inet_aton() failed\n" );
		return 1;
	}
	
	// Poslani zpravy se zadosti od pripojeni
	char join [50];
	snprintf ( join, sizeof ( join ), "JOIN %s %s", userName.c_str(), roomName.c_str() );
	if ( sendto ( fd, join, strlen(join), 0, (struct sockaddr *) & serverAddr, serverAddrLen ) < 0)
	{
		printf("join failed\n");
		return 1;
	}

	char buffer[100];
	
	
	int len = recvfrom( fd, buffer, 100, 0, (struct sockaddr *) & serverAddr, & serverAddrLen );
	if ( len <= 0 )
	{
		printf ( "chyba na strane serveru\n" );
		return 1;
	}
	buffer[len] = '\0';
	printf("%s\n", buffer);
	userid = atoi ( strtok ( buffer, "\0" ) );
	
	
	initscr();
//	curs_set ( FALSE );
	int parent_x, parent_y;
	getmaxyx ( stdscr, parent_y, parent_x );
	win_write = newwin ( 2, parent_x, parent_y - 2, 0 );
	WINDOW * win_listen = newwin ( parent_y - 2, parent_x, 0, 0 );
	
	client_listen cl ( fd, serverAddr, win_listen, win_write, &stop );
	run ( );
	
	delwin ( win_write );
	delwin ( win_listen );
	
	endwin();
	return 0;
}
