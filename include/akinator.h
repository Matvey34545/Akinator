#ifndef AKINATOR
#define AKINATOR

#include "bynar_tree.h"

enum ErrorAkinator
{
    AKIN_ERROR_NO         = 0,
    AKIN_ERROR_BYNAR_TREE = 1,
    AKIN_ERROR_OPEN_FILE  = 2,
    AKIN_ERROR_SYNTAX     = 3,
    AKIN_NULL_PTR         = 4,
    AKIN_ERROR_ALLOCATION = 5,
    AKIN_ERROR_READ       = 6,
    AKIN_ERROR_DATABASE   = 7,
    AKIN_BAD_GRAPHIS      = 8,
    AKIN_ERROR_STACK      = 9
};

struct Database
{
    size_t size;
    char *ptr;
};

struct ValueTop
{
    char* str;
    bool is_download_in_base;
};

ErrorAkinator find_object(BynarTree *tree);
void dtor_akinator(BynarTree* tree, Database *database);
ErrorAkinator choice_read_database(BynarTree *tree, Database *database);
ErrorAkinator give_definition(const BynarTree *tree);
ErrorAkinator compare_words(const BynarTree *tree);

#endif
