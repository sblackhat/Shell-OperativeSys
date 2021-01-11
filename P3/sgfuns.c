#include "sgfuns.h"

int Senal(char * sen) /*devuel el numero de senial a partir del nombre*/
{
int i;
for (i=0; sigstrnum[i].nombre != NULL; i++)
if (!strcmp(sen, sigstrnum[i].nombre))
return sigstrnum[i].senal;
return -1;
}

char * NombreSenal(int sen) /*devuelve el nombre senal a partir de la senal*/
{
/* para sitios donde no hay sig2str*/
int i;
for (i=0; sigstrnum[i].nombre != NULL; i++)
if (sen==sigstrnum[i].senal)
return sigstrnum[i].nombre;
return ("SIGUNKNOWN");
}