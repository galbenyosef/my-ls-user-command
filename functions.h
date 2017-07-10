
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#define TRUE 1
#define FALSE 0
#define FAIL -1
#define DEFAULT_PATH "."
#define MAX_SIZE 4096


#define GREEN  "\x1B[32m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <string.h>

static int iterator = 0;
/*
 * 
 */
void start(int argc,char** argv);
void printTotal(DIR* cwd,char path[]);
void printFileAttr(struct stat _stat_buff);
void printFileColor(struct stat _stat_buff,char* filename);
void printFileLink(struct stat _stat_buff,char path[]);


#endif /* FUNCTIONS_H */

