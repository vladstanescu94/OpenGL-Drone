#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLfloat ctlpoints1[4][4][3];
GLfloat ctlpoints2[4][4][3];

GLUnurbsObj *theNurb1;
GLUnurbsObj *theNurb2;
GLUquadric* qobj;

int bladeAngle = 90;
int bladeSpeed = 0;
static int tilt = 0, velocity = 0;

void CALLBACK IdleFunction(void);
void CALLBACK tiltLeft(void);
void CALLBACK tiltRight(void);
void CALLBACK droneUp(void);
void CALLBACK droneDown(void);

void CALLBACK tiltLeft(void)
{
	tilt = (tilt + 5) % 360;
}

void CALLBACK tiltRight(void)
{
	tilt = (tilt - 5) % 360;
}

void CALLBACK droneUp(void)
{
	if(bladeSpeed >0)
	velocity = (velocity + 1) % 360;
}

void CALLBACK droneDown(void)
{
	if (bladeSpeed >0)
	velocity = (velocity - 1) % 360;
}



void CALLBACK IdleFunction(void)
{

	if (GetKeyState(VK_SPACE)) {
		bladeSpeed++;
	}
	else
	{
		bladeSpeed = 0;
	}
	bladeAngle = (bladeAngle +bladeSpeed)%360;
	display();
}

void init_surface1(void)
{
	int u, v;
	for (u = 0; u < 4; u++) {
		for (v = 0; v < 4; v++) {
			ctlpoints1[u][v][0] = 2.0*((GLfloat)u - 1.5);
			ctlpoints1[u][v][1] = 2.0*((GLfloat)v - 1.5);

			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints1[u][v][2] = 1.5;
			else
				ctlpoints1[u][v][2] = -1.0;
		}
	}
}

void init_surface2(void)
{
	int u, v;
	for (u = 0; u < 4; u++) {
		for (v = 0; v < 4; v++) {
			ctlpoints2[u][v][0] = 2.0*((GLfloat)u - 1.5);
			ctlpoints2[u][v][1] = 2.0*((GLfloat)v - 1.5);

			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints2[u][v][2] = 1.5;
			else
				ctlpoints2[u][v][2] = -1.0;
		}
	}
}

/*  Se initializeaza proprietatile materialului, ale sursei de lumina
 *  si ale modelului de iluminare
 */
void myinit(void)
{	// coeficientii de reflexie pentru reflexia ambientala 
	// si cea difuza sunt cei impliciti
    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
/*  rflectanta speculara si exponentul de reflexie speculara 
	nu sunt la valorile implicite (0.0)   */
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
// valori implicite pentru intensitatea sursei LIGHT0
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
/*  pozitia sursei nu are valori implicite */
    GLfloat light_position[] = { 3.0, 3.0, 3.0, 0.0 };

    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);

	

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING); // activare iluminare
    glEnable(GL_LIGHT0);	// activare sursa 0

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	init_surface1();
	init_surface2();

	theNurb1 = gluNewNurbsRenderer(); // crearea obiectului suprafata spline
	gluNurbsProperty(theNurb1, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb1, GLU_DISPLAY_MODE, GLU_FILL);

	theNurb2 = gluNewNurbsRenderer(); // crearea obiectului suprafata spline
	gluNurbsProperty(theNurb2, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb2, GLU_DISPLAY_MODE, GLU_FILL);
}

void CALLBACK display(void)
{
	// punctele de control
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(tilt, 0, 0, 1);
	//glRotatef(-30, 1, 0, 0);
	glTranslatef(0, velocity,0 );

	//Drone Arms
	glPushMatrix();
	glTranslatef(-1,1,-3);
	glRotatef(-65, 1., 0., 0.);
	gluCylinder(qobj, .1, .1, .75, 10,1);
	glTranslatef(0,0,0.75);
	glRotatef(bladeAngle, 0., 0., 1);
	gluPartialDisk(qobj, 0.1, .3, 360, 1, 0, 90);
	glRotatef(-bladeAngle, 0., 0., 1);
	glTranslatef(0, 0, -0.75);
	glTranslatef(2, 0, 0);
	gluCylinder(qobj, .1, .1, .75, 10, 1);
	glTranslatef(0, 0, 0.75);
	glRotatef(bladeAngle, 0., 0., 1);
	gluPartialDisk(qobj, 0.1, .3, 360, 1, 0, 90);
	glRotatef(-bladeAngle, 0., 0., 1);
	glTranslatef(0, 0, -0.75);
	glTranslatef(0, -2.5, 0);
	gluCylinder(qobj, .1, .1, .75, 10, 1);
	glTranslatef(0, 0, 0.75);
	glRotatef(bladeAngle, 0., 0., 1);
	gluPartialDisk(qobj, 0.1, .3, 360, 1, 0, 90);
	glRotatef(-bladeAngle, 0., 0., 1);
	glTranslatef(0, 0, -0.75);
	glTranslatef(-2, 0, 0);
	gluCylinder(qobj, .1, .1, .75, 10, 1);
	glTranslatef(0, 0, 0.75);
	glRotatef(bladeAngle, 0., 0., 1);
	gluPartialDisk(qobj, 0.1, .3, 360, 1, 0, 90);
	glRotatef(-bladeAngle, 0., 0., 1);
	glTranslatef(0, 0, -0.75);
	glPopMatrix();

	////Drone Body
	glPushMatrix();
	glRotatef(-65, 1., 0., 0.);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0,4,0);

	gluBeginSurface(theNurb1);
	gluNurbsSurface(theNurb1,
		8, knots,
		8, knots,
		4 * 3,
		3,
		&ctlpoints1[0][0][0],
		4, 4,
		GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb1);

	glRotatef(180, 0., 1., 0.);
	glTranslatef(0, 0, 2);

	gluBeginSurface(theNurb2);
	gluNurbsSurface(theNurb2,
		8, knots,
		8, knots,
		4 * 3,
		3,
		&ctlpoints2[0][0][0],
		4, 4,
		GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb2);

	glPopMatrix();//end Drone Body

	glPopMatrix();
	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("ProiectGrafica");
    myinit();
	auxIdleFunc(IdleFunction);
	auxKeyFunc(AUX_LEFT, tiltLeft);
	auxKeyFunc(AUX_RIGHT, tiltRight);
	auxKeyFunc(AUX_UP, droneUp);
	auxKeyFunc(AUX_DOWN, droneDown);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
	gluDeleteQuadric(qobj);
    return(0);
}
