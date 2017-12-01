#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359
#define VAL unsigned long long int

using namespace std;

struct letiste
{
    double x;
    double y;
    double * uhly;
};

int compare ( const void * a, const void * b )
{
	if ( *(double*) a > *(double*) b )
		return 1;
	else if ( *(double*) a < *(double*) b )
		return -1;
	
	return 0;
}

int main ( void ) 
{
    VAL sum, i, trojuhelniku, y, x, first, last;
    
    scanf ( "%llu", & sum );
   
    letiste * pole = new letiste[sum];
    
    for ( i = 0; i < sum; i++ )
    {
        scanf ( "%lf %lf", & pole[i].x, & pole[i].y );
        pole[i].uhly = new double[ sum - 1 ];
    }

    trojuhelniku = ( sum * ( sum - 1) * ( sum - 2) ) / 6;
    
    for ( i = 0; i < sum; i++ )
    {
        x = 0;
        
        for ( y = 0; y < sum; y++ )
        {
            if ( i == y )
                continue;
            
            pole[i].uhly[x] = atan2 ( pole[y].x - pole[i].x, pole[y].y - pole[i].y ) / PI * 180;
            if ( pole[i].uhly[x] < 0 )
                pole[i].uhly[x] += 360;
            
            x++;
        }
        
        qsort ( pole[i].uhly, sum - 1 , sizeof ( double ), compare );
        
        first = 1;
        last = 9999;
        
        for ( y = 0; y < sum - 2; y++ )
        {            
            for ( ; (VAL) first < sum - 1; first++ )
            {
                if ( pole[i].uhly[first] - pole[i].uhly[y] > 89.99999999 )
                	break;
            }
            
            if ( last == 9999 )
                last = first;
            
            for ( ; (VAL) last < sum - 1; last++ )
            {               
                if ( pole[i].uhly[last] - pole[i].uhly[y] > 270.00000001 )
                    break;
            }
            trojuhelniku -= (VAL) ( last - first );
        }
    }
    
    printf ("%llu\n", trojuhelniku ); 
    return 0;
}

