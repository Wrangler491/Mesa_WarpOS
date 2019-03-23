#ifndef	AMIGAMESAacc_H
#define	AMIGAMESAacc_H

#define	Xx	(GLint *)x	//
#define	Yy	(GLint *)y	//
#define	RGBAi	(GLuint *)rgba	//
#define	RGBAi0	(((GLuint *)rgba) + 0)
#define	RGBAi1	(((GLuint *)rgba) + 1)
#define	RGBAi2	(((GLuint *)rgba) + 2)
#define	RGBAb	(GLubyte *)rgba	//
#define	RGBAb0	(((GLubyte *)rgba) + 0)
#define	RGBAb1	(((GLubyte *)rgba) + 1)
#define	RGBAb2	(((GLubyte *)rgba) + 2)
#define	RGBi	((GLuint *)rgb)
#define	RGBi0	(((GLuint *)rgb) + 0)
#define	RGBi1	(((GLuint *)rgb) + 1)
#define	RGBi2	(((GLuint *)rgb) + 2)
#define	RGBb	(GLubyte *)rgb	//
#define	RGBb0	(((GLubyte *)rgb) + 0)
#define	RGBb1	(((GLubyte *)rgb) + 1)
#define	RGBb2	(((GLubyte *)rgb) + 2)
#define	INDEXi	(GLuint *)index	//
#define	INDEXb	(GLubyte *)index	//
#define	I2Pi	((GLuint *)ItoP)
#define	I2Pb	(GLubyte *)ItoP	//
#define	P2Ii	((GLuint *)PtoI)
#define	P2Ib	((GLubyte *)PtoI)

#define	iXx	((GLint *)&x[i])
#define	iYy	((GLint *)&y[i])
#define	iRGBAi	((GLuint *)&rgba[i])
#define	iRGBAi0	((GLuint *)&rgba[i][0])
#define	iRGBAi1	((GLuint *)&rgba[i][1])
#define	iRGBAi2	((GLuint *)&rgba[i][2])
#define	iRGBAb	((GLubyte *)&rgba[i])
#define	iRGBAb0	((GLubyte *)&rgba[i][0])
#define	iRGBAb1	((GLubyte *)&rgba[i][1])
#define	iRGBAb2	((GLubyte *)&rgba[i][2])
#define	iRGBi	((GLuint *)&rgb[i])
#define	iRGBi0	((GLuint *)&rgb[i][0])
#define	iRGBi1	((GLuint *)&rgb[i][1])
#define	iRGBi2	((GLuint *)&rgb[i][2])
#define	iRGBb	((GLubyte *)&rgb[i])
#define	iRGBb0	((GLubyte *)&rgb[i][0])
#define	iRGBb1	((GLubyte *)&rgb[i][1])
#define	iRGBb2	((GLubyte *)&rgb[i][2])
#define	iINDEXi	((GLuint *)&index[i])
#define	iINDEXb	((GLubyte *)&index[i])

#endif
