// prints a precomputed lookup table for the sine generator
// author: stf
#include <stdio.h>
#include <math.h>
#include <cstdint>

#define PI 3.14159265

int main() {
        const float conversionFactor = (2*PI)/200;
        fprintf(stderr, "uint8_t sine_lookup[200] = {");
        for(int i=0; i<200; i++){
                uint8_t val = (sin(i*conversionFactor)*100)+100; 
                if(i!=199)
                    fprintf(stderr, "%u,", val);
                else 
                    fprintf(stderr, "%u};\n", val);
        }
        return 1;
}
