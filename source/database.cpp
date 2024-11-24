#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>

#include "database.h"

const char *NAME_DATABASE = "database.txt";

const char SYMBOL_NO_TOP   = '-';
const char SEP_SYMBOL      = '{';
const char BACK_SEP_SYMBOL = '}';
const char SEP_STR         = '\n';

static void writing_subtree(const Top *top, FILE *fp);
static ErrorAkinator write_in_memory(Database *database, const char *filename);
static void changes_symbols(Database *database, char src, char desrc);
static void skip_spaces(char **src);
static ErrorAkinator read_tree(char **str, Top **top, Top **parent_top, bool is_left_top);


ErrorAkinator write_database(const BynarTree *tree, const char* filename)
{
    if (filename == NULL)
        filename = NAME_DATABASE;

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
        return AKIN_ERROR_OPEN_FILE;

    if (tree->root != NULL)
        writing_subtree(tree->root, fp);

    fclose(fp);

    return AKIN_ERROR_NO;
}


static void writing_subtree(const Top *top, FILE *fp)
{
    fprintf(fp, "%c\n%s\n", SEP_SYMBOL, ((ValueTop*)top->element)->str);

    if (top->left != NULL)
        writing_subtree(top->left, fp);

    else
        fprintf(fp, "%c\n", SYMBOL_NO_TOP);

    if (top->right != NULL)
        writing_subtree(top->right, fp);

    else
        fprintf(fp, "%c\n", SYMBOL_NO_TOP);

    fprintf(fp, "%c\n", BACK_SEP_SYMBOL);
}

ErrorAkinator read_database(BynarTree *tree, const char *filename, Database *database)
{
    ErrorAkinator error = write_in_memory(database, filename);
    if (error != AKIN_ERROR_NO)
        return error;

    char *str = database->ptr;
    read_tree(&str, &tree->root, &tree->root, true);

    changes_symbols(database, SEP_STR, '\0');

    return AKIN_ERROR_NO;
}

static ErrorAkinator write_in_memory(Database *database, const char *filename)
{
    if (filename == NULL)
        filename = NAME_DATABASE;

    struct stat st = {};
    stat(filename, &st);
    database->size = st.st_size;

    database->ptr = (char*) calloc(database->size + 1, sizeof(char));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return AKIN_ERROR_OPEN_FILE;

    fread(database->ptr, sizeof(char), database->size, fp);

    fclose(fp);

    return AKIN_ERROR_NO;
}

static void changes_symbols(Database *database, char src, char desrc)
{
    for (int i = 0; i < database->size; i++)
    {
        if (database->ptr[i] == src)
            database->ptr[i] = desrc;
    }
}

static void skip_spaces(char **src)
{
    while (isspace(**src) != 0)
        (*src)++;
}

static ErrorAkinator read_tree(char **str, Top **top, Top **parent_top, bool is_left_top)
{
    skip_spaces(str);
    if (**str != SEP_SYMBOL)
        return AKIN_ERROR_SYNTAX;

    (*str)++;
    skip_spaces(str);

    ValueTop value_top = {*str, true};
    ErrorBynarTree error_tree = create_bynar_top(&value_top, sizeof(ValueTop), top);
    if (error_tree != BYNAR_OK)
        return AKIN_ERROR_BYNAR_TREE;

    if (*parent_top != NULL)
    {
        if (is_left_top)
            (*parent_top)->left = *top;

        else
            (*parent_top)->right = *top;
    }

    while (**str != SEP_STR && **str != '\0')
        (*str)++;

    if (**str == '\0')
        return AKIN_ERROR_SYNTAX;

    skip_spaces(str);

    if (**str != SYMBOL_NO_TOP)
        read_tree(str, &(*top)->left, top, /*is_left_top */ true);

    else
        (*str)++;

    skip_spaces(str);

    if (**str != SYMBOL_NO_TOP)
        read_tree(str, &(*top)->right, top, /*is_left_top */false);

    else
        (*str)++;

    skip_spaces(str);
    if (**str != BACK_SEP_SYMBOL)
        return AKIN_ERROR_SYNTAX;

    (*str)++;
    skip_spaces(str);

    return AKIN_ERROR_NO;
}




















