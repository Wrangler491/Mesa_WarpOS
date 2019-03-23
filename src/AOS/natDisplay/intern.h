const char *natRendererString(void);
const char *natRendererStringDB(void);
void natClearIndex(GLcontext *ctx, GLuint index);
void natClearColor(GLcontext *ctx,
		   GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLbitfield natClear(GLcontext *ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height);
GLbitfield natClearDB(GLcontext *ctx, GLbitfield mask, GLboolean all,
		      GLint x, GLint y, GLint width, GLint height);
void natSetIndex(GLcontext *ctx, GLuint index);
void natSetColor(GLcontext *ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLboolean natIndexMask(GLcontext *ctx, GLuint mask);
GLboolean natColorMask(GLcontext *ctx, GLboolean rmask, GLboolean gmask, GLboolean bmask, GLboolean amask);
GLboolean natLogicOp(GLcontext *ctx, GLenum op);
void natFastLineFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint pv);
line_func natChooseLineFunction(GLcontext *ctx);
line_func natChooseLineFunctionDB(GLcontext *ctx);
void natFastPointsFunction(GLcontext *ctx, GLuint first, GLuint last);
points_func natChoosePointsFunction(GLcontext *ctx);
points_func natChoosePointsFunctionDB(GLcontext *ctx);
GLboolean natDirectDrawPixels(GLcontext *ctx,
			      const struct gl_pixelstore_attrib *unpack,
			      GLsizei width, GLsizei height,
			      GLenum format, GLenum type,
			      const GLvoid *pixels);
void natFastTriangleFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint v2, GLuint pv);
triangle_func natChooseTriangleFunction(GLcontext *ctx);
triangle_func natChooseTriangleFunctionDB(GLcontext *ctx);
void natFastQuadFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLuint pv);
quad_func natChooseQuadFunction(GLcontext *ctx);
quad_func natChooseQuadFunctionDB(GLcontext *ctx);
void natFastRectFunction(GLcontext *ctx, GLint x, GLint y, GLint width, GLint height);
rect_func natChooseRectFunction(GLcontext *ctx);
rect_func natChooseRectFunctionDB(GLcontext *ctx);
void nat8WriteRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const  GLubyte rgba[][4], const GLubyte mask[]);
void nat8WriteRGBSpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const  GLubyte rgb[][3], const GLubyte mask[]);
void nat8WriteRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			 const  GLubyte rgba[][4], const GLubyte mask[]);
void nat8WriteRGBSpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const  GLubyte rgb[][3], const GLubyte mask[]);
void natDWriteRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const  GLubyte rgba[][4], const GLubyte mask[]);
void natDWriteRGBSpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const  GLubyte rgb[][3], const GLubyte mask[]);
void natDWriteMonoCISpan(const GLcontext *ctx, GLuint n, GLint x, GLint y, const GLubyte mask[]);
void natDWriteRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			 const  GLubyte rgba[][4], const GLubyte mask[]);
void natDWriteRGBSpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const  GLubyte rgb[][3], const GLubyte mask[]);
void natDWriteMonoCISpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			  const GLubyte mask[]);
void natGWriteRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const  GLubyte rgba[][4], const GLubyte mask[]);
void natGWriteRGBSpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const  GLubyte rgb[][3], const GLubyte mask[]);
void natGWriteRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			 const  GLubyte rgba[][4], const GLubyte mask[]);
void natGWriteRGBSpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const  GLubyte rgb[][3], const GLubyte mask[]);
void natWriteCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const GLuint index[],
		      const GLubyte mask[]);
void natWriteCI8Span(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		     const GLubyte index[],
		     const GLubyte mask[]);
void natWriteMonoCISpan(const GLcontext *ctx, GLuint n, GLint x, GLint y, const GLubyte mask[]);
void natWriteCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const GLuint index[],
			const GLubyte mask[]);
void natWriteCI8SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const GLubyte index[],
		       const GLubyte mask[]);
void natWriteMonoCISpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			  const GLubyte mask[]);
void natReadRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       GLubyte rgba[][4]);
void natReadCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void natReadCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void natReadRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		     GLubyte rgba[][4]);
void nat8WriteRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const  GLubyte rgba[][4], const GLubyte mask[]);
void nat8WriteRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const  GLubyte rgba[][4], const GLubyte mask[]);
void natDWriteRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const  GLubyte rgba[][4], const GLubyte mask[]);
void natDWriteMonoCIPixel(const GLcontext *ctx, GLuint n,
			 const GLint x[], const GLint y[],
			 const GLubyte mask[]);
void natDWriteRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const  GLubyte rgba[][4], const GLubyte mask[]);
void natDWriteMonoCIPixelDB(const GLcontext *ctx, GLuint n,
			    const GLint x[], const GLint y[],
			    const GLubyte mask[]);
void natGWriteRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const  GLubyte rgba[][4], const GLubyte mask[]);
void natGWriteRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const  GLubyte rgba[][4], const GLubyte mask[]);
void natWriteCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const GLuint index[], const GLubyte mask[]);
void natWriteMonoCIPixel(const GLcontext *ctx, GLuint n,
			 const GLint x[], const GLint y[],
			 const GLubyte mask[]);
void natWriteCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const GLuint index[], const GLubyte mask[]);
void natWriteMonoCIPixelDB(const GLcontext *ctx, GLuint n,
			    const GLint x[], const GLint y[],
			    const GLubyte mask[]);
void natReadRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
		       GLubyte rgba[][4], const GLubyte mask[]);
void natReadCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
		       GLuint index[], const GLubyte mask[]);
void natReadRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			 GLubyte rgba[][4], const GLubyte mask[]);
void natReadCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			 GLuint index[], const GLubyte mask[]);
