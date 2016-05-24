/*
    301 243 781
    kdehghan
    Kayhan Dehghani Mohammadi
*/

/*
**  a4.cpp
**  Assignment 4, CMPT-225, Spring 2015
**
**  Reads a sequence of integers (in the range of int's) from standard in;
**  It is interpreted as two lists, separated by a single 0
**/
#include <iostream>
#include <set> // for storing the keys in a dynamic data structure


using namespace std;

int main(){
	
    set<int> L1; // container for elements in up to 0
    set<int> L2; // container for elements after 0
    set<int>::reverse_iterator pos; // for travesing the L1 backward(finding the maximum)

    bool in_L2; // indicates if an element is in L2 or not
    
    int x ; // temporary variable for reading integers
    cin >> x; // read the first element

    //print Login and ID number 
    cout<<"301243781    kdehghan    Kayhan Dehghani Mohammadi"<<endl;

    // Read up to 0 and insert in L1
    while( !cin.eof() && ( x!= 0 ) ){
        
        L1.insert(x); // insert x into L1
        cin >> x; //read the next element
    }

    // Read elements after 0 and insert in L2
    cin >> x; // read the next element
    while( !cin.eof() ){

        L2.insert(x); // insert x into L2
        cin >> x; // read the next element
    }

    // findes the max val in L1 not existing in L2
    // Travesing the L1 for the maximum value 
    // if it exists in L2 keep looking
    for( pos = L1.rbegin(); pos != L1.rend(); pos++ ){
        
        in_L2 = ( L2.find( *pos ) != L2.end() ); // see if the current maximum exists in L2
        if( !in_L2 ){

            cout<<*pos<<endl; // max val in L1 not existing in L2
            return 0; 
        }

    }

    cout<<"NONE"<<endl; // all L1 elements are in L2


    return 0;
}
