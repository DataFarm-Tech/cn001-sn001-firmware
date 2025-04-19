#ifndef UTILS_H
#define UTILS_H

#include "config.h"

#define DEBUG() printf("%s: %d: %s\n", __func__, __LINE__, __FILE__)

#define PRINT_STR(to_print) printf("[%s]: %s\n", ID, to_print)
#define PRINT_INT(to_print) printf("[%s]: %d\n", ID, to_print)


#endif // UTILS_H