#include "counter.h"
#include <stdint.h>
static uint32_t counter_value = 0;

void counter_init(void)
{
    counter_value = 0;
}

void counter_increment(void)
{
    counter_value++;
}

uint32_t counter_get(void)
{
    return counter_value;
}
