#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct Vector { // Vector
	int8_t* pos;
	size_t  size;
};

struct Ray { // Vector
	int8_t pos[7];
	size_t size;
};

struct Vector8 { // Vector
	int8_t pos[8];
	size_t size;
};

#endif
