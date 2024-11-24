#include <string.h>
#include <stdio.h>

#include "akinator.h"
#include "database.h"
#include "stack.h"

const int SIZE_ANSWER = 100;

static bool func_search(const Top *top);
static Top *find_top(Top *top);
static bool enter_answer();
static ErrorAkinator enter_question(char **question, char **object);
static void free_sub_akinator(Top *top);
static bool is_have_in_subtree(const char *str, const Top *top, stack_t *stack_t);
static Top* find_min_common_top(stack_t *stack1, stack_t *stack2, Top *top);
static ErrorAkinator print_definition(const Top *top, stack_t *stack_t);


ErrorAkinator choice_read_database(BynarTree *tree, Database *database)
{
    printf("Please, enter the name of the file from which you want to download the database.\n");

    char filename[SIZE_ANSWER] = {};
    if (fgets(filename, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    filename[strlen(filename) - 1] = '\0';

    ErrorAkinator error = read_database(tree, filename, database);
    return error;
}

ErrorAkinator find_object(BynarTree *tree)
{
    if (tree->root == NULL)
        return AKIN_ERROR_BYNAR_TREE;

    Top *name_object = find_top(tree->root);
    if (name_object == NULL)
        return AKIN_ERROR_DATABASE;

    printf("You have made a wish for this object: %s?\n", ((ValueTop*)name_object->element)->str);

    if (enter_answer())
        return AKIN_ERROR_NO;

    char* object = (char*)calloc(SIZE_ANSWER, sizeof(char));
    if (object == NULL)
        return AKIN_ERROR_NO;

    char **difference_question = (char**)calloc(1, sizeof(char*));
    if (difference_question == NULL)
        return AKIN_ERROR_NO;

    *difference_question = (char*)calloc(SIZE_ANSWER, sizeof(char));
    if (difference_question == NULL)
        return AKIN_ERROR_NO;

    ErrorAkinator error = enter_question(difference_question, &object);
    if (error != AKIN_ERROR_NO)
        return error;

    ValueTop value_top = {object, false};
    ErrorBynarTree error_tree = create_bynar_top(&value_top, sizeof(ValueTop), &name_object->left);
    if (error_tree != BYNAR_OK)
        return AKIN_ERROR_BYNAR_TREE;

    error_tree = create_bynar_top(name_object->element, sizeof(ValueTop), &name_object->right);
    if (error_tree != BYNAR_OK)
        return AKIN_ERROR_BYNAR_TREE;

    free(name_object->element);
    name_object->element = difference_question;

    return AKIN_ERROR_NO;
}

static Top *find_top(Top *top)
{
    if (top == NULL)
        return top;

    if (top->left == NULL && top->right == NULL)
        return top;

    Top *return_top = NULL;

    if (func_search(top))
        return_top = find_top(top->left);

    else
        return_top = find_top(top->right);

    return return_top;
}

static bool func_search(const Top *top)
{
    printf("%s\n", *(char**)top->element);

    char answer[SIZE_ANSWER] = {};

    return enter_answer();
}

static bool enter_answer()
{
    char answer[SIZE_ANSWER] = {};

    if (fgets(answer, SIZE_ANSWER - 1, stdin) == NULL)
        return false;

    answer[strlen(answer) - 1] = '\0';

    while (1)
    {
        if (strcmp(answer, "YES") == 0)
            return true;

        if (strcmp(answer, "NO") == 0)
            return false;

        printf("Please enter \"YES\" or \"NO\"\n");

        if (fgets(answer, SIZE_ANSWER - 1, stdin) == NULL)
            return false;
        answer[strlen(answer) - 1] = '\0';
    }
    return false;
}

static ErrorAkinator enter_question(char **question, char **object)
{
    if (question == NULL || object == NULL)
        return AKIN_NULL_PTR;

    if (*object == NULL || *question == NULL)
        return AKIN_NULL_PTR;

    printf("Who is it?\n");
    if (fgets(*object, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    (*object)[strlen(*object) - 1] = '\0';

    printf("Enter the distinguishing question\n");
    if (fgets(*question, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    (*question)[strlen(*question) - 1] = '\0';
    return AKIN_ERROR_NO;
}

void dtor_akinator(BynarTree* tree, Database *database)
{
    free(database->ptr);
    if (tree->root != NULL)
        free_sub_akinator(tree->root);
}

static void free_sub_akinator(Top *top)
{
    if (top->left != NULL)
        free_sub_akinator(top->left);

    if (top->right != NULL)
        free_sub_akinator(top->right);

    if ( !((ValueTop*)top->element)->is_download_in_base)
        free(((ValueTop*)top->element)->str);

    free(top->element);
    free(top);
}

ErrorAkinator give_definition(const BynarTree *tree)
{
    printf("Please enter the word you need to know the definition of.\n");

    char object[SIZE_ANSWER] = {};
    if (fgets(object, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    object[strlen(object) - 1] = '\0';

    stack_t stack_t = {};
    ErrorStack error = create_stack(&stack_t, SIZE_ANSWER, sizeof(bool));
    if (error != STACK_ERROR_NO)
        return AKIN_ERROR_STACK;

    if (is_have_in_subtree(object, tree->root, &stack_t))
    {
        ErrorAkinator error_akin = print_definition(tree->root, &stack_t);
        return error_akin;
    }
    printf("Sorry, I don't know your words.\n"
           "You can add this word in guessing mode.\n");

    return AKIN_ERROR_NO;
}

static bool is_have_in_subtree(const char *str, const Top *top, stack_t *stack_t)
{
    ErrorStack error = STACK_ERROR_NO;

    if (top == NULL)
        return false;

    if (strcmp(str, ((ValueTop*)top->element)->str) == 0)
        return true;

    if (is_have_in_subtree(str, top->left, stack_t))
    {
        bool temp_for_push = true;
        error = push_stack(stack_t, &temp_for_push);
        if (error != STACK_ERROR_NO)
            return false;

        return true;
    }

    if (is_have_in_subtree(str, top->right, stack_t))
    {
        bool temp_for_push = false;
        error = push_stack(stack_t, &temp_for_push);
        if (error != STACK_ERROR_NO)
            return false;

        return true;
    }

    return false;
}

static ErrorAkinator print_definition(const Top *top, stack_t *stack_t)
{
    if (stack_t->size <= 0)
        return AKIN_ERROR_NO;

    printf("%s\n", ((ValueTop*)top->element)->str);
    bool direction = false;

    ErrorStack error = pop_stack(stack_t, &direction);
    if (error != STACK_ERROR_NO)
        return AKIN_ERROR_STACK;

    if (direction)
        print_definition(top->left, stack_t);

    else
        print_definition(top->right, stack_t);

    return AKIN_ERROR_NO;
}

static Top* find_min_common_top(stack_t *stack1, stack_t *stack2, Top *top)
{
    if (stack1->size <= 0 && stack2->size <= 0)
        return top;

    bool direction1 = false;
    bool direction2 = false;

    ErrorStack error = pop_stack(stack1, &direction1);
    if (error != STACK_ERROR_NO)
        return NULL;

    error = pop_stack(stack2, &direction2);
    if (error != STACK_ERROR_NO)
        return NULL;

    if (direction1 == direction2)
    {
        printf("%s\n", ((ValueTop*)top->element)->str);
        if (direction1)
            return find_min_common_top(stack1, stack2, top->left);

        return find_min_common_top(stack1, stack2, top->right);
    }
    error = push_stack(stack1, &direction1);
    if (error != STACK_ERROR_NO)
        return NULL;

    error = push_stack(stack2, &direction2);
    if (error != STACK_ERROR_NO)
        return NULL;

    return top;
}

ErrorAkinator compare_words(const BynarTree *tree)
{
    char words1[SIZE_ANSWER] = {};
    char words2[SIZE_ANSWER] = {};

    printf("Please enter the first word for comparison.\n");

    if (fgets(words1, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    words1[strlen(words1) - 1] = '\0';

    printf("Please enter the first word for comparison.\n");

    if (fgets(words2, SIZE_ANSWER - 1, stdin) == NULL)
        return AKIN_ERROR_READ;

    words2[strlen(words2) - 1] = '\0';

    stack_t stack1 = {};
    ErrorStack error_stack = create_stack(&stack1, SIZE_ANSWER, sizeof(bool));
    if (error_stack != STACK_ERROR_NO)
        return AKIN_ERROR_STACK;

    stack_t stack2 = {};
    error_stack = create_stack(&stack2, SIZE_ANSWER, sizeof(bool));
    if (error_stack != STACK_ERROR_NO)
        return AKIN_ERROR_STACK;

    if (!is_have_in_subtree(words1, tree->root, &stack1))
    {
        printf("Sorry, I don't know your words.\n"
           "You can add this word in guessing mode.\n");

        return AKIN_ERROR_NO;
    }

    if (!is_have_in_subtree(words2, tree->root, &stack2))
    {
        printf("Sorry, I don't know your words.\n"
           "You can add this word in guessing mode.\n");

        return AKIN_ERROR_NO;
    }
    printf("Similarities:\n");

    Top* min_common_top = find_min_common_top(&stack1, &stack2, tree->root);
    if (min_common_top == NULL)
        return AKIN_ERROR_STACK;

    printf("Differences %s:\n",words1);
    ErrorAkinator error = print_definition(min_common_top, &stack1);
    if (error != AKIN_ERROR_NO)
        return error;

    printf("Differences %s:\n",words2);
    error = print_definition(min_common_top, &stack2);
    return error;
}




