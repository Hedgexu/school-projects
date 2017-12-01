#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cmath>
using namespace std;
#endif /* __PROGTEST__ */

struct head {
	
	int64_t dataSize;
	head * prev;
	head * next;
	head * prevFree;
	head * nextFree;
	bool used;
};

head * firstFree;
head * lastFree;
int blocks;
head * begin;
head * end;

void memPrynt ( )
{
	head * ptr = begin;
	while ( true )
	{
		if ( ptr == firstFree )
			printf ("+");
		if ( ptr == lastFree )
			printf ( "-");
		printf ( "%u {%lli} %lli <---- %lli ----> %lli\n", ptr->used, ptr->dataSize, ( int64_t )ptr->prevFree, ( int64_t ) ptr, ( int64_t ) ptr->nextFree );
		if ( !ptr->next )
			break;
			
		ptr = ptr->next;
	}
	printf ("----------------------------------------------------\n" );
}

void memPrynt2 ( )
{
	head * ptr = firstFree;
	while ( true )
	{
//		printf ( "[%lli] %u - %lli\n", ( int64_t ) ptr, ptr->used, ptr->dataSize );
//		printf ( "%lli <---- %lli ----> %lli\n", ( int64_t )ptr->prevFree, ( int64_t ) ptr, ( int64_t ) ptr->nextFree ); 
		printf ( "{%lli} %lli <---- %lli ----> %lli\n", ptr->dataSize, ( int64_t )ptr->prevFree, ( int64_t ) ptr, ( int64_t ) ptr->nextFree );
		if ( !ptr->nextFree )
			break;
			
		ptr = ptr->nextFree;
	}
	printf ("----------------------------------------------------\n" );
}


void HeapInit ( void * memPool, int memSize )
{ 
	memset( memPool, 0, memSize );
	head * ptr = ( head * ) memPool;
	ptr->dataSize = ( int64_t ) memSize - sizeof ( head );
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->nextFree = NULL;
	ptr->prevFree = NULL;
	ptr->used = false;
	
	firstFree = ptr;
	lastFree = ptr;
	blocks = 0;
	begin = ( head * ) memPool;
	end = ( head * ) ( ( int64_t ) memPool + ( int64_t ) memSize );
}

void * HeapAlloc ( int size )
{
	head * ptr = firstFree;

	while ( ptr )
	{
		if ( ptr->dataSize > ( int64_t ) size + ( int64_t ) sizeof ( head ) )
		{
			head * tmp = ( head * ) ( ( int64_t ) ptr + size + sizeof ( head ) );
			if ( ptr->prevFree ) 
				ptr->prevFree->nextFree = tmp;
			else
				firstFree = tmp;
				
			if ( ptr->nextFree )
				ptr->nextFree->prevFree = tmp;
			else
				lastFree = tmp;

			tmp->dataSize = ptr->dataSize - ( int64_t ) size - sizeof ( head );
			tmp->used = false;
			tmp->next = ptr->next;
			tmp->prev = ptr;
			tmp->prevFree = ptr->prevFree;
			tmp->nextFree = ptr->nextFree;

			ptr->dataSize = size;
			ptr->used = true;
			ptr->next = tmp;
			ptr->nextFree = NULL;
			ptr->prevFree = NULL;
			
			blocks++;
			
			return ( void * ) ( ( int64_t ) ptr + sizeof ( head ) );
		}
		else if ( ptr->dataSize >= size )
		{		
			if ( ptr->prevFree )
				ptr->prevFree->nextFree = ptr->nextFree;
			else
				firstFree = ptr->nextFree;
				
			if ( ptr->nextFree )
				ptr->nextFree->prevFree = ptr->prevFree;
			else
				lastFree = ptr->prevFree;

			ptr->used = true;
			ptr->nextFree = NULL;
			ptr->prevFree = NULL;
			
			blocks++;
			
			return ( void * ) ( ( int64_t ) ptr + sizeof ( head ) );
		} 
	
		ptr = ptr->nextFree;
	}
	return NULL;
}
bool HeapFree ( void * blk )
{
	head * ptr = ( head * ) ( ( int64_t ) blk - sizeof ( head ) );
	if ( ptr < begin || ptr > end || blocks < 1 || ptr->dataSize < 1 )
		return false;
	
	if ( ptr->used == true )
	{
		ptr->used = false;
	
		if ( ptr->next && ptr->next->used == false &&  ptr->prev && ptr->prev->used == false )
		{
			// spojovani s prev a next blokem
			
			//printf("nutno doimplementovat\n");
		}
		else if ( ptr->prev && ptr->prev->used == false )
		{
			// spojovani s prev blokem
			ptr->prev->dataSize = ptr->prev->dataSize + ptr->dataSize + sizeof ( head );
			if ( ptr->next )
				ptr->next->prev = ptr->prev;
				
			ptr->prev->next = ptr->next;
			memset ( ptr, 0, sizeof ( head ) );
		}
		else if ( ptr->next && ptr->next->used == false )
		{
			// spojovani s next blokem
			ptr->dataSize = ptr->dataSize + ptr->next->dataSize + sizeof ( head );
			
			if ( ptr->next->next )
				ptr->next->next->prev = ptr;
				
			if ( ptr->next == lastFree )
				lastFree = ptr;
				
			if ( ptr->next == firstFree )
				firstFree = ptr;
				
			if ( ptr->next->prevFree )
				ptr->next->prevFree->nextFree = ptr;
				
			if ( ptr->next->nextFree )
				ptr->next->nextFree->prevFree = ptr;
				
			head * tmp = ptr->next;
			
			ptr->nextFree = ptr->next->nextFree;
			ptr->prevFree = ptr->next->prevFree;
			ptr->next = ptr->next->next;
			memset( tmp, 0, sizeof ( head ) );
		}
		else
		{
			if ( lastFree == NULL )
			{
				firstFree = ptr;
				lastFree = ptr;
				ptr->prevFree = NULL;
				ptr->nextFree = NULL;
			}
			else
			{
				lastFree->nextFree = ptr;
				ptr->prevFree = lastFree;
				ptr->nextFree = NULL;
				lastFree = ptr;
			}
		}
		blocks--;
		return true;
	}
	
	return false;
}

bool HeapFree2 ( void * blk )
{ 
	head * ptr = ( head * ) ( ( int64_t ) blk - sizeof ( head ) );
	if ( ptr < begin || ptr > end || blocks < 1 || ptr->dataSize < 1 )
		return false;
		
	if ( ptr->used == true )
	{
		ptr->used = false;	
		if ( ptr->next && ptr->next->used == false )
		{
			ptr->dataSize = ptr->dataSize + ptr->next->dataSize + sizeof ( head );
			
			if ( ptr->next == lastFree )
				{  printf("splneno1\n"); lastFree = ptr; }
				
			if ( ptr->next == firstFree )
				{ printf("splneno2\n"); firstFree = ptr; }
			
			if ( !ptr->next->prevFree )
			{
				if ( firstFree->nextFree )
					firstFree->nextFree->prevFree = ptr;
					
				firstFree = ptr;
			}
			else
				ptr->next->prevFree->nextFree = ptr;
				
				
			if ( !ptr->next->nextFree )
			{
				if ( lastFree->prevFree )
					lastFree->prevFree->nextFree = ptr;
					
				lastFree = ptr;
			}
			else
				ptr->next->nextFree->prevFree = ptr;
				
			if ( ptr->next->next )
				ptr->next->next->prev = ptr;
						
			head * tmp = ptr->next;
			ptr->nextFree = ptr->next->nextFree;
			ptr->prevFree = ptr->next->prevFree;
			ptr->next = ptr->next->next;
			//printf ( "%lli <---> %lli\n", ( int64_t ) ptr, ( int64_t ) firstFree->nextFree );
			memset ( tmp , 0 , sizeof ( head ) );
		}
		memPrynt();
		
		if ( ptr->prev && ptr->prev->used == false )
		{
			printf("toto se nepouzije\n");
			ptr->prev->dataSize = ptr->prev->dataSize + ptr->dataSize + sizeof ( head );
			if ( ptr->next )
				ptr->next->prev = ptr->prev;
				
			ptr->prev->next = ptr->next;
			memset ( ptr, 0, sizeof ( head ) );
		}
		else
		{
			if ( lastFree == NULL )
			{
				firstFree = ptr;
				lastFree = ptr;
				ptr->prevFree = NULL;
				ptr->nextFree = NULL;
			}
			else
			{
				lastFree->nextFree = ptr;
				ptr->prevFree = lastFree;
				ptr->nextFree = NULL;
				lastFree = ptr;
			}
		}
		blocks--;
		return true;
	}
	
	return false;
}
void HeapDone ( int * pendingBlk )
{ 
	* pendingBlk =  blocks;
}

#ifndef __PROGTEST__
int main ( void )
 {
   uint8_t       * p0, *p1, *p2, *p3, *p4;
   int             pendingBlk;
   static uint8_t  memPool[3 * 1048576];
   
//   HeapInit ( memPool, 1600 );
//   memPrynt();
//   assert ( ( p0 = (uint8_t*) HeapAlloc ( 450 ) ) != NULL );
//   memset ( p0, 0, 450 );
//   memPrynt();
//   assert ( ( p1 = (uint8_t*) HeapAlloc ( 450 ) ) != NULL );
//   memset ( p1, 0, 450 );
//   memPrynt();
//   assert ( ( p2 = (uint8_t*) HeapAlloc ( 450 ) ) != NULL );
//   memset ( p2, 0, 450 );
//   memPrynt();
//   assert ( ( p3 = (uint8_t*) HeapAlloc ( 110 ) ) != NULL );
//   memset ( p3, 0, 10 );
//   memPrynt();
//   assert ( HeapFree ( p0 ) );
//   assert ( ( p0 = (uint8_t*) HeapAlloc ( 200 ) ) != NULL );
//   memset ( p3, 0, 200 );
//   memPrynt();
//   assert ( ( p4 = (uint8_t*) HeapAlloc ( 200 ) ) != NULL );
//   memset ( p4, 0, 200 );
//   memPrynt();
//   assert ( HeapFree ( p0 ) );
//   assert ( ( p0 = (uint8_t*) HeapAlloc ( 10 ) ) != NULL );
//   memset ( p0, 0, 10 );
//     assert ( ( p0 = (uint8_t*) HeapAlloc ( 10 ) ) != NULL );
//   memset ( p0, 0, 10 );
//   HeapDone ( &pendingBlk );


   HeapInit ( memPool, 2097152 );
   assert ( ( p0 = (uint8_t*) HeapAlloc ( 512000 ) ) != NULL );
   memset ( p0, 0, 512000 );
   assert ( ( p1 = (uint8_t*) HeapAlloc ( 511000 ) ) != NULL );
   memset ( p1, 0, 511000 );
   assert ( ( p2 = (uint8_t*) HeapAlloc ( 26000 ) ) != NULL );
   memset ( p2, 0, 26000 );
   HeapDone ( &pendingBlk );
   assert ( pendingBlk == 3 );


   HeapInit ( memPool, 2097152 );
   assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
   memset ( p0, 0, 1000000 );
   assert ( ( p1 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
   memset ( p1, 0, 250000 );
   assert ( ( p2 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
   memset ( p2, 0, 250000 );
   assert ( ( p3 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
   memset ( p3, 0, 250000 );
   assert ( ( p4 = (uint8_t*) HeapAlloc ( 50000 ) ) != NULL );
   memset ( p4, 0, 50000 );
   assert ( HeapFree ( p2 ) );
   assert ( HeapFree ( p4 ) );
   assert ( HeapFree ( p3 ) );
   assert ( HeapFree ( p1 ) );
   assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
   memset ( p1, 0, 500000 );
   assert ( HeapFree ( p0 ) );
   assert ( HeapFree ( p1 ) );
   HeapDone ( &pendingBlk );
   assert ( pendingBlk == 0 );


   HeapInit ( memPool, 2359296 );
   assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
   memset ( p0, 0, 1000000 );
   assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
   memset ( p1, 0, 500000 );
   assert ( ( p2 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
   memset ( p2, 0, 500000 );
   assert ( ( p3 = (uint8_t*) HeapAlloc ( 500000 ) ) == NULL );
   assert ( HeapFree ( p2 ) );
   assert ( ( p2 = (uint8_t*) HeapAlloc ( 300000 ) ) != NULL );
   memset ( p2, 0, 300000 );
   assert ( HeapFree ( p0 ) );
   assert ( HeapFree ( p1 ) );
   HeapDone ( &pendingBlk );
   assert ( pendingBlk == 1 );


   HeapInit ( memPool, 2359296 );
   assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
   memset ( p0, 0, 1000000 );
   assert ( ! HeapFree ( p0 + 1000 ) );
   HeapDone ( &pendingBlk );
   assert ( pendingBlk == 1 );


   return 0;
}
#endif /* __PROGTEST__ */

