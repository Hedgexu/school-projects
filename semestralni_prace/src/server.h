#ifndef SERVER_H
#define	SERVER_H

#include "room.h"

#include <set>

using namespace std;
/**
 * Main class in server
 */
class server
{
	/**
	 * set with poiters to all users
	 */
	set < user *, comp_user_id > s_users;	
	
	/**
	 * set with pointers to all rooms
	 */
	set < room *, comp_room >  s_rooms;
	
	/**
	 * counter used for new userID
	 */
	unsigned int uzivatelu;
	
	/**
	 * User want leave server
	 *  @param usr pointer to user
	 */
	void userLeave ( user * usr );
	
	/**
	 * New user want connect to server
	 * @param usr pointer to user
	 * @param roomname name of room
	 */
	void userJoin ( user * usr , const char * roomname  );
	
	/**
	 * Waiting for new message and processing message
	 * @param msg text of message
	 * @param remoteAddr sockaddr
	 * @param remoteLen size of sockaddr
	 * @param fd file descriptor
	 */
	bool parse ( const char * msg, struct sockaddr & remoteAddr, socklen_t & remoteLen, const int fd );
	
	/**
	 * Seting for UDP connection
	 * @param name ip adress of server
	 * @param port port on server
	 */
	int openSrvSocket ( const char * name, int port );
	
	public:
	
	/**
	 * Destruction server and clean all variambles 
	 */
	~server ( );
	
	/**
	 * Initialization of server
	 */
	int init ( );
	
	
};

#endif	/* SERVER_H */
