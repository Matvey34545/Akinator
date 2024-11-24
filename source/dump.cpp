#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dump.h"

const char *DOT_FILE = "dump/dump.dot";
const char *PNG_FILE = "img/dot.png";

static void create_dot_top(const Top *top, int *number_top, FILE *fp);
static void create_communication(const Top *top, int *number_top, FILE *fp);
static ErrorAkinator create_dot_file(const char *filename, const BynarTree *tree);
static ErrorAkinator create_png_file(const char *filename, const char *png_file);

ErrorAkinator dump(const BynarTree *tree, const char *filename, const char *png_file)
{
    ErrorAkinator error = create_dot_file(filename, tree);
    if (error != AKIN_ERROR_NO)
        return error;

    error = create_png_file(filename, png_file);
    return error;
}

static ErrorAkinator create_png_file(const char *filename, const char *png_file)
{
    if (png_file == NULL)
        png_file = PNG_FILE;

    if (filename == NULL)
        filename = DOT_FILE;

    int size_str = strlen("dot -Tpng ") + strlen(" -o ") + strlen(filename) + strlen(png_file) + 1;

    char* str = (char*)calloc(size_str, sizeof(char));
    char* ptr = str;

    strcpy(ptr, "dot -Tpng ");
    ptr += strlen("dot -Tpng ");

    strcpy(ptr, filename);
    ptr += strlen(filename);

    ptr = strcpy(ptr, " -o ");
    ptr += strlen(" -o ");

    ptr = strcpy(ptr, png_file);
    ptr += strlen(png_file);

    int res = system(str);
    if (res != 0)
        return AKIN_BAD_GRAPHIS;

    free(str);

    return AKIN_ERROR_NO;
}

static ErrorAkinator create_dot_file(const char *filename, const BynarTree *tree)
{
    if (filename == NULL)
        filename = DOT_FILE;

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
        return AKIN_ERROR_OPEN_FILE;

    fprintf(fp, "digraph list{\nrankdir = HR\n");

    int number_top = 1;

    if (tree->root != NULL)
        create_dot_top(tree->root, &number_top, fp);

    fprintf(fp, "edge[color=black]\n");
    number_top = 1;

    if (tree->root != NULL)
        create_communication(tree->root, &number_top, fp);

    fprintf(fp, "}");
    fclose(fp);

    return AKIN_ERROR_NO;
}

static void create_dot_top(const Top *top, int* number_top, FILE *fp)
{
    int number_node = 0;
    int last_number_node = 0;

    const int SIZE_COLOR = 20;
    char color[SIZE_COLOR] = {};

    if (top->left == NULL && top->right == NULL)
        memcpy(color, "red", sizeof("red"));
    else
        memcpy(color, "green", sizeof("green"));

    #ifdef DEBUG
    fprintf(fp, "node%d [shape=Mrecord, style=\"filled\",  fillcolor = \"%s\", label = \"ptr top: %p\nis_download_in_base: %d | { <l%d> left: %p | <v%d> value: %s | <r%d> right: %p}\"]\n"
                ,*number_top, color, top, ((ValueTop*)top->element)->is_download_in_base, *number_top, top->left, *number_top, ((ValueTop*)top->element)->str, *number_top, top->right);
    #else
        fprintf(fp, "node%d [shape=Mrecord, style=\"filled\",  fillcolor = \"%s\", label = \"value: %s\"]\n", *number_top, color, *(char**)top->element);

    #endif
    (*number_top)++;

    if (top->left != NULL)
        create_dot_top(top->left, number_top, fp);

    if (top->right != NULL)
        create_dot_top(top->right, number_top, fp);
}

static void create_communication(const Top *top, int *number_top, FILE *fp)
{
    int this_top = *number_top;
    (*number_top)++;

    if (top->left != NULL)
    {
        #ifdef DEBUG
            fprintf(fp, "node%d:<l%d>->node%d:<v%d> [xlabel = \"YES\"]\n", this_top, this_top, *number_top, *number_top);
        #else
            fprintf(fp, "node%d->node%d [xlabel = \"YES\"]\n", this_top, *number_top);
        #endif
        create_communication(top->left, number_top, fp);
    }
    if (top->right != NULL)
    {
        #ifdef DEBUG
            fprintf(fp, "node%d:<r%d>->node%d:<v%d> [xlabel = \"NO\"]\n", this_top, this_top, *number_top, *number_top);
        #else
            fprintf(fp, "node%d->node%d [xlabel = \"NO\"]\n", this_top, *number_top);
        #endif
        create_communication(top->right, number_top, fp);
    }
}



