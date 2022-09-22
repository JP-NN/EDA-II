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

int main(){
    int lista[]={1,2,3,4,5,6,7,8,9};

    if(BusquedaLinealIt(lista,9,81)==true){
        printf("Si esta");
    }else{
        printf("no esta");
    }
}