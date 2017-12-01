#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#if defined (__cplusplus) && __cplusplus > 199711L
/* C++ 11 */
#include <thread>
#include <mutex>     
#include <condition_variable>
#endif /* __cplusplus */

using namespace std;

struct TRect {
    int m_X;
    int m_Y;
    int m_W;
    int m_H;
};

struct TCostProblem {
    int ** m_Values;
    int m_Size;
    int m_MaxCost;
    void ( * m_Done ) ( const TCostProblem *, const TRect * );
};

struct TCrimeProblem {
    double ** m_Values;
    int m_Size;
    double m_MaxCrime;
    void (* m_Done ) ( const TCrimeProblem *, const TRect * );
};

#endif /* __PROGTEST__ */

#define VAL long long int

bool FindByCost( int ** values, int size, int maxCost, TRect * res );
bool FindByCrime ( double ** values, int size, double maxCrime, TRect * res );

class problem {
	public:
	virtual void solve ( ) = 0;
	virtual ~ problem ( )
	{ }
};

class problemCost : public problem {

	public:
	const TCostProblem * data;
	
	problemCost ( const TCostProblem * data ) : data ( data )
	{ }
	
	~problemCost ( )
	{ }

	void solve (  )
	{
		TRect * res = new TRect;
		if ( FindByCost ( data->m_Values, data->m_Size, data->m_MaxCost, res ) )
			data->m_Done ( data, res );
		else 
			data->m_Done ( data, NULL );
		
		delete res;
	}	
	
};

class problemCrime : public problem {

	public:
	const TCrimeProblem * data;

	problemCrime ( const TCrimeProblem * data ) : data ( data )
	{ }
	
	~problemCrime ( )
	{ }

	void solve (  )
	{
		TRect * res = new TRect;
		if ( FindByCrime ( data->m_Values, data->m_Size, data->m_MaxCrime, res ) )
			data->m_Done ( data, res );
		else 
			data->m_Done ( data, NULL );
		
		delete res;
	}	
};

typedef const TCrimeProblem * ( * crimeFuncForInput ) ( void );
typedef const TCostProblem * ( * costFuncForInput ) ( void );

struct dataThread {

	pthread_mutex_t  g_Mtx;
	sem_t g_Full, g_Free;
	queue < problem * > fronta;
	
};

struct input1 {
	dataThread * dataT;
	crimeFuncForInput funkce;
};

struct input2 {
	dataThread * dataT;
	costFuncForInput funkce;
};

void * problemCrimeIn ( void * inputTmp )
{
	input1 * input = ( input1 * ) inputTmp;
	
	while ( true )
	{
		const TCrimeProblem * vstup = input->funkce ();

		if ( vstup == NULL )
			break;
		
		sem_wait ( & input->dataT->g_Free );
		pthread_mutex_lock ( & input->dataT->g_Mtx );
		input->dataT->fronta.push ( new problemCrime ( vstup ) );
		pthread_mutex_unlock ( & input->dataT->g_Mtx );
      	sem_post ( & input->dataT->g_Full );
	}
	
	return NULL;
}

void * problemCostIn ( void * inputTmp )
{
	input2 * input = ( input2 * ) inputTmp;
	while ( true )
	{
		const TCostProblem * vstup = input->funkce ();
		
		if ( vstup == NULL )
			break;
	
		sem_wait ( & input->dataT->g_Free );
		pthread_mutex_lock ( & input->dataT->g_Mtx );
		input->dataT->fronta.push ( new problemCost ( vstup ) );
   		pthread_mutex_unlock ( & input->dataT->g_Mtx );
      	sem_post ( & input->dataT->g_Full );
	}
	
	return NULL;
}

void * problemSolve ( void * inputTmp )
{
	dataThread * input = ( dataThread * ) inputTmp;

	problem * vstup;
	while ( true )
	{
		sem_wait ( & input->g_Full );
		pthread_mutex_lock ( & input->g_Mtx );
		vstup = input->fronta.front();
		if ( vstup != NULL )
		{
			input->fronta.pop();
			pthread_mutex_unlock ( & input->g_Mtx );
			sem_post ( & input->g_Free );
			vstup->solve();
			delete vstup;
		}
		else
		{
	   		pthread_mutex_unlock ( & input->g_Mtx );
      		sem_post ( & input->g_Full );
      		break;
		}
	}
	return NULL;
}

void MapAnalyzer( int threads, const TCostProblem * ( * costFunc ) ( void ), const TCrimeProblem * ( * crimeFunc ) ( void ) ) 
{
	// variables
	pthread_t prod1, prod2;
	pthread_attr_t attr;
	pthread_t * cons = new pthread_t [ threads ];
	dataThread dataProVlakna;
	input1 in1;
	input2 in2;
	
	// inicializace
	pthread_attr_init ( & attr );
	pthread_attr_setdetachstate ( & attr, PTHREAD_CREATE_JOINABLE );
	pthread_mutex_init ( & dataProVlakna.g_Mtx, NULL );
	
   	sem_init ( & dataProVlakna.g_Free, 0, 10 * threads );
   	sem_init ( & dataProVlakna.g_Full, 0, 0 );
   	
   	in1.funkce = crimeFunc;
   	in1.dataT =  & dataProVlakna;

   	in2.funkce = costFunc;
   	in2.dataT =  & dataProVlakna;
   	
   	// vytvoreni producentu	
   	pthread_create ( & prod1, & attr, ( void * ( * ) ( void * ) ) problemCrimeIn, & in1 );
   	pthread_create ( & prod2, & attr, ( void * ( * ) ( void * ) ) problemCostIn, & in2 );

   	// vytvoreni konzumentu
   	for ( int i = 0; i < threads; i++ )
   		pthread_create ( & cons [ i ], & attr, ( void * ( * ) ( void * ) ) problemSolve, & dataProVlakna );
   		
   	// pripojeni producentu
   	pthread_join ( prod1, NULL ); 
	pthread_join ( prod2, NULL );

   	// vlozeni neplatnych dat ( z duvodu ukonceni konzumentu )      	
    sem_wait ( & dataProVlakna.g_Free );
    pthread_mutex_lock ( & dataProVlakna.g_Mtx );
   	dataProVlakna.fronta.push ( NULL );
   	pthread_mutex_unlock ( & dataProVlakna.g_Mtx );
   	sem_post ( & dataProVlakna.g_Full );
   	
   	// pripojeni consumentu
   	for ( int i = 0; i < threads; i++ )
   		pthread_join ( cons [ i ] , NULL );	
   	 
   	// uklid
   	pthread_attr_destroy ( & attr ); 
   	pthread_mutex_destroy ( & dataProVlakna.g_Mtx );
   	sem_destroy ( & dataProVlakna.g_Free );
  	sem_destroy ( & dataProVlakna.g_Full );
  	delete [ ] cons;
}

bool FindByCost( int ** values, int size, int maxCost, TRect * res ) 
{
    // variables
    int a, b, c, d;
    VAL tmp_max = 0;
    
    if ( size < 1 )
    	return false;
    	
    // alokace pomocne matice
    VAL ** matice = new VAL * [ size ];
    for ( a = 0; a < size; a++ )
    	matice [ a ] = new VAL [ size ]; 
    	
    // pocatecni inicializace pomocne matice
    matice [ 0 ] [ 0 ] = values [ 0 ] [ 0 ];
    for ( a = 1; a < size; a++ )
    {
    	matice [ a ] [ 0 ] = matice [ a - 1 ] [ 0 ] + (VAL) values [ a ] [ 0 ];
    	matice [ 0 ] [ a ] = matice [ 0 ] [ a - 1 ] + (VAL) values [ 0 ] [ a ];
    }
    
    // vyplneni matice
    for ( a = 1; a < size; a++ )
    {
    	for ( b = 1; b < size; b++ )
    		matice [ a ] [ b ] = (VAL) values [ a ] [ b ] + matice [ a - 1 ] [ b ] + matice [ a ] [ b - 1 ] - matice [ a - 1 ] [ b - 1 ];	
    }

    // pruchod prvniho radku a sloupce
    for ( a = 0; a < size; a++ )
    {
    	if ( a + 1 > tmp_max && matice [ a ] [ 0 ] <= maxCost ) {
			tmp_max = a + 1; res->m_X = 0; res->m_Y = 0; res->m_W = 1; res->m_H = a; continue; }
		else if ( a + 1 > tmp_max && matice [ 0 ] [ a ] <= maxCost ) {
			tmp_max = a + 1; res->m_X = 0; res->m_Y = 0; res->m_W = a; res->m_H = 1; continue; }
		
		for ( b = 0; b < a; b++ )
		{
			if ( a - b > tmp_max && matice [ a ] [ 0 ] - matice [ b ] [ 0 ] <= maxCost ) {
				tmp_max = a - b; res->m_X = 0; res->m_Y = b + 1; res->m_W = 1; res->m_H = a - b; break; }
			else if ( a - b > tmp_max && matice [ 0 ] [ a ] - matice [ 0 ] [ b ] <= maxCost ) {
				tmp_max = a - b; res->m_X = b + 1; res->m_Y = 0; res->m_W = a - b; res->m_H = 1; break; }
		}
    }
    
    // pruchod zbytku matice
    for ( a = 1; a < size; a++ )
    {
    	for ( b = 1; b < size; b++ )
    	{
    		if ( ( a + 1 ) * ( b + 1 ) > tmp_max && matice [ a ] [ b ] <= maxCost ) {
    			tmp_max = ( a + 1 ) * ( b + 1 ); res->m_X = 0; res->m_Y = 0; res->m_W = b + 1; res->m_H = a + 1; continue; }
    		
    		for ( c = 0; c < b; c++ )
    		{
    			if ( ( a + 1 ) * ( b - c ) < tmp_max ) break;
    			else if ( ( a + 1 ) * ( b - c ) > tmp_max && matice [ a ] [ b ] - matice [ a ] [ c ] <= maxCost ) {
    				tmp_max = ( a + 1 ) * ( b - c ); res->m_X = c + 1; res->m_Y = 0; res->m_W = b - c; res->m_H = a + 1; break; }
    		}
    		
    		for ( c = 0; c < a; c++ )
    		{
    			if ( ( b + 1 ) * ( a - c ) < tmp_max ) break;
    			else if ( ( b + 1 ) * ( a - c ) > tmp_max && matice [ a ] [ b ] - matice [ c ] [ b ] <= maxCost ) {
    				tmp_max = ( b + 1 ) * ( a - c ); res->m_X = 0; res->m_Y = c + 1; res->m_W = b + 1; res->m_H = a - c; break; }
    				
    			for ( d = 0; d < b; d++ )
    			{
    				if ( ( b - d ) * ( a - c ) < tmp_max ) break;
    				else if ( ( b - d ) * ( a - c ) > tmp_max && matice [ a ] [ b ] - matice [ c ] [ b ] - matice [ a ] [ d ] + matice [ c ] [ d ] <= maxCost ) {
    					tmp_max = ( b - d ) * ( a - c ); res->m_X = d + 1; res->m_Y = c + 1; res->m_W = b - d; res->m_H = a - c; break; }
    			}
    		}
    	}
    }
    
    // uklid
    for ( a = 0; a < size; a++ )
        delete [] matice [ a ];
    
    delete [] matice;
    
    if ( tmp_max > 0 )
    	return true;

    return false;
}

bool FindByCrime ( double ** values, int size, double maxCrime, TRect * res ) 
{
    // variables
	VAL tmp1 = 0;
	VAL tmp2 = (VAL) size * (VAL) size * (VAL) size;
    while ( tmp1 < tmp2 )
    { tmp1++; }
     
    int x, y, i, tp;
    VAL tmp_max, area_with_top;
    stack<int> zasobnik;
    
    if ( size < 1 )
    	return false; 
    
    // alokace pomocne matice
    int ** matice = new int * [ size ];	
   	for ( y = 0; y < size; y++ )
        matice [ y ] = new int [ size ];

    // pocatecni inicializace pomocne matice
    for ( y = 0; y < size; y++ )
    {
        if ( values [ size - 1 ] [ y ]  <= maxCrime )
            matice [ size - 1 ] [ y ] = 1;
        else
            matice [ size - 1 ] [ y ] = 0;
    }
    
    // vyplneni matice
    for ( x =  size - 2; x >= 0; x-- )
    {
    	for ( y = 0; y < size; y++ )
    	{
    		if ( values [ x ] [ y ] <= maxCrime ) 
    			matice [ x ] [ y ] = matice [ x + 1 ] [ y ] + 1;
    		else
    			matice [ x ] [ y ] = 0;
    	}
    }
    
    // hledani maxima v histogramech pomoci zasobniku - zdroj pseudokod z internetu
    tmp_max = 0;
    
    for ( x = 0; x < size; x++ )
    {
    	i = 0;
    	
    	while ( i < size )
		{
		    if ( zasobnik.empty ( ) || matice [ x ] [ zasobnik.top ( ) ] <= matice [ x ] [ i ] )
		    	zasobnik.push( i++ );
		   	else
		  	{
		    	tp = zasobnik.top ( );
		        zasobnik.pop ();

		        area_with_top = (VAL) matice [ x ] [ tp ] * (VAL) ( zasobnik.empty ( ) ? i : i - zasobnik.top ( ) - 1 );

		        if ( tmp_max < area_with_top )
		        {
					tmp_max = area_with_top;
					res->m_W = zasobnik.empty() ? i : i - zasobnik.top() - 1;
					res->m_H = matice [ x ] [ tp ];
					res->m_X = i - res->m_W;
					res->m_Y = x;

		        }
		    }
		}

	   while ( !zasobnik.empty() )
		{
		    tp = zasobnik.top();
		    zasobnik.pop();
		    area_with_top = (VAL) matice [ x ] [ tp ] * (VAL) ( zasobnik.empty() ? i : i - zasobnik.top() - 1 );

		    if ( tmp_max < area_with_top )
		    {
		        tmp_max = area_with_top;
				res->m_W = zasobnik.empty() ? i : i - zasobnik.top() - 1;
				res->m_H = matice [ x ] [ tp ];
				res->m_X = i - res->m_W;
				res->m_Y = x;
			}
		}
    }
    
    // uklid
    for ( y = 0; y < size; y++ )
        delete [] matice [ y ];
    
    delete [] matice;
    
    if ( tmp_max == 0 )
    	return false;
    	
    return true;
}

//-------------------------------------------------------------------------------------------------


#ifndef __PROGTEST__
pthread_mutex_t g_Mtx;
int g_Cost, g_Crime;

//-------------------------------------------------------------------------------------------------
void costNotify ( const TCostProblem * p, const TRect * r )
{
	printf ( "costNotify\n" );
	for ( int i = 0; i < p -> m_Size; i ++ )
		delete [] p -> m_Values[i];

	delete [] p -> m_Values;
	delete p;
}                                                             

//-------------------------------------------------------------------------------------------------

const TCostProblem * costGeneratorFunc ( void )
 {
   printf ( "costGen\n" );
   pthread_mutex_lock ( & g_Mtx );
   bool gen = g_Cost-- > 0;
   pthread_mutex_unlock ( & g_Mtx );
   
   if ( ! gen ) return NULL;
   
   TCostProblem * p = new TCostProblem;
   
   int size 
   =  ( int ) ( 10 + 100.0 * rand () / RAND_MAX );
   int ** mat = new int * [ size ];
   for ( int y = 0; y < size; y ++ )
    {
      mat[y] = new int [size];
      for ( int x = 0; x < size; x ++ )
       mat[y][x] = (int)( -500 + 2000.0 * rand () / RAND_MAX );
    }
  
   p -> m_Values  = mat;
   p -> m_Size    = size;
   p -> m_MaxCost = (int)(500.0 * size * size * rand () / RAND_MAX );
   p -> m_Done    = costNotify;
   return p;
 }

//-------------------------------------------------------------------------------------------------

void crimeNotify ( const TCrimeProblem * p, const TRect * r )
{
	printf ( "crimeNotify\n" );
	for ( int i = 0; i < p -> m_Size; i ++ )
		delete [ ] p -> m_Values [ i ];

	delete [ ] p -> m_Values;
	delete p;
}                                                  

//-------------------------------------------------------------------------------------------------

const TCrimeProblem * crimeGeneratorFunc ( void )
{
	printf ( "crimeGen\n" );
	pthread_mutex_lock ( & g_Mtx );
	bool gen = g_Crime-- > 0;
	pthread_mutex_unlock ( &g_Mtx );

	if ( ! gen ) 
		return NULL;

	TCrimeProblem * p = new TCrimeProblem;

	int size = ( int ) ( 30 + 200.0 * rand ( ) / RAND_MAX );
	double ** mat = new double * [ size ];
	for ( int y = 0; y < size; y ++ )
	{
		mat [ y ] = new double [ size ];
		for ( int x = 0; x < size; x++ )
   		mat [ y ] [ x ] = 1000.0 * rand ( ) / RAND_MAX;
	}

	p -> m_Values = mat;
	p -> m_Size  = size;
	p -> m_MaxCrime = 1000.0 * rand () / RAND_MAX;
	p -> m_Done = crimeNotify;
	return p;
}
 
//-------------------------------------------------------------------------------------------------

void oneTest ( int threads, int cost, int crime )
{
	pthread_mutex_init ( & g_Mtx, NULL );
	g_Cost = cost;
	g_Crime = crime;
	MapAnalyzer ( threads, costGeneratorFunc, crimeGeneratorFunc );
	pthread_mutex_destroy ( & g_Mtx );
}  
                                                    
//-------------------------------------------------------------------------------------------------

int main ( void )
{
	oneTest ( 3, 10, 10 );
	oneTest ( 50, 100, 200 );
	return 0;
} 
#endif

