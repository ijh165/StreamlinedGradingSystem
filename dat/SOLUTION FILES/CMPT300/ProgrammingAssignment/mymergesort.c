#include <stdio.h>
#include <stdlib.h>


void merge(int arr[],int first, int mid, int last){

	int len=last-first+1;
	int lefpos=first;
	int rightpos=mid+1;
	int newpos=0;
	int temparr[len];

	while(lefpos<=mid && rightpos<= last){
		
		if(arr[lefpos]<arr[rightpos])
			temparr[newpos++]=arr[lefpos++];
		else
			temparr[newpos++]=arr[rightpos++];
	}
	while(lefpos<=mid)
		temparr[newpos++]=arr[lefpos++];
	while(rightpos<=last)
		temparr[newpos++]=arr[rightpos++];
	for(int i=0;i<len;i++)
		arr[i+first]=temparr[i];
}
void mrgesort(int arr[], int first, int last){

	int mid=(first+last)/2;
	if(first==last)
		return;
	mrgesort(arr, first, mid);
	mrgesort(arr,mid+1,last);
	merge(arr, first, mid, last);

}
int main(){

	int arr[10]={4,5,-3,6,8,-56,245,-245,24555,0};
	mrgesort(arr,0,9);
	for(int i=0;i<10;i++)
		printf("%d\n",arr[i]);

}