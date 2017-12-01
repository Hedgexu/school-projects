#ifndef ROOM_H
#define	ROOM_H

using namespace std;

#include "Mess.h"

#include <set>

#include <netinet/in.h>

class room;

/**
 * This class keep informations about clients
 */ 
class user
{
	/**
	 * Name of room
	 */
	const string u_name;
	
	/**
	 * sockaddr for sending messages.
	 */
	const struct sockaddr address;
	
	/**
	 * Size of struct sockaddr address
	 */
	const socklen_t addLen;
	
	/**
	 * Uniq userid
	 */
	const unsigned int u_id;
	
	/**
	 * Pointer to room of user
	 */
	room * u_room;
	
	/**
	 * File descriptor for sending messages
	 */
	const int * fd;
	
	public:
	
	/**
	 * Initialization of variables
	 * @param u_id uniq ID of user
	 * @param u_name name of user
	 * @param addr sockaddr
	 * @param len size of struct sockaddr
	 * @param fd file descriptor
	 */
	user ( const unsigned int u_id, const char * u_name, struct sockaddr & addr, socklen_t & len, const int * fd );
	
	/**
	 * Initialization of variables for temporary users
	 * @param u_id uniq ID of user
	 * @param addr sockaddr
	 */
	user ( const unsigned int u_id, struct sockaddr & addr );
	
	/**
	 * Getter for u_room 
	 * @return pointer to user room
	 */
	room * getMistnost ( ) const;
	
	/**
	 * Getter for u_name
	 * @return string user name
	 */
	string getName ( ) const;
	
	/**
	 * Getter for u_name u_id
	 * @return int user id
	 */
	int getId ( ) const;
	
	/**
	 * Setter for u_room
	 * @param u_room pointer to room os user
	 */ 
	void setMistnost ( room * u_room );
	
	/**
	 * Send message this user
	 * @param text is text of message
	 */
	void sentMessage ( const string & text );
	
	/**
	 * Remove client from room
	 * @param stom is type off quit 
	 */
	void quit ( bool stop );
};

/**
 * comparator for users
 * @param first user 1
 * @param second user 2
 */
struct comp_user_id 
{
    bool operator () ( const user * first, const user * second ) const
    {
        return first->getId() < second->getId();
    }
};


/**
 * This class keep informations about rooms
 */
class room 
{
	/**
	 * Name of room
	 */
	const string r_name;
	
	/**
	 * Set with pointers to all users in room
	 */
	set < user *, comp_user_id > r_users;
	
	public:	
	
	/**
	 * Initialization of variables.
	 * @param name is name of this room
	 */
	room ( const string &  name );
	
	/**
	 * Send message to all users in room
	 * @param msg is message
	 */
	void sent ( Mess * msg ) const;
	
	/**
	 * Clear room from users
	 */
	void remove ( );
	
	/**
	 * Test if this room is empty
	 * @return bool if this room is without users
	 */
	bool isClear ( ) const;
	
	/**
	 * Getter to r_name
	 * @return string name of room
	 */
	string getName ( ) const;
	
	/**
	 * Add new user to this room
	 * @param usr is pointer to user
	 */
	void addUser ( user * usr );
	
	/**
	 * Remove user from this room
	 * @param usr is pointer to user
	 */
	void removeUser ( user * usr );
};

/**
 * comparator for rooms
 * @param first room 1
 * @param second room 2
 */
struct comp_room 
{
    bool operator () ( const room * first, const room * second ) const
    {
        return first->getName() < second->getName();
    }
};

#endif	/* ROOM_H */


