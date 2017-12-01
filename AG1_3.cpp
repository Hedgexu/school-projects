#include <cstdio>
#include <stdint.h>
#include <iostream>

#define VAL unsigned long long int

using namespace std;

class uzel
{
	public:
	int vazeb;
	VAL suma;
	VAL pruduct;
	uzel * * array;
	int done;
	
	uzel ( VAL velikost = 1 ) : vazeb ( velikost ), suma ( 0 ), pruduct ( 1 ), done ( 0 )
	{
		array = new uzel * [ velikost ];  
	}
};

struct zaznam
{
	uzel * u_This;
	zaznam * z_Next;
	
	zaznam ( uzel * u_This, zaznam * z_Next = NULL ) : u_This ( u_This ), z_Next ( z_Next )
	{ }
};

class zasobnik
{
	public:
	zaznam * z_Top;
	
	zasobnik ( ) : z_Top ( NULL )
	{ }
	
	void clear ( )
	{
		z_Top = NULL;
	}
	
	void push ( uzel * u_New )
	{
		if ( z_Top )
			z_Top = new zaznam ( u_New, z_Top );
		else
			z_Top = new zaznam ( u_New );
	}
	
	bool empty ( )
	{
		if ( z_Top ) 
			return false;
		
		return true;
	}
	
	void pop ( )
	{
		z_Top = z_Top->z_Next;
	}
};

VAL solve ( uzel * pole )
{
/*
1. vstup do ditete a this dej na zasobnik
2. projdi pole deti a do tech co nejsou rovny max na zasobniku se zanor
3. pokud mas vsechny deti splnene vypocti sam sebe z hodnot ulozenych v detech
4. vrat se k bodu 2
*/
	zasobnik zas;
	int i,y;
	uzel * tmp;
	
	for ( i = 0; i < pole[0].vazeb; i++ )
	{
		zas.clear ( );
		zas.push ( & pole[0] );
		tmp = pole[0].array[i];
		while ( !zas.empty( ) )
		{
			if ( tmp->done  == tmp->vazeb )
			{
				for ( y = 0; y < tmp->vazeb; y++ )
				{
					if ( tmp->array[y] == zas.z_Top->u_This ) 
						continue;
						
				 	tmp->suma += ( tmp->array[y]->suma + tmp->array[y]->pruduct ) % 536870909; 
				 	tmp->suma = tmp->suma % 536870909;
				 	tmp->pruduct *= ( 1 + tmp->array[y]->pruduct ) % 536870909;
				 	tmp->pruduct = tmp->pruduct % 536870909;
				}
				tmp = zas.z_Top->u_This;
				zas.pop ( );
				tmp->done++;
				
			}
			else
			{
				if ( tmp->array[tmp->done] == zas.z_Top->u_This )
				{
					tmp->done++;
					continue;
				}
				zas.push ( tmp );
				tmp = tmp->array[tmp->done];
			}
		}
	}

	for ( i = 0; i < pole[0].vazeb; i++ )
	{
		pole[0].suma = ( pole[0].suma + pole[0].array[i]->suma + pole[0].array[i]->pruduct ) % 536870909;
		pole[0].pruduct = ( pole[0].pruduct * ( 1 + pole[0].array[i]->pruduct ) )% 536870909;
	}
	return (pole[0].suma + pole[0].pruduct) % 536870909;

}


int main ( void )
{
	int i, y, radku, na_radce, scan;

	scanf ( "%i", & radku );
	uzel * pole = new uzel[radku];
	
	for ( i = 0; i < radku; i++ )
    {
        scanf ( "%i", & na_radce );
        pole[i] = uzel ( na_radce );
        for ( y = 0; y < na_radce; y++ )
        {
        	 scanf ( "%i", & scan );
        	 pole[i].array[y] = & pole[scan];
       	}
	}
	
	cout << solve ( pole ) << endl;
	//cout << pole[0].suma << endl;
	//cout << pole[0].pruduct << endl;
	
	return 0;
}
