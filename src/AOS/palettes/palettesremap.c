#include <stdio.h>
#include <math.h>

int main(int argc, char **argv) {
  short int i, j, r;
  int remap[256];

  memset(remap, sizeof(remap), 0xFF);

  srand(42);
  printf("static const GLshort remap[palNumColsMax] = {\n  ");

  for (i = 0; i < 64; i++) {
    /* try to find a number not found before */
    while (1) {
      r = 0 + (rand() % (64 - 0));
      for (j = 0; j < i; j++)
        if (remap[j] == r)
          break;
      if (j == i) {
        printf("0x%02x,", r);
        break;
      }
    }

    remap[i] = r;
  }
  printf("\n  ");

  for (i = 64; i < 189; i++) {
    /* try to find a number not found before */
    while (1) {
      r = 64 + (rand() % (189 - 64));
      for (j = 64; j < i; j++)
        if (remap[j] == r)
          break;
      if (j == i) {
        printf("0x%02x,", r);
        break;
      }
    }

    remap[i] = r;
  }
  printf("\n  ");

  for (i = 189; i < 255; i++) {
    /* try to find a number not found before */
    while (1) {
      r = 189 + (rand() % (256 - 189));
      for (j = 189; j < i; j++)
        if (remap[j] == r)
          break;
      if (j == i) {
        printf("0x%02x,", r);
        break;
      }
    }

    remap[i] = r;
  }
  r = 189;
  while (1) {
    for (j = 189; j < 255; j++)
      if (remap[j] == r)
        break;
    if (j == 255) {
      printf("0x%02x\n};\n", r);
      break;
    }
    r++;
  }

  return 0;
}
