/* 
*/

#include <iostream>
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <cstdlib>
#include <math.h>

#include "assignment2.h"

using namespace std;

float x_angle	= 0.0; 
float y_angle	= 0.0; 
float x_offset   = 0.0;
float y_offset   = 0.0;
float z_offset   = 0.0;

float scale_size = 5; 

int  obj_mode   = OBJ_SOLID;

bool show_bbox	= false;
bool show_normals = false;
bool show_texImg  = false;
bool show_axis	  = false;

bool drawTextures	 = false;
bool drawVertexColor  = false;
bool genTextureCoords = false;

	   Object oObject;
static GLuint texObj;
static GLubyte texArray[6][6][4];


static Point  lightPos			   (0.f, 0.f, 5.f, 1.f);
static Point  camPos			   (0.f, 0.f, 5.f, 1.f);

static GLColor4 lightColorAmbient  = {0.1, 0.1, 0.1, 1.f};
static GLColor4 lightColorDiffuse  = {0.6, 0.6, 0.6, 1.f};
static GLColor4 lightColorSpecular = {0.8, 0.0, 0.0, 1.f};

static float	constantAtt		= 1.f;
static float	linearAtt		  = 0.4f;
static float	quadraticAtt	   = 0.0f;

static GLColor4 matColorAmbient  = {0.9, 0.9, 0.9, 1.f};
static GLColor4 matColorDiffuse  = {0.9, 0.9, 0.9, 1.f};
static GLColor4 matColorSpecular = {0.9, 0.9, 0.9, 1.f};

static float  matShininess	   = 60; 

void reset()
{
	x_angle	= 0.0; 
	y_angle	= 0.0; 
	x_offset   = 0.0;
	y_offset   = 0.0;
	z_offset   = 0.0;
	
	scale_size = 5; 
	obj_mode   = OBJ_SOLID;

	show_bbox	= false;
	show_normals = false;
	show_texImg  = false;
	show_axis	  = false;

	drawTextures	 = false;
	drawVertexColor  = false;
	genTextureCoords = false;
}

/**
 * Undo the camera transformations to the model view so that I can
 * operate on the verticies independent of the camera.
 * Then use the simplified phong lighting model.
 */
void updateVertexColor(Matrix mCam, Matrix mModelView)
{
	// Invert the camera
	Matrix mCamInv;
	invert(mCamInv, mCam);

	// Create a matrix of just the object transformations
	Matrix transform;
	mult(transform, mModelView, mCamInv);

	/* calculate per vertex lighting for a single point light  */
	/* loop over all vertices and update oObject.vertices[i].c */
	for (int i = 0; i < oObject.vertices.size(); i++)
	{
		// Get and transform the vertex's point and norm
		Vertex &v = oObject.vertices[i];
		Point p(v.v);
		Vector n(v.n);
		mult(p,transform,p);
		mult(n,transform,n);
		normalize(n);

		// Create vectors to the light source and eye respectively.
		// (In this case, they're the same, but let's be general)
		// Get the length of l before normalizing it.
		Vector l;
		Vector e;
		subPnt(l,lightPos,p);
		subPnt(e,camPos,p);
		float d = length(l);
		normalize(l);
		normalize(e);

		// Calculate and normalize the halfway vector
		Vector h;
		add(h,l,e);
		normalize(h);

		// Calculate attenuation
		float att = 1 / 
				(constantAtt + linearAtt*d + quadraticAtt*pow(d,2));
		// Calculate the dot product between the vertex's norm and
		// the light and halfway vectors respectively
		float nl = max(0.0f, dot(n, l));
		float nh = max(0.0f, dot(n, h));

		// Calculate and assign the color
		Color c;
		for (int i = R; i <= B; i++) {
			c[i] = att * (
					matColorAmbient[i]*lightColorAmbient[i] +
					matColorDiffuse[i]*lightColorDiffuse[i]*nl +
					matColorSpecular[i]*lightColorSpecular[i]
							*pow(nh,matShininess)
					);
		}
		v.c = c;
	}
}

void calcPlaneMapping(void)
{
	/* calculate plane mapping								 */
	float tx = oObject.bBox.minP[X];
	float ty = oObject.bBox.minP[Y];
	float dx = oObject.bBox.maxP[X] - tx;
	float dy = oObject.bBox.maxP[Y] - ty;

	/* loop over all vertices and update oObject.vertices[i].t */
	for (int i = 0; i < oObject.vertices.size(); i++) {
		Vertex &v = oObject.vertices[i];
		v.t[S] = (v.v[X] - tx) / dx;
		v.t[T] = (v.v[Y] - ty) / dy;
	}
}

void calcCylindricalMapping(void)
{
	/* calculate cylindrical mapping						   */
	float ty = oObject.bBox.minP[Y];
	float dy = oObject.bBox.maxP[Y] - ty;
	/* loop over all vertices and update oObject.vertices[i].t */
	for (int i = 0; i < oObject.vertices.size(); i++) {
		Vertex &v = oObject.vertices[i];
		Point p = v.v;

		v.t[S] = (atan2(p[Z],p[X]) + PI) / (2*PI);
		v.t[T] = (p[Y] - ty) / dy;
	}

}

void calcSphereMapping(void)
{
	/* calculate spherical mapping							 */
	/* loop over all vertices and update oObject.vertices[i].t */
	for (int i = 0; i < oObject.vertices.size(); i++) {
		Vertex &v = oObject.vertices[i];
		Point p = v.v;

		Vector r(p);

		v.t[S] = (acos(p[Y]/length(r)))/PI;
		v.t[T] = (atan2(p[Z],p[X]) + PI) / (2*PI);
	}
}

void drawNormals(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	/* fill in normal rendering here */
	/* loop over all faces and draw the normals */
	for(unsigned int i = 0; i < oObject.faces.size(); i++)
	{
		Vertex &v1 = oObject.vertices[oObject.faces[i].v1];
		Vertex &v2 = oObject.vertices[oObject.faces[i].v2];
		Vertex &v3 = oObject.vertices[oObject.faces[i].v3];

		Vector a(v3.v[X]-v2.v[X],v3.v[Y]-v2.v[Y],v3.v[Z]-v2.v[Z],1);
		Vector b(v1.v[X]-v2.v[X],v1.v[Y]-v2.v[Y],v1.v[Z]-v2.v[Z],1);
		Point c( (v1.v[X] + v2.v[X] + v3.v[X]) / 3,
					(v1.v[Y] + v2.v[Y] + v3.v[Y]) / 3,
					(v1.v[Z] + v2.v[Z] + v3.v[Z]) / 3,
					1);
		 
		Vector n;
		cross(n,a,b);
		normalize(n);
		Point cn;
		addScaled(cn, c, n, 0.01);

		glBegin(GL_LINES);
		glVertex4fv(c.gldata());
		glVertex4fv(cn.gldata());
		glEnd();
	}	 
}


void initTexture(void)
{
	/* create texture image, texture environment and download the texture */
	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);

	// Hard code a single row
	GLubyte oneRow[6][4] = {
		{ 128, 0, 255, 255 },
		{ 0, 0, 255, 255 },
		{ 0, 255, 255, 255 },
		{ 0, 255, 0, 255 },
		{ 255, 255, 0, 255 },
		{ 255, 0, 0, 255}
	};

	// Then copy to the remaining rows, staggering by one
	int i = 1;
	for (int row = 0; row < 6; row++) {
		i = (i + 7) % 6;
		for (int col = 0; col < 6; col++) {
			i = (i + 1) % 6;
			texArray[row][col][0] = oneRow[i][0];
			texArray[row][col][1] = oneRow[i][1];
			texArray[row][col][2] = oneRow[i][2];
			texArray[row][col][3] = oneRow[i][3];
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 6, 6, 0, GL_RGBA, GL_UNSIGNED_BYTE, texArray);
} //initTexture

void setupTexture(void)
{
	/* enable texturing and bind texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texObj);
}

void finishTexture(void)
{
	/* disable texturing and unbind texture */
	glDisable(GL_TEXTURE_2D);
}

void setupTexGen(void)
{
	/* setup and enable texture coordinate generation */
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

void finishTexGen(void)
{
	/* disable texture coordinate generation */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void disp(void)
{
	// Just clean the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// setup the perspective projection
	glMatrixMode  (GL_PROJECTION ); 
	glLoadIdentity(			  ); 
	gluPerspective(60, 1, .1, 100); 

	glMatrixMode  (GL_MODELVIEW); 
	glLoadIdentity(			); 

	gluLookAt(camPos[X], 
			  camPos[Y],
			  camPos[Z], 
			  0, 0, 0,
			  0, 1, 0); 

	Matrix mCam;

	if(drawVertexColor == true)
	{
		glGetFloatv(GL_MODELVIEW_MATRIX, mCam);
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos.gldata());

	glLightfv(GL_LIGHT0, GL_AMBIENT,			   lightColorAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE,			   lightColorDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR,			  lightColorSpecular);

	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION,  constantAtt );
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION,	linearAtt   );
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAtt);

	if(show_texImg)
	{
		setupTexture();

		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0,0.0); glVertex2d(-0.5,-0.5);
			glTexCoord2d(1.0,0.0); glVertex2d( 0.5,-0.5);
			glTexCoord2d(1.0,1.0); glVertex2d( 0.5, 0.5);
			glTexCoord2d(0.0,1.0); glVertex2d(-0.5, 0.5);
		}
		glEnd();

		finishTexture();
	}

	// rotate and scale the object
	glTranslatef(x_offset, y_offset, z_offset);
	glRotatef   (x_angle,  0, 1, 0); 
	glRotatef   (y_angle,  1, 0, 0); 
	glScalef	(scale_size, scale_size, scale_size); 

	if (show_axis)
	{
		drawAxis();
	}

	if(show_bbox == true)
	{
		drawBoundingBox();
	}

	if(show_normals == true)
	{
		drawNormals();
	}

	if(drawVertexColor == false)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
	{
		Matrix mModelView;
		
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelView);

		updateVertexColor(mCam, mModelView);
	}

	if(drawTextures == true)
	{	
		setupTexture();

		if(genTextureCoords == true)
		{
			setupTexGen();
		}
	}

	if(obj_mode == OBJ_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(obj_mode == OBJ_SOLID || obj_mode == OBJ_EDGE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   matColorAmbient );
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   matColorDiffuse );
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  matColorSpecular);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess	);

	drawObject(drawVertexColor);

	if(obj_mode == OBJ_EDGE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glPolygonOffset(0, 10);
		glEnable(GL_POLYGON_OFFSET_LINE);

		glColor3f(0.0f, 0.0, 1.0f);

		drawObject(false);

		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(0, 0);
	}


	if(drawTextures == true)
	{
		finishTexture();

		if(genTextureCoords == true)
		{
			finishTexGen();
		}
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

  
	// swap the buffers
	glutSwapBuffers(); 
}

