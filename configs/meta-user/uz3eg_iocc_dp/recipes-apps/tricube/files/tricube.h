/*******************************************************************************************************************************************

  Initial Version - 10/27/2016 - Email - alok.gupta@xilinx.com
  Xilinx Inc.
        
  MULTI  COLOR CUBE APP

 *******************************************************************************************************************************************/


//Include Header Files
/*********************************************************************************************************************************************************/
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <string>
#define GLES_VERSION 2
#include <GLES2/gl2.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "unistd.h"
#include <sys/time.h>




//Constants
/*********************************************************************************************************************************************************/
/* Asset directories and filenames. */




//Macros
/*********************************************************************************************************************************************************/
//Earlier ENABLE_THREAD_EXECUTION was defined as EXECUTE_ROTATION , Naming Convention changed
//#define ENABLE_THREAD_EXECUTION
/*********************************************************************************************************************************************************/
int WINDOW_WIDTH = 1260;
int WINDOW_HEIGHT = 720;
#define SHOW_FPS 1
inline float degreesToRadians(float degrees)
{
	return M_PI * degrees / 180.0f;
}
#define GL_CHECK(x) \
	x; \
{ \
	GLenum glError = glGetError(); \
	if(glError != GL_NO_ERROR) { \
		printf("glGetError() = %i (0x%.8x) at %s:%i\n", glError, glError, __FILE__, __LINE__); \
		exit(1); \
	} \
}


//Static/Global Variables
/*********************************************************************************************************************************************************/
int windowWidth = -1;
int windowHeight = -1;
GLint iLocPosition;
GLint iLocColor;
GLint TiLocPosition;
GLint TiLocColor;

GLint iLocMVP;



EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;
EGLConfig eglConfig;
EGLint eglConfigCount;


static GLuint program;
static int FPS = 0;
Display *dpy = NULL;
char *dpyName = NULL;
Window window;



//Structures
/*********************************************************************************************************************************************************/
static const EGLint context_attribs[] = {
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
};

static const EGLint config_attribs[] = {
      EGL_RED_SIZE, 1,
      EGL_GREEN_SIZE, 1,
      EGL_BLUE_SIZE, 1,
      EGL_DEPTH_SIZE, 1,
      EGL_RENDERABLE_TYPE, 
      EGL_OPENGL_ES2_BIT,
      EGL_NONE
};

class Vec2
{
	public:
		int x, y;
};

class Vec3
{
	public:
		int x, y, z;
};

class Vec4
{
	public:
		int x, y, z, w;
};

class Vec2f
{
	public:
		float x, y;
};

class Vec3f
{
	public:
		float x, y, z;

		void normalize(void)
		{
			float length = sqrt(x * x + y * y + z * z);

			x /= length;
			y /= length;
			z /= length;
		}

		static Vec3f cross(const Vec3f& vector1, const Vec3f& vector2)
		{
			/* Floating point vector to be returned. */
			Vec3f crossProduct;

			crossProduct.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
			crossProduct.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
			crossProduct.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

			return crossProduct;
		}

		static float dot(Vec3f& vector1, Vec3f& vector2)
		{
			return (vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z);
		}
};

class Vec4f
{
	public:
		float x, y, z, w;

		void normalize(void)
		{
			float length = sqrt(x * x + y * y + z * z + w * w);

			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
};



class Matrix
{
	private:
		float elements[16];
		static Matrix multiply(Matrix *left, Matrix *right);
		static const float identityArray[];
	public:
		float* getAsArray(void);
		Matrix(void);
		float& operator[] (unsigned element);
		Matrix operator* (Matrix right);
		Matrix& operator=(const Matrix &another);
		Matrix(const float* array);
		static Matrix identityMatrix;
		static Vec4f vertexTransform(Vec4f *vector, Matrix *matrix);
		static Vec3f vertexTransform(Vec3f *vector, Matrix *matrix);
		static void matrixTranspose(Matrix *matrix);
		static Matrix createRotationX(float angle);
		static Matrix createRotationY(float angle);
		static Matrix createRotationZ(float angle);
		static Matrix createTranslation(float x, float y, float z);
		static Matrix createScaling(float x, float y, float z);
		void print(void);
		static Matrix matrixPerspective(float FOV, float ratio, float zNear, float zFar);
		static Matrix matrixCameraLookAt(Vec3f eye, Vec3f center, Vec3f up);
		static Matrix matrixOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);
		static Matrix matrixInvert(Matrix *matrix);
		static float matrixDeterminant(float *matrix);
		static float matrixDeterminant(Matrix *matrix);
		static Matrix matrixScale(Matrix *matrix, float scale);

};



//Function Declrations
/*********************************************************************************************************************************************************/
void prepareGraphics (int WINDOW_WIDTH,int WINDOW_HEIGHT);
void doGraphics(void);
void runGraphics(void);
void CleanupEGL(void);
void ComputeFrameRate (void);
unsigned ComputeTickCount ();

//Primitives Data (Vertex,texture,Color,Normal)
/*********************************************************************************************************************************************************/
/* 3D data. Vertex range -0.5..0.5 in all axes.
 * Z -0.5 is near, 0.5 is far. */
const float vertices[] =
{
	/* Front face. */
	/* Bottom left */
	-0.5,  0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	/* Top right */
	-0.5,  0.5, -0.5,
	0.5,  0.5, -0.5,
	0.5, -0.5, -0.5,
	/* Left face */
	/* Bottom left */
	-0.5,  0.5,  0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5,  0.5,
	/* Top right */
	-0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	-0.5, -0.5, -0.5,
	/* Top face */
	/* Bottom left */
	-0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	-0.5,  0.5, -0.5,
	/* Top right */
	-0.5,  0.5,  0.5,
	0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	/* Right face */
	/* Bottom left */
	0.5,  0.5, -0.5,
	0.5, -0.5,  0.5,
	0.5, -0.5, -0.5,
	/* Top right */
	0.5,  0.5, -0.5,
	0.5,  0.5,  0.5,
	0.5, -0.5,  0.5,
	/* Back face */
	/* Bottom left */
	0.5,  0.5,  0.5,
	-0.5, -0.5,  0.5,
	0.5, -0.5,  0.5,
	/* Top right */
	0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5, -0.5,  0.5,
	/* Bottom face */
	/* Bottom left */
	-0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5,
	/* Top right */
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
};

const float colors[] =
{
	/* Front face */
	/* Bottom left */
	1.0, 0.0, 0.0, /* red */
	1.0, 0.0, 0.0, /* blue */
	1.0, 0.0, 0.0, /* green */
	/* Top right */
	0.0, 1.0, 0.0, /* red */
	0.0, 1.0, 0.0, /* yellow */
	0.0, 1.0, 0.0, /* blue */
	/* Left face */
	/* Bottom left */
	0.0, 0.0, 1.0, /* white */
	0.0, 0.0, 1.0, /* green */
	0.0, 0.0, 1.0, /* cyan */
	/* Top right */
	1.0, 1.0, 1.0, /* white */
	1.0, 0.0, 1.0, /* red */
	0.0, 1.0, 0.0, /* green */
	/* Top face */
	/* Bottom left */
	1.0, 1.0, 1.0, /* white */
	1.0, 1.0, 0.0, /* yellow */
	1.0, 0.0, 0.0, /* red */
	/* Top right */
	1.0, 0.0, 1.0, /* white */
	0.0, 1.0, 1.0, /* black */
	1.0, 1.0, 0.0, /* yellow */
	/* Right face */
	/* Bottom left */
	0.0, 1.0, 0.0, /* yellow */
	1.0, 1.0, 1.0, /* magenta */
	0.0, 1.0, 1.0, /* blue */
	/* Top right */
	1.0, 1.0, 0.0, /* yellow */
	1.0, 0.0, 0.0, /* black */
	1.0, 1.0, 1.0, /* magenta */
	/* Back face */
	/* Bottom left */
	0.0, 1.0, 1.0, /* black */
	0.0, 1.0, 1.0, /* cyan */
	1.0, 0.0, 1.0, /* magenta */
	/* Top right */
	0.0, 0.0, 0.0, /* black */
	1.0, 1.0, 1.0, /* white */
	0.0, 1.0, 1.0, /* cyan */
	/* Bottom face */
	/* Bottom left */
	1.0, 1.0, 1.0, /* green */
	1.0, 0.0, 1.0, /* magenta */
	0.0, 1.0, 1.0, /* cyan */
	/* Top right */
	0.0, 1.0, 0.0, /* green */
	1.0, 0.0, 1.0, /* blue */
	1.0, 0.0, 1.0, /* magenta */
};


/* Simple triangle. */
const float triangleVertices[] =
{
     0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
};

/* Per corner colors for the triangle (Red, Green, Green). */
const float triangleColors[] =
{
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
};

