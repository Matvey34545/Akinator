#include <string.h>
#include <stdio.h>

#include "bynar_tree.h"
#include "database.h"
#include "dump.h"
#include "akinator.h"

void menu();

int main()
{
    BynarTree tree = {};
    create_bynar_tree(&tree, sizeof(char*));

    menu();

    char choice = '\0';
    ErrorAkinator error = AKIN_ERROR_NO;
    Database database = {};
    choice_read_database(&tree, &database);
    find_object(&tree);
    dump(&tree, NULL, NULL);
    give_definition(&tree);
    compare_words(&tree);
    //scanf(" %c", &choice);

//    while (choice != 'g')
//    {
//    switch (choice)
//    {
//        case 'a':
//            error = choice_read_database(&tree, &database);
//            if (error != AKIN_ERROR_NO)
//                return error;
//            break;
//
//        case 'b':
//            error = find_object(&tree);
//            if (error != AKIN_ERROR_NO)
//                return error;
//            break;
//
//        case 'c':
//            error = give_definition(&tree);
//            if (error != AKIN_ERROR_NO)
//                return error;
//            break;
//
//        case 'd':
//            error = dump(&tree, NULL, NULL);
//            if (error != AKIN_ERROR_NO)
//                return error;
//            break;
//
//        case 'f':
//            menu();
//            break;
//
//        case 'g':
//            break;
//
//        default:
//            printf("Sorry, I didn't recognize the command.\n");
//            menu();
//            break;
//    }
//    scanf(" %c ", &choice);
//    }

}

void menu()
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

