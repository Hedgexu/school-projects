#ifndef MESS_H
#define	MESS_H

#include <string>
#include <cstring>

using namespace std;

/**
 * class with virtual method
 */
class Mess 
{
    protected:
    
    /**
     * Name of user
     */
    string m_User;
    
    /**
     * Time of message
     */ 
    string m_DateTime;
    
    public:
    
    /**
     * Initialization of variables.
     * @param userName is author of message
     */
    Mess ( const string & userName );
    
    /**
     * Pure virtual method
     */
    virtual string printMess (  ) const = 0;
    
    /**
     * Virtual destructor
     */
    virtual ~Mess() { }
};

/*
 * Message from user
 */
class usrMess : public Mess
{
	/**
	 * Text of message
	 */
    string m_Text;
    
    public:
    
    /**
     * Initialization of variables.
     * @param userName is author of message
     * @param messText is text of message
     */
    usrMess ( const string & userName, const string & messText );
    
    /**
     * Convert Mess to string
     * @return message as string
     */
    string printMess (  ) const;   
};

/**
 * Message from server
 */ 

class svrMess : public Mess
{
	/**
	 * Type of system message
	 */
    int m_Version;
    
    public:
    
    /**
     * Initialization of variables.
     * @param userName is originator of message
     * @param version is version of system message
     */
    svrMess ( const string & userName, const int version );
    
    /**
     * Convert Mess to string
     * @return message as string
     */
    string printMess (  ) const;
};

#endif	/* MESS_H */
