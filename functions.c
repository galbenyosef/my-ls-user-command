#include "functions.h" 

void start(int argc,char** argv){
    while(TRUE){
        
        DIR* cwd;
        char* path;
        char newpath[MAX_SIZE];
        struct dirent* dirContent;

        if (argc==1){
            if (iterator == argc)
               break;
        }            
        else{
            if (iterator == argc-1)
               break;
        }
        iterator++;
            

        if (argc == 1) path = DEFAULT_PATH;
        else path = argv[iterator];
		
        /* traverse back to home folder */
        if (path[0]=='~'){
            char* user = getenv("USER");
            char home[80];
            memcpy(home,"/home/",sizeof("/home/"));

            memcpy(&home[strlen(home)],user,sizeof(user)); 
            home[strlen(home)+strlen(user)]='\0';
            memcpy(newpath,home,strlen(home)+1);
            memcpy(path,path+1,strlen(path));
        }

        newpath[strlen(path)+strlen(newpath)+1]='\0';
        newpath[strlen(path)+strlen(newpath)]='/';
        memcpy(&newpath[strlen(newpath)],path,strlen(path));
        
        /* open directory to read its contents*/
        if(! (cwd = opendir(newpath)) ){
            printf("ls: cannot access '%s': No such file or directory\n",path);
            continue;
        }
        /* get total blocks in directory */
        printTotal(cwd,newpath);
        
        /* start reading files */
        while( (dirContent=readdir(cwd)) != NULL){
            
            struct stat stat_buff;

            char* filename = dirContent->d_name;
 
            memcpy(&newpath[strlen(newpath)],filename,strlen(filename)+1);
            
            //path[strlen(path)]='\0';
            
            if( lstat( newpath, &stat_buff ) == FAIL ){
                perror("could not find file");
                continue;
            }
            
            printFileAttr(stat_buff);
            
            printFileColor(stat_buff,filename);
            
            printFileLink(stat_buff,newpath);
            
            newpath[strlen(newpath)-strlen(filename)]='\0';
            
            printf("\n");
        }
        
        if( closedir(cwd) == FAIL)
            printf("ls: failed closing '%s':\n",path);
    }
}

void printTotal(DIR* cwd,char path[]){
    
    struct dirent* dirContent;
    
    long total;
    while( (dirContent=readdir(cwd)) != NULL){
        struct stat stat_buff;
        char* filename = dirContent->d_name;
        memcpy(&path[strlen(path)],filename,strlen(filename)+1);
        //path[strlen(path)]='\0';
        if(! stat( path, &stat_buff ) == FAIL )
                total+=stat_buff.st_blocks;
        path[strlen(path)-strlen(filename)]='\0';
    }
    printf("total %ld\n",total);

    rewinddir(cwd);
}

void printFileAttr(struct stat _stat_buff){

    struct passwd* pwuid;
    struct group* gruid;
    struct tm* timeinfo;
    char timebuffer[MAX_SIZE];

    if (S_ISDIR(_stat_buff.st_mode)){
        printf("d");
    }
    else if (S_ISLNK(_stat_buff.st_mode)){
        printf("l");
    }
    else
        printf("-");

    printf( (_stat_buff.st_mode & S_IRUSR) ? "r" : "-");
    printf( (_stat_buff.st_mode & S_IWUSR) ? "w" : "-");
    printf( (_stat_buff.st_mode & S_IXUSR) ? "x" : "-");
    printf( (_stat_buff.st_mode & S_IRGRP) ? "r" : "-");
    printf( (_stat_buff.st_mode & S_IWGRP) ? "w" : "-");
    printf( (_stat_buff.st_mode & S_IXGRP) ? "x" : "-");
    printf( (_stat_buff.st_mode & S_IROTH) ? "r" : "-");
    printf( (_stat_buff.st_mode & S_IWOTH) ? "w" : "-");
    printf( (_stat_buff.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %lu",_stat_buff.st_nlink);

    pwuid = getpwuid(_stat_buff.st_uid);
    gruid = getgrgid(_stat_buff.st_gid);

    printf(" %s",pwuid->pw_name);
    printf(" %s",gruid->gr_name);
    printf(" %ld",_stat_buff.st_size);

    timeinfo = localtime (&_stat_buff.st_mtim.tv_sec);
    strftime (timebuffer,MAX_SIZE," %b %d %R",timeinfo);

    printf(" %s",timebuffer);   
   
}

void printFileColor(struct stat _stat_buff,char* filename){

    bool isExec=FALSE;
    
    isExec = ((_stat_buff.st_mode & S_IXUSR) || (_stat_buff.st_mode & S_IXGRP) ||
        (_stat_buff.st_mode & S_IXOTH)) ? TRUE : FALSE;

    if (isExec){
        if (S_ISDIR(_stat_buff.st_mode))
            printf(CYAN" %s" RESET,filename);
        else
            printf(GREEN" %s" RESET,filename);
    }
    else{
        printf(" %s",filename);
    }
}

void printFileLink(struct stat _stat_buff,char path[]){
    
    char linkbuffer[MAX_SIZE];
    
    if (S_ISLNK(_stat_buff.st_mode)){
        int target_link;
        if ((target_link=readlink(path, linkbuffer, MAX_SIZE)) > 0){
            linkbuffer[target_link]='\0';
            printf(" -> %s",linkbuffer);
        }
    }
}
