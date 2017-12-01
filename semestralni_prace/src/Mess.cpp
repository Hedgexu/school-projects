#include "Mess.h"

using namespace std;

const string currentDateTime (  )
{
    time_t now = time( 0 );
    struct tm tstruct;
    char buf[80];
    tstruct = * localtime ( & now );
    strftime( buf, sizeof( buf ), /*%d.%m.%Y */"%X", & tstruct );
    return buf;
}

Mess::Mess( const string & userName ) : m_User( userName ), m_DateTime( currentDateTime() )
{ }

usrMess::usrMess ( const string & userName, const string & messText ) : Mess( userName ), m_Text( messText )
{ } 

string usrMess::printMess(  ) const
{
    return ( m_DateTime + " <" + m_User + "> : " + m_Text + "\n" );
}

svrMess::svrMess ( const string & userName, const int version ) : Mess( userName ), m_Version( version )
{  }

string svrMess::printMess (  ) const
{
    switch ( m_Version )
    {
        case 1:
        	return ( m_DateTime  + " : " + m_User + " join this channel.\n" );
            break;
        case 2:
            return ( m_DateTime  + " : " + m_User + " leave this channel.\n" );
            break;
        default:
            return ( "Default system message.\n" );
            break;
    }
}
