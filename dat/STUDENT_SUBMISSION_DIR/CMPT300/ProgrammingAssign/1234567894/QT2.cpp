//
//  Test Program 2 for Dynamic Int Queue Class
//
#include <iostream>   // for I/O facilities
#include "queue.h" //  basic_int_queue declarations
using namespace std;

int main (int argc, char * const argv[]) {
        // Make a queue, and verify that empty() says it is empty. 
        
   bool OK = true ;
   Queue q1 ;
   Queue q2 ;
   if( q1.size() != 0 ) OK = false ; 
   for( int i = 1 ; i <= 10 ; i++ ){
      q1.enqueue(100 + i);
      q2.enqueue(200 + i);
   }
   if( q1.size() != 10 ) OK = false ;
   if( q2.size() != 10 ) OK = false ;

   for( int i = 1 ; i <= 20 ; i++ ){
      q2.enqueue( q1.dequeue() );
      q1.enqueue( q2.dequeue() );
   }
   if( q1.size() != 10 ) OK = false ;
   if( q2.size() != 10 ) OK = false ;
  
   while( !q1.empty() ){
      q1.dequeue();
   }
   while( !q2.empty() ){
      q2.dequeue();
   }

   if( q1.size() != 0 ) OK = false ;
   if( q2.size() != 0 ) OK = false ;

   if( OK ){
      cout << "GOOD" << endl ;
      return 0 ;
   }else{
      cout << "BAD" << endl ;
      return -1 ;
   }
}
