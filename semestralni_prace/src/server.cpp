#include "server.h"

#include <iostream>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>

using namespace std;

#define BUFSIZE 100

void server::userLeave ( user * usr )
{
	set < user *, comp_user_id >::iterator itr = s_users.find ( usr );
	
	if ( itr == s_users.end() )
	{
		cout << "Somthink is wrong -> " << usr->getId() << endl;
		delete usr;
		return;
	}
	
	(*itr)->getMistnost()->removeUser( *itr );
	if ( (*itr)->getMistnost()->isClear() )
	{
		cout << "room was deleted" << endl;
		s_rooms.erase ( (*itr)->getMistnost() );
		delete (*itr)->getMistnost();
	}
	else
		(*itr)->getMistnost()->sent ( new svrMess ((*itr)->getName ( ), 2 ) );
		
	user * tmp = *itr;
	tmp->quit(false);
	s_users.erase( *itr );
	delete tmp;
	delete usr;
}

void server::userJoin ( user * usr , const char * roomname  )
{
	s_users.insert ( usr );
	room * tmp = new room ( roomname );
	set < room *, comp_room >::const_iterator itr = s_rooms.find ( tmp ); 
	if ( itr == s_rooms.end() )
	{
		// new room
		cout << "new room -> " << roomname << endl;
		cout << "user " << usr->getName ( ) << "(" << usr->getId() << ")" <<  " join to room " << roomname << endl;
		s_rooms.insert ( tmp );
		tmp->addUser ( usr );
		usr->setMistnost ( tmp );		
	}
	else
	{
		// existing room
		cout << "user " << usr->getName ( ) << "(" << usr->getId() << ")" <<  " join to room " << roomname << endl;
		delete tmp;
		(*itr)->addUser ( usr );
		usr->setMistnost ( *itr ); 
		(*itr)->sent ( new svrMess ( usr->getName ( ), 1 ) );
	}
}

bool server::parse ( const char * msg, struct sockaddr & remoteAddr, socklen_t & remoteLen, const int fd )
{
	char pole[100];
	strcpy ( pole, msg );
	
	cout << "inmsg: " << pole << endl;
	
	char * word = strtok ( pole, " " );
	
	
	if ( strcmp (word, "JOIN") == 0)
	{
		// pripojeni uzivatele
		cout <<"Mess type JOIN" << endl;
		word = strtok ( NULL, " " );
		if (!word)
			return false;
		char * word2 =  strtok ( NULL, "\0" );
		if (!word2)
			return false;

		userJoin ( new user ( ++uzivatelu, word,  remoteAddr, remoteLen, & fd ), word2 );

	}		
	else if ( strcmp ( word, "QUIT") == 0 )
	{
		// odpojeni uzivatele
		cout <<"Mess type QUIT" << endl;
		word = strtok ( NULL, " " );
		struct sockaddr remoteAddr;
		user * tmp = new user ( atoi ( word ), remoteAddr );
		set < user *, comp_user_id >::iterator itr = s_users.find ( tmp );
		userLeave ( new user ( atoi ( word ), remoteAddr ) );
		delete tmp;
	}
	else if ( strcmp ( word, "STOP") == 0 )
	{
		// killnout server
		cout <<"Mess type STOP" << endl;
		set < room *, comp_room >::iterator itr = s_rooms.begin();
		for ( ; itr != s_rooms.end(); ++itr )
		{
			(*itr)->remove ( );
			delete (*itr);
		
		}
		
		s_rooms.clear();
		s_users.clear();
		
		return false;
		
	}
	else 
	{
		// pridat zpravu
		int id = atoi( word );
		word = strtok ( NULL, ":" );
		cout << "<" << id << ">" << "<" << word << ">" << endl;
		
		struct sockaddr remoteAddr;
		user * tmp = new user ( id, remoteAddr );
		set < user *, comp_user_id >::iterator itr = s_users.find ( tmp );
		if ( itr == s_users.end() )
		{
			cout << "Somethink is wrong -> bad user ID: " << id << endl;
			return false;
		}
		cout << "Add new message" << endl;
		(*itr)->getMistnost()->sent ( new usrMess ( (*itr)->getName( ), word ) );
		delete tmp;
	}
	return true;

}

int server::openSrvSocket ( const char * name, int port )
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


	if ( bind ( fd, ai -> ai_addr, ai -> ai_addrlen ) == -1 )
	{
		close ( fd );
		freeaddrinfo ( ai );
		cout << "bind failed" << endl;
		return -1;
	}

	freeaddrinfo ( ai );

	return fd;
}

server::~server ( )
{
	while ( s_rooms.begin() != s_rooms.end() )
	{
		delete *s_rooms.begin();
		s_rooms.erase ( s_rooms.begin() );
	}
}


int server::init ( )
{
	int fd = openSrvSocket ( "127.0.0.1", 12345 );
	if ( fd < 0 ) return 1;

	char buffer[BUFSIZE];
	
	uzivatelu = 0;
	bool tmp = true;
	
	while ( tmp )
	{
		struct sockaddr remoteAddr;
		socklen_t remoteLen = sizeof ( remoteAddr );
        cout << "waiting on port 12345" << endl;

		int recvlen = recvfrom(fd, buffer, BUFSIZE, 0, &remoteAddr, &remoteLen);

        cout << "received " << recvlen << " bytes" << endl;			

		if (recvlen > 0)
		{
			buffer[recvlen] = '\0';
            cout << "message: " << buffer << endl;
			tmp = parse ( buffer, remoteAddr, remoteLen, fd );
		}
	}

	close ( fd );
	return 0;
}
