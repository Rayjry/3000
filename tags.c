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
           "    remove | rm <filepath> <tagname>     - Remove a tag from file \n"
           "    addtag | at <tagname>                - Create a new tag \n"
           "    removetag | rmt <tagname>             - Delete a tag \n"
           "    list | ls                            - List all tags or list for a file \n"
           "    search <tagname>                - Search all files with a tag name \n");
    return 0;
}

char *get_current_directory(){
    char *buffer;
    char *dir;
    long size;

    size = pathconf(".", _PC_PATH_MAX);
    if ((buffer = (char *)malloc((size_t)size)) != NULL)
        dir = getcwd(buffer, (size_t)size);
    //printf("current dir is %s\n", dir);
    return dir;
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

char *tag_exists(char *tagName) {
    char *tagPath = get_tag_dir(tagName);
    if (access(tagPath, F_OK) != -1){
        //printf("exist %s\n", tagPath);
        return tagPath;
    }
    else
    return NULL;
}

int get_linked_file_name(char *tagName) {}

int create_tag(char *tagName) {
    check_create_tag_folder(tagName);
}

int delete_tag(char *tagName) {
    char *tag = tag_exists(tagName);
    char cmd[256];
    if ( tag == NULL){
        printf("tag %s does not exist\n", tagName);
        return -1;
    }

    memset(cmd, '\0', sizeof(cmd));
    strcpy(cmd, "rm -R ");
    strcat(cmd, tag);
    system(cmd);
}

int add_file_tag(char *filePath, char *tagName) {
    char *tag = tag_exists(tagName);
    if (access(filePath, F_OK) == -1){
        printf("file %s does not exist\n", filePath);
        return -1;
    }
    if ( tag == NULL) {
        printf("tag %s does not exist, create one\n", tagName);
        check_create_tag_folder(tagName);
    }
    char *fileDir = get_tag_dir(tagName); 
    strcat(fileDir, "/");
    strcat(fileDir, filePath);

    struct stat st = {0};
    if (stat(fileDir, &st) == -1)
    {
        char cmd[256];
        memset(cmd, '\0', sizeof(cmd));
        strcpy(cmd, "touch ");
        strcat(cmd, fileDir);

        FILE *fp = fopen(fileDir, "w");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }else{
            fputs(get_current_directory(), fp);
        }
        fclose(fp);
    }
    else
    {
        printf("tag %s has already been added to file %s\n",tagName, filePath);
    }
}

int search_file_for_tag(char *tagName) {
    char tagFile[256];
    char *tag = tag_exists(tagName);
    memset(tagFile, '\0', sizeof(tagFile));
    if (tag == NULL){
        printf("tag %s does not exist\n", tagName);
        return -1;
    }
    else 
    {
        strcpy(tagFile, tag);
        struct dirent *de; 
        FILE *fp;
        char buff[256];
        DIR *dr = opendir(tagFile);
        char fileName[256];
        memset(fileName, '\0', sizeof(fileName));
        if (dr == NULL) 
        {
            printf("Could not open current directory" );
            return 0;
        }

        while ((de = readdir(dr)) != NULL) {
            if ((strcmp(de->d_name, ".") !=0) && (strcmp(de->d_name, "..") !=0))
            { 
                printf("file name: %s; ", de->d_name);    
                strcpy(fileName, tagFile);
                strcat(fileName, "/");
                strcat(fileName, de->d_name);
                fp = fopen(fileName, "r");
                fscanf(fp, "%s", buff);
                printf("location:%s\n",buff );
            }
        }
 
    closedir(dr);    
    }

}

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

int remove_file_tag(char *filePath, char *tagName) {
    char *tag = tag_exists(tagName);
    if (access(filePath, F_OK) == -1){
        printf("file %s does not exist in current directory\n", filePath);
        return -1;
    }
    if ( tag == NULL) {
        printf("tag %s does not exist\n", tagName);
        return -1;
    }
    strcat(tag, "/");
    strcat(tag, filePath);
    remove(tag);

}

int remove_all_tag(){

    struct dirent *de;  
    char *base_file = get_tags_dir();
    char cmd[256];
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
    //get_current_directory();

    if (argc == 1)
    {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "add") == 0)
    {
        printf("add \n");
        if (argc != 4){
            printf("wrong input\n");
            usage();
            return -1;
        }
        add_file_tag(argv[2], argv[3]);
    }
    else if ((strcmp(argv[1], "remove") == 0) || (strcmp(argv[1], "rm") == 0))
    {
        printf("remove \n");
        if (argc != 4){
            printf("wrong input\n");
            usage();
            return -1;
        }
        remove_file_tag(argv[2], argv[3]);
    }
    else if ((strcmp(argv[1], "addtag") == 0) || (strcmp(argv[1], "at") == 0))
    {
        printf("addtag \n");
        create_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "removetag") == 0) || (strcmp(argv[1], "rmt") == 0))
    {
        printf("removetag \n");
        delete_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "search") == 0) || (strcmp(argv[1], "se") == 0))
    {
        printf("search \n");
        search_file_for_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "list") == 0) || (strcmp(argv[1], "ls") == 0))
    {
        printf("list \n");
        list_tags();
    }
    else if (strcmp(argv[1], "exist") == 0)
    {
        printf("exist \n");
        tag_exists(argv[2]);
    }
    else if ((strcmp(argv[1], "removealltag") == 0) || (strcmp(argv[1], "rma") == 0))
    {
        printf("remove all tags \n");
        remove_all_tag();
    }
    else
    {
        usage();
    }

    
    return 0;
}

