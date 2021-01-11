
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"

   
int createListH (Listh * L){
   (*L) =(Listh) malloc (sizeof (tListh));
    if ( L != NULL){
    
    (*L) -> lastPos = -1;
    
    return 1;
    }
    else {
        perror("Impossible to allocate memory while creating the list.");
        return 0;
    }
}   

int lastPosLH (Listh L){
    return L->lastPos;
}

int insertItemH (Listh L, char * input){    
   
    if (L ->lastPos < DIMENSION){
        L->lastPos++; 
        L->elements[L->lastPos].line = strdup(input); 
        return 1;
    }
    else{
        perror("The list if full.\n");
        return 0;
    }
}

void deleteListH(Listh *L){
    free(*L);     //memset(*s, int cnt, int size) sets the array pointed by s, to the position size, the constant cnt
    createListH (L);
}

void printListh (Listh L){
    int i;
    if ( L->lastPos >= 0 ) {
        for (i = 0; i <= L->lastPos; i++){
            printf("%s", L->elements[i].line);
        }
    }
}
