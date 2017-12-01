#ifndef CLIENT_LISTEN_H
#define	CLIENT_LISTEN_H

#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <ncurses.h>

using namespace std;

/**
 * Waiting for messages from the server
 */
class client_listen
{
    /**
     * File descriptor
     */
	int fd;
        
    /**
     * sockaddr_in
     */
	struct sockaddr_in serverAddr;
        
    /**
     * ncurses WINDOW for printing messages
     */
	WINDOW * win_listen;
        
    /**
     * ncurses WINDOW for loading old positin od cursor
     */
	WINDOW * win_write;
        
    /**
     * X coordinate of cursor
     */
	int cursorX;
        
    /**
     * Y coordinate of cursor
     */
	int cursorY;
        
    /**
     * Vector < string > with messages
     */
	vector < string > messages;
        
    /**
     * Size of struct sockaddr_in serverAddr
     */
	socklen_t serverAddrLen;
        
    /**
     * Number of lines ncurses WINDOW win and max size of vector
     */
	int lines;
        
    /**
     * Pointer to bool in second class on main thread
     */
	bool * stop;
        
	/**
     * Get cursor back to old position
     */
	void LoadCursorPosition ( );

    /**
     * Saving coordinates of cursor
     */
	void storeCursorPosition ( );

	public:
            
    /**
     * Initialization of variables.
     * @param fd int fd
     * @param serverAddr struct sockaddr_in
     * @param win ncurses WINDOW
     * @param win2 ncurses WINDOW for returning cursor
     * @param stop pointer to bool
     */
	client_listen ( int fd, struct sockaddr_in serverAddr, WINDOW * win, WINDOW * win2, bool * stop );
        
    /**
     * Print all from vector < string > to WINDOW
     */
	void print ( );
        
    /**
     * Method waiting for incoming messages
     */
	void init ( );
        
    /**
     * Add new string to vector 
     * @param mess string
     */
	void addMess ( string &  mess );
};

#endif	/* CLIENT_LISTEN_H */
