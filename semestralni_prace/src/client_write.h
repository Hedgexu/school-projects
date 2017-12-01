#ifndef CLIENT_WRITE_H
#define	CLIENT_WRITE_H

#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <ncurses.h>

using namespace std;

/*
 * Initialization of client and waiting for user input
 */
class client_write
{
	/**
	 * userid from server
	 */
	int userid;
	
	/**
     * File descriptor for connection
     */
	int fd;
	
	/**
	 * ncurses WINDOW for getstr( ) from user
	 */
	WINDOW * win_write;
	
	/*
	 * sockaddr_in for connection
	 */
	struct sockaddr_in serverAddr;
	
	/**
     * size of sockaddr_in
     */
	socklen_t serverAddrLen;
	
	/**
	 * Bool used from second thread as pointer of status
	 */
	bool stop;
	
	/**
	 * Open CliSocket to server (UDP - inspiration Edux TCP)
	 * @param name of server
     * @param port port in server
	 */
	int openCliSocket ( const char * name, int port );

	
	public:
	
	/**
	 * Initialization of variables.
	 * @param userName name of user
	 * @param roomName room of user
	 */
	int init ( const string & userName, const string & roomName  );
	
	/**
	 * Run method this class, wainting for user input
	 */
	void run ( );
	
	/**
	 * Split a message from user and call sendxxxx
	 * @param str message from user
	 */
	bool parse ( char * str );
	
	/**
	 * Send normal message to server
	 * @param str text of message
	 */
	void sendMess ( char * str );
	
	/**
	 * Send STOP message to server (kill server)
	 */
	void sendSTOP ( );
	
	/**
	 * Send QUIT message to server (user disconnected)
	 */
	void sendQUIT ( );
};

#endif	/* CLIENT_WRITE_H */
