

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include<pwd.h>
#define MAXLINE 2048 //memory gives 16 bits by 16 bits, so use a 16 multiple
#define MAXNAME 1024 
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"

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
    for (int i = 1; i < size; ++i)
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

   for (int i = flags+1; i <= size; ++i){
        printf("Listar : %s\n\n", tr[i]);
        RecDir(tr[i],r,l,v);
        printf("\n");
   }
    }
    else if (size == 1) {
        //printf("here");
        RecDir(tr[1],r,l,v);
    }
    else Usage();
}


int main(int argc, char  *argv[])
{   
    CmdListar(argv,argc-1);
}
