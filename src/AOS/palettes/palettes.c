#if 0
    GLshort i, r, g, b;

    i = 0;
    for (r = 0x10; r <= 0xF0; r += 0x20)	/* 0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0 */
    for (g = 0x10; g <= 0xF0; g += 0x20)	/* 0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0 */
    for (b = 0x20; b <= 0xE0; b += 0x40) {	/* 0x20, 0x60, 0xA0, 0xE0 */
      amesa->Palette[i].orgEntry.r = r;
      amesa->Palette[i].orgEntry.g = g;
      amesa->Palette[i].orgEntry.b = b;
      i++;
    }
#endif
static const struct ColorEntry Palette332[palNumCols332] = {
#include "palettes.inc8"
};

#if 0
    GLshort i, j;

    for (i = j = 0; i < palNumColsGrey; i++, j += 4) {
      amesa->Palette[i].orgEntry.r = j;
      amesa->Palette[i].orgEntry.g = j;
      amesa->Palette[i].orgEntry.b = j;
    }
#endif
static const struct ColorEntry PaletteGrey[palNumColsGrey] = {
#include "palettes.incg"
};

static const struct ColorEntry Palette[palNumCols] = {
#include "palettes.incw"
};

static const struct ColorEntry PaletteHPCR[palNumColsHPCR] = {
#include "palettes.inch"
};
