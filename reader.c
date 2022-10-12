#include "reader.h"

int state = GC_READER_STATE_READ_TYPE;

char buff[BUFF_SIZE + 1];
uint32_t buff_iter = 0;

int is_end_char(char c) {
    return c == '\0' || c == COMMENT_SYMBOL || c == END_SYMBOL || c == END_SYMBOL2;
}

int read_to_buff(char c) {
    if(buff_iter >= BUFF_SIZE - 1) {
        return 1;
    }
    buff[buff_iter++] = c;
    buff[buff_iter] = '\0';
    return 0;
}

void set_param(uint8_t type, gc_reader *reader) {
    switch(type) {
    case 'T':
        reader->T = atoi(buff);
        break;
    case 'S':
        reader->S = atoi(buff);
        break;
    case 'P':
        reader->P = atoi(buff);
        break;
    case 'X':
        reader->X = atof(buff);
        break;
    case 'Y':
        reader->Y = atof(buff);
        break;
    case 'Z':
        reader->Z = atof(buff);
        break;
    case 'U':
        reader->U = atof(buff);
        break;
    case 'V':
        reader->V = atof(buff);
        break;
    case 'W':
        reader->W = atof(buff);
        break;
    case 'I':
        reader->I = atof(buff);
        break;
    case 'J':
        reader->J = atof(buff);
        break;
    case 'D':
        reader->D = atof(buff);
        break;
    case 'H':
        reader->H = atof(buff);
        break;
    case 'F':
        reader->F = atof(buff);
        break;
    case 'R':
        reader->R = atof(buff);
        break;
    case 'E':
        reader->E = atof(buff);
        break;
    }
    buff_iter = 0;
}

int is_checksum_ok(char *input, gc_reader *reader) {
    int cs = 0;
    for(int i = 0; input[i] != CHECKSUM_SYMBOL && input[i] != '\0'; i++) {
        cs = cs ^ input[i];
    }
    cs &= 0xff;
    return cs == reader->checksum;
}

int read_code(char *input, gc_reader *reader) {
    uint8_t param_type;
    int found_checksum = 0;
    state = GC_READER_STATE_READ_TYPE;
    if(input[0] == 'N') {
        state = GC_READER_STATE_READ_LINE_NUMBER;
    }
    for(int i = 0; !is_end_char(input[i]); i++) {
        if(input[i] == CHECKSUM_SYMBOL) {
            state = GC_READER_STATE_READ_CHECKSUM;
            i++;
        }
        switch(state) {
        case GC_READER_STATE_READ_TYPE:
            reader->code_type = input[i];
            state = GC_READER_STATE_READ_CODE;
            break;
        case GC_READER_STATE_READ_CODE:
            for(; input[i] != SEPARATOR_SYMBOL && !is_end_char(input[i]) && input[i] != CHECKSUM_SYMBOL; i++) {
                if(read_to_buff(input[i])) {
                    return GC_READER_ERROR_BUFF_OVERFLOW;
                }
            }
            if(buff_iter <= 0) {
                return GC_READER_ERROR_CODE_READ;
            }
            reader->code_id = atoi(buff);
            buff_iter = 0;
            state = GC_READER_STATE_READ_PARAM;
            break;
        case GC_READER_STATE_READ_PARAM:
            param_type = input[i];
            for(i++; input[i] != SEPARATOR_SYMBOL && !is_end_char(input[i]) && input[i] != CHECKSUM_SYMBOL; i++) {
                if(read_to_buff(input[i])) {
                    return GC_READER_ERROR_BUFF_OVERFLOW;
                }
            }
            if(buff_iter <= 0) {
                return GC_READER_ERROR_PARAM_READ;
            }
            set_param(param_type, reader);
            break;
        case GC_READER_STATE_READ_CHECKSUM:
            for(; input[i] != ' ' && !is_end_char(input[i]); i++) {
                if(read_to_buff(input[i])) {
                    return GC_READER_ERROR_BUFF_OVERFLOW;
                }
            }
            reader->checksum = atoi(buff);
            found_checksum = 1;
            buff_iter = 0;
            break;
        case GC_READER_STATE_READ_LINE_NUMBER:
            for(i++; input[i] != ' ' && !is_end_char(input[i]) && input[i] != CHECKSUM_SYMBOL; i++) {
                if(read_to_buff(input[i])) {
                    return GC_READER_ERROR_BUFF_OVERFLOW;
                }
            }
            if(buff_iter <= 0) {
                return GC_READER_ERROR_LINE_NUMBER_READ;
            }
            reader->line_number = atoi(buff);
            buff_iter = 0;
            state = GC_READER_STATE_READ_TYPE;
            break;
        }
        if(input[i] == CHECKSUM_SYMBOL) {
            state = GC_READER_STATE_READ_CHECKSUM;
        }
    }

    return (!found_checksum || is_checksum_ok(input, reader)) ? GC_READER_ERROR_NOT_OCCURED : GC_READER_ERROR_CHECKSUM_ERROR;
}