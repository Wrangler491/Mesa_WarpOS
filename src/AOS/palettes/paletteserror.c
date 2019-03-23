#include <stdio.h>
#include <math.h>

#define	ERROR11(e)	e < 0 ? -(e *  1) : (e *  1)
#define	ERROR56(e)	e < 0 ? -(e *  5) : (e *  5)
#define	ERROR66(e)	e < 0 ? -(e *  6) : (e *  6)
#define	ERRORRY(e)	e < 0 ? -(e * 21) : (e * 21)
#define	ERRORGY(e)	e < 0 ? -(e * 72) : (e * 72)
#define	ERRORBY(e)	e < 0 ? -(e *  7) : (e *  7)

int main(int argc, char **argv) {
  short int i;

  /* maxsum = 768, 1:1:1 */
  for (i = -256; i < 256; i++)
    printf("  {0x%04x,0x%04x,0x%04x},\n", ERROR11(i), ERROR11(i), ERROR11(i));

  /* maxsum = 4096, 5:6:5 */
  for (i = -256; i < 256; i++)
    printf("  {0x%04x,0x%04x,0x%04x},\n", ERROR56(i), ERROR66(i), ERROR56(i));

  /* maxsum = 25245, Y */
  for (i = -256; i < 256; i++)
    printf("  {0x%04x,0x%04x,0x%04x},\n", ERRORRY(i), ERRORGY(i), ERRORBY(i));
};
