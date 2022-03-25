#include "glos.h"

#include <gl.h>
#include <glu.h>
#include <glaux.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK idleFunction(void);

// Camera
GLfloat glOrthoSize = 400;

// Rotation
GLfloat alfa = 100;
GLfloat beta = 100;
GLfloat rotateSpeed = 3;

// Wall Properties
GLfloat wallThickness = 40;
GLfloat wallHeight = 400;

// Idle function
GLint animationSpeed = 40;
GLint angleLimitCorp = 30;
GLfloat corp = 0.0f;
GLfloat bat = 0.0f;
GLfloat corpSpeed = 1.0f;
GLfloat batSpeed = 0.366f; // 11 grade
GLboolean isBellGoingRight = 1;

// Material Properties
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.3, 0.3, 0.3, 1.0 };//coeficient de reflexie ambientala
GLfloat mat_diffuseBell[] = { 0.96, 0.79, 0.09, 1.0 };//coeficient de reflexie difuza
GLfloat mat_diffuseWall[] = { 0.58, 0.54, 0.53, 1.0 };//coeficient de reflexie difuza
GLfloat mat_diffuseRoof[] = { 0.8, 0.23, 0.16, 1.0 };//coeficient de reflexie difuza
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };//coeficient de reflexie speculara
GLfloat no_shininess[] = { 0.0 };//exponentul de reflexie speculara
GLfloat low_shininess[] = { 5.0 };//exponentul de reflexie speculara
GLfloat high_shininess[] = { 100.0 };//exponentul de reflexie speculara
//GLfloat mat_emission[] = { 0.1, 0.1, 0.1, 0.0 };//intensitatea luminii emise
GLfloat mat_emission[] = { 0.05, 0.05, 0.05, 0.0 };

// Light Properties
GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat diffuse[] = { 1.0, 1.0, 1, 1.0 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat position[] = { 100.0, 50.0, 90.0, 0.0 };
GLfloat position[] = { -10, 0, 10, 0.0 };

// General
GLUquadric* qobj;
GLuint IDtextura;

// Textures
GLuint roof, walls, bell, background;

void apply_texture(const wchar_t* s)
{

	AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);


	if (pImagineTextura != NULL)
	{
		glGenTextures(1, &IDtextura);

		glBindTexture(GL_TEXTURE_2D, IDtextura);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}

	if (pImagineTextura)
	{
		if (pImagineTextura->data)
			free(pImagineTextura->data);

		free(pImagineTextura);
	}

}


void CALLBACK  RotesteDreapta() {
	beta = 0;
	beta += rotateSpeed;
}

void CALLBACK RotesteStanga() {
	beta = 0;
	beta -= rotateSpeed;
}

void CALLBACK RotesteSus() {
	alfa = 0;
	alfa += rotateSpeed;
}

void CALLBACK RotesteJos() {
	alfa = 0;
	alfa -= rotateSpeed;
}

void CALLBACK idleFunction(void)
{
	if ((corp != angleLimitCorp && isBellGoingRight) || (corp != -angleLimitCorp && !isBellGoingRight)) {
		if (isBellGoingRight) {
			corp += corpSpeed;
			bat -= batSpeed;
		}
		else {
			corp -= corpSpeed;
			bat += batSpeed;
		}
	}
	else {
		isBellGoingRight = !isBellGoingRight;
	}

	alfa = 0;
	beta = 0;
	display();
	Sleep(animationSpeed);
}

void myinit() {
	glDepthFunc(GL_LESS);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//mode for texture application
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_LIGHTING); // activate light
	glEnable(GL_LIGHT0); // activate 1-st light source
	glEnable(GL_TEXTURE_2D);

	wchar_t* sir;

	sir = L"roof.bmp";
	apply_texture(sir);
	roof = IDtextura;

	sir = L"wall.bmp";
	apply_texture(sir);
	walls = IDtextura;

	sir = L"bell.bmp";
	apply_texture(sir);
	bell = IDtextura;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	qobj = gluNewQuadric();
}

void CALLBACK display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//background
	glTranslatef(-alfa * 10, -beta * 10, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-300, -500.0, -500.0);
		glTexCoord2f(0.0, 3.0); glVertex3f(-300, -500.0, 500.0);
		glTexCoord2f(3.0, 3.0); glVertex3f(-300, 500.0, 500.0);
		glTexCoord2f(3.0, 0.0); glVertex3f(-300, 500.0, -500.0);
	glEnd();

	glTranslatef(alfa * 10, beta * 10, 0);

	// Back left column
	/*glPushMatrix();
		glTranslatef(-alfa * 10, -beta * 10, -(wallHeight / 2));
		gluCylinder(qobj, 100, 100, 500, 4, 4);
	glPopMatrix();*/

	glRotatef(alfa, 1, 0, 0);
	glRotatef(beta, 0, 0, 1);

	gluQuadricDrawStyle(qobj, GLU_FILL);
	glEnable(GL_DEPTH_TEST);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseRoof);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	gluQuadricTexture(qobj, TRUE);

	glBindTexture(GL_TEXTURE_2D, roof);
	// Roof
	glPushMatrix();
		glTranslatef(0, 0, -(wallHeight / 2) - 100);
		glRotatef(45, 0, 0, 1);
		gluCylinder(qobj, 1, 350, 100, 4, 4);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseWall);
	glBindTexture(GL_TEXTURE_2D, walls);
	// Back left column
	glPushMatrix();
		glTranslatef(-150, 150, -(wallHeight / 2));
		glRotatef(45, 0, 0, 1);
		gluCylinder(qobj, wallThickness, wallThickness, wallHeight, 4, 4);
	glPopMatrix();

	// Back right column
	glPushMatrix();
		glTranslatef(-150, -150, -(wallHeight / 2));
		glRotatef(45, 0, 0, 1);
		gluCylinder(qobj, wallThickness, wallThickness, wallHeight, 4, 4);
	glPopMatrix();

	// Front left column
	glPushMatrix();
		glTranslatef(150, 150, -(wallHeight / 2));
		glRotatef(45, 0, 0, 1);
		gluCylinder(qobj, wallThickness, wallThickness, wallHeight, 4, 4);

		/*glTranslatef(-30, -35, 70);
		gluSphere(qobj, 15, 25, 25);*/
	glPopMatrix();

	// Front right column
	glPushMatrix();
		glTranslatef(150, -150, -(wallHeight / 2));
		glRotatef(45, 0, 0, 1);
		gluCylinder(qobj, wallThickness, wallThickness, wallHeight, 4, 4);
	glPopMatrix();

	// Floor
	glPushMatrix();
		glTranslatef(0, 0, 200);
		auxSolidBox(400, 400, 20);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseBell);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	glBindTexture(GL_TEXTURE_2D, bell);
	// Clopotul
	glPushMatrix();
		glRotatef(corp, 1, 0, 0);

		// Corpul clopotului
		gluCylinder(qobj, 90, 90, 100, 20, 40);

		// Partea de sus a corpului
		gluSphere(qobj, 89.6, 25, 25);

		// Baza Clopotului
		glTranslatef(0, 0, 75);
		gluCylinder(qobj, 90, 110, 40, 20, 40);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseWall);

	// Partea, care bate clopot
	glPushMatrix();
		glRotatef(bat, 1, 0, 0);

		gluCylinder(qobj, 10, 10, 150, 20, 40);
	glPopMatrix();

	auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	//if (!h) return;
	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(60, 1, 150, 0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(0, 0, 550, 0, -40, 0, 0, 0.2, 0);
	//glTranslatef(0.0, 0.0, -20.0);
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-glOrthoSize * (GLfloat)w / (GLfloat)h,
		glOrthoSize * (GLfloat)w / (GLfloat)h, -glOrthoSize, glOrthoSize, -glOrthoSize, glOrthoSize);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);

	auxInitPosition(250, 250, 500, 500);
	auxInitWindow("Clopot");

	myinit();

	auxKeyFunc(AUX_LEFT, RotesteStanga);
	auxKeyFunc(AUX_RIGHT, RotesteDreapta);
	auxKeyFunc(AUX_UP, RotesteSus);
	auxKeyFunc(AUX_DOWN, RotesteJos);

	auxIdleFunc(idleFunction);
	auxReshapeFunc(myReshape);
	auxMainLoop(display);

	return 0;
}