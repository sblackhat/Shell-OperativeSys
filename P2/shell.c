

#define _XOPEN_SOURCE 500 //for nftw()
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>
#include <ftw.h>    //this one is for nftw()
#include "list.h"  //this command imports the file 'List.c', which is the location of the list.
#include "array.h"
//now defines
#define MAXLINE 2048 //memory gives 16 bits by 16 bits, so use a 16 multiple
#define MAXNAME 1024 
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"

/************************************************/

struct COMMAND{
    char *name;
    void (*pfunc) (/*char * tr[], int att, MemoryList A, tListH L*/) ;   //specified inside prcess input

};

void CmdAutores(char * tr[]){
 
    
    if (tr[0] != NULL){
    if (strcmp(tr[0], "-l")==0){
        printf("logins: adrian.haz@udc.es / s.valle@udc.es\n");
        return;
    }
    
    if (strcmp(tr[0], "-n")==0){
        printf("names: Adrián Haz García, Sergio Valle Trillo\n");
        return;
    }
    else {printf("Command not found\n");
        return;
    }
    }   
    printf("Adrián Haz García, login: adrian.haz@udc.es\nSergio Valle Trillo, login: s.valle@udc.es\n");
    return;
}
    


void CmdPid(char * tr[]){
    
    if(tr[0] != NULL){ 
        if (strcmp(tr[0],"-p")==0){
            printf("parent process pid: %d\n",getppid());
            
        }
        else {
            printf("Command not found\n");
            
        }   
    }
    else{
        printf("process pid: %d\n",getpid());
    }
        
}

void CmdCdir (char * tr[])
{   
    char dir[MAXNAME];
    if (tr[0] == NULL){
        printf("%s\n", getcwd(dir, MAXNAME));
    }
    else {
        if (chdir(tr[0])==-1){
        perror("can not change directory");
        }
    }
}

void CmdFecha (){
    time_t currentDate;
    struct tm *localDate;

    time( &currentDate );
    localDate = localtime( &currentDate );

    int day   = localDate->       tm_mday;
    int month = localDate->    tm_mon + 1;  // Months goes from 0 to 11, so we sum 1
    int year  = localDate->tm_year + 1900;  // This function started counted years from 1900

    printf("Local day (Day/Month/Year): %d/%d/%d\n", day, month, year);
    

}

void CmdHora ()
{
    time_t currentTime;                     // Declaration of currentTime variable
    struct tm *localTime;                   // Data and time info are saved in structure tm (localTime)

    time( &currentTime );                   // Gets the current time
    localTime = localtime( &currentTime );  // Converts the current time into the local time

    int hour   = localTime->tm_hour;        // Now we have the hour, minute and second in int
    int min    = localTime->tm_min;         
    int sec    = localTime->tm_sec;
    
    printf("Local current time: %d: %d: %d\n", hour, min, sec);
}


void CmdHist(char * tr[], int numtr,MemoryList A, Listh L){
    if (tr[0] != NULL && strcmp(tr[0],"-c")==0 ){
        //clear history == delete list
        deleteListH(&L);
        return;
    }
    else
    {
        //acces history == print list
        printListh(L);  
        return;
    }
}


void CmdExit(){
    exit(0);
}


//*************************** P R A C T I C E   1 ***************************//

void CmdCrear (char *tr[]){
    if (tr[0] != NULL){
        
        if (strcmp(tr[0], "-d") == 0){
            
                if (tr[1] == NULL){
                    //list files in the working directory
                    char buf [MAXLINE];
                    getcwd(buf, MAXLINE);
                    printf("Current working directory: %s\n", buf);
                    return;
                }
        
                else {
                    //create folder with name = 'tr[1]'
                    mkdir(tr[1], 0700);
                    return;
                }
        }
    
        else{ 
            
                if(tr[1] == NULL) {
                    //create empty file with the given name
                    FILE* myfile = fopen(tr[0], "w");   
                    if (myfile == NULL){
                        printf("Impossible to create the file.");
                        exit(EXIT_FAILURE);
                    }
                    fclose(myfile);
                    return;
                }
                
                else {
                //throw error
                exit(EXIT_FAILURE);
                }
            }
    }
    else {
        //throw error
        exit(EXIT_FAILURE);
    }
}

/*
    @removeFiles
    - Is called inside 'CmdBorrar' function
    - It receives the name of the path to delete, and the other 3 variables are used for the nftw method used in 'CmdBorrar':
                    """""The second argument is a pointer to a stat buffer containing information on the object.
                    The third argument is an integer giving additional information. Its value is one of the following:

                    FTW_D
                        for a directory
                    FTW_DNR
                        for a directory that cannot be read
                    FTW_DP
                        for a directory whose subdirectories have been visited. 
                        (This condition will only occur if FTW_DEPTH is included in flags.)
                    FTW_F
                        for a file
                    FTW_NS
                        for an object other than a symbolic link on which stat() could not be successfully executed. 
                        If the object is a symbolic link, and stat() failed, 
                        it is unspecified whether nftw() passes FTW_SL or FTW_NS to the user-supplied function.
                    FTW_SL
                        for a symbolic link
                    FTW_SLN
                        for a symbolic link that does not name an existing file. 
                        (This condition will only occur if FTW_PHYS is not included in flags.)

                    The fourth argument is a pointer to an FTW structure. """""
    
    - This function is static because we only want to use it in this file.c 
*/
static int removeFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if(remove(pathname) < 0) //remove automatically uses rmdir for directories-
    {                       // -and unlink for files
        perror("ERROR: remove");
        return -1;
    }
    return 0;
}


void CmdBorrar (char *tr[]){
    if (tr[0] != NULL){
        if (strcmp(tr[0], "-r") == 0) {
            if (tr[1] == NULL){
                //list files in the working directory
                char buf [MAXLINE];
                getcwd(buf, MAXLINE);
                printf("Current working directory: %s\n", buf);
                return;
            }
            else {
                //delete directory and all of its contents
                /* Ideas that could work:
                    // opendir readdir closedir
                    //remove an empty directory ---> rmdir      we can use 'remove' it calls rmdir if directory and unlink if file  
                    //remove a file ---> unlink()
                    //to distinguish between file and directory ---> stat() & S_ISDIR(stat.st_mode)
                    //use nftw to access to the files of the directory. Add FTW_DEPTH|FTW_MOUNT|FTW_PHYS */
                /*
                    @nftw
                    - Arguments: the path, function provided by the user,  the directory depth for the search, flags.
                    - This flags do:
                        ·FTW_DEPTH
                            If set, nftw() will report all files in a directory before reporting the directory itself. 
                            That´s because nftw has a preorder nature, but we can´t use it bc directories wouldn´t be empty
                        ·FTW_MOUNT
                            If set, nftw() will only report files in the same file system as path. (we don´t want to delete 
                            our complete file system :) )
                        ·FTW_PHYS
                            If set, nftw() performs a physical walk and does not follow symbolic links. (follow symbolic links
                            could imply a directory change)
                */

                if( nftw(tr[1], removeFiles, 32, FTW_DEPTH | FTW_MOUNT | FTW_PHYS) == 0){
                    return;
                }
                else {
                    printf("Something happened\n");
                    return;
                }
            }
        }
        
        else{
            //delete directory if it´s empty
            rmdir(tr[0]); //deletes directory only if it is empty
            return;
        }
    }
    return;
}

#include<pwd.h>

char *uid_to_name(uid_t uid)
/*
 * returns pointer to username associated with uid, uses getpw()
 * 
 * */
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL){
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

#include<grp.h>

char * gid_to_name(gid_t gid)
/*
 * returns pointer to group number gid. used getgrgid(3)
 * 
 * */
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if((grp_ptr = getgrgid(gid)) == NULL){
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}
void typeOfFile (mode_t mode, char str[]){
    
    strcpy(str, "----------");      // default = no perms
  switch (mode & S_IFMT){
    case S_IFDIR :  str[0] = 'd';  break;     // directory?
    case S_IFCHR :  str[0] = 'c';  break;     // char devices
    case S_IFBLK :  str[0] = 'b';  break;     // block device
    case S_IFLNK :  str[0] = 'l';  break;     //SysLink
    case S_IFREG :  str[0] = '-';  break;     //Regular file
    case S_IFSOCK :  str[0] = 's';  break;    //Socket
    case S_IFIFO :  str[0] = 'p';  break;     //Pipe
    default : str[0] = '?';  }                //Unknown type of file

    if (mode & S_IRUSR) str[1]='r';        
    if (mode & S_IWUSR) str[2]='w';    //Owner 
    if (mode & S_IXUSR) str[3]='x';

    if (mode & S_IRGRP) str[4]='r';
    if (mode & S_IWGRP) str[5]='w';    //Group      
    if (mode & S_IXGRP) str[6]='x';

    if (mode & S_IROTH) str[7]='r';
    if (mode & S_IWOTH) str[8]='w';    //Others
    if (mode & S_IXOTH) str[9]='x';

    if (mode & S_ISUID) str[3]='s';
    if (mode & S_ISGID) str[6]='s'; /*setuid, setgid y stickybit*/
    if (mode & S_ISVTX) str[9]='t';

}


void showInfo (char * filename,struct stat * fileStat){
    char tiempo[20];
    printf("%-7d ",(int) fileStat->st_ino);
    char modestr[11];
    typeOfFile(fileStat->st_mode, modestr);
    printf("%-10s",modestr);
    printf(" %-1d ", (int)fileStat->st_nlink);
    printf(" %-s", uid_to_name(fileStat->st_uid));
    printf(" %-s", gid_to_name(fileStat->st_gid));
    printf(" %-ld", (long)fileStat->st_size);
    strftime(tiempo,20,"%b %d %R",localtime(&fileStat->st_mtime));
    printf(" %-s",tiempo );
    switch (fileStat->st_mode & S_IFMT){
        case S_IFDIR :  printf(BLUE " %s\n"WHITE  ,filename); ;  break; 
        case S_IFLNK :  printf(GREEN " %s\n"WHITE ,filename); ;  break; 
    default : printf(WHITE" %s\n",filename);
    }
 }

void showInfo_simple (char * filename,struct stat * fileStat){
    
     printf("%ld", (long)fileStat->st_size);
     switch (fileStat->st_mode & S_IFMT){
        case S_IFDIR :  printf(BLUE " %s : \n" WHITE ,filename); ;  break; 
        case S_IFLNK :  printf(GREEN "%10s%s\n"WHITE," ",filename); ;  break; 
    default : printf(WHITE "%10s%s\n"," ",filename);
    }
 }


void CmdInfo (char * tr[],int size){
    int i = 0;
    struct stat fileStat;
    for (i = 0; i < size; ++i)
    {
        if(lstat(tr[i],&fileStat) == -1)     // cannot stat
        perror(tr[i]);

        else if(lstat(tr[i],&fileStat) != -2){
            showInfo(tr[i],&fileStat);
         }
        else {
            printf("Cannot access %s: No such file or directory\n",tr[i] );
        }
    }
}


void Usage() {

    fprintf(stderr, "\nListar: lists the directories and/or files supplied to it as command line\n");
    fprintf(stderr, "Syntax : listar [-l] [-v] [-r] name1 name2 name3 ....\n");
    fprintf(stderr, "\nOptions\n-r\tlist subdirectories recursively\n-l\tstands for long listing\n-v\tAvoid listing hidden elements\n");
}
#include <dirent.h> 

void RecDir(char * basePath,int flag_r,int flag_l,int flag_v) {
    char path[MAXLINE];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    struct stat fileStat;
    // Unable to open directory stream
    if (!dir)
        fprintf(stderr,"Cannot access %s: No such file or directory\n",basePath );

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            //Check option -v 
            if (!( flag_v == 1 && strncmp(dp->d_name,".",1) == 0)){
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            //Check the -l option
            if(flag_l == 1 && lstat(path,&fileStat) != -2)
            showInfo(path,&fileStat);
            else if (lstat(path,&fileStat) != -2){
                showInfo_simple(path,&fileStat);
            }
            if (flag_r && (fileStat.st_mode & S_IFDIR)) {
                RecDir(path,flag_r,flag_l,flag_v);
                printf("\n");}
            }
        }
    }

    closedir(dir);
}

void CmdListar (char * tr[],int size){
    int r = 0,l = 0,v = 0;
    int flags = 0;
    if (size > 1){
        
    for (int i = 0; i < size; ++i)
        {   
                if (strcmp(tr[i],"-r") == 0){
                    r = 1; flags++; 
                }
                else if (strcmp(tr[i],"-l") == 0){
                l = 1; flags++;
                }
                else if (strcmp(tr[i],"-v") == 0){
                v = 1; flags++;
                }
                else {
                    Usage();
                    break;
                    }
        }

   for (int i = flags; i < size && flags != 0; ++i)
        RecDir(tr[i],r,l,v);
    }
    else if (size == 1) {
        //printf("here");
        RecDir(tr[0],r,l,v);
    }
    else Usage();
}

/********************* A S S I G M E N T     2   ******************/

void Cmd_AsignarMalloc (char * tr[], MemoryList A){
    if (tr[0] == NULL) 
    { printMalloc(A); return; }
    else {
        char timec[26]; time_t currentTime; struct tm *localTime;                     // Declaration of currentTime variable
            
        // Data and time info are saved in structure tm (localTime)
        time( &currentTime );                   // Gets the current time
        localTime = localtime( &currentTime );  // Converts the current time into the local time            
        
        int size = strtol(tr[0], NULL, 10); //Converts string into integer
         
        void * p =(void*) malloc(size);
        strftime(timec,26,"%a %b %d %T %Y",localTime); //local time to string timec
        
        printf("allocated %d at %p\n", size, p);
        
        insertItem(A, p, size, timec, Malloc, NULL, 0, 0);
    }
}

    void * MmapFichero (char * file, int protection, MemoryList A){
        int df, map = MAP_PRIVATE, mode = O_RDONLY;
        struct stat s;
        void * p;
        char timec[26]; time_t currentTime; struct tm *localTime;
        time( &currentTime ); localTime = localtime( &currentTime );


        if (protection & PROT_WRITE) mode = O_RDWR;
        
        if( (stat (file, &s) == -1) || (df = open (file, mode) == -1)  )
            return NULL;

        if ((p = mmap (NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
            return NULL;
        
        /*save mmap address (p, s.st_size, file, df....); */
        strftime(timec,26,"%a %b %d %T %Y",localTime);

        
        insertItem(A, p, s.st_size, timec, Mapped,file ,df , 0);
        return p;
    }
/* 
+   I am just having problems inserting the items inside mmap fichero
+   i 've just realized that i have to insert the key and the size
+   but first of all i have to find them.
*/
void Cmd_AsignarMmap (char * tr[], MemoryList A){
    char * perm;
    void * p;
    int protection = 0;
    
    
    if (tr[0] == NULL)
    {/*List Memory Addresses with mmap; */ 
        printShared (A);
        return;
    }
    if ( (perm = tr[1]) !=NULL  &&  strlen(perm)<4 ) {
        if (strchr(perm, 'r') != NULL) protection|=PROT_READ;
        if (strchr(perm, 'w') != NULL) protection|=PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection|=PROT_EXEC;
    }
    if ( (p = MmapFichero(tr[0], protection, A)) == NULL)
        perror ("Impossible to map the file");
    
    else
    {
        printf ("file %s mapped at %p\n", tr[0], p);
    }
    
    
}

void * ObtenerMemoriaShmget (key_t key, size_t tam, MemoryList A){
    void * p;
    int aux, id, flags = 0777;
    struct shmid_ds s;
    char timec[26]; time_t currentTime; struct tm *localTime;
    time( &currentTime ); localTime = localtime( &currentTime );

    if (tam)    //if tam is not 0 it create it in exclusive mode
        {   flags = flags |IPC_CREAT | IPC_EXCL;    }
                //if tam is 0 tries to access to an already created
    
    if (key == IPC_PRIVATE) //useless
        {   errno = EINVAL; return (NULL);    }
    
    if ((id = shmget (key, tam, flags)) == -1)
        {   return (NULL);    }
    
    if ((p = shmat(id, NULL, 0)) == (void *) -1){
        aux = errno;    //if already created and impossible to map
        if (tam)        //removes
            {   shmctl(id, IPC_RMID, NULL); }
        errno = aux;
        return (NULL);
    }

    shmctl (id, IPC_STAT, &s);
    strftime(timec,26,"%a %b %d %T %Y",localTime);
    /*Save in Shared Memory addresses (p, s.shm_segsz, key...);*/
    insertItem (A, p, tam, timec, Shared,"",0, key);
    return (p);

    
}

void Cmd_AsignarCreateShared (char * tr[], MemoryList A){
    
    key_t k;
    size_t tam = 0;
    void * p;

    if (tr[0] == NULL || tr[1] == NULL)
        { printShared(A); return; }
    
    k = (key_t) strtol(tr[0], NULL, 10);

    if (tr[1] != NULL)
        {   tam = (size_t) atoll(tr[1]) ;}
    
    if ( (p = ObtenerMemoriaShmget(k, tam, A)) == NULL)
        {   perror("Impossible to get memory shmget");   }
    
    else
    {   printf ("Shmget memory with key %d assigned at %p\n", k, p);    }
    
}

void Cmd_AsignarShared (char * tr[], MemoryList A){

    key_t k;
    size_t tam = 0;
    void * p;

    if (tr[0] == NULL ){
        printShared(A);
        return;
    }

    k = (key_t) strtol(tr[0], NULL, 10);
    if ( (p = ObtenerMemoriaShmget(k, tam, A)) == NULL)
        {   perror ("Impossible to get memory shmget"); }
    
    else
    {   printf ("Shmget memory with key %d assigned at %p\n", k, p);    }

}

void CmdAsignar (char * tr[], int size, MemoryList A){
    //use function 'atol' to convert the memory
    //number to char *
    // & to get #address // nothing if an int
    
    if (tr[0] == NULL)
    {   printSimple(A); return;}
    
    if (strcmp(tr[0], "-malloc") == 0)
    {   Cmd_AsignarMalloc(tr+1, A); return; }
    
    if (strcmp(tr[0] , "-mmap") == 0)
    {   Cmd_AsignarMmap(tr+1, A); return; }

    if (strcmp(tr[0], "-crearshared") == 0)
    {   Cmd_AsignarCreateShared(tr+1, A); return; }

    if (strcmp(tr[0], "-shared") == 0)
    {   Cmd_AsignarShared(tr+1, A); return; }

}

void Cmd_DesasignarMalloc (char * tr[], MemoryList A){
    if ( (tr[0] == NULL) )
    {   //Prints list of addresses allocated with -malloc;
        printMalloc(A);
        return; 
    }
    else {
        int size = strtol(tr[0], NULL, 10);
        deleteElement(A, Malloc,size,"",0);
        return;    
    }
}

void Cmd_DesasignarMmap (char * tr[], MemoryList A){
    if (tr[0] == NULL) 
    {
        printMmap(A);
        return;
    }   
    int pos = findElementMapped(A, Mapped, tr[0]);
    if ( pos == (-1) ) 
    {
        printMmap(A);
        return;
    }
    deallocMmap(A, pos);
}

void Cmd_DesasignarShared (char * tr[], MemoryList A){
    if (tr[0] == NULL)
    {
        printShared(A);
        return;
    }   
    int key = strtol(tr[0], NULL, 10);
    int pos = findElementShared(A, Shared, key);
    if ( pos == -1)
    {
        printShared(A);
        return;
    }
    deallocShared(A, pos);
    return;
}

void Cmd_DesasignarAddress (char * tr[], MemoryList A){
    if (tr[0] == NULL){ perror("Command not valid.\n"); return;}

    void * auxad = (void *) strtoul (tr[0], NULL, 16);
    int pos = findAddress (A, auxad);
    
    char * filter = "";
    filter = solveTypeOfAlloc(A->nodes[pos].typeOfAllocation);
      
    if (pos == -1){ perror("Memory address not found"); return; }

    if (strcmp (filter, "malloc") == 0)
    {   delete(A, pos); return; }

    if (strcmp (filter, "mmap") == 0)
    {   deallocMmap(A, pos); return; }

    if (strcmp (filter, "shared") == 0)
    {   deallocShared(A, pos); return; }
}

void CmdDesasignar (char * tr[], int size, MemoryList A){

    if (tr[0] == NULL)
    {   printSimple(A); return; }

    if (strcmp(tr[0], "-malloc") == 0)
    {   Cmd_DesasignarMalloc (tr+1, A); return; }

    if (strcmp(tr[0], "-mmap") == 0)
    {   Cmd_DesasignarMmap (tr+1, A); return; }
    
    if (strcmp(tr[0], "-shared") == 0)
    {   Cmd_DesasignarShared (tr+1, A); return; }

    else
    {   Cmd_DesasignarAddress (tr, A); return; }
}

void CmdBorrarkey (char * tr[]){
    char * key = tr[0]; 
    key_t cl;

    if ( (key == NULL) || ( ( cl =(key_t) strtoul(key,NULL,10) ) == IPC_PRIVATE) )
    {
        printf("  rmkey  clave_valide\n");
        return;
    }
    int id;
    if ((id = shmget (cl, 0, 0666)) == -1 )
    {
        perror ("Impossible to obtain the shared memory.\n");
        return;
    }
    if ( shmctl(id, IPC_RMID, NULL) == -1)
    {
        perror ("Impossible to remove the shared memory.\n");
        return;
    }

}


void CmdMem (char * tr[], MemoryList A, int numtr){
    if (tr[0] == NULL)
    {
        int a = 3; int b = 4; int c = 4;
        /*prints the memory addresses of three program functions, three extern (global) variables
        and three automatic (local) variables.*/
        printf("Program function 1 <printMalloc> : 0x%p.\n", &printMalloc);
        printf("Program function 2 <printShared> : 0x%p.\n", &printShared);
        printf("Program function 3 <printMmap> : 0x%p.\n", &printMmap);
        printf("Global variable 1 <char * tr[]> : 0x%p.\n", &tr);
        printf("Global variable 2 <MemoryList A> : 0x%p.\n", &A);
        printf("Global variable 3 <int numtr> : 0x%p.\n", &numtr);
        printf("Local variable 1 <a> : 0x%p.\n", &a);
        printf("Local variable 2 <b> : 0x%p.\n", &b);
        printf("Local variable 3 <c> : 0x%p.\n", &c);
        return;
    }
    if (strcmp (tr[0], "-malloc") == 0){ printMalloc(A); return;}
    if (strcmp (tr[0], "-shared") == 0){ printShared(A); return;}
    if (strcmp (tr[0], "-mmap"  ) == 0){ printMmap  (A); return;}
    if (strcmp (tr[0], "-all"   ) == 0){ printSimple(A); return;}
}

#define TAMANO 2048
void doRecursiva (int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parameter n:%d at %p.\n", n, &n);
    printf("static array at: %p.\n", estatico);
    printf("automatic array at: %p.\n",automatico);
    n--;
    if (n>0) doRecursiva(n);
}

void CmdRecursiva (char * tr[]){
    int n = strtol(tr[0], NULL, 10);
    doRecursiva(n);
}


void readAux (char *path, void *p, ssize_t i, struct stat stats) {
    int file;

     if ((file=open(path, O_RDONLY)) == -1)
            printf("Could not open the specified file\n");

  if ((i==-1) || (i > stats.st_size))
            i=(ssize_t) stats.st_size;

    if (read(file,p, i)==-1) {
        close(file);
        printf("Could not write %ld bytes of file %s%s%s in %p\n", i, BLUE , path , WHITE, p);
    }
  else printf("%ld bytes of file %s%s%s have been written in %p\n", i, BLUE , path, WHITE, p);
    close (file);
}

void CmdRead (char* tr[], int size ) {
  struct stat stats;
  if (lstat(tr[1], &stats) == 0) {
    if (size ==4)
      readAux(tr[1], (char*)strtoul(tr[2],NULL,16), (ssize_t) atoi(tr[3]), stats);
    else if (size ==3)
      readAux(tr[1], (char*)strtoul(tr[2],NULL,16),(ssize_t) -1, stats);
    else printf("Number of arguments not valid\n");
  }
  else perror(tr[1]);
}

void writeAux (char *path, int perm, void *p, ssize_t i) {
    int file;

    if ((file=open(path,perm,0777)) == -1)
            printf("Could not open the specified file");

    if (write(file,p, i)==-1) {
        close(file);
        printf("Could not write the file %s%s%s with %ld bytes of %p\n", BLUE , path, WHITE, i, p);
        }
  else printf("The file %s%s%s has been written with %ld bytes in %p\n",  BLUE , path, WHITE, i, p);
    close (file);
}

void CmdWrite (char* tr[], int size) {
    if (size==4)
        writeAux(tr[1], O_RDONLY | O_EXCL | O_WRONLY | O_CREAT, (char*)strtoul(tr[2],NULL,16), (ssize_t) atoi(tr[3]));
  else if ((size==5) && (strcmp(tr[1], "-o")==0))
      writeAux(tr[2], O_RDONLY | O_WRONLY | O_TRUNC | O_CREAT , (char*)strtoul(tr[3],NULL,16), (ssize_t) atoi(tr[4]));
    else printf("Arguments not valid\n");
}
void CmdVolcar(char *tr[]){
  char* mem=(char*)strtoul(tr[0],NULL,16);
  int lenght = 25;
  int j = 0,count = 0,flag = 1;
  int actualLine = 0;
   if (tr[1]!=NULL){
        lenght = atoi(tr[1]);
    }
   lenght *=2;

  for(j= 0; j <= lenght-1; j++){
    actualLine = j/25;
    if (flag == 1 && j % 25 == 0 ){printf("\n");}
    if (j % 50 == 0 ){printf("\n");}
    if( (count == 0 && actualLine % 2 == 0)  || (count > 0 && j % 50 < count)) {
        if(isprint(mem[j-25*(actualLine/2)]))
            printf("%3c",mem[j-25*(actualLine/2)]);
         else printf("  ");
         if (count > 0 && j % 50 == count-1){printf("\n");}
        }
    
    else {
         printf("%3x",mem[j-count-25*((actualLine+flag)/2)]);
    }
    if (flag == 1 && (lenght / 50 == j / 50))
        { 
            count = (lenght - j)/2;
            flag--;
        }
  } 
  printf("\n");
} 

void CmdLlenar (char * tr[],int size){
    int value = 128;
    int number =  65;
    char input[3] = "";
    void* address = (void*)strtoul(tr[0],NULL,16);
    if (size > 1){
        sscanf(tr[1], "%d", &value);
        if(size == 3){
        if(strncmp(tr[2],"0x",2) == 0){
        number = (int)strtol(tr[2], NULL, 16);
        }
        else if (strncmp(tr[2],"'",1) == 0){
            strncpy(input,tr[2],3);
            number = (int) input[1];
        }
        else {
            number = atoi(tr[2]);
             }
        }
        memset(address,number,value);
    }
}

/****************************************************/
int TrocearCadena(char * cadena, char * trozos[])
{ int i=0;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL){
    return 0;
    }
    
    i = 1;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL){
        
        i++;
    }
    return i;
}

struct COMMAND cmd[]={
    {"autores", CmdAutores},
    {"pid", CmdPid},
    {"cdir", CmdCdir},
    {"fecha", CmdFecha},
    {"hora", CmdHora},
    {"fin", CmdExit},
    {"end", CmdExit},
    {"exit", CmdExit},
    {"hist", CmdHist},
    {"crear", CmdCrear},
    {"borrar", CmdBorrar},
    {"listar", CmdListar},
    {"info", CmdInfo},
    {"asignar", CmdAsignar},
    {"desasignar", CmdDesasignar},
    {"borrarkey", CmdBorrarkey},
    {"mem", CmdMem},
    {"volcar", CmdVolcar},
    {"llenar", CmdLlenar},
    {"recursiva", CmdRecursiva},
    {"rfich", CmdRead},
    {"wfich", CmdWrite},
    {NULL, NULL}
};


void ProcessInput (char * inp, Listh L, MemoryList A)
{
    char *tr[MAXLINE/2];
    int i;
    int numtr= 0;
    insertItemH(L,inp);
         //HERE types exception
    //int *j = TrocearCadena(inp, tr);
    if ( ( numtr = TrocearCadena(inp, tr)) == 0){
        
        return;
    }
    
    numtr = numtr -1;               
    
    for (i = 0; cmd[i].name != NULL; i++){
        
        if (strcmp(tr[0], cmd[i].name) == 0){   //check the attributes that we give to the functions
            (*cmd[i].pfunc)(tr+1, numtr, A, L);    // Once you find the function (tr[0]), you move the position of tr 1 el. -> now the attribute is located at tr[0]
            return;
        }
    }
    printf("%s not found\n",tr[0]);
}
    

int main (int argc, char *argv[])
{
    int i;
    char input [MAXLINE];
    //we initialize the list for the history
    Listh L;
    MemoryList A;
    if(createListH(&L) && createList(&A)){

    for (i=0; i<argc; i++){
        printf("main's %dth argument value is: %s\n", i, argv[i]);
    }
    while(1){
            printf (WHITE "#) ");
            fgets (input, MAXLINE, stdin);
            ProcessInput (input, L, A);
            
    }

    deleteListH(&L);
    deleteList(&A);
    }
    else (printf(" Insuficient memory available"));
}
