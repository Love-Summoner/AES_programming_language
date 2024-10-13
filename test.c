#include <stdio.h>
#include <stdlib.h>


int ptr_size(char* ptr){
    int i = 0;

    while(ptr[i] != '\0'){
        i++;
    }
    return i;
}
int main(){
    void *variety = (void*)malloc(sizeof(char) * 4);
    variety = 22;
    printf("%d", (int)variety);

}

