#ifndef _ASSIGNMENT2_H_
#define _ASSIGNMENT2_H_

#include <vector>

#define TRANSFORM_NONE	  0
#define TRANSFORM_ROTATE	1
#define TRANSFORM_SCALE	 2
#define TRANSFORM_TRANSLATE 3

#define OBJ_WIREFRAME	   0
#define OBJ_SOLID		   1
#define OBJ_EDGE			2
#define OBJ_BBOX			3

#define IMG_WIDTH  64
#define IMG_HEIGHT 64

#define X 0
#define Y 1
#define Z 2
#define W 3

#define R 0
#define G 1
#define B 2

#define S 0
#define T 1

#define PI 3.1415926535

struct Point
{
  private:

	float data[4];

  public:

	Point(void)
	{
		data[X] =
			data[Y] =
			data[Z] = 0.f;
		data[W] = 1.f;
	}

	Point(float x, float y, float z, float w)
	{
		data[X] = x;
		data[Y] = y;
		data[Z] = z;
		data[W] = w;
	}

	float &operator [](unsigned int idx)
	{
		return data[idx];
	}

	const float *gldata(void)
	{
		return data;
	}
};

struct Vector
{
  private:

	float data[4];

  public:

	Vector(void)
	{
		data[X] =
			data[Y] =
			data[Z] =
			data[W] = 0.f;
	}

	Vector(float x, float y, float z, float w)
	{
		data[X] = x;
		data[Y] = y;
		data[Z] = z;
		data[W] = w;
	}

	Vector(Point p)
	{
		data[X] = p[X];
		data[Y] = p[Y];
		data[Z] = p[Z];
		data[W] = p[W];
	}

	float &operator [](unsigned int idx)
	{
		return data[idx];
	}
};

struct Color
{
  private:

	float data[3];

  public:

	Color(void)
	{
		data[R] =
			data[G] =
			data[B] = 0.f;
	}

	Color(float r, float g, float b)
	{
		data[R] = r;
		data[G] = g;
		data[B] = b;
	}

	float &operator [](unsigned int idx)
	{
		return data[idx];
	}
};

typedef float TexCoord[2];
typedef float GLColor4[4];

typedef float Matrix[16];

struct Vertex
{
	// Vertex Coordinate
	Point	v;

	// Vertex Normal
	Vector   n;

	// Vertex TextureCorrdinate
	TexCoord t;

	// Vertex Color
	Color	c;
};

struct BoundingBox
{
	Point  minP;
	Point  maxP;
	Vector vSize;
};

struct Face
{
	// Vertex Index
	int v1;
	int v2;
	int v3;
};

struct Object
{
	std::vector<Vertex> vertices;
	std::vector<Face  > faces;

	BoundingBox		 bBox;
};

inline
void subPnt(Vector &vResult, Point &p1, Point &p2)
{
	vResult[X] = p1[X] - p2[X];
	vResult[Y] = p1[Y] - p2[Y];
	vResult[Z] = p1[Z] - p2[Z];
	vResult[W] = p1[W] - p2[W];
}

inline
void add(Vector &vResult, Vector &v1, Vector &v2)
{
	vResult[X] = v1[X] + v2[X];
	vResult[Y] = v1[Y] + v2[Y];
	vResult[Z] = v1[Z] + v2[Z];
	vResult[W] = v1[W] + v2[W];
}

inline
void addScaled(Point &result, Point &p, Vector &v, float s)
{
	result[X] = p[X] + (v[X] * s);
	result[Y] = p[Y] + (v[Y] * s);
	result[Z] = p[Z] + (v[Z] * s);
	result[W] = p[W] + (v[W] * s);
}

inline
void mult(Point &result, Matrix m, Point &pp)
{
	Point p(pp);
	for(int i = 0; i < 4; ++i)
	{
		result[i] =
			m[i +  0] * p[0] +
			m[i +  4] * p[1] +
			m[i +  8] * p[2] +
			m[i + 12] * p[3];
	}
}

inline
void mult(Vector &result, Matrix m, Vector &vv)
{
	Vector v(vv);
	for(int i = 0; i < 4; ++i)
	{
		result[i] =
			m[i +  0] * v[0] +
			m[i +  4] * v[1] +
			m[i +  8] * v[2] +
			m[i + 12] * v[3];
	}
}

inline
void mult(Matrix &result, Matrix a, Matrix b)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[j+4*i] = 
				a[  4*i] * b[4*0+j] +
				a[1+4*i] * b[4*1+j] +
				a[2+4*i] * b[4*2+j] +
				a[3+4*i] * b[4*3+j];
		}
	}
}

inline
void assign(Vector &v1, Vector &v2)
{
	v1[X] = v2[X];
	v1[Y] = v2[Y];
	v1[Z] = v2[Z];
	v1[W] = v2[W];
}

inline
float dot(Vector &v1, Vector &v2)
{
	return v1[X] * v2[X] + v1[Y] * v2[Y] + v1[Z] * v2[Z];
}

inline
void cross(Vector& result, Vector &v1, Vector &v2) {
	result[X] = v1[Y]*v2[Z] - v2[Y]*v1[Z];
	result[Y] = -(v1[X]*v2[Z] - v2[X]*v1[Z]);
	result[Z] = v1[X]*v2[Y] - v2[X]*v1[Y];
}

inline
float length(Vector &v)
{
	return sqrtf(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]);
}

inline
void normalize(Vector &v)
{
	float fLength = length(v);

	fLength = 1.f / fLength;

	v[X] *= fLength;
	v[Y] *= fLength;
	v[Z] *= fLength;
	v[W] *= fLength;
}

inline
float maxComponent3(Vector &v)
{
	float returnValue = v[X];

	if(v[Y] > returnValue)
	{
		returnValue = v[Y];
	}

	if(v[Z] > returnValue)
	{
		returnValue = v[Z];
	}

	return returnValue;
}

inline
void dumpMatrix(Matrix m)
{
	fprintf(stderr, "[\n");
	for(int i = 0; i < 4; ++i)
	{
		fprintf(stderr, "  %f %f %f %f\n",
				m[0 * 4 + i],
				m[1 * 4 + i],
				m[2 * 4 + i],
				m[3 * 4 + i]);
	}
	fprintf(stderr, "]\n");
}

inline void dumpVector(Vector v)
{
	fprintf(stderr, "<%f, %f, %f, %f>\n",
			v[X], v[Y], v[Z], v[W]);
}

inline
void dumpPnt(Point &p)
{
	fprintf(stderr, "[%f %f %f %f]\n",
			p[X], p[Y], p[Z], p[W]);
}

inline
void dumpPnt(const char *szInfo, Point &p)
{
	fprintf(stderr, "%s\n%f %f %f %f\n",
			szInfo,
			p[0],
			p[1],
			p[2],
			p[3]);
}

inline
void combineColor(Color &result, GLColor4 lc, GLColor4 mc, float s)
{
	result[R] += lc[R] * s * mc[R];
	result[G] += lc[G] * s * mc[G];
	result[B] += lc[B] * s * mc[B];
}

inline
void invert(Matrix &result, Matrix m)
{
	// Inverting a 4x4 matrix is non-trivial, so I decided to
	// use existing code, modified slightly.
	// I obtained the implementation of this method from
	// https://stackoverflow.com/a/1148405/1052757
	// and the poster reports he obtained this version from
	// an older version of Mesa.
	Matrix inv;
	float det;

	inv[0] = m[5] * m[10] * m[15] -
			 m[5] * m[11] * m[14] -
			 m[9] * m[6] * m[15] +
			 m[9] * m[7] * m[14] +
			 m[13] * m[6] * m[11] -
			 m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
			  m[4] * m[11] * m[14] +
			  m[8] * m[6] * m[15] -
			  m[8] * m[7] * m[14] -
			  m[12] * m[6] * m[11] +
			  m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9]* m[15] -
			 m[4] * m[11] * m[13] -
			 m[8] * m[5]* m[15] +
			 m[8] * m[7]* m[13] +
			 m[12] * m[5] * m[11] -
			 m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
			   m[4] * m[10] * m[13] +
			   m[8] * m[5] * m[14] -
			   m[8] * m[6] * m[13] -
			   m[12] * m[5] * m[10] +
			   m[12] * m[6] * m[9];

	det = m[0] * inv[0] +
		m[1] * inv[4] +
		m[2] * inv[8] +
		m[3] * inv[12];
	if (det == 0) {
		std::cerr << "Matrix determinant is 0!  Cannot invert; bailing!" << std::endl;
		for (int i = 0; i < 16; i++) {
			result[i] = (float) 0xdeadbeef;
		}
		return;
	}

	inv[1] = -m[1] * m[10] * m[15] +
			  m[1] * m[11] * m[14] +
			  m[9] * m[2] * m[15] -
			  m[9] * m[3] * m[14] -
			  m[13] * m[2] * m[11] +
			  m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
			 m[0] * m[11] * m[14] -
			 m[8] * m[2] * m[15] +
			 m[8] * m[3] * m[14] +
			 m[12] * m[2] * m[11] -
			 m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
			  m[0] * m[11] * m[13] +
			  m[8] * m[1] * m[15] -
			  m[8] * m[3] * m[13] -
			  m[12] * m[1] * m[11] +
			  m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
			  m[0] * m[10] * m[13] -
			  m[8] * m[1] * m[14] +
			  m[8] * m[2] * m[13] +
			  m[12] * m[1] * m[10] -
			  m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
			 m[1] * m[7] * m[14] -
			 m[5] * m[2] * m[15] +
			 m[5] * m[3] * m[14] +
			 m[13] * m[2] * m[7] -
			 m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
			  m[0] * m[7] * m[14] +
			  m[4] * m[2] * m[15] -
			  m[4] * m[3] * m[14] -
			  m[12] * m[2] * m[7] +
			  m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
			  m[0] * m[7] * m[13] -
			  m[4] * m[1] * m[15] +
			  m[4] * m[3] * m[13] +
			  m[12] * m[1] * m[7] -
			  m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
			   m[0] * m[6] * m[13] +
			   m[4] * m[1] * m[15] -
			   m[4] * m[2] * m[13] -
			   m[12] * m[1] * m[6] +
			   m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
			  m[1] * m[7] * m[10] +
			  m[5] * m[2] * m[11] -
			  m[5] * m[3] * m[10] -
			  m[9] * m[2] * m[7] +
			  m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
			 m[0] * m[7] * m[10] -
			 m[4] * m[2] * m[11] +
			 m[4] * m[3] * m[10] +
			 m[8] * m[2] * m[7] -
			 m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
			   m[0] * m[7] * m[9] +
			   m[4] * m[1] * m[11] -
			   m[4] * m[3] * m[9] -
			   m[8] * m[1] * m[7] +
			   m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
			  m[0] * m[6] * m[9] -
			  m[4] * m[1] * m[10] +
			  m[4] * m[2] * m[9] +
			  m[8] * m[1] * m[6] -
			  m[8] * m[2] * m[5];

	det = 1.0 / det;

	for (int i = 0; i < 16; i++) {
		result[i] = inv[i] * det;
	}
}

inline
void transpose(Matrix result, Matrix m)
{
	for (int r = 0; r < 4; r++) {
		for (int c = r+1; c < 4; c++) {
			float tmp = m[4*r+c];
			result[4*r+c] = m[4*c+r];
			result[4*c+r] = tmp;
		}
	}
}

extern float x_angle;
extern float y_angle;
extern float x_offset;
extern float y_offset;
extern float z_offset;
extern float scale_size;

extern int  obj_mode;

extern bool show_bbox;
extern bool show_normals;
extern bool show_texImg;
extern bool show_axis;

extern bool drawTextures;
extern bool drawVertexColor;
extern bool genTextureCoords;

extern Object oObject;


void drawObject(bool useVertexColors);
void drawBoundingBox(void);
void drawAxis();
void calcPlaneMapping(void);
void calcCylindricalMapping(void);
void calcSphereMapping(void);

void initTexture(void);

void disp(void);
void reset();


#endif
