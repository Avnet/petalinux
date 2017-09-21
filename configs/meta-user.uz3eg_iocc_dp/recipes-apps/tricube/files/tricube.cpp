/*******************************************************************************************************************************************

  Initial Version - 10/27/2016 - Email - alok.gupta@xilinx.com
  Xilinx Inc.

  MULTI COLOR CUBE APP

  Matrix calculations are being pulled from ARM SDK
 *******************************************************************************************************************************************/

#include "tricube.h"
#include "assert.h"

using std::string;

//These are just matrix functions...

/* Identity matrix. */
const float Matrix::identityArray[16] =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

Matrix Matrix::identityMatrix = Matrix(identityArray);

Matrix::Matrix(const float* array)
{
	memcpy(elements, array, 16 * sizeof(float));
}

Matrix::Matrix(void)
{
}

float& Matrix::operator[] (unsigned element)
{
	if (element > 15)
	{
		printf("Matrix only has 16 elements, tried to access element %d", element);
		exit(1);
	}
	return elements[element];
}

Matrix Matrix::operator* (Matrix right)
{
	return multiply(this, &right);
}

Matrix& Matrix::operator= (const Matrix &another)
{
	if(this != &another)
	{
		memcpy(this->elements, another.elements, 16 * sizeof(float));
	}

	return *this;
}

float* Matrix::getAsArray(void)
{
	return elements;
}

float Matrix::matrixDeterminant(float *matrix)
{
	float result = 0.0f;

	result  = matrix[0] * (matrix[4] * matrix[8] - matrix[7] * matrix[5]);
	result -= matrix[3] * (matrix[1] * matrix[8] - matrix[7] * matrix[2]);
	result += matrix[6] * (matrix[1] * matrix[5] - matrix[4] * matrix[2]);

	return result;
}

float Matrix::matrixDeterminant(Matrix *matrix)
{
	float matrix3x3[9];
	float determinant3x3 = 0.0f;
	float result = 0.0f;

	/* Remove (i, j) (1, 1) to form new 3x3 matrix. */
	matrix3x3[0] = matrix->elements[ 5];
	matrix3x3[1] = matrix->elements[ 6];
	matrix3x3[2] = matrix->elements[ 7];
	matrix3x3[3] = matrix->elements[ 9];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	determinant3x3 = matrixDeterminant(matrix3x3);
	result += matrix->elements[0] * determinant3x3;

	/* Remove (i, j) (1, 2) to form new 3x3 matrix. */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 9];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	determinant3x3 = matrixDeterminant(matrix3x3);
	result -= matrix->elements[4] * determinant3x3;

	/* Remove (i, j) (1, 3) to form new 3x3 matrix. */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 5];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	determinant3x3 = matrixDeterminant(matrix3x3);
	result += matrix->elements[8] * determinant3x3;
	/* Remove (i, j) (1, 4) to form new 3x3 matrix. */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 5];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[ 9];
	matrix3x3[7] = matrix->elements[10];
	matrix3x3[8] = matrix->elements[11];
	determinant3x3 = matrixDeterminant(matrix3x3);
	result -= matrix->elements[12] * determinant3x3;

	return result;
}

Matrix Matrix::matrixInvert(Matrix *matrix)
{
	Matrix result;
	float matrix3x3[9];

	/* Find the cofactor of each element. */
	/* Element (i, j) (1, 1) */
	matrix3x3[0] = matrix->elements[ 5];
	matrix3x3[1] = matrix->elements[ 6];
	matrix3x3[2] = matrix->elements[ 7];
	matrix3x3[3] = matrix->elements[ 9];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[0] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (1, 2) */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 9];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[4] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (1, 3) */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 5];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[13];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[8] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (1, 4) */
	matrix3x3[0] = matrix->elements[ 1];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 5];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[ 9];
	matrix3x3[7] = matrix->elements[10];
	matrix3x3[8] = matrix->elements[11];
	result.elements[12] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (2, 1) */
	matrix3x3[0] = matrix->elements[ 4];
	matrix3x3[1] = matrix->elements[ 6];
	matrix3x3[2] = matrix->elements[ 7];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[1] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (2, 2) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[10];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[5] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (2, 3) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[14];
	matrix3x3[8] = matrix->elements[15];
	result.elements[9] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (2, 4) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 2];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 6];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[ 8];
	matrix3x3[7] = matrix->elements[10];
	matrix3x3[8] = matrix->elements[11];
	result.elements[13] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (3, 1) */
	matrix3x3[0] = matrix->elements[ 4];
	matrix3x3[1] = matrix->elements[ 5];
	matrix3x3[2] = matrix->elements[ 7];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[ 9];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[15];
	result.elements[2] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (3, 2) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[ 9];
	matrix3x3[5] = matrix->elements[11];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[15];
	result.elements[6] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (3, 3) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 5];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[15];
	result.elements[10] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (3, 4) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 3];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 5];
	matrix3x3[5] = matrix->elements[ 7];
	matrix3x3[6] = matrix->elements[ 8];
	matrix3x3[7] = matrix->elements[ 9];
	matrix3x3[8] = matrix->elements[11];
	result.elements[14] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (4, 1) */
	matrix3x3[0] = matrix->elements[ 4];
	matrix3x3[1] = matrix->elements[ 5];
	matrix3x3[2] = matrix->elements[ 6];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[ 9];
	matrix3x3[5] = matrix->elements[10];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[14];
	result.elements[3] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (4, 2) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 2];
	matrix3x3[3] = matrix->elements[ 8];
	matrix3x3[4] = matrix->elements[ 9];
	matrix3x3[5] = matrix->elements[10];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[14];
	result.elements[7] = matrixDeterminant(matrix3x3);

	/* Element (i, j) (4, 3) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 2];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 5];
	matrix3x3[5] = matrix->elements[ 6];
	matrix3x3[6] = matrix->elements[12];
	matrix3x3[7] = matrix->elements[13];
	matrix3x3[8] = matrix->elements[14];
	result.elements[11] = -matrixDeterminant(matrix3x3);

	/* Element (i, j) (4, 4) */
	matrix3x3[0] = matrix->elements[ 0];
	matrix3x3[1] = matrix->elements[ 1];
	matrix3x3[2] = matrix->elements[ 2];
	matrix3x3[3] = matrix->elements[ 4];
	matrix3x3[4] = matrix->elements[ 5];
	matrix3x3[5] = matrix->elements[ 6];
	matrix3x3[6] = matrix->elements[ 8];
	matrix3x3[7] = matrix->elements[ 9];
	matrix3x3[8] = matrix->elements[10];
	result.elements[15] = matrixDeterminant(matrix3x3);

	/* The adjoint is the transpose of the cofactor matrix. */
	matrixTranspose(&result);

	/* The inverse is the adjoint divided by the determinant. */
	result = matrixScale(&result, 1.0f / matrixDeterminant(matrix));

	return result;
}

Matrix Matrix::matrixScale(Matrix *matrix, float scale)
{
	Matrix result;

	for(int allElements = 0; allElements < 16; allElements ++)
	{
		result.elements[allElements] = matrix->elements[allElements] * scale;
	}

	return result;
}

void Matrix::matrixTranspose(Matrix *matrix)
{
	float temp;

	temp = matrix->elements[1];
	matrix->elements[1] = matrix->elements[4];
	matrix->elements[4] = temp;

	temp = matrix->elements[2];
	matrix->elements[2] = matrix->elements[8];
	matrix->elements[8] = temp;

	temp = matrix->elements[3];
	matrix->elements[3] = matrix->elements[12];
	matrix->elements[12] = temp;

	temp = matrix->elements[6];
	matrix->elements[6] = matrix->elements[9];
	matrix->elements[9] = temp;

	temp = matrix->elements[7];
	matrix->elements[7] = matrix->elements[13];
	matrix->elements[13] = temp;

	temp = matrix->elements[11];
	matrix->elements[11] = matrix->elements[14];
	matrix->elements[14] = temp;
}

Matrix Matrix::createScaling(float x, float y, float z)
{
	Matrix result = identityMatrix;

	result.elements[ 0] = x;
	result.elements[ 5] = y;
	result.elements[10] = z;

	return result;
}

Matrix Matrix::createTranslation(float x, float y, float z)
{
	Matrix result = identityMatrix;

	result.elements[12] = x;
	result.elements[13] = y;
	result.elements[14] = z;

	return result;
}

Matrix Matrix::matrixPerspective(float FOV, float ratio, float zNear, float zFar)
{
	Matrix result = identityMatrix;

	FOV = 1.0f / tan(FOV * 0.5f);

	result.elements[ 0] = FOV / ratio;
	result.elements[ 5] = FOV;
	result.elements[10] = -(zFar + zNear) / (zFar - zNear);
	result.elements[11] = -1.0f;
	result.elements[14] = (-2.0f * zFar * zNear) / (zFar - zNear);
	result.elements[15] = 0.0f;

	return result;
}

Matrix Matrix::matrixCameraLookAt(Vec3f eye, Vec3f center, Vec3f up)
{
	Matrix result = identityMatrix;

	Vec3f cameraX, cameraY;

	Vec3f cameraZ = {center.x - eye.x, center.y - eye.y, center.z - eye.z};
	cameraZ.normalize();

	cameraX = Vec3f::cross(cameraZ, up);
	cameraX.normalize();

	cameraY = Vec3f::cross(cameraX, cameraZ);

	/*
	 * The final cameraLookAt should look like: 
	 *
	 * cameraLookAt[] = { cameraX.x,        cameraY.x,   -cameraZ.x,  0.0f,
	 *                                        cameraX.y,    cameraY.y,   -cameraZ.y,  0.0f,
	 *                                        cameraX.z,    cameraY.z,   -cameraZ.z,  0.0f,
	 *                                       -eye.x,           -eye.y,               -eye.z,          1.0f };
	 */

	result[0]  = cameraX.x;
	result[1]  = cameraY.x;
	result[2]  = -cameraZ.x;

	result[4]  = cameraX.y;
	result[5]  = cameraY.y;
	result[6]  = -cameraZ.y;

	result[8]  = cameraX.z;
	result[9]  = cameraY.z;
	result[10] = -cameraZ.z;

	result[12] = -eye.x;
	result[13] = -eye.y;
	result[14] = -eye.z;

	return result;
}


Matrix Matrix::matrixOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix result = identityMatrix;

	result.elements[ 0] = 2.0f / (right - left);
	result.elements[12] = -(right + left) / (right - left);

	result.elements[ 5] = 2.0f / (top - bottom);
	result.elements[13] = -(top + bottom) / (top - bottom);

	result.elements[10] = -2.0f / (zFar - zNear);
	result.elements[14] = -(zFar + zNear) / (zFar - zNear);

	return result;
}

Matrix Matrix::createRotationX(float angle)
{
	Matrix result = identityMatrix;

	result.elements[ 5] = cos(degreesToRadians(angle));
	result.elements[ 9] = -sin(degreesToRadians(angle));
	result.elements[ 6] = sin(degreesToRadians(angle));
	result.elements[10] = cos(degreesToRadians(angle));

	return result;
}

Matrix Matrix::createRotationY(float angle)
{
	Matrix result = identityMatrix;

	result.elements[ 0] = cos(degreesToRadians(angle));
	result.elements[ 8] = sin(degreesToRadians(angle));
	result.elements[ 2] = -sin(degreesToRadians(angle));
	result.elements[10] = cos(degreesToRadians(angle));

	return result;
}

Matrix Matrix::createRotationZ(float angle)
{
	Matrix result = identityMatrix;

	result.elements[0] = cos(degreesToRadians(angle));
	result.elements[4] = -sin(degreesToRadians(angle));
	result.elements[1] = sin(degreesToRadians(angle));
	result.elements[5] = cos(degreesToRadians(angle));

	return result;
}

Matrix Matrix::multiply(Matrix *left, Matrix *right)
{
	Matrix result;

	for(int row = 0; row < 4; row ++)
	{
		for(int column = 0; column < 4; column ++)
		{
			/*result.elements[row * 4 + column]  = left->elements[0 + row * 4] * right->elements[column + 0 * 4];
			  result.elements[row * 4 + column] += left->elements[1 + row * 4] * right->elements[column + 1 * 4];
			  result.elements[row * 4 + column] += left->elements[2 + row * 4] * right->elements[column + 2 * 4];
			  result.elements[row * 4 + column] += left->elements[3 + row * 4] * right->elements[column + 3 * 4];*/
			float accumulator = 0.0f;
			for(int allElements = 0; allElements < 4; allElements ++)
			{
				accumulator += left->elements[allElements * 4 + row] * right->elements[column * 4 + allElements];
			}
			result.elements[column * 4 + row] = accumulator;
		}
	}

	return result;
}

Vec4f Matrix::vertexTransform(Vec4f *vertex, Matrix *matrix)
{
	Vec4f result;

	result.x  = vertex->x * matrix->elements[ 0];
	result.x += vertex->y * matrix->elements[ 4];
	result.x += vertex->z * matrix->elements[ 8];
	result.x += vertex->w * matrix->elements[12];

	result.y  = vertex->x * matrix->elements[ 1];
	result.y += vertex->y * matrix->elements[ 5];
	result.y += vertex->z * matrix->elements[ 9];
	result.y += vertex->w * matrix->elements[13];

	result.z  = vertex->x * matrix->elements[ 2];
	result.z += vertex->y * matrix->elements[ 6];
	result.z += vertex->z * matrix->elements[10];
	result.z += vertex->w * matrix->elements[14];

	result.w  = vertex->x * matrix->elements[ 3];
	result.w += vertex->y * matrix->elements[ 7];
	result.w += vertex->z * matrix->elements[11];
	result.w += vertex->w * matrix->elements[15];

	return result;
}


Vec3f Matrix::vertexTransform(Vec3f *vertex, Matrix *matrix)
{
	Vec3f result;
	Vec4f extendedVertex;

	extendedVertex.x = vertex->x;
	extendedVertex.y = vertex->y;
	extendedVertex.z = vertex->z;
	extendedVertex.w = 1.0f;

	result.x  = extendedVertex.x * matrix->elements[ 0];
	result.x += extendedVertex.y * matrix->elements[ 4];
	result.x += extendedVertex.z * matrix->elements[ 8];
	result.x += extendedVertex.w * matrix->elements[12];

	result.y  = extendedVertex.x * matrix->elements[ 1];
	result.y += extendedVertex.y * matrix->elements[ 5];
	result.y += extendedVertex.z * matrix->elements[ 9];
	result.y += extendedVertex.w * matrix->elements[13];

	result.z  = extendedVertex.x * matrix->elements[ 2];
	result.z += extendedVertex.y * matrix->elements[ 6];
	result.z += extendedVertex.z * matrix->elements[10];
	result.z += extendedVertex.w * matrix->elements[14];

	return result;
}


//Matrix Functions end...


void prepareGraphics (int width,int height)
{
	EGLint major, minor, n;
	GLuint vertex_shader, fragment_shader;
	GLint ret;

	static const char *VS_SOURCE =
		"precision mediump float;\n"
		"attribute vec4 av4position;\n"
		"attribute vec3 av3colour;\n"
		"uniform mat4 mvp;\n"
		"varying vec3 vv3colour;\n"
		"void main()\n"
		"{\n"
		"    vv3colour = av3colour;\n"
		"    gl_Position = mvp * av4position;\n"
		"}\n";

	static const char *FS_SOURCE =
		"precision mediump float;\n"
		"varying vec3 vv3colour;\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = vec4(vv3colour, 1.0);\n"
		"}\n";


	printf("Calling eglGetDisplay\n");
	eglDisplay = eglGetDisplay (dpy);

	if (!eglInitialize (eglDisplay, &major, &minor))
	{
		printf ("failed to initialize\n");
	}

	printf ("Display Used %p & EGL versions are %d.%d\n", eglDisplay, major,
			minor);
	printf ("Your EGL Version is \"%s\"\n", eglQueryString (eglDisplay, EGL_VERSION));
	printf ("Your EGL Vendor is \"%s\"\n", eglQueryString (eglDisplay, EGL_VENDOR));

	if (!eglBindAPI (EGL_OPENGL_ES_API))
	{
		printf ("failed to bind api EGL_OPENGL_ES_API\n");
	}

	if (!eglChooseConfig (eglDisplay, config_attribs, &eglConfig, 1, &eglConfigCount))
	{
		printf("Error: couldn't get an EGL visual config\n");
		exit(1);
	}


	EGLint vid;
	assert(eglConfig);
	assert(eglConfigCount > 0);

	if (!eglGetConfigAttrib(eglDisplay,eglConfig, EGL_NATIVE_VISUAL_ID, &vid)) {
		printf("Error: eglGetConfigAttrib() failed\n");
		exit(1);
	}


	eglContext = eglCreateContext (eglDisplay, eglConfig, EGL_NO_CONTEXT, context_attribs);
	if (eglContext == EGL_NO_CONTEXT)
	{
		fprintf (stderr, "\nFailed to create EGL context.\n");
		exit (-1);
	}


	eglSurface = eglCreateWindowSurface (eglDisplay, eglConfig, window, NULL);

	if (eglSurface == EGL_NO_SURFACE)
	{
		printf ("failed to create egl surface\n");
	}

	eglMakeCurrent (eglDisplay, eglSurface, eglSurface, eglContext);

	//Pass 1 Program Shader
	vertex_shader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertex_shader, 1, &VS_SOURCE, NULL);
	glCompileShader (vertex_shader);
	glGetShaderiv (vertex_shader, GL_COMPILE_STATUS, &ret);

	if (!ret)
	{
		char *log;

		printf ("vertex shader compilation failed!:\n");
		glGetShaderiv (vertex_shader, GL_INFO_LOG_LENGTH, &ret);
		if (ret > 1)
		{
			log = (char *) malloc (ret);
			glGetShaderInfoLog (vertex_shader, ret, NULL, log);
			printf ("%s", log);
		}

	}

	fragment_shader = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragment_shader, 1, &FS_SOURCE, NULL);
	glCompileShader (fragment_shader);
	glGetShaderiv (fragment_shader, GL_COMPILE_STATUS, &ret);

	if (!ret)
	{
		char *log;

		printf ("fragment shader compilation failed!:\n");
		glGetShaderiv (fragment_shader, GL_INFO_LOG_LENGTH, &ret);

		if (ret > 1)
		{
			log = (char *) malloc (ret);
			glGetShaderInfoLog (fragment_shader, ret, NULL, log);
			printf ("%s", log);
		}

	}

	program = glCreateProgram ();
	glAttachShader (program, vertex_shader);
	glAttachShader (program, fragment_shader);
	glLinkProgram (program);
	glGetProgramiv (program, GL_LINK_STATUS, &ret);
	if (!ret)
	{
		char *log;

		printf ("program linking failed!:\n");
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &ret);

		if (ret > 1)
		{
			log = (char *) malloc (ret);
			glGetProgramInfoLog (program, ret, NULL, log);
			printf ("%s", log);
		}

	}

	glUseProgram (program);

}


void runGraphics()
{
	GL_CHECK(glUseProgram(program));

	/* Enable attributes for position, color and texture coordinates etc. */
	GL_CHECK(glEnableVertexAttribArray(iLocPosition));
	GL_CHECK(glEnableVertexAttribArray(iLocColor));


	/* Populate attributes for position, color and texture coordinates etc. */
	GL_CHECK(glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices));
	GL_CHECK(glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors));


	static float angleX = 0, angleY = 0, angleZ = 0;
	/*
	 * Do some rotation with Euler angles. It is not a fixed axis as
	 * quaternions would be, but the effect is nice.
	 */
	Matrix modelView = Matrix::createRotationX(angleX);
	Matrix rotation = Matrix::createRotationY(angleY);

	modelView = rotation * modelView;

	rotation = Matrix::createRotationZ(angleZ);
	Matrix translate1 = Matrix::createTranslation(-0.5,0.0,0.0);

	modelView =  translate1 *rotation * modelView;

	/* Pull the camera back from the cube */
	modelView[14] -= 3.5;

	Matrix perspective = Matrix::matrixPerspective(45.0f, windowWidth/(float)windowHeight, 0.01f, 100.0f);
	Matrix modelViewPerspective = perspective * modelView;

	GL_CHECK(glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, modelViewPerspective.getAsArray()));

	/* Update cube's rotation angles for animating. */
	angleX += 2.0;
	angleY += 2.0;
	angleZ += 0.0;

	if(angleX >= 360) angleX -= 360;
	if(angleX < 0) angleX += 360;
	if(angleY >= 360) angleY -= 360;
	if(angleY < 0) angleY += 360;
	if(angleZ >= 360) angleZ -= 360;
	if(angleZ < 0) angleZ += 360;

	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));


	/* Pass the triangle vertex positions to the shader */
	GL_CHECK(glVertexAttribPointer(TiLocPosition, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices));
	GL_CHECK(glEnableVertexAttribArray(TiLocPosition));

	/* Pass the vertex colours to the shader */
	GL_CHECK(glVertexAttribPointer(TiLocColor, 4, GL_FLOAT, GL_FALSE, 0, triangleColors));
	GL_CHECK(glEnableVertexAttribArray(TiLocColor));

	Matrix translate = Matrix::createTranslation(1.5,0.0,0.0);
	modelView = translate * rotation * modelView;
	modelViewPerspective = perspective * modelView;

	GL_CHECK(glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, modelViewPerspective.getAsArray()));

	GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));




}

void doGraphics()
{

	/* Get attribute locations of non-fixed attributes like colour and texture coordinates. */
	iLocPosition = GL_CHECK(glGetAttribLocation(program, "av4position"));
	iLocColor = GL_CHECK(glGetAttribLocation(program, "av3colour"));

	TiLocPosition = GL_CHECK(glGetAttribLocation(program, "av4position"));
	TiLocColor = GL_CHECK(glGetAttribLocation(program, "av3colour"));


	printf("iLocPosition = %i\n", iLocPosition);
	printf("iLocColor   = %i\n", iLocColor);

	/* Get uniform locations */
	iLocMVP = GL_CHECK(glGetUniformLocation(program, "mvp"));

	printf("iLocMVP = %i\n", iLocMVP);

	GL_CHECK(glEnable(GL_CULL_FACE));
	GL_CHECK(glEnable(GL_DEPTH_TEST));

	/* Set clear screen color. */
	GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

}

void CleanupEGL(void)
{

	eglBindAPI(EGL_OPENGL_ES_API);
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, eglContext);
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(eglDisplay, eglContext);
	eglDestroySurface(eglDisplay, eglSurface);
	eglTerminate(dpy);


}

	static void
usage(void)
{
	printf("Usage:\n");
	printf("  -display <displayname>  set the display to run on\n");
	printf("  -info                   display OpenGL renderer info\n");
}

int main(int argc, char **argv)
{
	printf("Start Main\n");
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-display") == 0) {
			dpyName = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "-info") == 0) {
			printf("TRUE\n");
		}
		else {
			usage();
			return -1;
		}
	}
	printf("Calling OpenDisplay\n");
	dpy = XOpenDisplay (dpyName);
	if (!dpy)
	{
		fprintf (stderr, "\nFAILED: to open display\n");
		exit (-1);
	}

	printf("Calling XCreateSimpleWindow\n");
	window =  XCreateSimpleWindow (dpy, RootWindow (dpy, DefaultScreen (dpy)), 1, 1, WINDOW_WIDTH, WINDOW_HEIGHT, 0, BlackPixel (dpy,DefaultScreen(dpy)), BlackPixel (dpy, DefaultScreen (dpy)));
	printf("Calling XStoreName\n");
	XStoreName (dpy, window, "Zynq UltraScale+ MPSoC - MultiColor tricube Application");
	printf("Calling XMapWindow\n");
	XMapWindow (dpy, window);

	printf("Calling prepareGraphics\n");
	prepareGraphics(WINDOW_WIDTH,WINDOW_HEIGHT);
	printf("Calling doGraphics\n");
	doGraphics ();
	bool finish = false;
	while(!finish)
	{
		ComputeFrameRate();
		runGraphics();
		eglSwapBuffers (eglDisplay, eglSurface);
	}

	CleanupEGL();

}



//To Compute Frame Rate
unsigned ComputeTickCount ()
{
	struct timeval tv;
	if (gettimeofday (&tv, NULL) != 0)
		return 0;

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void ComputeFrameRate ()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	float currentTime = ComputeTickCount () * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		if (SHOW_FPS == 1)
/*
*			fprintf (stderr, "\nCurrent Frames Per Second: %d\n\n",
*					(int) framesPerSecond);
*/
		framesPerSecond = 0;
	}
}

