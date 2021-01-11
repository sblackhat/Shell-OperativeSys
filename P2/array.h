
#define DIMENSION 4096
#define MAXLINE 2048
/*array: Elements in the list are stored in a statically allocated
array of nodes, so the list type is a pointer to a structure containing
the array of nodes and optionally one or more integers (depending
on the implementation: nextin and nextout indexes, counter . . . ).
For the purpose of this lab assigment, we can assume the array
dimension to be 4096 (which should be declared a named constant,
and thus easily modiable).*/

    //so, it´s a structure that contain the array of all the elements, and some other values
    //then, the list type is a type which points to that structure.

    //typedef helps us to rename the data types, so we´ll use it to create a 'Listh' data type
    
typedef struct Nodeh{
        char * line;
    }tNodeh;
    
typedef struct
    {
        struct Nodeh elements[DIMENSION];
        int lastPos;
    }tListh;
    
typedef tListh * Listh; //now the Listh is the structure 'DATA'
                                 //L is a pointer to a 'Listh' data type

    /*check this, there must be an error in the data type because while compiling,
      it says that i´m not using the pointer*/

int createListH (Listh * L);
int lastPosLH (Listh L);
int insertItemH (Listh L, char * input);
void deleteListH(Listh *L);
void printListh (Listh L);
