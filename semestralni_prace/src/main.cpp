#include <iostream>

#include "client_write.h"
#include "client_listen.h"
#include "server.h"

using namespace std;

int main ( int argc, char * argv [] )
{
	string tmp1, tmp2;

	if ( argc == 2 )
	{
		tmp1 = (string) argv[1];
		if ( tmp1 != "-server" )
		{
			 cout << "Formats:" << endl;
			 cout << "./novotad1 -server" << endl;
			 cout << "./novotad1 <username> <roomname>" << endl;
			 return 1;
		
		}
			
		server s1;
		return s1.init();
	}
	else if ( argc == 3 )
	{
		tmp1 = (string) argv[1];
		tmp2 = (string) argv[2];
		client_write cl;
		return cl.init ( tmp1, tmp2 );
	}
	
	cout << "Formats:" << endl;
	cout << "./novotad1 -server" << endl;
	cout << "./novotad1 <username> <roomname>" << endl;
	return 1;


}
