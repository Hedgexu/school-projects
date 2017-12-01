#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <list>
#include <stdint.h>

using namespace std;
#endif /* __PROGTEST__ */

class Uzel;

int counter = 0;
vector < Uzel * > prevs;
set < int > out;

class Uzel 
{
	public:
	map < int, Uzel * > hrany;
	map < int, Uzel * > hranyI;
	int otevren;
	int zavren;
	int oznaceni;
	int cisloKomp;
	
	Uzel ( ) : otevren ( -1 ) , zavren ( -1 ), oznaceni ( -1 ), cisloKomp ( -1 )
	{ }
	
	void checkIt ( int cislo )
	{
		if ( cisloKomp == -1 )
		{
			cisloKomp = cislo;
			
			auto itr = 	hranyI.begin( );

			while ( true )
			{
				if ( itr == hranyI.end( ) )
					break;
				
				(*itr).second->checkIt2 ( cislo );
				itr++;
			}
		}
	}
	
	void checkIt2 ( int cislo )
	{
		if ( cisloKomp == -1 )
		{
			cisloKomp = cislo;
			
			auto itr = 	hranyI.begin( );

			while ( true )
			{
				if ( itr == hranyI.end( ) )
					break;
				
				(*itr).second->checkIt2 ( cislo );
				itr++;
			}
		}
		else if ( cislo != cisloKomp )
			out.insert (  oznaceni );
	}
	
	void expand ( )
	{
		if ( otevren != -1 )
			return;
			
		otevren = ++counter;	
			
		auto itr = 	hrany.begin( );

		while ( true )
		{
			if ( itr == hrany.end( ) )
				break;
				
			(*itr).second->expand ( );
			itr++;
		}
		
		zavren = ++counter;
		prevs.insert ( prevs.begin( ), this );
	}
};

void pridejHranu ( Uzel * from, Uzel * to )
{
	from->hrany.insert ( pair < int, Uzel * > ( to->oznaceni, & (*to) ) );
	to->hranyI.insert ( pair < int, Uzel * > ( from->oznaceni, & (*from) ) );
}

void printData ( const vector < Uzel > * uzly )
{
	for ( auto itr1 = uzly->begin( ); itr1 != uzly->end( ); ++itr1 )
		cout << (*itr1).oznaceni << " --> " << (*itr1).otevren << " : " << (*itr1).zavren << endl;

}

void fit ( const char * inFile, const char * outFile )
{
	counter = 0;
	prevs.clear();
	out.clear();


	int pocetUzlu, from, to;

	ifstream reading;
	ofstream writing;

    reading.open ( inFile, ios::in );
    
    if ( !reading.good( ) )
    	return;
    	
    writing.open ( outFile, ios::out );
    
    if ( ! writing.good( ) )
        return; 
        
    reading >> pocetUzlu;
    vector < Uzel > uzly ( pocetUzlu );
    for ( int i = 0; i < pocetUzlu; i++ )
    	uzly [ i ] . oznaceni = i;
    
    while ( true )
    {
    	reading >> from >> to;
    	if ( !reading.good( ) )
    		break;
    		
    	pridejHranu ( & uzly [ from ], & uzly [ to ] );
    }
    
    reading.close();
    
    for ( int i = 0; i < pocetUzlu; i++ )
    	uzly [ i ] . expand( );
    	
	for ( auto itr = prevs.begin( ); itr != prevs.end( ); ++itr )
		(*itr)->checkIt( (*itr)->oznaceni );
		
	for ( auto itr = out.begin( ); itr != out.end( ); ++itr )
		writing << (*itr) << endl;

    writing.close();
}


#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
	fit ( "ukol2_in0.txt" , "ukol2_out0.txt" );
	fit ( "ukol2_in3.txt" , "ukol2_out3.txt" );
	return 0;
}
#endif /* __PROGTEST__ */
