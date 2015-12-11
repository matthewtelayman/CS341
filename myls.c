//Matt Layman CS341 myls.c
//Implements ls commands

#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>      // for typedefs 
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>     // directory entry structure, for struct dirent and DIR

//function prototypes
void Listfile(char name[]);
void Listinode(char name[]);
void RecDir(char *name);
void Filestat(char name[]);


int main (int argc, char *argv[])
{
   char ar[3];
   if (argc == 1)
   {
     // default: print file names in the current directory
      Listfile(".");
   }
   else
      strcpy(ar, *++argv);
      if (strcmp(ar, "-i") == 0)
         Listinode(".");
         printf("\n");

      if (strcmp(ar, "-R") == 0)
         RecDir(".");
         printf("\n");


      if (strcmp(ar, "-l") == 0)
         Filestat(".");
         printf("\n");


   return 0;
}


// print file names for name (%ls)
void Listfile(char name[])
{
   struct dirent *direntpointer;
   DIR *direct;
  
   if ((direct=opendir(name)) == NULL)   // opendir function sets up a connection between directory on disk and the process buffer
   {
      fprintf(stderr, "myls: can't open %s\n", name);
      return;
   }
   else
   {
    while ((direntpointer=readdir(direct)) != NULL) // readdir reads a record from the connection
    printf("%s ", direntpointer->d_name);

      closedir(direct); // close the connection
   }
} // listfile

// print inodes for name (%ls -i)
void Listinode(char name[])
{
   struct dirent *direntpointer;
   DIR *direct;
  
   if ((direct=opendir(name)) == NULL)  // opendir function sets up a connection between directory on disk and the process buffer
   {
      fprintf(stderr, "myls: can't open %s\n", name);
      return;
   }
   else
   {
     while ((direntpointer=readdir(direct)) != NULL) // readdir reads a record from the connection
      printf("%ld %s  ", (long) direntpointer->d_ino, direntpointer->d_name);

      closedir(direct); // close the connection
   }
} // listfile

//recursively print file names for directiories/sub-directories (%ls -R)
void RecDir(char *path) {
    DIR *dp = opendir(path);
    if(!dp) {
        perror(path);
        return;
    }
    struct dirent *ep;
    char newdir[512];
    printf("\n%s :\n", path);
    while((ep = readdir(dp)))
        if(strncmp(ep->d_name, ".", 1))
            printf("\t%s\n", ep->d_name);
    closedir(dp);
    dp = opendir(path);
    while((ep = readdir(dp))) if(strncmp(ep->d_name, ".", 1)) {
        if(ep->d_type == 4) {
            sprintf(newdir, "%s/%s", path, ep->d_name);
            RecDir(newdir);
        }
    }
    closedir(dp);
}


//implement %ls -l
void Filestat(char name[])
{
   struct dirent *direntpointer;
   struct stat mystat;
   DIR *direct;

  
   if ((direct=opendir(name)) == NULL)  // opendir function sets up a connection between directory on disk and the process buffer
   {
      fprintf(stderr, "myls: can't open %s\n", name);
      return;
   }
   else
   {
     while ((direntpointer=readdir(direct)) != NULL) // readdir reads a record from the connection
     {
      if(stat(direntpointer->d_name, &mystat) < 0)
          return;
      struct passwd *pw = getpwuid(mystat.st_uid);
      struct group *gr = getgrgid(mystat.st_gid);
      time_t t = mystat.st_mtime;
      struct tm lt;
      localtime_r(&t, &lt);
      char timebuff[100];
      char *tim;
      strftime(timebuff, sizeof(timebuff), "%c", &lt);
      tim = strdup(timebuff);

      printf((S_ISDIR(mystat.st_mode)) ? "d" : "-");
      printf((mystat.st_mode & S_IRUSR) ? "r" : "-");
      printf((mystat.st_mode & S_IWUSR) ? "w" : "-");
      printf((mystat.st_mode & S_IXUSR) ? "x" : "-");
      printf((mystat.st_mode & S_IRGRP) ? "r" : "-");
      printf((mystat.st_mode & S_IWGRP) ? "w" : "-");
      printf((mystat.st_mode & S_IXGRP) ? "x" : "-");
      printf((mystat.st_mode & S_IROTH) ? "r" : "-");
      printf((mystat.st_mode & S_IWOTH) ? "w" : "-");
      printf((mystat.st_mode & S_IXOTH) ? "x" : "-");
      printf(" %ld %s %s %ld %s %s\n", (long) mystat.st_nlink, pw->pw_name, gr->gr_name, (long) mystat.st_size, tim, direntpointer->d_name);

     }
    closedir(direct); // close the connection
   }
}
