#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>

#define VAL long long int

using namespace std;


struct thrZaznam
{
    string name;
    VAL value;
    //thrZaznam * thr_Prev;
    thrZaznam * thr_Left;
    thrZaznam * thr_Right;
    
    thrZaznam ( string name ) : name ( name )
    {
        thr_Left = NULL;
        thr_Right = NULL;
        value = 0;
    }
};

// -----------------------------------------------------------------------------

class thr
{
public:
    
    thrZaznam * max;
    thrZaznam * root;

    thr (  )
    {
        root = new thrZaznam ( "o" );
        root->value = 0;
        max = root;
    }
    
    void insert ( VAL value, string txt )
    {
        thrZaznam * tmp = find (root, txt);
        tmp->value += value;

        if ( tmp != max )
        {
            if ( max->value < tmp->value || ( max->value == tmp->value && max->name > tmp->name) )
                max = tmp;
        }
    }
    
    thrZaznam * find ( thrZaznam * root, string txt )
    {        
		if ( root->name > txt )
        {
            if ( root->thr_Left )
                return find ( root->thr_Left, txt );
            else
            {
                root->thr_Left = new thrZaznam ( txt );
                return root->thr_Left;
            }
        }
        else if ( root->name < txt )
        {
            if ( root->thr_Right )
                return find ( root->thr_Right, txt );
            else
            {
                root->thr_Right = new thrZaznam ( txt );
                return root->thr_Right;
            }
        }

        return root;
    }
};

// -----------------------------------------------------------------------------
struct zasZaznam
{
    VAL value;
    VAL citac;
    zasZaznam * z_Prev;
    zasZaznam * z_Next;
    
    zasZaznam ( VAL value, zasZaznam * z_Prev ) : value ( value ), z_Prev ( z_Prev )
    {
        citac = 0;
        z_Next = NULL;
    }
};

class zas
{
    zasZaznam * begin;
    zasZaznam * end;
public:
    
    zas ( )
    {
        begin = new zasZaznam( 9223372036854775807, NULL );
        end = begin;
    }
    
    bool push ( VAL value)
    {
        end->citac += value;
        if ( end->citac > end->value )
            return false;
        
        end->z_Next = new zasZaznam ( value, end );
        end = end->z_Next;
        return true;
    }
    
    bool pop ( VAL value )
    {
        if ( value != end->value )
            return false;
        
        end = end->z_Prev;        
        end->z_Next = NULL;
        return true;
 
    }
    
    bool test ( VAL last )
    {
        if ( begin == end && begin->citac == last )
            return true;
        
        return false;
    }
};

// -----------------------------------------------------------------------------

int main ( void ) 
{
    VAL radku, i, tmp;
    string txt;
    
    cin >> radku;
    thr strom;
    zas zasobnik;
    for ( i = 0; i < radku; i++ )
    {
        cin >> tmp;
        cin.get();
        getline ( cin, txt );
        strom.insert( abs(tmp), txt );
        
        if ( tmp < 0 )
        {
            if ( !zasobnik.push ( abs(tmp) ) )
                break;
        }
        else
        {
            if ( !zasobnik.pop ( abs(tmp) ) )
                break;
        }
    }
    
    if ( i == radku && zasobnik.test ( tmp ) )
        cout << "Ano" << endl;
    else
        cout << "Ne" << endl;
    
    for ( i++ ; i < radku; i++ )
    {
        cin >> tmp;
        cin.get();
        getline ( cin, txt );
        strom.insert( abs(tmp), txt );
    }
    
    cout << strom.max->name << endl;
    
    return 0;
}

