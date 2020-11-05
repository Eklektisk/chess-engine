#include "DataTypes.h"

int8_t rayStep(struct Ray* ray, int8_t* index) {
	if(*index >= ray->size || *index < 0) {
		return -1;
	}

	return ray->pos[(*index)++];
}

