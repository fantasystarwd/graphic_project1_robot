#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../freeglut/include/GL/glut.h"
#include<iostream>
using namespace std;
static int draw_mode = 1;
static int action = 0;
static GLuint inner_mode = GL_SMOOTH;
static GLuint tex_state = GL_TRUE;
static int state_chang;

float heada, headx, heady, headz;											//neck
float leftHanda[2], leftHandx[2], leftHandy[2], leftHandz[2];				//left hand
float rightHanda[2], rightHandx[2], rightHandy[2], rightHandz[2];			//right hand
float leftLega[2], leftLegx[2], leftLegy[2], leftLegz[2];					//left leg
float rightLega[2], rightLegx[2], rightLegy[2], rightLegz[2];				//right leg
float headxd, headyd, headzd;											//move the head
float leftHandxd, leftHandyd, leftHandzd;								//move left hand
float rightHandxd, rightHandyd, rightHandzd;							//move right hand
float leftLegxd, leftLegyd, leftLegzd;									//move leftleg
float rightLegxd, rightLegyd, rightLegzd;								//move right leg
float bodyxd, bodyyd, bodyzd;											//move the body
int old_rot_x = 0;
int old_rot_y = 0;

int rot_x = 0;
int rot_y = 0;

int record_x = 0;
int record_y = 0;
int state = 1;
int step = 0;//decide which part of action
float headside = 2.5;														//length of head                  
float bodyside = 5;															//length of body
float limbside = 3;															//length of hand and leg
static int WinWidth = 600;
static int WinHeight = 600;
enum {CYLINDER=100};														//used for calllist
enum {StandBy=0,Walk,Jump,ChangeColor,Shoot,Destruct,Transform};			//the action name
int motivation = 0;															//decide which action
void head()
{
	glTranslated(headxd, headyd, headzd);
	glColor3f(0, 1, 0);
	glutSolidCube(headside);
	glTranslatef(0,headside*0.75,0);
	glScalef(1.2, 0.5, 1.2);
	glutSolidCube(headside);
}
void leftHand()
{
		glTranslated(leftHandxd, leftHandyd, leftLegzd);
    	glTranslatef((bodyside*0.125) + (bodyside / 2) - 0.3, (bodyside / 2) * 3 / 5, 0.0);

		glPushMatrix();														//first ankle
		glRotatef(90, 0, 1, 0);
		glCallList(CYLINDER);
		glPopMatrix();
		
		glRotatef(leftHanda[0], leftHandx[0], leftHandy[0], leftHandz[0]);
		glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0);
		
		glPushMatrix();														//upper lefthand
		glScalef(0.8, 2, 1);
		glColor3ub(255, 177, 98);
		glutSolidCube(limbside);
		glPopMatrix();

		glTranslatef(-0.3 - limbside*0.8 / 2, -limbside * 3 / 4, 0);
		glRotatef(leftHanda[1], leftHandx[1], leftHandy[1], leftHandz[1]);

		glPushMatrix();														//second ankle
		glRotatef(90, 0, 1, 0);
		glCallList(CYLINDER);
		glPopMatrix();

		glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0.0);

		glPushMatrix();														//lower lefthand
		glScalef(0.9, 2, 1.1);
		glColor3ub(89, 197, 255);
		glutSolidCube(limbside);
		glPopMatrix();
}
void rightHand()
{
	glTranslated(rightHandxd, rightHandyd, rightLegzd);
	glTranslatef(0.3 - (bodyside / 2) - (bodyside*0.125), (bodyside / 2) * 3 / 5, 0.0);

	glPushMatrix();														//first ankle
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glRotatef(rightHanda[0], rightHandx[0], rightHandy[0], rightHandz[0]);
	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0);

	glPushMatrix();														//upper righthand
	glScalef(0.8, 2, 1);
	glColor3ub(255, 177, 98);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(rightHanda[1], rightHandx[1], rightHandy[1], rightHandz[1]);

	glPushMatrix();														//second ankle
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower righthand
	glScalef(0.9, 2, 1.1);
	glColor3ub(89, 197, 255);
	glutSolidCube(limbside);
	glPopMatrix();
}
void leftLeg()
{
	glTranslated(leftLegxd, leftLegyd, leftLegzd);
	glTranslatef(0, 1 - bodyside, 0);

	glPushMatrix();														//first ankle
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glRotatef(leftLega[0], leftLegx[0], leftLegy[0], leftLegz[0]);
	glTranslatef(0.3 + limbside*0.8 / 2, -1 - limbside / 2, 0);

	glPushMatrix();														//upper leftleg
	glScalef(0.8, 2, 1);
	glColor3ub(255, 177, 98);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(leftLega[1], leftLegx[1], leftLegy[1], leftLegz[1]);

	glPushMatrix();														//second ankle
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower leftleg
	glScalef(0.9, 2, 1.1);
	glColor3ub(89, 197, 255);
	glutSolidCube(limbside);
	glPopMatrix();
}
void rightLeg()
{
	glTranslated(rightLegxd,rightLegyd,rightLegzd);
	glTranslatef(0, 1 - bodyside, 0);

	glPushMatrix();														//first ankle
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glRotatef(rightLega[0], rightLegx[0], rightLegy[0], rightLegz[0]);
	glTranslatef(-0.3 - limbside*0.8 / 2, -1 - limbside / 2, 0);

	glPushMatrix();														//upper rightleg
	glScalef(0.8, 2, 1);
	glColor3ub(255, 177, 98);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(rightLega[1], rightLegx[1], rightLegy[1], rightLegz[1]);

	glPushMatrix();														//second ankle
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower rightleg
	glScalef(0.9, 2, 1.1);
	glColor3ub(89, 197, 255);
	glutSolidCube(limbside);
	glPopMatrix();
}
void body()
    {
	 glTranslated(bodyxd, bodyyd, bodyzd);
	 glPushMatrix();
	 glScalef(1, 1, 0.5);         //top part
	 glColor3f(1,0,0);
	 glutSolidCube(bodyside);
	 glPopMatrix();
	 glPushMatrix();
	 glTranslatef(bodyside*0.625,bodyside/4,0);
	 glScalef(0.25, 0.5, 0.5);
	 glColor3f(1, 0, 0);
	 glutSolidCube(bodyside);
	 glPopMatrix();
	 glPushMatrix();
	 glTranslatef(-bodyside*0.625, bodyside / 4, 0);
	 glScalef(0.25, 0.5, 0.5);
	 glColor3f(1, 0, 0);
	 glutSolidCube(bodyside);
	 glPopMatrix();
	 glPushMatrix();
	 glTranslatef(0, -3.75, 0);
	 glScalef(0.2, 0.5, 0.5);
	 glColor3f(1, 0, 0);
	 glutSolidCube(bodyside);
	 glPopMatrix();
    }
void standby() {
	state = 1;
	step = 0;
	bodyxd = 0;
	bodyyd = 0;
	bodyzd = 0;

	leftHanda[0] = 0;
	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
	leftHanda[1] = 0;
	leftHandx[1] = 1;
	leftHandy[1] = 0;
	leftHandz[1] = 0;
	
	rightHanda[0] = 0;
	rightHandx[0] = 1;
	rightHandy[0] = 0;
	rightHandz[0] = 0;
	rightHanda[1] = 0;
	rightHandx[1] = 0;
	rightHandy[1] = 0;
	rightHandz[1] = 0;

	leftLega[0] = 0;
	leftLegx[0] = 1;
	leftLegy[0] = 0;
	leftLegz[0] = 0;
	leftLega[1] = 0;
	leftLegx[1] = 1;
	leftLegy[1] = 0;
	leftLegz[1] = 0;

	rightLega[0] = 0;
	rightLegx[0] = 1;
	rightLegy[0] = 0;
	rightLegz[0] = 0;
	rightLega[1] = 0;
	rightLegx[1] = 0;
	rightLegy[1] = 0;
	rightLegz[1] = 0;
}
void walk()
{
	
	for (int i = 0; i < 30; i++)
	{
		if (rightHanda[1] > -30)
		{
			rightHanda[0]--;
			rightHanda[1]--;
			leftHanda[0]++;
			leftHanda[1]--;
		}
	}
	switch(step)
	{
	    case 0:
			bodyyd = -rightLega[0] * (2 - sqrt(3 / 2)) / 30;
			rightHanda[0]+=2;
			leftHanda[0]-=2;
			rightLega[0]++;
			rightLega[1]=rightLega[0];
			leftLega[0]-=2;
			leftLega[1]=-leftLega[0];
			if(rightLega[0]>=30)
			  {
				step = 1;
			  }
			break;
		case 1:
			bodyyd = -rightLega[0] * (2 - sqrt(3 / 2)) / 30;
			rightHanda[0]-=2;
			leftHanda[0]+=2;
			rightLega[0]--;
			rightLega[1] = rightLega[0] ;
			leftLega[0] += 2;
			leftLega[1] =-leftLega[0];
			if (rightLega[0] == 0)
			{
				step = 2;
			}
			break;
		case 2:
			bodyyd = -leftLega[0] * (2 - sqrt(3 / 2)) / 30;
			rightHanda[0]+=2;
			leftHanda[0]-=2;
			leftLega[0]++;
			leftLega[1] = leftLega[0];
			rightLega[0] -= 2;
			rightLega[1] =-rightLega[0];
			if (leftLega[0] >= 30)
			{
				step = 3;
			}
			break;
		case 3:
			bodyyd = -leftLega[0] * (2 - sqrt(3 / 2)) / 30;
			rightHanda[0]-=2;
			leftHanda[0]+=2;
			leftLega[0]--;
			leftLega[1] = leftLega[0];
			rightLega[0] += 2;
			rightLega[1] =-rightLega[0];
			if (leftLega[0] == 0)
			{
				step = 0;
			}
			break;
	}
	rightHandx[0] = 1;
	rightHandy[0] = 0;
	rightHandz[0] = 0;
	rightHandx[1] = 1;
	rightHandy[1] = 0;
	rightHandz[1] = 0;

	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
	leftHandx[1] = 1;
	leftHandy[1] = 0;
	leftHandz[1] = 0;

	rightLegx[0] = 1;
	rightLegy[0] = 0;
	rightLegz[0] = 0;
	rightLegx[1] = 1;
	rightLegy[1] = 0;
	rightLegz[1] = 0;

	leftLegx[0] = 1;
	leftLegy[0] = 0;
	leftLegz[0] = 0;
	leftLegx[1] = 1;
	leftLegy[1] = 0;
	leftLegz[1] = 0;
}
//finish
void jump()
{
	
	switch (step)
	{
	case 0:
		bodyyd = rightLega[0] * (2 - sqrt(2)) / 45;
		rightLega[0]--;
		rightLega[1]+=2;
		leftLega[0]--;
		leftLega[1]+=2;
		if (rightLega[0] <= -45)
		{
			step = 1;
		}
		break;
	case 1:
		bodyyd += 2;

		rightLega[0] += 3;
		rightLega[1] -= 6;
		leftLega[0] += 3;
		leftLega[1] -= 6;
		if (rightLega[0] == 0)
		{

			step = 2;
		}
		break;
	case 2:
		bodyyd -= 1;
		if (bodyyd <= 0)
		{
			bodyyd = 0;
			step = 3;
		}
		break;
	case 3:
		bodyyd = rightLega[0] * (2 - sqrt(2)) / 45;
		rightLega[0]--;
		rightLega[1] += 2;
		leftLega[0]--;
		leftLega[1] += 2;
		if (rightLega[0] <= -45)
		{
			step = 4;
		}
		break;
	case 4:
		bodyyd = rightLega[0] * (2 - sqrt(2)) / 45;
	
		rightLega[0]+=3;
		rightLega[1]-=6;
		leftLega[0]+=3;
		leftLega[1]-=6;
		if (rightLega[0] == 0)
		{
			motivation = 0;
			step = 0;
		}
		break;
	}
	rightHandx[0] = 1;
	rightHandy[0] = 0;
	rightHandz[0] = 0;
	rightHandx[1] = 1;
	rightHandy[1] = 0;
	rightHandz[1] = 0;

	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
	leftHandx[1] = 1;
	leftHandy[1] = 0;
	leftHandz[1] = 0;

	rightLegx[0] = 1;
	rightLegy[0] = 0;
	rightLegz[0] = 0;
	rightLegx[1] = 1;
	rightLegy[1] = 0;
	rightLegz[1] = 0;

	leftLegx[0] = 1;
	leftLegy[0] = 0;
	leftLegz[0] = 0;
	leftLegx[1] = 1;
	leftLegy[1] = 0;
	leftLegz[1] = 0;
}
void changeColor()
{
	
}
void shoot()
{
	if(rightHanda[1]>-90)
	{
		rightHanda[1]--;
		leftHanda[1]--;
	}
	rightHandx[1] = 1;
	rightHandy[1] = 0;
	rightHandz[1] = 0;

	leftHandx[1] = 1;
	leftHandy[1] = 0;
	leftHandz[1] = 0;
}
void destruct()
{

}
void transform()
{
	
}
void test()
{
	if (leftHanda[0] >=-90)
	{
		leftHanda[0]--;
	}
	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
}
void draw_robot(void)
{
	body();																				//draw body
	glPushMatrix();
	glTranslatef(0, bodyside / 2 + headside / 2, 0);									//draw head
	head();
	glPopMatrix();
	
	glPushMatrix();																		//left hand
	leftHand();
	glPopMatrix();
	
	glPushMatrix();																		//right hand
	rightHand();
	glPopMatrix();

	glPushMatrix();                                          							//left leg
	leftLeg();
	glPopMatrix();

	glPushMatrix();														                //right leg
	rightLeg();
	glPopMatrix();

}
void draw()
{
	glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);
	glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);
	draw_robot();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(inner_mode);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f,1.0f,500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
	draw();
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	WinWidth = w;
	WinHeight = h;
	GLfloat aspect;
	aspect = (GLfloat)w / (GLfloat)h;
	//glViewport(0,0,(GLsizei)w,(GLsizei)h);
	if (aspect > 1.0f)
		glViewport((w - h) / 2.0f, 0, (GLsizei)h, (GLsizei)h);
	else
		glViewport(0, (h - w) / 2.0f, (GLsizei)w, (GLsizei)w);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 1.0f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
}
void init(void)
{
	GLfloat  ambientLight[] = { 1, 1, 1, 0.0f };
	GLfloat  diffuseLight[] = { 0, 0, 0, 1.0f };
	GLfloat  specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat  ambientLight2[] = { 0.9f, 0.1f, 0.1f, 1.0f };
	GLfloat  specular2[] = { 0.3f, 0.3f, 0.3f, 1.3f };
	GLfloat  specref[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat  pos[] = { 0.0f, 20.0f, -100.0f, 1.0f };
	GLfloat  dir[] = { 0.0f, 0.0f, -1.0f };

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ambientLight2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}
void Mouse(int button, int state, int x, int y)
{
	if (state)
	{
		record_x += x - old_rot_x;
		record_y += y - old_rot_y;

		rot_x = 0;
		rot_y = 0;
	}
	else
	{
		old_rot_x = x;
		old_rot_y = y;
	}
}

void MotionMouse(int x, int y)
{
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
}

void menu(int selection)
{
	motivation = selection;
	step = 0;
	standby();
	glutPostRedisplay();
}
void timerFunction(int value)
{
	switch(motivation)
	{
	case 0:
		standby();
		break;
	case 1:
		walk();
		break;
	case 2:
		jump();
		break;
	case 3:
		changeColor();
		break;
	case 4:
		shoot();
		break;
	case 5:
		destruct();
		break;
	case 6:
		transform();
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(15, timerFunction, 1);
}
int main(int argc, char** argv)
{
	int main_menu, action_menu, count_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//glutInitWindowSize(1200,600);	//< Dual view
	glutInitWindowSize(WinWidth, WinHeight);	//< Single view

	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	init();
	glutCreateMenu(menu);
	glutAddMenuEntry("Stand By", StandBy);
	glutAddMenuEntry("Walk", Walk);
	glutAddMenuEntry("Jump", Jump);
	glutAddMenuEntry("Change Color", ChangeColor);
	glutAddMenuEntry("Shoot", Shoot);
	glutAddMenuEntry("Destruct", Destruct);
	glutAddMenuEntry("Transform", Transform);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	standby();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(Mouse);
	glutMotionFunc(MotionMouse);
	glNewList(CYLINDER, GL_COMPILE);
	GLUquadric *Ankle = gluNewQuadric();
	glColor3ub(0, 0, 0);
	gluCylinder(Ankle, 0.5, 0.5, 3, 15, 15);
	gluDeleteQuadric(Ankle);
	glEndList();
	glutTimerFunc(15, timerFunction, 1);
	glutMainLoop();
	return 0;

}