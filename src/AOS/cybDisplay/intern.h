const char *cybRendererString(void);
const char *cybRendererStringDB(void);
void cybClearIndex(GLcontext *ctx, GLuint index);
void cybClearColor(GLcontext *ctx,
		   GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLbitfield cybClear(GLcontext *ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height);
GLbitfield cybClearDB(GLcontext *ctx, GLbitfield mask, GLboolean all,
		      GLint x, GLint y, GLint width, GLint height);
void cybSetIndex(GLcontext *ctx, GLuint index);
void cybSetColor(GLcontext *ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLboolean cybIndexMask(GLcontext *ctx, GLuint mask);
GLboolean cybColorMask(GLcontext *ctx, GLboolean rmask, GLboolean gmask, GLboolean bmask, GLboolean amask);
GLboolean cybLogicOp(GLcontext *ctx, GLenum op);
void cybFastLineFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint pv);
line_func cybChooseLineFunction(GLcontext *ctx);
line_func cybChooseLineFunctionDB(GLcontext *ctx);
GLboolean cybDirectDrawPixels(GLcontext *ctx,
                              GLint x, GLint y, GLsizei width, GLsizei height,
                              GLenum format, GLenum type,
                              const struct gl_pixelstore_attrib *unpack,
                              const GLvoid *pixels );
GLboolean cybDirectDrawPixelsDB(GLcontext *ctx,
                                GLint x, GLint y, GLsizei width, GLsizei height,
                                GLenum format, GLenum type,
                                const struct gl_pixelstore_attrib *unpack,
                                const GLvoid *pixels );
void cybFastPointsFunction(GLcontext *ctx, GLuint first, GLuint last);
points_func cybChoosePointsFunction(GLcontext *ctx);
points_func cybChoosePointsFunctionDB(GLcontext *ctx);
void cybFastTriangleFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint v2, GLuint pv);
triangle_func cybChooseTriangleFunction(GLcontext *ctx);
triangle_func cybChooseTriangleFunctionDB(GLcontext *ctx);
void cybFastQuadFunction(GLcontext *ctx, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLuint pv);
quad_func cybChooseQuadFunction(GLcontext *ctx);
quad_func cybChooseQuadFunctionDB(GLcontext *ctx);
void cybFastRectFunction(GLcontext *ctx, GLint x, GLint y, GLint width, GLint height);
rect_func cybChooseRectFunction(GLcontext *ctx);
rect_func cybChooseRectFunctionDB(GLcontext *ctx);
void cyb8ReadCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cyb8ReadCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cybDReadCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cybDReadCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cybGReadCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cybGReadCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y, GLuint index[]);
void cybWriteRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const  GLubyte rgba[][4], const GLubyte mask[]);
void cybWriteRGBSpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		     const  GLubyte rgb[][3], const GLubyte mask[]);
void cybWriteCI32Span(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		      const GLuint index[],
		      const GLubyte mask[]);
void cybWriteCI8Span(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		     const GLubyte index[],
		     const GLubyte mask[]);
void cybWriteMonoCISpan(const GLcontext *ctx, GLuint n, GLint x, GLint y, const GLubyte mask[]);
void cybWriteRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const  GLubyte rgba[][4], const GLubyte mask[]);
void cybWriteRGBSpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const  GLubyte rgb[][3], const GLubyte mask[]);
void cybWriteCI32SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
			const GLuint index[],
			const GLubyte mask[]);
void cybWriteCI8SpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       const GLubyte index[],
		       const GLubyte mask[]);
void cybWriteMonoCISpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y, const GLubyte mask[]);
void cybReadRGBASpan(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		     GLubyte rgba[][4]);
void cybReadRGBASpanDB(const GLcontext *ctx, GLuint n, GLint x, GLint y,
		       GLubyte rgba[][4]);
void cyb8ReadCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			GLuint index[], const GLubyte mask[]);
void cyb8ReadCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  GLuint index[], const GLubyte mask[]);
void cybDReadCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			GLuint index[], const GLubyte mask[]);
void cybDReadCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  GLuint index[], const GLubyte mask[]);
void cybGReadCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			GLuint index[], const GLubyte mask[]);
void cybGReadCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  GLuint index[], const GLubyte mask[]);
void cybWriteRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const  GLubyte rgba[][4], const GLubyte mask[]);
void cybWriteCI32Pixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			const GLuint index[], const GLubyte mask[]);
void cybWriteMonoCIPixel(const GLcontext *ctx, GLuint n,
			  const GLint x[], const GLint y[],
			  const GLubyte mask[]);
void cybWriteRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const  GLubyte rgba[][4], const GLubyte mask[]);
void cybWriteCI32PixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			  const GLuint index[], const GLubyte mask[]);
void cybWriteMonoCIPixelDB(const GLcontext *ctx, GLuint n,
			    const GLint x[], const GLint y[],
			    const GLubyte mask[]);
void cybReadRGBAPixel(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
		       GLubyte rgba[][4], const GLubyte mask[]);
void cybReadRGBAPixelDB(const GLcontext *ctx, GLuint n, const GLint x[], const GLint y[],
			 GLubyte rgba[][4], const GLubyte mask[]);
