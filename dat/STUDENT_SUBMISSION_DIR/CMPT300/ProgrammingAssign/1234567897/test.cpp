/*************************************************************
   Test Program for Basic Heap Class - Preliminiary Version.
**************************************************************/
#include <iostream>
#include "heap.h"
using namespace std;

void heapTest();
 
int main(){
      heapTest();

      return 0;
}

/*
void Show( Heap & H, string s ){
      cout << s << ".capacity=" << H.capacity() << endl;
      cout << s << ".size=" << H.size() << endl;
      cout << s << "=" ; H.display(); cout << endl;
      cout << "-----------------------\n";
} 
*/
 
void heapTest(){
    
  int * ElementArr = new int[10];
  int * PriorityArr = new int[10];

  // Some priorities.
  // (In an order that is not a heap, to help spot bugs.)
  // The 999 is supposed to not end up in the heap.
  PriorityArr[0] = 9;
  PriorityArr[1] = 1;
  PriorityArr[2] = 7;
  PriorityArr[3] = 3;
  PriorityArr[4] = 2;
  PriorityArr[5] = 8;
  PriorityArr[6] = 999;

  // Some element
  // (Numbered so that the last digit is the corresponding 
  // priority, and the first digit is the order they appear,
  // as a debugging aid.)
  // The 999 is supposed to not end up in the heap.
  ElementArr[0] = 109;
  ElementArr[1] = 201;
  ElementArr[2] = 307;
  ElementArr[3] = 403;
  ElementArr[4] = 502;
  ElementArr[5] = 608;
  ElementArr[6] = 999;

  // A heap made from the first 6 elements of the two arrays.
  Heap * H1 = new Heap( PriorityArr, ElementArr, 7, 10 );

  // A second heap.  Some priorities are distinct from 
  // those in the first, and some are duplicates.
  Heap * H2 = new Heap(16);
  H2->insert(91,0);
  H2->insert(92,4);
  H2->insert(93,5);
  H2->insert(94,6);
  H2->insert(94,7);
  H2->insert(94,3);

  // A heap containing the union of pairs from the first two heaps.
  Heap * H3 = new Heap( *H1, *H2, 10 );

  cout << "Contents of heap H1:\n";
  while( !H1->empty() ){
      cout << "< " << H1->peekMinPriority() << ", " << H1->peekMin() << ">" << endl;
      H1->extractMin();
  }

  cout << "\nContents of heap H2:\n";
  while( !H2->empty() ){
      cout << "< " << H2->peekMinPriority() << ", " << H2->peekMin() << ">" << endl;
      H2->extractMin();
  }
 
  cout << "\nContents of heap H3:\n";
  while( !H3->empty() ){
      cout << "< " << H3->peekMinPriority() << ", " << H3->peekMin() << ">" << endl;
      H3->extractMin();
  }

    /*
      Heap G;
      G.insert(99,9);
      G.insert(22,2);
      G.insert(33,3);
      G.insert(11,1);
      G.printHeap();
      Heap L;
      Heap H(G,L,6);

      H.printHeap();
      
      H.extractMin();
      H.extractMin();
      H.extractMin();
      H.extractMin();

      H.printHeap();

      H.insert(55,5);
      H.insert(333,3);
      H.insert(00,0);
      H.insert(554,4);
      H.insert(55,6);

      H.printHeap();

      Heap M(G,H,7);

      M.printHeap();
      cout<<"The capacity of M is: "<<M.capacity()<<endl;
      cout<<"The size of M is: "<<M.size()<<endl; 

    */

      delete[] ElementArr;
      delete[] PriorityArr;
      delete H1;
      delete H2;
      delete H3;
}

