#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_TAGS_HOME = "/"
#define DEFAULT_TAGS_DIR = ".tags"

int usage()
{
    printf("Command not specified. \n"
           "tags <option> <arguments> \n"
           "Options: \n"
           "    add <filepath> <tagname>        - Add a new tag to file \n"
           "    remove <filepath> <tagname>     - Remove a tag from file \n"
           "    addtag <tagname>                - Create a new tag \n"
           "    removetag <tagname>             - Delete a tag \n"
           "    list                            - List all tags or list for a file \n"
           "    search <tagname>                - Search all files with a tag name \n");
    return 0;
}

char *get_tags_dir()
{

    // char *dir = ;
    // return;
}

int get_tag_dir() {}

int check_creat_base_folder()
{
}

int get_linked_file_name() {}

int creat_tag() {}

int delete_tag() {}

int tag_file() {}

int tag_exists() {}

int create_random_string() {}

int search_tag() {}

int list_tags() {}

int remove_tag() {}

int main(int argc, char *argv[])
{
    system("dir");
    // printf("%sred\n", KRED);
    // printf("%sgreen\n", KGRN);
    // printf("%syellow\n", KYEL);
    // printf("%sblue\n", KBLU);
    // printf("%smagenta\n", KMAG);
    // printf("%scyan\n", KCYN);
    // printf("%swhite\n", KWHT);
    // printf("%snormal\n", KNRM);

    if (argc == 1)
    {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "add") == 0)
    {
        printf("add \n");
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        printf("remove \n");
    }
    else if (strcmp(argv[1], "addtag") == 0)
    {
        printf("addtag \n");
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
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        printf("help \n");
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
