#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t calculate_padding(uint16_t options_length) {
    return (4 - (options_length % 4)) % 4;
}



int main(int argc, char** argv) {

    int l = atoi (argv[1]);

    printf ("%d\n", calculate_padding(l));
    printf ("%d\n", calculate_padding2(l));
   
}