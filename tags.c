#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <ftw.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

int usage();
char *get_base_dir();
char *get_tag_dir(char *tagName);
void check_create_base_folder();
void check_create_tag_folder(char *tagName);
char *get_current_directory(char *fileName);
int ftwremove(const char *name, const struct stat *status, int type);
int ftwlistfile(const char *name, const struct stat *status, int type);
int ftwsearch(const char *name, const struct stat *status, int type);

//menu function
void add_tag(char *tagName);
void remove_tag(char *tagName);
void add_to_file(char *fileName, char *tagName);
void remove_from_file(char *fileName, char *tagName);
void list_all_tags();
void list_tags_of_file(char *fileName);
void search_file_for_tag(char *tagName);
void remove_all_tag();

// note: check it before use it!!
struct stat fileStat = {0};

// undone: check and rename

int main(int argc, char *argv[])
{

    // check & create if base folder exists
    check_create_base_folder();

    if (argc == 1)
    {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "add") == 0 || strcmp(argv[1], "a") ==0)
    {
        // printf("add \n");
        if (argc != 4)
        {
            printf("wrong input\n");
            usage();
            return 0;
        }
        add_to_file(argv[2], argv[3]);
    }
    else if ((strcmp(argv[1], "remove") == 0) || (strcmp(argv[1], "r") == 0))
    {
        // printf("remove \n");
        if (argc != 4)
        {
            printf("wrong input\n");
            usage();
            return 0;
        }
        remove_from_file(argv[2], argv[3]);
    }
    else if ((strcmp(argv[1], "addtag") == 0) || (strcmp(argv[1], "at") == 0))
    {
        // printf("addtag \n");
        if (argc != 3)
        {
            printf("wrong input\n");
            usage();
            return 0;
        }
        add_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "removetag") == 0) || (strcmp(argv[1], "rt") == 0))
    {
        // printf("removetag \n");
        if (argc != 3)
        {
            printf("wrong input\n");
            usage();
            return 0;
        }
        remove_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "search") == 0) || (strcmp(argv[1], "s") == 0))
    {
        // printf("search");
        if (argc != 3)
        {
            printf("wrong input\n");
            usage();
            return 0;
        }
        printf("search \n");
        search_file_for_tag(argv[2]);
    }
    else if ((strcmp(argv[1], "list") == 0) || (strcmp(argv[1], "l") == 0))
    {
        if (argc == 2)
        {
            // printf("list all tags \n");
            list_all_tags();
        }
        else if (argc == 3)
        {
            // printf("list tag of a file \n");
            list_tags_of_file(argv[2]);
        }
        else
        {
            printf("wrong input");
            usage();
        }
    }
    else if ((strcmp(argv[1], "removealltags") == 0) || (strcmp(argv[1], "rat") == 0))
    {
        // printf("remove all tags \n");
        remove_all_tag();
    }
    else
    {
        printf("wrong input");
        usage();
    }

    return 0;
}

int usage()
{
    printf("tags <option> <arguments> \n"
           "Options: \n"
           "    add | a <filename> <tagname>       - Link a tag with a file \n"
           "    addtag | at <tagname>              - Create a new tag \n"
           "    remove | r <filename> <tagname>    - Remove a tag from file \n"
           "    removetag | rt <tagname>           - Delete a tag \n"
           "    removealltag| rat                  - Remove all tags \n"
           "    search | s <tagname>               - Search all files with a tag name \n"
           "    list | l                           - List all tags \n"
           "    list | l <tagname>                 - List all tags of a file \n");
    return 0;
}

// note: remember to free *basedir
char *get_base_dir()
{
    struct passwd *pws;
    pws = getpwuid(geteuid());
    char *dir = (char *)malloc(100 * sizeof(char));

    strcpy(dir, "/home/");
    strcat(dir, pws->pw_name); // get dir with user name
    strcat(dir, "/.tags");

    return dir;
}

// note: remember to free *tagdir
char *get_tag_dir(char *tagName)
{
    char *tagdir = (char *)malloc(100 * sizeof(char));
    char *basedir = get_base_dir();

    strcpy(tagdir, basedir);
    strcat(tagdir, "/");
    strcat(tagdir, tagName);

    free(basedir);
    return tagdir;
}

// note: remember to free *cwd
// check if file exist before, this function wont check
char *get_current_directory(char *fileName)
{
    char temp[1024];
    getcwd(temp, sizeof(temp));

    char *cwd = (char *)malloc(1024 * sizeof(char));
    strcpy(cwd, temp);
    strcat(cwd, "/");
    strcat(cwd, fileName);

    return cwd;
}

void check_create_base_folder()
{
    char *basedir = get_base_dir();

    struct stat st = {0};

    if (stat(basedir, &st) == -1)
    {
        int check = mkdir(basedir, 0777);
        if (check == 0)
        { // printf("base folder created \n");
        }
        else
        {
            printf("Failed to create base folder\n");
        }
    }
    free(basedir);
}

void check_create_tag_folder(char *tagName)
{
    char *tagdir = get_tag_dir(tagName);
    struct stat st = {0};

    if (stat(tagdir, &st) == -1)
    {
        int check = mkdir(tagdir, 0777);
        if (check == 0)
        { //printf("tag folder created \n");
        }
        else
        {
            printf("Failed to create tag folder\n");
        }
    }
    free(tagdir);
}

int ftwremove(const char *name, const struct stat *status, int type)
{
    remove(name);
    return 0;
}

int ftwsearch(const char *name, const struct stat *status, int type)
{
    if (type != FTW_D)
    {
        // get file size
        FILE *fp;
        fp = fopen(name, "r");
        fseek(fp, 0L, SEEK_END);
        int filesize = ftell(fp);
        fclose(fp);

        // read with mapped memory
        int fd;
        void *file_memory;
        fd = open(name, O_RDWR, S_IRUSR | S_IWUSR);
        file_memory = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);

        char *tagname = (char *)malloc(strlen(file_memory) * sizeof(char));
        sscanf(file_memory, "%s", tagname);
        printf("    %s", tagname);
        file_memory += strlen(file_memory) + 5;
        char *filename = (char *)malloc(strlen(file_memory) * sizeof(char));
        sscanf(file_memory, "%s", filename);
        printf("    %s", filename);
        file_memory += strlen(file_memory) + 5;
        char *filepath = (char *)malloc(strlen(file_memory) * sizeof(char));
        sscanf(file_memory, "%s", filepath);
        printf("    %s\n", filepath);

        free(tagname);
        free(filename);
        free(filepath);
        munmap(file_memory, filesize);
    }

    return 0;
}

int ftwlistfile(const char *name, const struct stat *status, int type)
{
    if (type != FTW_D)
    {
        int id = 0;
        char idstr[30]; //id
        id = fileStat.st_ino;
        sprintf(idstr, "%d", id);

        char tagfiledir[100];

        strcpy(tagfiledir, name);
        char *substring;
        substring = strtok(tagfiledir, "/");

        substring = strtok(NULL, "/");
        substring = strtok(NULL, "/");
        substring = strtok(NULL, "/");
        substring = strtok(NULL, "/");

        if (strcmp(substring, idstr) == 0)
        {
            int fd;
            int filesize = 50;
            void *file_memory;
            fd = open(name, O_RDWR, S_IRUSR | S_IWUSR);
            file_memory = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            close(fd);

            char *tagname = (char *)malloc(strlen(file_memory) * sizeof(char));
            sscanf(file_memory, "%s", tagname);
            printf("    %s\n", tagname);

            free(tagname);
            munmap(file_memory, filesize);
        }
    }
    return 0;
}

/*  note: menu function list 
    */
void add_tag(char *tagName)
{
    check_create_tag_folder(tagName);
}

void remove_tag(char *tagName)
{
    char *tagdir = get_tag_dir(tagName);

    ftw(tagdir, ftwremove, 1);
    ftw(tagdir, ftwremove, 1);

    free(tagdir);
}

void add_to_file(char *fileName, char *tagName)
{
    // check if file (in cwd) exist
    struct stat st = {0};

    if (stat(fileName, &st) < 0)
    {
        printf("file %s does not exist\n", fileName);
        return;
    }
    // check & create if tag (folder) exist
    check_create_tag_folder(tagName);

    // get file (in cwd) info
    int id = 0;
    char idstr[30]; //id
    char *filepath; //path
    id = st.st_ino;
    sprintf(idstr, "%d", id);
    filepath = get_current_directory(fileName);

    // path of file (in tag foldr)
    char *tagfile = get_tag_dir(tagName);
    strcat(tagfile, "/");
    strcat(tagfile, idstr);

    int filesize;
    int fd;
    void *file_memory;

    filesize = strlen(tagName) + strlen(fileName) + strlen(filepath) + 15;
    //update if exist, create if not
    fd = open(tagfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    for (int i = 0; i < filesize; i++)
        write(fd, " ", 1);
    write(fd, "\n", 1); // write a NULL at EOF
    file_memory = mmap(NULL, filesize, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    sprintf((char *)file_memory, "%s\n", tagName);
    file_memory += strlen(tagName) + 5; //note: important!! space between Name and path is 5!!
    sprintf((char *)file_memory, "%s\n", fileName);
    file_memory += strlen(fileName) + 5;
    sprintf((char *)file_memory, "%s\n", filepath);

    //release memory
    munmap(file_memory, filesize);
    free(filepath);
    free(tagfile);
}

void remove_from_file(char *fileName, char *tagName)
{
    //check if file(in cwd) exists
    struct stat st = {0};

    if (stat(fileName, &st) < 0)
    {
        printf("file %s does not exist\n", fileName);
        return;
    }

    // get file (in cwd) info
    int id = 0;
    char idstr[30]; //id
    id = st.st_ino;
    sprintf(idstr, "%d", id);

    // path of file (in tag foldr)
    char *tagfile = get_tag_dir(tagName);
    strcat(tagfile, "/");
    strcat(tagfile, idstr);
    remove(tagfile); //remove it

    free(tagfile);
}

void list_all_tags()
{
    struct dirent *de;
    char *base_file = get_base_dir();

    DIR *dr = opendir(base_file);

    if (dr == NULL)
    {
        printf("Could not open current directory");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0))
            printf("%s\n", de->d_name);
    }

    closedir(dr);
    free(base_file);
}

void list_tags_of_file(char *fileName)
{
    if (stat(fileName, &fileStat) < 0)
    {
        printf("file %s does not exist\n", fileName);
        return;
    }
    else
    {
        printf("%s: \n", fileName);
    }

    char *base = get_base_dir();
    ftw(base, ftwlistfile, 1);
    free(base);
}

void search_file_for_tag(char *tagName)
{

    char *tagdir = get_tag_dir(tagName);
    ftw(tagdir, ftwsearch, 1);
    free(tagdir);
}

void remove_all_tag()
{

    printf("Do you really want to delete all tags?  y/n  ");

    char check[50];
    scanf("%s", check);
    if (strcmp(check, "y") == 0)
    {
        struct dirent *de;
        char *base_file = get_base_dir();
        char cmd[256];
        memset(cmd, '\0', sizeof(cmd));
        strcpy(cmd, "rm -R ");
        strcat(cmd, base_file);
        strcat(cmd, "/*");
        system(cmd);
        return;
    }
    else if (strcmp(check, "n") == 0)
    {
        return;
    }
    else
    {
        printf("wrong input.\n\n");
        remove_all_tag();
        return;
    }
    return;
}
