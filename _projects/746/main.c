/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

#include <stdint.h>


int main(void) {
  int i;
  uint8_t table[380*1024] = {1};


  for (i = 0; i < 100; i++) {
    printf("Hello World %d!\n", table[i]);
  }
  do {
    i++;
  } while (1);
}

/*************************** End of file ****************************/
