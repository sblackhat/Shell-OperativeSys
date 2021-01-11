#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIMENSION 4096

/*array: Elements in the list are stored in a statically allocated
array of nodes, so the list type is a pointer to a structure containing
the array of nodes and optionally one or more integers (depending
on the implementation: nextin and nextout indexes, counter . . . ).
For the purpose of this lab assigment, we can assume the array
dimension to be 4096 (which should be declared a named constant,
and thus easily modiable).*/

    //so, it´s a structure that contain the array of all the elements, and some other values
    //then, the list type is a type which points to that structure.

    //typedef helps us to rename the data types, so we´ll use it to create a 'tList' data type
    
    struct DATA
    {
        char elements[DIMENSION];
        int lastPos;
    } ;
    
    typedef struct DATA * tList; //now the tList is the structure 'DATA'
                                 //L is a pointer to a 'tList' data type

    /*check this, there must be an error in the data type because while compiling,
      it says that i´m not using the pointer*/
   
void createList (tList * L){
    
    *L=(tList) malloc (DIMENSION * sizeof (tList));
    (*L) -> lastPos = 0;
    
}    

int next (tList L, int position){
    if ( ( (L -> elements[0]) != '\0')  & (position < DIMENSION)){
        position = position +1;
        return position;
    }
    return 0;
}

int previous (tList L, int position){
    if ( (L != NULL) & (position < 1)) {
        position = position - 1;
        return position;
    }
    return 0;
}

int lastPosL (tList L){
    int i = 0;
    i = (L -> lastPos);
    return i;
}

int insertItem (tList *L, char * input){    //i think that this fails
    int a;          //a is going to save the length of the inserted char
    
    a = strlen(input);
    
    if (  ( ((*L)->lastPos) + a) < DIMENSION   ){
    
        (*L) -> elements[(*L) -> lastPos] = *input;
       
        for (int i = 0; i < a; i++){
            (*L) -> elements[(*L) -> lastPos] = input[i];
            (*L) -> lastPos = ((*L) -> lastPos) + 1; 
        }
        //we introduce manually a \0 in order to know when a command ends
        (*L) -> elements[(*L) -> lastPos] = '\0';
        (*L) -> lastPos = ((*L) -> lastPos) + 1; 
        
        return 1;
    }
    else{
        return 0;
    }
}

void deleteList (tList *L){
    (*L) -> lastPos = 0;
    free(*L);     //memset(*s, int cnt, int size) sets the array pointed by s, to the position size, the constant cnt
    createList(L);
    
    return ;
}

void printList (tList L){
    int a = 0;
    a = lastPosL(L);
    if ( a != 0 ) {
        for (int i = 0; i < a; i++){
            
            if (L -> elements[i] == '\0'){
                printf("\n");
            }
            else {
                printf("%c", (L -> elements[i]));
            }
        }
    }
    return;
}
