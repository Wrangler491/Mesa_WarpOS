example differences to GetPen (2189 accesses)
this is the err_sum function, below are the
err_function functions:

    if ((err = pen - err_function) < 0)
      err_diff -= err;
    else
      err_diff += err;

//  grey-level = (21 * R) + (71 * G) + (7 * B) / 99
//  return amesa->Palette[((21 * R) + (71 * G) + (7 * B)) / (99 << 2)].PenNo;				/* 64 grey-levels instead of 256 */

  6921
  6704
  6743

//  grey-level = (27 * R) + (91 * G) + (9 * B) / 128
//  return amesa->Palette[((27 * R) + (91 * G) + (9 * B)) >> 9].PenNo;					/* c * 1.28 / 1.28 */

  7541
  7356
  7400

//  grey-level = (24 * R) + (96 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 4) + (R << 3) + (G << 6) + (G << 5) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */

  5698
  5480
  5513

//  grey-level = (32 * R) + (96 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (B << 3) + B) >> 9].PenNo;			/* aprox. to shifts */

  3462
  2945
  2963

//  grey-level = (32 * R) + (96 * G) + (10 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (B << 3) + (B << 1)) >> 9].PenNo;		/* aprox. to shifts */

  3398
  2860
  2877

//  grey-level = (32 * R) + (97 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + G + (B << 3) + B) >> 9].PenNo;		/* aprox. to shifts */

  3284
  2711
  2728

//  grey-level = (32 * R) + (98 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (G << 1) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */
//  return amesa->Palette[((R <<= 5) += (G <<= 1) + (G <<= 4) + (G <<= 1) + (B <<= 3) + B) >> 9].PenNo;	/* aprox. to shifts */
  __asm__("
	.equ	paloffs,%4
	.equ	amesa,%3
	.equ	pal,a0
	.equ	r,%1
	.equ	g,%0
	.equ	b,%2

	lslw	#5,r		|                                                  (r << 5)
	addw	b,r		|                                              b + (r << 5)
	lslw	#3,b		|                                   (b << 3)
	addw	b,r		|                                   (b << 3) + b + (r << 5)
	addw	g,g		|                        (g << 1)
	addw	g,r		|                        (g << 1) + (b << 3) + b + (r << 5)
	lslw	#4,g		|             (g << 5)
	addw	g,r		|             (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	addw	g,g		|  (g << 6)
	addw	g,r		|  (g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	lsrw	#9,r		| ((g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)) >> 9
	moveb	amesa@(paloffs+9,r:w:8),r"
	: "=d" (r)
	: "d" (r)
	, "d" (g)
	, "d" (b)
	, "a" (amesa)
	, "" (offsetof(amesa, Palette))
	: "a0", "cc");

  3185
  2555
  2569

//  grey-level = (32 * R) + (100 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (G << 2) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */

  3338
  2615
  2625

//  grey-level = (32 * R) + (104 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (G << 3) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */

  4828
  4636
  4653

//  grey-level = (33 * R) + (96 * G) + (9 * B) / 128
//  return amesa->Palette[((R << 5) + R + (G << 6) + (G << 5) + (B << 3) + B) >> 9].PenNo;			/* aprox. to shifts */

  3381
  2837
  2854

//  grey-level = (21 * R) + (71 * G) + (7 * B) / 99
//  return amesa->Palette[(amesa->MatchErrors[R][0] +
//			   amesa->MatchErrors[G][1] +
//			   amesa->MatchErrors[B][2]) / 396].PenNo;					/* build from tables */

  6221
  5886
  5923

