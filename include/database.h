#ifndef DATABASE
#define DATABASE

#include "bynar_tree.h"
#include "akinator.h"

ErrorAkinator write_database(const BynarTree *tree, const char* filename);
ErrorAkinator read_database(BynarTree *tree, const char *filename, Database *database);

#endif
