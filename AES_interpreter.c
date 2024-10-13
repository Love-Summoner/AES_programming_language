#include "AES_interpreter.h"

read_state state = NONE;
//var_read_mode vaiable_mode = NO_VAR;

int main(int argc, char* argv[])
{
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

    while (fgets(file_data, max_line_size, aes_file) != NULL)
    {
        parse_line(file_data);
    }
    

    free(file_data);
    fclose(aes_file);
}

char last_delim;
char* delimeters = "\t\"  (;[])\n{}";
char* default_delim = "\t\"  (;[])\n{}";
char target_delim;

void parse_line(char* line_content)
{
    char* tok = custom_tok(line_content);
    bool ready = false;

    while (tok != NULL)
    {
        //printf("%s%c", tok, last_delim);
        if(ready)
        {
            
        }
        switch (state)
        {
        case NONE:
            check_word(tok);
            function_check(tok);
            break;
        case VARIABLE:

            break;
        case EXISTING_FUNCTION:

            break;
        default:
            break;
        }
        

        if(last_delim == ')')
        {
            delimeters = default_delim;
            target_delim = 'b';
        }
        if(tok != NULL && last_delim == target_delim)
        {
            switch (state)
        {
            case VARIABLE:

                break;
            case EXISTING_FUNCTION:
                ready = !ready;
                break;
            default:
                break;
        }
        }
        tok = custom_tok(NULL);

        
    }
}

int check_word(char* word)
{
    /*if(!strcmp(word, "void"))
    {
        state = VARIABLE;
    }*/
}
char* cur_func;
int function_check(char* word)
{
    if(!strcmp(word, "print"))
    {
        state = EXISTING_FUNCTION;
        //vaiable_mode = STRING;
        delimeters = "()\",";
        target_delim = '(';
        cur_func = (char*)malloc(sizeof(word));
        strcpy(cur_func, word);
    }
}
char* token_line = NULL;
int cur_character = 0;
char* custom_tok(char *line)
{
    
    char* result = (char*)malloc(sizeof(line));

    if(line != NULL)
    {
        cur_character = 0;
        token_line = (char*)malloc(sizeof(line) + 1);
        strcpy(token_line, line);
    }
    
    if(strchr(delimeters, token_line[cur_character]) != NULL)
        {
            last_delim = token_line[cur_character];
            cur_character += 1;    
            return "";
        }
    for(int i = cur_character; i < strlen(token_line); i++)
    {
        
        if(strchr(delimeters, token_line[i]) != NULL)
        {
            result[i-cur_character] = 0;
            cur_character = i + 1;
            last_delim = token_line[i];
            return result;
        }
        result[i-cur_character] = token_line[i];
    }
    cur_character = 0;
    free(result);
    return NULL;
}


void run_func(void* data)
{
    if(!strcmp(cur_func, "print"))
    {
        printf("%s", (char*)data);
    }
}