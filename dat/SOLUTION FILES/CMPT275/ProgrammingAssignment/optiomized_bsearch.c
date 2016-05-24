#include <stdio.h>
#include <stdlib.h>



int b_search(int arr[], int len, int target){

    //int temp=arr[len-1];

    //arr[len-1]=target;
    int first = 0;

    int last = len-1;

    int mid = (first + last) / 2;

    while(first <= last && arr[mid] != target){

        if(target < arr[mid]){

            last = mid-1;

        }
        else
        {

            first = mid+1;
        }
        mid = (first + last) / 2;

    }

    if(first>last){
        return -1;

    }
    else{
        return mid;
    }


}


int main(void){
    int arr[10]={1,2,3,4,5,6,7,8,9,10};
    int index=b_search(arr,10,11);
    printf("%d\n",index );

}