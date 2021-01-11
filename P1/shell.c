
#define _XOPEN_SOURCE 500 //for nftw()
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ftw.h>    //this one is for nftw()
#include "array.c"  //this command imports the file 'array.c', which is the location of the list.
//now defines
#define MAXLINE 2048 //Multiple of 4
#define MAXNAME 1024 
//Colorful life is always better
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"

/************************************************/

struct COMMAND{
    char *name;
    void (*pfunc) (/*char * tr[], int size, tList * L*/) ;   //specified inside prcess input

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
    int month = localDate->    tm_mon + 1;  // Range of month 0..11 -> so we add one
    int year  = localDate->tm_year + 1900;  // Starting year 1900

    printf("Local day (Day/Month/Year): %d/%d/%d\n", day, month, year);
    

}

void CmdHora ()
{
    time_t currentTime = time( &currentTime ); // Declaration of currentTime variable
    struct tm *lTime = localtime( &currentTime );  // Data and time info are saved in structure tm (localTime)
 
    printf("Local current time: %02d:%02d:%02d\n",  lTime->tm_hour, lTime->tm_min, lTime->tm_sec);
}


void CmdHist(char * tr[], int numtr, tList *    L){
    if (tr[0] != NULL && strcmp(tr[0],"-c")==0 ){
        //clear history == delete list
        deleteList(L);
        return;
    }
    else
    {
        //acces history == print list
        printList(*L);  
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
                printf("Create error.\n");
                }
            }
    }
    else {
        //throw error
        perror("Non arguments found\n");
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
    printf("Non arguments found\n");
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

//Used in Listar and Info: Constructs the file permss.
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

//Used in option -l
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

//Used when -l is not selected
void showInfo_simple (char * filename,struct stat * fileStat){
    
     printf("%ld", (long)fileStat->st_size);
     switch (fileStat->st_mode & S_IFMT){
        case S_IFDIR :  printf(BLUE " %10s%s \n" WHITE," ",filename); ;  break; 
        case S_IFLNK :  printf(GREEN "%10s%s\n"WHITE," ",filename); ;  break; 
    default : printf(WHITE "%10s%s\n"," ",filename);
    }
 }


void CmdInfo (char * tr[],int size){
    int i = 0;
    struct stat fileStat;
    for (i = 0; i < size; ++i)
    {
        if(lstat(tr[i],&fileStat) == -1) {    // cannot stat
        perror(tr[i]);
        }

        else if(lstat(tr[i],&fileStat) != -2){
            showInfo(tr[i],&fileStat);
         }
        else {
            printf("Cannot access %s: No such file or directory\n",tr[i] );
        }
    }
}

//Shows the info of the use of listar
void Usage() {

    fprintf(stderr, "\nListar: lists the directories and/or files supplied to it as command line\n");
    fprintf(stderr, "Syntax : listar [-l] [-v] [-r] name1 name2 name3 ....\n");
    fprintf(stderr, "\nOptions\n-r\tlist subdirectories recursively\n-l\tstands for long listing\n-v\tAvoid listing hidden elements\n");
}
#include <dirent.h> 

//Used to recursively access the dirs
void RecDir(char * basePath,int flag_r,int flag_l,int flag_v) {
    char path[MAXLINE];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    struct stat fileStat;
    // Unable to open directory stream
    if (!dir){
        fprintf(stderr,"Cannot access %s: No such file or directory\n",basePath );
    }
    else{
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
            //Check the option -r
            if (flag_r && (fileStat.st_mode & S_IFDIR)) {
                printf("\n");
                RecDir(path,flag_r,flag_l,flag_v);
                }
            }
        }
    }

    closedir(dir);
    }
}

//Possible options : -r -v -l
void CmdListar (char * tr[],int size){
    int r = 0,l = 0,v = 0;
    int flags = 0;
    if (size > 1){
    
    //Use int flags to set the options
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
                
        }

   for (int i = flags; i < size; ++i){
        printf("Listar : %s\n\n", tr[i]);
        RecDir(tr[i],r,l,v);
        printf("\n");
   }
    }
    else if (size == 1) {
        //printf("here");
        RecDir(tr[0],r,l,v);
    }
    else Usage();
}


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
    {NULL, NULL}
};


void ProcessInput (char * inp, tList * L)
{
    char *tr[MAXLINE/2];
    int i;
    int numtr= 0;
    
         //HERE types exception
    //int *j = TrocearCadena(inp, tr);
    if ( ( numtr = TrocearCadena(inp, tr)) == 0){
        
        return;
    }
    insertItem(L, inp);
    numtr = numtr -1;               
    
    for (i = 0; cmd[i].name != NULL; i++){
        
        if (strcmp(tr[0], cmd[i].name) == 0){   //check the attributes that we give to the functions
            (*cmd[i].pfunc)(tr+1, numtr, L);    // Once you find the function (tr[0]), you move the position of tr 1 el. -> now the attribute is located at tr[0]
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
    tList L;
    createList(&L); 
    
    for (i=0; i<argc; i++){
        printf("main's %dth argument value is: %s\n", i, argv[i]);
    }
    while(1){
            printf (WHITE "#) ");
            fgets (input, MAXLINE, stdin);
            ProcessInput (input, &L);
            
    }
}
