#ifndef GC_READER_H
#define GC_READER_H

#include "types.h"
#include <stdlib.h>
#include <stdint.h>

#define BUFF_SIZE 20

#define SEPARATOR_SYMBOL ' '
#define CHECKSUM_SYMBOL '*'
#define COMMENT_SYMBOL ';'
#define END_SYMBOL '\n'
#define END_SYMBOL2 '\r'

#define GC_READER_STATE_READ_TYPE 0
#define GC_READER_STATE_READ_CODE 1
#define GC_READER_STATE_READ_PARAM 2
#define GC_READER_STATE_READ_CHECKSUM 3
#define GC_READER_STATE_READ_LINE_NUMBER 4

#define GC_READER_ERROR_NOT_OCCURED 0
#define GC_READER_ERROR_BUFF_OVERFLOW 1
#define GC_READER_ERROR_CODE_READ 2
#define GC_READER_ERROR_PARAM_READ 3
#define GC_READER_ERROR_CHECKSUM_ERROR 4
#define GC_READER_ERROR_LINE_NUMBER_READ 5

int read_code(char *input, gc_reader *reader);

#endif