#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum{
    RECEIVE_INT = 0,
    RECEIVE_STRING = 1,
    RECEIVE_VOID = 2
}receive_states;

typedef enum{
    INT = 4,
    STRING = 1,
    SHORT = 2,
    FLOAT = 4,
    DOUBLE = 8
}variable_types;

typedef enum
{
    NONE = 0,
    VARIABLE = 1,
    NEW_FUNCTION = 2,
    EXISTING_FUNCTION = 3
}read_state;
/*typedef enum
{
    NO_VAR = 0,
    VOID = 1,
    INT = 2,
    STRING = 3
}var_read_mode;*/

typedef struct   {
    char* func_name;
    void** env_stack;
    char** var_names;
    char** function_to_run;
    variable_types type;
    int indexes[2];
}function;

typedef struct {
    function* functions;
    char** global_var_names;
    void** global_stack;
    int func_count;
}program;

typedef enum{
    WAITING = 0,
    INIT_FUNCTION = 1,
    INIT_VAR = 3,
    RUN_EXISTING_FUNC = 4,
    RECEIVE_VAl
}interpretor_states;

typedef enum{
    GLOBAL, 
    LOCAL
}Scope;

void parse_line(char* line_content);
int check_word(char* word);
int function_check(char* word);
char* custom_tok(char *line);
void allocate(char* line);
variable_types get_var(char* variable);
char* set_function(char* existing_funcion);
void create_function(char* line_data);

int function_exists(char* name);
char* get_delims_in_line(char* line, char*delims);
void add_func(char* func_name);
bool compare_strings(char* str1, char* str2);

void search_for_line_type(char* line);
void add_variable(char* var_name, void* data);