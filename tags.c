#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define DEFAULT_TAGS_HOME = "/"
#define DEFAULT_TAGS_DIR = ".tags"

int usage()
{
    printf("Command not specified. \n"
           "tags <option> <arguments> \n"
           "Options: \n"
           "    add <filepath> <tagname>        - Link a tag with a file \n"
           "    remove <filepath> <tagname>     - Remove a tag from file \n"
           "    addtag | at <tagname>                - Create a new tag \n"
           "    removetag <tagname>             - Delete a tag \n"
           "    list                            - List all tags or list for a file \n"
           "    search <tagname>                - Search all files with a tag name \n");
    return 0;
}

char *get_tags_dir() // note: remember to free *basedir
{

    struct passwd *pws;
    pws = getpwuid(geteuid());
    char *dir = (char *)malloc(50 * sizeof(char)); // undone: check if dir is too long, over 50

    strcpy(dir, "/home/");
    strcat(dir, pws->pw_name); // get dir with user name
    strcat(dir, "/.tags");

    return dir;
}

char *get_tag_dir(char *tagName) // note: remember to free *tagdir
{
    char *tagdir = (char *)malloc(50 * sizeof(char));
    // undone: check if dir is too long, over 50
    char *basedir = get_tags_dir();

    strcpy(tagdir, basedir);
    strcat(tagdir, "/");
    strcat(tagdir, tagName);

    free(basedir);
    return tagdir;
}

void check_create_base_folder()
{
    char *basedir = get_tags_dir();

    struct stat st = {0};
    if (stat(basedir, &st) == -1)
    {
        mkdir(basedir, 0700);

        // int check = mkdir(basedir, 0777);
        // printf("----check base folder: check: %d\n", check);
        // if (check == 0)
        // {
        //     printf("base folder created \n");
        // }
        // else
        // {
        //     printf("STH WRONG with create base folder\n");
        // }
    }
    // else
    // {
    //     printf("basedir already exist\n");
    // }
    free(basedir);
}

void check_create_tag_folder(char *tagName)
{
    // note: tmp tag name for testing
    // char *newtagname = (char *)malloc(50 * sizeof(char));
    // strcpy(newtagname, "new222tagname");

    char *tagdir = get_tag_dir(tagName);

    struct stat st = {0};
    if (stat(tagdir, &st) == -1)
    {
        mkdir(tagdir, 0777);

        // int check = mkdir(tagdir, 0777);
        // if (check == 0)
        // {
        //     printf("tag folder created \n");
        // }
        // else
        // {
        //     printf("STH WRONG with create tag folder\n");
        // }
    }
    // else
    // {
    //     printf("tagdir already exist\n");
    // }

    free(tagdir);
}


int get_linked_file_name() {}

int create_tag(char *tagName) {
    check_create_tag_folder(tagName);
}

int delete_tag() {}

int tag_file(char *tagName) {
    check_create_tag_folder(tagName);
}

int tag_exists() {}

int create_random_string() {}

int search_tag() {}

int list_tags() {
    struct dirent *de; 
    char *base_file = get_tags_dir();

    /*char cmd[1024];                           //syscall
    memset(cmd, '\0', sizeof(cmd));
    strcpy(cmd, "ls ");
    strcat(cmd, base_file);
    system(cmd);*/    

    DIR *dr = opendir(base_file);

    if (dr == NULL) 
    {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL) {
        if ((strcmp(de->d_name, ".") !=0) && (strcmp(de->d_name, "..") !=0))
            printf("%s\n", de->d_name);    
           
    }
 
    closedir(dr);    

    return 0;
}

int remove_tag() {}

int remove_all_tag(){

    struct dirent *de;  
    char *base_file = get_tags_dir();
    char cmd[1024];
    memset(cmd, '\0', sizeof(cmd));
    strcpy(cmd, "rm -R ");
    strcat(cmd, base_file);
    strcat(cmd, "/*");
    //printf("%s\n", cmd);
    system(cmd);

    return 0;
}



int main(int argc, char *argv[])
{

    // check & create if base folder exists
    check_create_base_folder();

    if (argc == 1)
    {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "add") == 0)
    {
        printf("add \n");
        tag_file(argv[2]);
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        printf("remove \n");
    }
    else if ((strcmp(argv[1], "addtag") == 0) || (strcmp(argv[1], "at") == 0))
    {
        printf("addtag \n");
        create_tag(argv[2]);
    }
    else if (strcmp(argv[1], "removetag") == 0)
    {
        printf("removetag \n");
    }
    else if (strcmp(argv[1], "search") == 0)
    {
        printf("search \n");
    }
    else if (strcmp(argv[1], "list") == 0)
    {
        printf("list \n");
        list_tags();
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        printf("help \n");
        usage();
    }
    else if ((strcmp(argv[1], "removealltag") == 0) || (strcmp(argv[1], "rma") == 0)){
        printf("remove all tags \n");
        remove_all_tag();
    }
    else
    {
        usage();
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);
    else
        perror("getcwd() error");

    return 0;
}

