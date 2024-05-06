#include "delete.h"
void delete(element_t *element) {
    element_clear(*element);
    free(element);
}
