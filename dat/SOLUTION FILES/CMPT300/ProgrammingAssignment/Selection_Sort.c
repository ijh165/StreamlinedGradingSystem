#include <stdio.h>

int main(int argc, char const *argv[])
{
	int len=10;
	int arr[10]={2,5,4,6,3,1,0,8,9,7};
	for(int i=0; i< len-1 ; i++){
		int minpos=i;
		for(int j=i+1; j< len; j++){
			if(arr[j]<arr[minpos]){
				
				minpos=j;
			}
		}
		int temp=arr[minpos];
		arr[minpos]=arr[i];
		arr[i]=temp;
	}
	for(int i=0;i< len;i++){
		printf("%d\n",arr[i] );
	}
	return 0;
}