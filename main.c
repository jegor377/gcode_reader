#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"

const size_t INPUT_SIZE = 8;

int read_gcode(char input, gc_reader *reader, int *iter) {
    int done = 0;
    int error_code = read_code(reader, input, &done);
    if(error_code != GC_READER_ERROR_NOT_OCCURED) {
        printf("ERROR OCCURED!\n");
        return 1;
    } else {
        (*iter)++;
    }
    if(done) {
        printf("%c %d -> X=%f Y=%f S=%d rc=%d c=%d fc=%d\n", reader->code_type, reader->code_id, reader->X, reader->Y, reader->S, reader->read_checksum, reader->checksum, reader->found_checksum);
        memset(reader, 0, sizeof(gc_reader));
    }
    return 0;
}

int main(void) {
    FILE *fd = fopen("examples/example2.gcode", "r");
    if(!fd) {
        printf("Nie mozna otworzyc pliku :(\n");
        return 1;
    }

    gc_reader reader;
    memset(&reader, 0, sizeof(reader));

    char buff[INPUT_SIZE];
    int n = -1, iter = 0;

    while(n != 0) {
        if(n - iter <= 0) {
            n = fread(buff, sizeof(char), INPUT_SIZE, fd);
            if(n == 0 && feof(fd)) {
                read_gcode('\0', &reader, &iter);
                break;
            }
            iter = 0;
        }
        read_gcode(buff[iter], &reader, &iter);
    }
    
    fclose(fd);
    return 0;
}