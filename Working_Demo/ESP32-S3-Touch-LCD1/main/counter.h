#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>

void counter_init(void);
void counter_increment(void);
uint32_t counter_get(void);

#endif // COUNTER_H
