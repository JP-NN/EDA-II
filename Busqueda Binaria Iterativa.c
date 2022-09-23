#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int BusquedaBinariaIt(int list[], int tam, int key){
   int x0=0;
   int x1=tam-1;
   while(x0<=x1){
        int mid=(x0+x1)/2;
        if(list[mid]==key){
            return mid;
        }
        if(key<list[mid]){
            x1=mid-1;
        }else{
            x0=mid+1;
        }
    } 
    return -1;
}

int main(){
    int lista[]={1,2,3,4,5,6,7,8,9};

    printf("%d",lista[2]);
}