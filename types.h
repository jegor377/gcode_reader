#ifndef GC_TYPES_H
#define GC_TYPES_H

#include <stdint.h>

typedef struct gc_reader {
    uint8_t code_type;
    uint32_t code_id;
    
    uint32_t T;
    uint32_t S;
    uint32_t P;
    
    double X;
    double Y;
    double Z;
    double U;
    double V;
    double W;
    
    double I;
    double J;
    double D;
    double H;

    double F;
    double R;
    double E;

    uint32_t line_number;
    uint32_t checksum;
} gc_reader;

#endif