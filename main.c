#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"

const size_t INPUT_SIZE = 256;

int main(void) {
    FILE *fp = fopen("examples/example2.gcode", "r");
    if(!fp) {
        printf("Nie mozna otworzyc pliku :(\n");
        return 1;
    }

    gc_reader reader;
    char *buff = NULL;
    size_t buff_size = 0;
    ssize_t nread; 
    
    while((nread = getline(&buff, &buff_size, fp)) != -1) {
        memset(&reader, 0, sizeof(gc_reader));
        int err_code = read_code(buff, &reader);
        printf("%c %u X%f Y%f Z%f E%f F%f S%d | ERROR CODE = %d\n",
            reader.code_type,
            reader.code_id,
            reader.X,
            reader.Y,
            reader.Z,
            reader.E,
            reader.F,
            reader.S,
            err_code);
        if(err_code) {
            getchar();
        }
    }
    
    free(buff);
    fclose(fp);
    return 0;
}