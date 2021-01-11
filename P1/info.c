

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#define MAXLINE 2048 //memory gives 16 bits by 16 bits, so use a 16 multiple
#define MAXNAME 1024 
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"








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
    if (size>0){
    for (i = 1; i <= size; ++i)
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
    else{
        printf("Error: non arguments found.\n");
        return;
    }
}

int main (int argc, char *argv[]){
    CmdInfo(argv, argc-1);
}
