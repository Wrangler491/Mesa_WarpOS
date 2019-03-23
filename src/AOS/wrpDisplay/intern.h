const char *wrpRendererString(void);
const char *wrpRendererStringDB(void);
void wrpClearIndex(GLcontext *ctx, GLuint index);
void wrpClearColor(GLcontext *ctx,
		   GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLbitfield wrpClearTD(GLcontext *ctx, GLbitfield mask,
		      GLboolean all, GLint x, GLint y, GLint width, GLint height);
GLbitfield wrpClear(GLcontext *ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height);
GLbitfield wrpClearDB(GLcontext *ctx, GLbitfield mask, GLboolean all,
		      GLint x, GLint y, GLint width, GLint height);
void wrpSetIndex(GLcontext *ctx, GLuint index);
void wrpSetColor(GLcontext *ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a);
GLboolean wrpIndexMask(GLcontext *ctx, GLuint mask);
GLboolean wrpColorMask(GLcontext *ctx, GLboolean rmask, GLboolean gmask, GLboolean bmask, GLboolean amask);
GLboolean wrpLogicOp(GLcontext *ctx, GLenum op);
void wrpDrawLineFlat(GLcontext *ctx, GLuint v1, GLuint v2, GLuint pv);
void wrpDrawLineSmooth(GLcontext *ctx, GLuint v1, GLuint v2, GLuint pv);
void wrpDrawLineSmoothTwo(GLcontext *ctx, GLuint v1, GLuint v2, GLuint pv);
line_func wrpChooseLineFunction(GLcontext *ctx);
void wrpDrawLineStrip(GLcontext *ctx, GLuint first, GLuint last);
linestrip_func wrpChooseLineStripFunction(GLcontext *ctx);
GLboolean wrpDirectDrawPixels(GLcontext *ctx,
			      const struct gl_pixelstore_attrib *unpack,
			      GLsizei width, GLsizei height,
			      GLenum format, GLenum type,
			      const GLvoid *pixels);
GLboolean wrpDirectDrawPixelsDB(GLcontext *ctx,
			        const struct gl_pixelstore_attrib *unpack,
			        GLsizei width, GLsizei height,
			        GLenum format, GLenum type,
			        const GLvoid *pixels);
void wrpDrawPointFlat(GLcontext *ctx, GLuint first, GLuint last);
void wrpDrawPointSmooth(GLcontext *ctx, GLuint first, GLuint last);
void wrpDrawPointSmoothTwo(GLcontext *ctx, GLuint first, GLuint last);
points_func wrpChoosePointsFunction(GLcontext *ctx);
void wrpDrawTriangleFlat(GLcontext *ctx, GLuint v1, GLuint v2,
			  GLuint v3, GLuint pv);
void wrpDrawTriangleSmooth(GLcontext *ctx, GLuint v1, GLuint v2,
			   GLuint v3, GLuint pv);
void wrpDrawTriangleSmoothTwo(GLcontext *ctx, GLuint v1, GLuint v2,
			      GLuint v3, GLuint pv);
triangle_func wrpChooseTrianglesFunction(GLcontext *ctx);
void wrpDrawQuadFlat(GLcontext *ctx, GLuint v1, GLuint v2,
		      GLuint v3, GLuint v4, GLuint pv);
void wrpDrawQuadSmooth(GLcontext *ctx, GLuint v1, GLuint v2,
			GLuint v3, GLuint v4, GLuint pv);
void wrpDrawQuadSmoothTwo(GLcontext *ctx, GLuint v1, GLuint v2,
			   GLuint v3, GLuint v4, GLuint pv);
quad_func wrpChooseQuadFunction(GLcontext *ctx);
void wrpDrawTriStrip(GLcontext *ctx, GLuint first, GLuint last);
tristrip_func wrpChooseTriStripFunction(GLcontext *ctx);
void wrpDrawTriFan(GLcontext *ctx, GLuint zero, GLuint first, GLuint last);
trifan_func wrpChooseTriFanFunction(GLcontext *ctx);
void wrpSetupFlat(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupFlatZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraud(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraudZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupFlatFog(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupFlatFogZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraudFog(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraudFogZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupFlatTex(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupFlatTexZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraudTex(GLcontext *ctx, GLuint start, GLuint end);
void wrpSetupGouraudTexZ(GLcontext *ctx, GLuint start, GLuint end);
void wrpCopyFlat(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyFlatZ(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraud(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraudZ(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyFlatFog(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyFlatFogZ(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraudFog(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraudFogZ(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyFlatTex(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyFlatTexZ(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraudTex(W3D_Vertex *dest, W3D_Vertex *source);
void wrpCopyGouraudTexZ(W3D_Vertex *dest, W3D_Vertex *source);
rastersetupfunc wrpChooseRasterSetupFunction(GLcontext *ctx);
GLuint wrpDepthTestSpan(GLcontext *ctx,
			GLuint n, GLint x, GLint y, const GLdepth z[],
			GLubyte mask[]);
void wrpDepthTestPixels(GLcontext *ctx,
			GLuint n, const GLint x[], const GLint y[],
			const GLdepth z[], GLubyte mask[]);
void wrpReadDepthSpanFloat(GLcontext *ctx,
			   GLuint n, GLint x, GLint y, GLfloat depth[]);
void wrpReadDepthSpanInt(GLcontext *ctx,
			 GLuint n, GLint x, GLint y, GLdepth depth[]);
void wrpSetupFog(GLcontext *ctx);
void wrpDeleteTexture(GLcontext *ctx, struct gl_texture_object *tObj);
void wrpBindTexture(GLcontext *ctx, GLenum target,
		    struct gl_texture_object *tObj);
void wrpTexParameter(GLcontext *ctx, GLenum target,
		     struct gl_texture_object *tObj,
		     GLenum pname, const GLfloat *params);
GLboolean wrpConvertTexture(W3D_Context *TDcontext, W3Dtexobj *to,
			    W3Dteximg *ti,
			    const struct gl_texture_image *image,
			    GLint internalformat, GLint xoffset, GLint yoffset,
			    GLsizei width, GLsizei height, int *bppix);
void wrpTexImage_generic(GLcontext *ctx, GLenum target,
			 struct gl_texture_object *tObj, GLint level,
			 GLint xoffset, GLint yoffset,
			 GLsizei width, GLsizei height,
			 GLint internalFormat,
			 const struct gl_texture_image *image,
			 GLboolean fullUpdate);
void wrpTexSubImage(GLcontext *ctx, GLenum target,
		    struct gl_texture_object *tObj, GLint level,
		    GLint xoffset, GLint yoffset,
		    GLsizei width, GLsizei height,
		    GLint internalFormat,
		    const struct gl_texture_image *image);
void wrpTexImage(GLcontext *ctx, GLenum target,
		 struct gl_texture_object *tObj, GLint level,
		 GLint internalFormat,
		 const struct gl_texture_image *image);
void wrpTexEnv(GLcontext *ctx, GLenum pname, const GLfloat *param);
