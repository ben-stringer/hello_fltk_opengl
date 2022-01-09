
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

static int  press_x;
static int  press_y; 
static int  xform_mode = TRANSFORM_NONE; 
static bool bDoScale = false;

static GLColor4 black = {0.f, 0.f, 0.f, 0.f};

void drawObject(bool useVertexColors)
{
	for(unsigned int i = 0; i < oObject.faces.size(); i++)
	{
		Vertex &v1 = oObject.vertices[oObject.faces[i].v1];
		Vertex &v2 = oObject.vertices[oObject.faces[i].v2];
		Vertex &v3 = oObject.vertices[oObject.faces[i].v3];

		glBegin(GL_TRIANGLES);
		{
			if(drawTextures == true)
			{
				glTexCoord2f(v1.t[S], v1.t[T]);
			}

			if(useVertexColors == true)
			{
				//cout << v1.c[R] << ", " << v1.c[G] << ", " << v1.c[B] << endl;
				glColor3f(v1.c[R], v1.c[G], v1.c[B]);
			}

			glNormal3f(v1.n[X], v1.n[Y], v1.n[Z]);
			glVertex3f(v1.v[X], v1.v[Y], v1.v[Z]);

			if(drawTextures == true)
			{
				glTexCoord2f(v2.t[S], v2.t[T]);
			}

			if(useVertexColors == true)
			{
				glColor3f(v2.c[R], v2.c[G], v2.c[B]);
			}

			glNormal3f(v2.n[X], v2.n[Y], v2.n[Z]);
			glVertex3f(v2.v[X], v2.v[Y], v2.v[Z]);

			if(drawTextures == true)
			{
				glTexCoord2f(v3.t[S], v3.t[T]);
			}

			if(useVertexColors == true)
			{
				glColor3f(v3.c[R], v3.c[G], v3.c[B]);
			}

			glNormal3f(v3.n[X], v3.n[Y], v3.n[Z]);
			glVertex3f(v3.v[X], v3.v[Y], v3.v[Z]);
		}
		glEnd();
	}
}

void drawBoundingBox(void)
{
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_LINE_LOOP);				
	{
		glVertex3f(oObject.bBox.minP[X], 
				   oObject.bBox.minP[Y], 
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.maxP[X], 
				   oObject.bBox.minP[Y], 
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.maxP[X],  
				   oObject.bBox.maxP[Y], 
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.minP[X],  
				   oObject.bBox.maxP[Y], 
				   oObject.bBox.minP[Z]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);		
	{
		glVertex3f(oObject.bBox.minP[X], 
				   oObject.bBox.minP[Y], 
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.maxP[X],
				   oObject.bBox.minP[Y], 
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.maxP[X],  
				   oObject.bBox.maxP[Y], 
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.minP[X], 
				   oObject.bBox.maxP[Y], 
				   oObject.bBox.maxP[Z]);
	}
	glEnd();

	glBegin(GL_LINES);
	{
		glVertex3f(oObject.bBox.minP[X], 
				   oObject.bBox.minP[Y], 
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.minP[X],
				   oObject.bBox.minP[Y], 
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.maxP[X], 
				   oObject.bBox.minP[Y],
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.maxP[X],
				   oObject.bBox.minP[Y],
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.maxP[X],  
				   oObject.bBox.maxP[Y], 
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.maxP[X],  
				   oObject.bBox.maxP[Y],  
				   oObject.bBox.maxP[Z]);

		glVertex3f(oObject.bBox.minP[X],
				   oObject.bBox.maxP[Y],
				   oObject.bBox.minP[Z]);

		glVertex3f(oObject.bBox.minP[X],
				   oObject.bBox.maxP[Y],  
				   oObject.bBox.maxP[Z]);
	}
	glEnd();

} // drawBoundingBox

void drawAxis() {
	glBegin(GL_LINES);
	glColor3f(1.f,0.f,0.f);
	glVertex3f(-10,0,0);
	glVertex3f(10,0,0);
	glColor3f(0.f,1.f,0.f);
	glVertex3f(0,-10,0);
	glVertex3f(0,10,0);
	glColor3f(0.f,0.f,1.f);
	glVertex3f(0,0,-10);
	glVertex3f(0,0,10);
	//Draw a unit vector
	glColor3f(1.f,1.f,0.f);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glEnd();
}

void scaleToUnitBox(void)
{
	Point maxP;
	Point minP;

	minP[X] = minP[Y] = minP[Z] = -1e35;
	minP[X] = minP[Y] = minP[Z] =  1e35;

	Vector vBoxSize;
	Point  bboxCenterP;

	for(unsigned int i = 0; i < oObject.vertices.size(); i++)
	{
		Vertex &v = oObject.vertices[i];

		if(v.v[X] < minP[X])
			minP[X] = v.v[X];

		if(v.v[Y] < minP[Y])
			minP[Y] = v.v[Y];

		if(v.v[Z] < minP[Z])
			minP[Z] = v.v[Z];


		if(v.v[X] > maxP[X])
			maxP[X] = v.v[X];

		if(v.v[Y] > maxP[Y])
			maxP[Y] = v.v[Y];

		if(v.v[Z] > maxP[Z])
			maxP[Z] = v.v[Z];
	}

	subPnt(vBoxSize, maxP, minP);

	addScaled(bboxCenterP, minP, vBoxSize, 0.5);

	fprintf(stderr, "original bbox : %f %f %f -> %f %f %f\n",
			minP[X],
			minP[Y],
			minP[Z],
			maxP[X],
			maxP[Y],
			maxP[Z]);

	fprintf(stderr, "original bbox : %f %f %f | %f %f %f\n",
			bboxCenterP[X],
			bboxCenterP[Y],
			bboxCenterP[Z],
			vBoxSize[X],
			vBoxSize[Y],
			vBoxSize[Z]);

	float modelScale = 1.f / maxComponent3(vBoxSize);

	fprintf(stderr, "%f %f\n", maxComponent3(vBoxSize), modelScale);

	for(unsigned int i = 0; i < oObject.vertices.size(); i++)
	{
		Vertex &v = oObject.vertices[i];

		v.v[X] = (v.v[X] - bboxCenterP[X]) * modelScale;
		v.v[Y] = (v.v[Y] - bboxCenterP[Y]) * modelScale;
		v.v[Z] = (v.v[Z] - bboxCenterP[Z]) * modelScale;
	}	

	oObject.bBox.minP[X] = (minP[X] - bboxCenterP[X]) * modelScale;
	oObject.bBox.minP[Y] = (minP[Y] - bboxCenterP[Y]) * modelScale;
	oObject.bBox.minP[Z] = (minP[Z] - bboxCenterP[Z]) * modelScale;

	oObject.bBox.maxP[X] = (maxP[X] - bboxCenterP[X]) * modelScale;
	oObject.bBox.maxP[Y] = (maxP[Y] - bboxCenterP[Y]) * modelScale;
	oObject.bBox.maxP[Z] = (maxP[Z] - bboxCenterP[Z]) * modelScale;

	oObject.bBox.vSize[X] = vBoxSize[X] * modelScale;
	oObject.bBox.vSize[Y] = vBoxSize[Y] * modelScale;
	oObject.bBox.vSize[Z] = vBoxSize[Z] * modelScale;
}

bool readFile(const char *szFilename)
{
	FILE   *pFile;
	char	cType;
	int	 rc;

	Vertex  tmpVertex;
	Face	tmpFace;	

	int vid;
	int fid;

	tmpVertex.v[3] = 1.f;
	tmpVertex.n[3] = 0.f;
	
	tmpVertex.t[S] = 0.f;
	tmpVertex.t[T] = 0.f;

	tmpVertex.c[R] = 1.f;
	tmpVertex.c[G] = 0.f;
	tmpVertex.c[B] = 0.f;

	pFile = fopen(szFilename, "r");

	if(pFile != NULL)
	{
		cType = fgetc(pFile);

		while(cType != EOF)
		{
			switch(cType)
			{
				case 'V' : 
				{
					rc = fscanf(pFile, 
								"ertex %d %f %f %f {normal=(%f %f %f)}\n", 
								&vid, 
								&tmpVertex.v[X], 
								&tmpVertex.v[Y], 
								&tmpVertex.v[Z], 
								&tmpVertex.n[X], 
								&tmpVertex.n[Y], 
								&tmpVertex.n[Z]);

					if(rc == 7)
					{
						oObject.vertices.push_back(tmpVertex);
					}
					break;
				}

				case 'F' :
				{
					rc = fscanf(pFile, "ace %d %d %d %d\n", 
								&fid, 
								&tmpFace.v1, 
								&tmpFace.v2, 
								&tmpFace.v3);
					if(rc == 4)
					{
						//store this face
						--tmpFace.v1;
						--tmpFace.v2;
						--tmpFace.v3;

						oObject.faces.push_back(tmpFace);
					}
				}
				break;

				case '#' : //this is a comment
				{
					do //read till end of the line
					{
						cType = fgetc(pFile);
					} while(cType != EOF && cType != '\n');

					break;
				}
			}

			cType = fgetc(pFile);
		}
		
		return true;
	}
	else
	{
		fprintf(stderr, "could not read file %s\n", szFilename);
		return false;
	}
}

void mymouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) 
	{
		press_x = x; press_y = y; 

		if (button == GLUT_LEFT_BUTTON)
		{
			xform_mode = TRANSFORM_ROTATE; 
		}
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			xform_mode = TRANSFORM_SCALE; 
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			xform_mode = TRANSFORM_TRANSLATE; 
		}

	}
	else if (state == GLUT_UP) 
	{
		xform_mode = TRANSFORM_NONE; 
	}
}

void mymotion(int x, int y)
{
	if(xform_mode == TRANSFORM_ROTATE) 
	{
		x_angle += (x - press_x)/5.0; 

		if(x_angle > 180) 
		{
			x_angle -= 360; 
		}
		else if(x_angle <-180) 
		{
			x_angle += 360; 
		}

		press_x = x; 
	   
		y_angle += (y - press_y)/5.0; 

		if(y_angle > 180) 
		{
			y_angle -= 360; 
		}
		else if(y_angle <-180) 
		{
			y_angle += 360; 
		}
	  
		press_y = y; 
	}
	else if(xform_mode == TRANSFORM_SCALE)
	{
		if(bDoScale == true)
		{
			float old_size = scale_size;
		
			scale_size *= (1 + (y - press_y)/60.0); 

			if(scale_size <0) 
			{
				scale_size = old_size; 
			}
		}
		else
		{
			z_offset -= (y - press_y) / 90.0;
		}

		press_y = y; 
	}
	else if(xform_mode == TRANSFORM_TRANSLATE)
	{
		x_offset += (x - press_x) / 90.0;
		y_offset -= (y - press_y) / 90.0;
		press_x = x;
		press_y = y;
	}

	// force the redraw function
	glutPostRedisplay(); 
}

void mykey(unsigned char key, int x, int y)
{
	cout << "Key " << key << " is pressed.  ";
	switch(key) 
	{
		case 'w': 
			obj_mode = OBJ_WIREFRAME;
			cout << "Draw the object in wireframe.";
			break; 
		case 's':
			obj_mode = OBJ_SOLID;
			cout << "Draw the object in solid.";
			break;
		case 'e':
			obj_mode = OBJ_EDGE;
			cout << "Draw the object in solid+wireframe.";
			break;

		case 'B':
		case 'b':
			show_bbox = !show_bbox;
			cout << "Showing bounding box: " << (show_bbox ? "yes" : "no");
			break;

		case 'N':
		case 'n':
			show_normals = !show_normals;
			cout << "Showing normals: " << (show_normals ? "yes" : "no");
			break;

		case 'T':
		case 't':
			show_texImg = !show_texImg;
			cout << "Showing texImg: " << (show_texImg ? "yes" : "no");
			break;

		case '0':
			drawTextures	 = false;
			genTextureCoords = false;
			cout << "Showing untextured model";
			break;

		case '1':
			drawTextures	 = true;
			genTextureCoords = false;
			calcPlaneMapping();
			cout << "Showing textured model, plane mapping";
			break;

		case '2':
			drawTextures	 = true;
			genTextureCoords = false;
			calcCylindricalMapping();
			cout << "Showing textured model, cylindrical mapping";
			break;

		case '3':
			drawTextures	 = true;
			genTextureCoords = false;
			calcSphereMapping();
			cout << "Showing textured model, spherical mapping";
			break;

		case 'g':
		case 'G':
			drawTextures	 = true;
			genTextureCoords = true;
			cout << "Showing textured model, OpenGL Generated texture coordinates";
			break;

		case 'L':
		case 'l':
			drawVertexColor = !drawVertexColor;
			cout << "Simulating light: " << (drawVertexColor == 0 ? "off" : "on");
			break;

		case 'M':
		case 'm':
			bDoScale = !bDoScale;
			cout << "bDoScale is " << bDoScale;
			break;

		case 'X':
		case 'x':
			show_axis = !show_axis;
			cout << "show_axis is " << show_axis;
			break;
		case '+':
		case '=':
			scale_size += 0.1;
			break;
		case '_':
		case '-':
			scale_size -= 0.1;
			break;
		case 'r':
		case 'R':
			reset();
			break;
		case '[':
			x_angle -= 0.1;
			break;
		case ']':
			x_angle += 0.1;
			break;
		case '/':
			y_angle -= 0.1;
			break;
		case ';':
			y_angle += 0.1;
			break;
		case 27:	
			cout << "Exiting";
			exit(0);
	}
	cout << endl;

	// force the redraw function
	glutPostRedisplay(); 
}


int main(int argc, char **argv)
{
	static int win;

	if(argc > 1)
	{
		//read m file
		if(readFile(argv[1]) == false)
		{
			fprintf(stderr, "error loading file\n");
			return 0;
		}

		scaleToUnitBox();
	}
	else
	{
		if(readFile("./eight.m") == false)
		{
			fprintf(stderr, "model filename missing\n");
			return 0;
		}

		scaleToUnitBox();
	}


	// normal initialisation
	glutInit(&argc, argv);
	// use double buffer to get better results on animation
	// use depth buffer for hidden surface removal
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	//glutInitWindowSize	(500,500);
	//glutInitWindowPosition(100,100);
	glutInitWindowSize	(1000,1000);
	glutInitWindowPosition(600,1560);

	win = glutCreateWindow("Assignment2 Transformation");
  
	// set the clearcolor and the callback
	glClearColor(0.0,0.0,0.0,0.0);

	// register your callback functions
	glutDisplayFunc (disp	);
	glutMouseFunc   (mymouse );
	glutMotionFunc  (mymotion);
	glutKeyboardFunc(mykey   );
	
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_NORMALIZE); 
	
	glLineWidth(2); 

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	initTexture();

	glBindTexture(GL_TEXTURE_2D, 0);
	// enter the main loop
	glutMainLoop();

	return 0;
}
