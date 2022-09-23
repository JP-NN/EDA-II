#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool BusquedaLinealIt(int list[],int tam, int key){
    for(int i=0;i<tam-1;i++){
        if(list[i]==key){
            return true;
        }
    }
    return false;
}

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

    if(BusquedaLinealIt(lista,9,81)==true){
        printf("Si esta");
    }else{
        printf("no esta");
    }

    printf("\nEl numero ingresado se encuentra en el indice:%d",BusquedaBinariaIt(lista,9,3));
}