#ifndef Arduino_h
#define Arduino_h

#include <cstdint>
#include <cmath>
#define min(x, y) (((x) < (y)) ? (x) : (y))

void delay(unsigned long);
long random(long);
long random(long, long);

#endif