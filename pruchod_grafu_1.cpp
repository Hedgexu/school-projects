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

#define VAL int

struct Cesta;
class Mesto;

struct Cesta {

	Mesto * zMesta;
	Mesto * doMesta;
	VAL cenaCesty;
	
	Cesta ( Mesto * zMesta, Mesto * doMesta, const VAL cenaCesty ) 
	: zMesta ( zMesta ), doMesta ( doMesta ), cenaCesty ( cenaCesty )
	{ }
};


auto compCesta = [ ] ( const Cesta * a, const Cesta * b ) { return a->cenaCesty > b->cenaCesty; };
typedef priority_queue < Cesta *, vector < Cesta * >, decltype ( compCesta ) > CestaQueue;


class Mesto {

	public:
	
	VAL idMesta;
	bool vGrafu;
	bool expandovan;
	vector < Cesta > koleje;
	set < VAL > hranyGrafu;
	Mesto ( )
	{
		vGrafu = false;
		expandovan = false;
	}
	
	bool addLine ( Mesto * from, Mesto * to, VAL cena );
	void expand ( CestaQueue * fronta );
};

bool compare2 ( const Cesta cestaA, const Cesta cestaB ) { return cestaA.doMesta < cestaB.doMesta; }

void Mesto::expand ( CestaQueue * fronta )
{
	expandovan = true;
	for ( vector < Cesta >::iterator itr = koleje.begin(); itr != koleje.end(); ++itr )
		fronta->push ( & ( * itr ) );
}

bool Mesto::addLine ( Mesto * from, Mesto * to, VAL cena ) 
{
	Cesta tmp = Cesta ( from, to, cena );
	if ( !binary_search ( koleje.begin(), koleje.end(), tmp, compare2 ) )
	{
		vector < Cesta >::iterator itr = lower_bound ( koleje.begin(), koleje.end(), tmp, compare2 );
		koleje.insert ( itr, tmp );
		return true;
	}
		
	return false;
}


bool pridejMestuHranu ( Mesto * mestoA, Mesto * mestoB, VAL cenCesty )
{
	if ( mestoA->addLine ( mestoA, mestoB, cenCesty ) )
	{
		mestoB->addLine ( mestoB, mestoA, cenCesty );
		return true;
	}
	
	return false;
}

void printdata( const vector < Mesto > * mesta )
{
	for ( vector < Mesto > ::const_iterator itr1 = mesta->begin(); itr1 != mesta->end(); ++itr1 )
	{
		cout << (*itr1).idMesta;
		
		for ( vector < Cesta > :: const_iterator itr2 = (*itr1).koleje.begin(); itr2 != (*itr1).koleje.end(); ++itr2 )
			cout << " (" << (*itr2).zMesta->idMesta << ":" << (*itr2).doMesta->idMesta << ":" << (*itr2).cenaCesty << ")";
			
		cout << endl;
	}
}

void cd( const char * inFile, const char * outFile )
{
	VAL pocetMest, pocetCest, mestoA, mestoB, cenaCesty, i;
	char znak;
	VAL cena = 0;

	ifstream reading;
	ofstream writing;

    reading.open ( inFile, ios::in );
    
    if ( !reading.good ( ) )
    	return;
    	
    writing.open ( outFile, ios::out );
    
    if ( ! writing.good ( ) )
        return; 

    reading >> pocetMest >> pocetCest; 
    vector < Mesto > mesta ( pocetMest );
    
    for ( i = 0; i < pocetMest; i++ )
    	mesta [ i ] . idMesta = i;
    	
    while ( true )
    {
    	reading >> mestoA >> mestoB;
    	
    	Mesto * mA = & mesta [ mestoA ];
    	Mesto * mB = & mesta [ mestoB ];
		
    	pridejMestuHranu ( mA, mB, 0 );
		mA->hranyGrafu.insert( mestoB );
		mB->hranyGrafu.insert( mestoA );
    	
    	if ( reading.peek() != ',' )
    		break;
    		
    	reading >> znak;
    }
    
    for ( i = 0; i < pocetCest; i++ )
    {
    	reading >> mestoA >> mestoB >> cenaCesty;
    	
    	if ( !pridejMestuHranu ( & mesta [ mestoA ], & mesta [ mestoB ], cenaCesty ) )
    		cena += cenaCesty;
    }
    
    reading.close();
//    printdata( & mesta );
    CestaQueue prioritniFronta ( compCesta );
    
    mesta [ 0 ].expand ( & prioritniFronta );

	bool done = false;
	vector < Mesto >::iterator itr1 = mesta.begin();
	
	while ( !done )
	{
		while ( !prioritniFronta.empty() ) 
		{
			Cesta * tmp = prioritniFronta.top();
			prioritniFronta.pop();
			Mesto * mA = tmp->zMesta;
    		Mesto * mB = tmp->doMesta;

			if ( !mB->expandovan )
			{
				mA->hranyGrafu.insert( mB->idMesta );
				mB->hranyGrafu.insert( mA->idMesta );
				cena += tmp->cenaCesty;
				mB->expand ( & prioritniFronta );
			}
    	}
    	
    	done = true;
    	
    	for ( ; itr1 != mesta.end(); ++itr1 )
    	{
    		if ( !(*itr1).expandovan )
    		{
    			(*itr1).expand ( & prioritniFronta );
    			done = false;
    			break;
    		}
    	} 
    }
    
    writing << cena << endl;
    
    for ( itr1 = mesta.begin(); itr1 != mesta.end(); ++itr1 )
    {
    	writing << (*itr1).idMesta;
    	
    	for ( auto itr2 = (*itr1).hranyGrafu.begin(); itr2 != (*itr1).hranyGrafu.end(); ++itr2 )
    		writing << " " << (*itr2);
    		
    	writing << endl;
    		
    }
    
    writing.close();
}


#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
	cd ( "ukol1_in0.txt" , "ukol1_out0.txt" );
	return 0;
}
#endif /* __PROGTEST__ */
