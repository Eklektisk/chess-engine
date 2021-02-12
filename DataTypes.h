#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Vector { // Vector
	int8_t* pos;
	size_t  size;
} Vector;

typedef struct Ray { // Vector
	int8_t pos[7];
	size_t size;
} Ray;

typedef struct Vector8 { // Vector
	int8_t pos[8];
	size_t size;
} Vector8;

#endif
