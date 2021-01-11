
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HistList.h"

   
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

int insertItemH (Listh L, char input[MAXLINE]){    
   
    if (L ->lastPos < DIMENSION){
        L->lastPos++; 
        strcpy(L->elements[L->lastPos].line,input); 
        return 1;
    }
    else{
        perror("The list if full.\n");
        return 0;
    }
}

void deleteListH(Listh L){
    free(L); 
    L= NULL;    
}

void printListh (Listh L){
    int i;
    if (L != NULL)
        for (i = 0; i <= L->lastPos; i++)
            printf("%s", L->elements[i].line);
}

void removeElements(Listh L){
    L->lastPos = -1;
}
/*
int main(){
    Listh L;
    if (createListH(&L)){
    char line[MAXLINE] = "Hello";
    char line1[MAXLINE] = "Hello2";
    char line2[MAXLINE] = "Hello3";
    insertItemH(L,line);
    insertItemH(L,line1);
    insertItemH(L,line2);
    insertItemH(L,line);
    printListh(L);
    deleteListH(&L);
    }
    return 0;
}*/
