// #include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <iostream>

#define VAL unsigned long long int

using namespace std;

// ----- MergeSort -------------------------------------------------------------

void merge ( VAL * array, VAL * aux, VAL left, VAL middle, VAL right )
{
    VAL i1 = left;
    VAL i2 = middle + 1;
    VAL j = left;
    
    while ( ( i1 <= middle ) && ( i2 <= right ) )
    {
        if ( array[i1] <= array[i2] )
        {
            aux[j] = array[i1];
            i1++;
        }
        else
        {
            aux[j] = array[i2];
            i2++;
        }
        j++;
    }
    
    while ( i1 <= middle )
    {
        aux[j] = array[i1];
        i1++;
        j++;
    }
    
    while ( i2 <= right )
    {
        aux[j] = array[i2];
        i2++;
        j++;
    }
    for ( VAL i = left; i <= right; i++ )
        array[i] = aux[i];
}

void mergeSort ( VAL * array, VAL * aux, VAL left, VAL right ) 
{
    if ( left < right )
    {
        VAL middle = ( left + right ) / 2;
        mergeSort ( array, aux, left, middle );
        mergeSort ( array, aux, middle + 1, right );
        merge ( array, aux, left, middle, right );
    }
}

VAL find ( VAL tmp, VAL * pole, VAL low, VAL max )
{
	VAL left, right, center;
	left = low;
	right = max;
	
	while ( left < right )
	{
		center = ( left + right ) / 2;
		if ( pole[center] == tmp )
			return center;
		else if ( pole[center] > tmp )
			right = center - 1;
		else
			left = center + 1;
	
	}
	return left;
}

int main ( void ) 
{
    VAL stromu;
    VAL obvod;
    VAL trojuhel = 0;
    VAL i; 
    VAL navic = 0;
    VAL index;
    VAL tmp;

    
    scanf ( "%llu %llu", & stromu, & obvod );
    
    VAL * pole = new VAL[stromu];
    
    for ( i = 0; i < stromu; i++ )
    	scanf ( "%llu", & pole[i] );
    
    VAL * pomocne = new VAL[stromu];
    mergeSort( pole, pomocne, 0, stromu - 1);
    
    trojuhel = ( stromu * ( stromu - 1 ) * ( stromu - 2 ) ) / 6;
    
    // ----------------------------------------------------------
    
    // nebude problem s lichim obvodem

    for ( i = 0; i < stromu; i++ )
    {
    	pole[i] *= 2;
    }
    	
    // ----------------------------------------------------------
    
    // Pro kazdy bod najit protilehli 
    /*obvod i všechny hodnoty zvětším na dvojnásobek abych se zbavil problémem kdy je lichý obvod

pro každý bod najdu logaritmicky protějšek

pokud je protějšek posouvám jej dokud to není o něco málo

rozdíl se hodí do vzorce a vyplivne počet neplatných trojúhelníků*/
    
    // ----------------------------------------------------------
    

    // cout << navic << endl;
    
    for ( i = 0; i < stromu; i++ )
    {
    	if ( pole[i] < obvod )
    	{
    		tmp = pole[i] + obvod;
    	
    		if ( tmp > pole[stromu - 1] )
    		{   		
				tmp = stromu - i - 1;
    			navic += tmp * ( tmp - 1 );
    		}
    		else
    		{
    			index = find ( tmp, pole, 0, stromu - 1 );

				while ( pole[index] >= tmp )
					index--;
									
				tmp = index - i;
				navic += tmp * ( tmp - 1 );
    		}
    		
    	}
    	else
    	{
    		tmp = pole[i] - obvod;
    		
    		if ( tmp <= pole[0] )
    		{
    			tmp = stromu - i - 1;
    			navic += tmp * ( tmp - 1 );
    		}
    		else
    		{
    			index = find ( tmp, pole, 0, stromu - 1 );
	
    			while ( pole[index] >= tmp )
					index--;
					
				tmp = stromu + index - i;
				navic += tmp * ( tmp - 1 ) ;
				
				
    		}  	
    	}	
    }		
    
	printf ("%llu\n", (trojuhel - navic / 2) ); 
    
   // delete[] pole;
   // delete[] pomocne;
 
    return 0;
}

