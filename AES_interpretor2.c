#include "AES_interpreter.h"

program executable;
interpretor_states state = WAITING;
Scope cur_scope = GLOBAL;
int current_func = 0;

int main(int argc, char* argv[])
{
    executable.func_count = 0;
    FILE *aes_file = fopen(argv[1], "r");

    if(aes_file == NULL)
    {
        printf("AES file does not exist");
        fclose(aes_file);
        return 1;
    }

    fseek(aes_file, 0L, SEEK_END);
    char *file_data = (char*)malloc(ftell(aes_file)*sizeof(char));
    int max_line_size = ftell(aes_file);

    fseek(aes_file, 0L, 0);
    
    executable.functions = (function*)malloc(sizeof(function));
    add_func("init");
    add_func("main");
    
    while (fgets(file_data, max_line_size, aes_file) != NULL)
    {
        switch (cur_scope)
        {
        case GLOBAL:
            if(strchr(file_data, '(') != NULL && strchr(file_data, ')') != NULL)
            {
                state = INIT_FUNCTION;
            }
            break;
        case LOCAL:
            break;
        default:
            break;
        }
        
        allocate(file_data);
    }
    printf("%s",executable.functions[executable.func_count-1].func_name);
    free(file_data);
    fclose(aes_file);
    free_program();
}

char* delimeters = "\t \n();";
void allocate(char* line){
    switch (state)
    {
    case INIT_FUNCTION:
        create_function(line);
        break;
    default:
        search_for_line_type(line);
        break;
    }
}

void search_for_line_type(char* line){
    char * line_copy = (char*)malloc(sizeof(line));
    strcpy(line_copy, line);
    char* delims = "\t \n()=", cur_var[20];
    char* tok = strtok(line, delims);
    char* delim_order = get_delims_in_line(line, delims);
    variable_types type;
    bool record = false, assign = false;

    
    int i = 0;

    while(tok != NULL){
        if(type = get_var(tok)){
            state = INIT_VAR;
        }
        if(is_existing_function(tok)){
            run_print_function(line_copy);
        }

        switch (state)
        {
        case INIT_VAR:
            switch (i)
            {
            case 1:
                add_variable(tok, NULL);
                strcpy(cur_var, tok);
                
                if(strchr(line, '=') == NULL){
                    delims = ";=\n";
                    record = true;
                }
                break;
            
            default:
                if(record){
                        add_variable(cur_var, tok);
                        delims = "\t \n()=";
                        record = false;
                        }
                break;
            }
        case RUN_EXISTING_FUNC:
            break;
        }
        tok = strtok(NULL, delims);
        i++;
        
    }
    if(strchr(line, ';')){
            state = WAITING;
        }
    if(executable.functions[current_func].indexes[1] >0)
        printf("%s", executable.functions[current_func].env_stack[0]);

    free(line_copy);
}

void create_function(char* line_data){
    char* delims = "\t \n()";
    char* tok = strtok(line_data, delims);
    char* delim_order = get_delims_in_line(line_data, delims);
    variable_types type;
    int func_index = 0;

    int i = 0;

    
    while(tok != NULL){
        switch (i)
        {
        case 0:
            type = get_var(tok);
            break;
        case 1:
            if((current_func = function_exists(tok))){
            }
            else{
                add_func(tok);
            }
            break;
        default:
            break;
        }
        tok = strtok(NULL, delims);
        i++;
    }
    
    cur_scope = LOCAL;
    state = WAITING;
}

int function_exists(char* name){
    for(int i = 0; i < executable.func_count; i++){
        if(!strcmp(name, executable.functions[i].func_name)){
            return i+1;
        }
    }
    return 0;
}
char* get_delims_in_line(char* line, char*delims){
    char* result = (char*)malloc(sizeof(line));
    int j = 0;
    bool word_start = false;

    for(int i = 0; i < strlen(line); i++){
        if(strchr(delims, line[i]) != NULL && word_start){
            result[j] = line[i];
            j++;
            word_start = false;
        }
        else if(strchr(delims, line[i]) == NULL && !word_start){
            word_start = true;
        }
    }
    result[j] = 0;

    return result;
}

variable_types get_var(char* variable){
        if(!strcmp(variable, "int"))
            return INT;
        if(!strcmp(variable, "string"))
            return STRING;
        if(!strcmp(variable, "float"))
            return FLOAT;
        return 0;
    }

bool set_function(char* existing_funcion){
    if(!strcmp(existing_funcion, "print")){
        executable.functions[current_func].function_to_run = (char**)malloc(sizeof(char*));
        executable.functions[current_func].function_to_run[0] = (char*)malloc(sizeof(existing_funcion));

        strcpy(executable.functions[current_func].function_to_run[0], existing_funcion);

        return 1;
    }
    return 0;
}
void run_print_function(char *line){
    char* delims = "()";
    char* tok = strtok(line, delims);
    tok = strtok(NULL, delims);

    printf("%s\n", line);
    while (tok != NULL)
    {
        if(strchr(line,'\"') != NULL){
            
            printf(tok);
        }
        tok = strtok(NULL, delims);
    }
    
}



void add_func(char* func_name){
    function * temp = (function*)realloc(executable.functions, sizeof(function) * (executable.func_count + 1));
    executable.functions = NULL;
    executable.functions = (function*)malloc(sizeof(function) * (executable.func_count+2));
    executable.functions = temp;
    executable.functions[executable.func_count].func_name = (char*)malloc(sizeof(func_name));
    strcpy(executable.functions[executable.func_count].func_name, func_name);
    executable.func_count++;
    executable.functions[executable.func_count].indexes[0] = 0;
    executable.functions[executable.func_count].indexes[1] = 0;
}

bool compare_strings(char* str1, char* str2){
    if(sizeof(str1) != sizeof(str2)){
        return false;
    }
    int i = 0;
    for(int i = 0; i < strlen(str2);i++){
        if(str1[i] != str2[i])
            return false;
    }
    
    return true;
}
void add_variable(char* var_name, char* data){
    bool already_ended = false;

    if(executable.functions[current_func].indexes[1] == 0){
        executable.functions[current_func].var_names = (char**)malloc(sizeof(char*));
        executable.functions[current_func].var_names[0] = (char*)malloc(sizeof(var_name));
        strcpy(executable.functions[current_func].var_names[0], var_name);
        executable.functions[current_func].indexes[1] = 1;
        already_ended = true;
    }
    if(executable.functions[current_func].indexes[0] == 0){
        executable.functions[current_func].env_stack = (char**)malloc(sizeof(char*));
        executable.functions[current_func].indexes[0] = 1;
        already_ended = true;
    }
    if(already_ended)
        return;
    
    int size = sizeof(executable.functions[current_func].var_names)/sizeof(char*);
    for(int i = 0; i < size; i++){
        if(!strcmp(executable.functions[current_func].var_names[i], var_name)){
            executable.functions[current_func].env_stack[i] = (char*)malloc(sizeof(data) + sizeof(char));
            strcpy(executable.functions[current_func].env_stack[i], data);
        }
    }
    char **temp_name = (char**)realloc(executable.functions[current_func].var_names, sizeof(char*)*(executable.functions[current_func].indexes[1]+2) + sizeof(char*));
    char **temp_data = (char**)realloc(executable.functions[current_func].env_stack, sizeof(char*)*(executable.functions[current_func].indexes[0]+2) + sizeof(char*));

    strcpy(temp_name[executable.functions[current_func].indexes[1]], var_name);
    strcpy((char*)temp_data[executable.functions[current_func].indexes[0]], (char*)data);

    
    executable.functions[current_func].var_names = NULL;
    executable.functions[current_func].env_stack = NULL;

    executable.functions[current_func].var_names = (char**)malloc(sizeof(char*) * executable.functions[current_func].indexes[1]+1);
    executable.functions[current_func].env_stack = (char**)malloc(sizeof(char*) * executable.functions[current_func].indexes[0]+1);;

    executable.functions[current_func].var_names = temp_name;
    executable.functions[current_func].env_stack = temp_data;

    executable.functions[current_func].indexes[1]++;
    executable.functions[current_func].indexes[0]++;
}

void free_program(){
    for(int i = 0; i < executable.func_count; i++){
        for(int j = 0; j < executable.functions[i].indexes[1]; j++){
            free(executable.functions[i].env_stack[j]);
            free(executable.functions[i].var_names[j]);
        }
    }
    free(executable.functions);
}

bool is_existing_function(char* func_name){
    printf("%s", func_name);
    if(!strcmp(func_name, "print")){
        return true;
    }
    return false;
}