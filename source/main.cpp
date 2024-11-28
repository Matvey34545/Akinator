#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "bynar_tree.h"
#include "database.h"
#include "dump.h"
#include "akinator.h"

static void menu();
static ErrorAkinator executes_command(char choice, BynarTree *tree, Database *database);
static ErrorAkinator exit_programm(BynarTree *tree, Database *database);

int main()
{
    BynarTree tree = {};
    create_bynar_tree(&tree, sizeof(char*));

    menu();

    char choice = '\0';
    ErrorAkinator error = AKIN_ERROR_NO;
    Database database = {};
    scanf("%c", &choice);
    getchar();

    while (choice != 'g')
    {
        error = executes_command(choice, &tree, &database);
        if (error != AKIN_ERROR_NO)
                return error;

        scanf("%c", &choice);
        getchar();
    }
    error = exit_programm(&tree, &database);
    return error;
}

static void menu()
{
    printf("Hi! My name is Akinator.\n"
           "Download the database (a).\n"
           "Guess the word (b).\n"
           "Give a definition (ñ).\n"
           "Print the database (d).\n"
           "Compare words (e).\n"
           "Open the menu (f).\n"
           "Exiting the program (g).\n\n");
}

static ErrorAkinator executes_command(char choice, BynarTree *tree, Database *database)
{
    ErrorAkinator error = AKIN_ERROR_NO;

    switch (choice)
    {
        case 'a':
            error = choice_read_database(tree, database);
            if (error != AKIN_ERROR_NO)
                return error;

            break;

        case 'b':
            error = find_object(tree);
            if (error != AKIN_ERROR_NO)
                return error;
            break;

        case 'c':
            error = give_definition(tree);
            if (error != AKIN_ERROR_NO)
                return error;
            break;

        case 'd':
            error = dump(tree, NULL, NULL);
            if (error != AKIN_ERROR_NO)
                return error;
            break;

        case 'f':
            menu();
            break;

        case 'g':
            break;

        default:
            printf("Sorry, I didn't recognize the command.\n");
            menu();
            break;
    }
    return AKIN_ERROR_NO;
}

static ErrorAkinator exit_programm(BynarTree *tree, Database *database)
{
    printf("Are you want save database?\n");

    if (enter_answer())
    {
        printf("Please enter the name of the file where you want to save the file.\n");

        char filename[SIZE_ANSWER] = {};
        if (fgets(filename, SIZE_ANSWER - 1, stdin) == NULL)
            return AKIN_ERROR_READ;

        filename[strlen(filename) - 1] = '\0';

        ErrorAkinator error = write_database(tree, filename);
        if (error != AKIN_ERROR_NO)
            return error;
    }
    dtor_akinator(tree, database);

    return AKIN_ERROR_NO;
}



