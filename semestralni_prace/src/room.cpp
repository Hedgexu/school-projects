#include "room.h"

#include <iostream>

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------

user::user ( const unsigned int u_id, const char * u_name, struct sockaddr & addr, socklen_t & len, const int * fd ) 
: u_name ( ( string ) u_name ), address ( addr ), addLen ( len ), u_id ( u_id ), fd (fd)
{
	char msg[5];
	snprintf(msg, sizeof(msg), "%u", u_id);
	cout << "uzivatelske id: " << msg << endl;
	sentMessage ((string)msg);
}

user::user ( const unsigned int u_id, struct sockaddr & addr ) : address ( addr ), addLen ( 0 ), u_id ( u_id )
{ }

room * user::getMistnost ( ) const
{
	return u_room;
}

string user::getName ( ) const
{
	return u_name;
}

int user::getId ( ) const
{
	return u_id;
}

void user::setMistnost ( room * u_room )
{
    this->u_room = u_room; 
}

void user::sentMessage ( const string & text )
{
	sendto ( *fd, text.c_str(), text.size(), 0, & address, addLen );
}

void user::quit ( bool stop )
{
	string msg;
	if ( stop )
	{
		msg = "Server is off. Press enter to exit the client.";
		sentMessage ( msg );
	}
	msg = "QUIT";
	sentMessage ( msg );
}



room::room ( const string &  name ) : r_name ( name )
{ }

void room::sent ( Mess * msg ) const
{
	string text = msg->printMess();
	set < user *, comp_user_id >::const_iterator itr = r_users.begin();
	for ( ; itr != r_users.end(); ++itr )
	{
		(*itr)->sentMessage ( msg->printMess().c_str() ); 
	}

	delete msg;	
}

void room::remove ( )
{
	set < user *, comp_user_id >::iterator itr =  r_users.begin();	
	for ( ; itr != r_users.end(); ++itr )
	{
		user * tmp = *itr;
		tmp->quit(true);
		delete tmp;
	}
	r_users.clear();
}

bool room::isClear ( ) const
{
	return r_users.size() == 0;
}

string room::getName ( ) const
{
	return ( string ) r_name;
}

void room::addUser ( user * usr )
{
	r_users.insert ( usr );
}

void room::removeUser ( user * usr )
{
	r_users.erase ( usr );
}
