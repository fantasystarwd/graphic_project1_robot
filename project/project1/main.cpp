#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../freeglut/include/GL/glut.h"
#include "imageloader.h"
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
float bodya, bodyx, bodyy, bodyz;											//body
float alla, allx, ally, allz;												//all rotate

float headxd, headyd, headzd;											//move the head
float leftHandxd, leftHandyd, leftHandzd;								//move left hand
float rightHandxd, rightHandyd, rightHandzd;							//move right hand
float leftLegxd, leftLegyd, leftLegzd;									//move leftleg
float rightLegxd, rightLegyd, rightLegzd;								//move right leg
float bodyxd, bodyyd, bodyzd,bodyry,bodyrz;								//move the body r for rotate
float cannonyd;															//move cannon
float bulletyd[3];														//shoot bullet
float destructy[2],destructz[2];										//move when destruct
float anklex;															//move for transform
float limby[2],limbz[2];										//move for transform		
float bodyw;															//move for transform

bool singleColortest;
int colorR, colorG, colorB;

int old_rot_x = 0;
int old_rot_y = 0;

int rot_x = 0;
int rot_y = 0;

int record_x = 0;
int record_y = 0;
int state = 1;
int step = 0;
//decide which part of action
float headside = 2.5;													
//length of head                  
float bodyside = 5;															
//length of body
float limbside = 3;															
//length of hand and leg
static int WinWidth = 600;
static int WinHeight = 600;
enum { CYLINDER = 100 ,SPHERE};													//used for calllist
enum { StandBy = 0, Walk, Jump, ChangeColor, Shoot, Destruct, Transform };	//the action name
enum Color { Black, White, Red, Green, Blue ,Turn};
Color colorChoose;
int motivation = 0;	//decide which action
GLint _textureId[2];
const float FLOOR_SIZE = 20.0f; //The length of each side of the floor


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width, image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}

void DrawBackGround()
{
	glPushMatrix();
	//glTranslatef(0,0,-40);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-10 ,-10, -3);
	glTexCoord2f(1, 0);
	glVertex3f(10, -10, -3);
	glTexCoord2f(1, 1);
	glVertex3f(10, 10, -3);
	glTexCoord2f(0, 1);
	glVertex3f(-10, 10, -3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void head()
{
	glTranslated(headxd, headyd, headzd);
	glTranslated(0, destructy[0], destructz[0]);
	glTranslated(0, 1.45*destructy[1], 1.45*destructz[1]);
	glRotated(bodya, bodyx, bodyy, bodyz);
	if (!singleColortest)
		glColor3f(0, 1, 0);
	else
		glColor3ub(colorR, colorG, colorB);

	glPushMatrix();
	glTranslated(0, 0, limbz[0]);
	glutSolidCube(headside);
	glPopMatrix();

	glTranslatef(0, headside*0.75, 0);
	glScalef(1.2, 0.5, 1.2);
	glutSolidCube(headside);
}
void leftHand()
{
	glTranslated(leftHandxd, leftHandyd, leftLegzd);
	glTranslatef((bodyside*0.125) + (bodyside / 2) - 0.3, (bodyside / 2) * 3 / 5, 0.0);
	glRotatef(leftHanda[0], leftHandx[0], leftHandy[0], leftHandz[0]);
	glPushMatrix();														//first ankle
	glTranslated(1+anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	
	glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0);

	glPushMatrix();														//upper lefthand
	glScalef(0.8, 2, 1);

	if (!singleColortest)
		glColor3ub(255, 177, 98);
	else
		glColor3ub(colorR, colorG, colorB);

	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(leftHanda[1], leftHandx[1], leftHandy[1], leftHandz[1]);

	glPushMatrix();														//second ankle
	glTranslated(1+anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower lefthand
	glTranslated(0, limby[1], 0);
	glScalef(0.9, 2, 1.1);

	if (!singleColortest)
		glColor3ub(89, 197, 255);
	else
		glColor3ub(colorR, colorG, colorB);

	glutSolidCube(limbside);
	glPopMatrix();
}
void rightHand()
{
	glTranslated(rightHandxd, rightHandyd, rightLegzd);
	
	glTranslatef(0.3 - (bodyside / 2) - (bodyside*0.125), (bodyside / 2) * 3 / 5, 0.0);
	glRotatef(rightHanda[0], rightHandx[0], rightHandy[0], rightHandz[0]);
	glPushMatrix();														//first ankle
	glRotatef(180, 0, 1, 0);
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	
	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0);

	glPushMatrix();														//upper righthand
	glScalef(0.8, 2, 1);
	if (!singleColortest)
		glColor3ub(255, 177, 98);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(rightHanda[1], rightHandx[1], rightHandy[1], rightHandz[1]);

	glPushMatrix();														//second ankle
	glRotatef(180, 0, 1, 0);
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower righthand
	glTranslated(0, limby[1], 0);
	glScalef(0.9, 2, 1.1);
	if (!singleColortest)
		glColor3ub(89, 197, 255);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();
}
void leftLeg()
{
	glTranslated(leftLegxd, leftLegyd, leftLegzd);
	glTranslated(0, destructy[0], destructz[0]);
	glTranslated(0, 0.55*destructy[1], 0.55*destructz[1]);
	glTranslatef(0, 1 - bodyside, 0);

	glPushMatrix();														//first ankle
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glRotatef(leftLega[0], leftLegx[0], leftLegy[0], leftLegz[0]);
	glTranslatef(0.3 + limbside*0.8 / 2, -1 - limbside / 2, 0);
	glTranslated(0, limby[0], 0);
	glPushMatrix();														//upper leftleg
	
	glScalef(0.8, 2, 1);
	if (!singleColortest)
		glColor3ub(255, 177, 98);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(leftLega[1], leftLegx[1], leftLegy[1], leftLegz[1]);

	glPushMatrix();														//second ankle
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower leftleg
	glTranslated(0, limby[1], 0);
	glScalef(0.9, 2, 1.1);
	if (!singleColortest)
		glColor3ub(89, 197, 255);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();
}
void rightLeg()
{
	glTranslated(rightLegxd, rightLegyd, rightLegzd);
	glTranslated(0, destructy[0], destructz[0]);
	glTranslatef(0, 1 - bodyside, 0);
	

	glPushMatrix();														//first ankle
	glRotatef(180, 0, 1, 0);
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glRotatef(rightLega[0], rightLegx[0], rightLegy[0], rightLegz[0]);
	glTranslatef(-0.3 - limbside*0.8 / 2, -1 - limbside / 2, 0);
	glTranslated(0, limby[0], 0);
	glPushMatrix();														//upper rightleg
	
	glScalef(0.8, 2, 1);
	if (!singleColortest)
		glColor3ub(255, 177, 98);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();

	glTranslatef(0.3 + limbside*0.8 / 2, -limbside * 3 / 4, 0);
	glRotatef(rightLega[1], rightLegx[1], rightLegy[1], rightLegz[1]);

	glPushMatrix();														//second ankle
	glRotatef(180, 0, 1, 0);
	glTranslated(1 + anklex, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(CYLINDER);
	glPopMatrix();

	glTranslatef(-0.3 - limbside*0.8 / 2, -limbside / 2, 0.0);

	glPushMatrix();														//lower rightleg
	glTranslated(0, limby[1], 0);
	glScalef(0.9, 2, 1.1);
	if (!singleColortest)
		glColor3ub(89, 197, 255);
	else
		glColor3ub(colorR, colorG, colorB);
	glutSolidCube(limbside);
	glPopMatrix();
}
void body()
{
	glTranslated(bodyxd, bodyyd, bodyzd);
	glRotated(alla, allx, ally, allz);
	glPushMatrix();
	if (!singleColortest)
		glColor3f(1, 0, 0);
	else
		glColor3ub(colorR, colorG, colorB);

	glTranslated(0, destructy[0], destructz[0]);
	glTranslated(0, destructy[1], destructz[1]);
	glRotated(bodya, bodyx, bodyy, bodyz);
	glPushMatrix();
	glScalef(1, 1, 0.5);         //top part
	glutSolidCube(bodyside);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(bodyside*0.625, bodyside / 4, 0);
	glScalef(0.25, 0.5, 0.5);
	glutSolidCube(bodyside);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-bodyside*0.625, bodyside / 4, 0);
	glScalef(0.25, 0.5, 0.5);
	glutSolidCube(bodyside);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -3.75, 0);
	glScalef(bodyw, 0.5, 0.5);
	glutSolidCube(bodyside);
	glPopMatrix();

	glPopMatrix();
}
void cannon(){
	glPushMatrix();
	glTranslated(0,-cannonyd,0);
	glRotated(90, 1, 0, 0);
	glCallList(CYLINDER);
	glPopMatrix();
}
void bullet(float bullety){
	glPushMatrix();
	glTranslated(0,bullety,0);
	if (bullety <= 0)
	{
		glCallList(SPHERE);
	}
	glPopMatrix();
}
void standby() {
	state = 1;
	step = 0;

	singleColortest = false;
	colorR = colorG = colorB = 0;
	colorChoose = Black;

	anklex= 0;

	bodyw = 0.2;

	limby[0] = 0;
	limby[1] = 0;
	limbz[0] = 0;
	limbz[1] = 0;

	alla = 0;
	allx = 0;
	ally = 0;
	allz = 0;

	bodya = 0;
	bodyx = 0;
	bodyy = 0;
	bodyz = 0;
	bodyxd = 0;
	bodyyd = 0;
	bodyzd = 0;
	bodyry = 0;
	bodyrz = 0;

	leftHandxd = 0;
	leftHandyd = 0;
	leftHandzd = 0;
	leftHanda[0] = 0;
	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
	leftHanda[1] = 0;
	leftHandx[1] = 1;
	leftHandy[1] = 0;
	leftHandz[1] = 0;

	rightHandxd = 0;
	rightHandyd = 0;
	rightHandzd = 0;
	rightHanda[0] = 0;
	rightHandx[0] = 1;
	rightHandy[0] = 0;
	rightHandz[0] = 0;
	rightHanda[1] = 0;
	rightHandx[1] = 0;
	rightHandy[1] = 0;
	rightHandz[1] = 0;

	leftLegxd = 0;
	leftLegyd = 0;
	leftLegzd = 0;
	leftLega[0] = 0;
	leftLegx[0] = 1;
	leftLegy[0] = 0;
	leftLegz[0] = 0;
	leftLega[1] = 0;
	leftLegx[1] = 1;
	leftLegy[1] = 0;
	leftLegz[1] = 0;

	rightLegxd = 0;
	rightLegyd = 0;
	rightLegzd = 0;
	rightLega[0] = 0;
	rightLegx[0] = 1;
	rightLegy[0] = 0;
	rightLegz[0] = 0;
	rightLega[1] = 0;
	rightLegx[1] = 0;
	rightLegy[1] = 0;
	rightLegz[1] = 0;

	cannonyd = 0;
	bulletyd[0] = 0;
	bulletyd[1] = 0;
	bulletyd[2] = 0;
	destructy[0] = 0;
	destructz[0] = 0;
	destructy[1] = 0;
	destructz[1] = 0;
}
void walk()
{
	switch (step)
	{
	case 0:
		rightHanda[1]--;
		leftHanda[1]--;
		rightLega[0]--;
		//rightLega[0] turn to -15 degree
		rightLega[1] += 2;
		//rightLega[1] turn to 30 degree
		if (rightLega[0] <= -15)
		{
			rightLega[0] = -15;
			step = 1;
		}
		break;
	case 1:
		rightHanda[0]--;
		leftHanda[0]++;
		bodyyd = rightLega[0] * 1.5 / 30;
		rightLega[0]--;
		//rightLega[0] turn to -30 degree
		leftLega[0]++;
		//leftLega[0] turn to 15 degree
		if (rightLega[0] <= -30)
		{
			rightLega[0] = -30;
			step = 2;
		}
		break;
	case 2:
		rightHanda[0]+=0.5;
		leftHanda[0]-=0.5;
		bodyyd = rightLega[0] * 1.5 / 30;
		rightLega[0]++;
		//rightLega[0] turn to 0 degree
		rightLega[1]--;
		//rightLega[1] turn to 0 degree
		leftLega[0] += 0.5;
		//leftLega[0] turn to 30 degree
		leftLega[1] += 0.5;
		//leftLega[1] turn to 15 degree
		if (rightLega[0] >= 0)
		{
			rightLega[0] = 0;
			step = 3;
		}
		break;
	case 3:
		rightHanda[0]+=0.5;
		leftHanda[0]-=0.5;
		bodyyd = leftLega[0] * 1.5 / 30;
		rightLega[0] += 0.25;
		//rightLega[0] turn to 15 degree
		leftLega[0]--;
		//leftLega[0] turn to -30 degree
		leftLega[1] += 0.25;
		//leftLega[1] turn to 30 degree
		if (leftLega[0] <= -30)
		{
			leftLega[0] = -30;
			step = 4;
		}
		break;
	case 4:
		rightHanda[0]-=0.5;
		leftHanda[0]+=0.5;
		bodyyd = leftLega[0] * 1.5 / 30;
		rightLega[0] += 0.5;
		//rightLega[1] turn to 30 degree
		rightLega[1] += 0.5;
		//rightLega[1] turn to 15 degree
		leftLega[0] ++;
		//leftLega[0] turn to 0 degree
		leftLega[1] --;
		//leftLega[1] turn to 0 degree
		if (rightLega[0] >= 30)
		{
			rightLega[0] = 30;
			step = 5;
		}
		break;
	case 5:
		rightHanda[0] -= 0.5;
		leftHanda[0] += 0.5;
		bodyyd = rightLega[0] * 1.5 / 30;
		rightLega[0]--;
		//rightLega[0] turn to -30 degree
		rightLega[1] += 0.25;
		//rightLega[1] turn to 30 degree
		leftLega[0] += 0.25;
		//leftLega[0] turn to 15 degree
		if (rightLega[0] <= -30)
		{
			rightLega[0] = -30;
			step = 2;
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
		rightLega[1] += 2;
		leftLega[0]--;
		leftLega[1] += 2;
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

		rightLega[0] += 3;
		rightLega[1] -= 6;
		leftLega[0] += 3;
		leftLega[1] -= 6;
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
//finish
void changeColor()
{
	singleColortest = true;
	switch (colorChoose)
	{
	case White:
		colorR = colorG = colorB = 255;
		break;
	case Black:
		colorR = colorG = colorB = 0;
		break;
	case Red:
		colorR = 255;
		colorG = colorB = 0;
		break;
	case Green:
		colorG = 255;
		colorR = colorB = 0;
		break;
	case Blue:
		colorB = 255;
		colorR = colorG = 0;
		break;
	case Turn:
		switch (step)
		{
		case 0:
			colorR++;
			if (colorR >= 255)
			{
				step = 1;
			}
			break;
		case 1:
			colorG++;
			if (colorG >= 255)
			{
				step = 2;
			}
			break;
		case 2:
			colorB++;
			if (colorB >= 255)
			{
				step = 3;
			}
			break;
		case 3:
			colorR--;
			if (colorR <= 0)
			{
				step = 4;
			}
			break;
		case 4:
			colorG--;
			if (colorG <= 0)
			{
				step = 5;
			}
			break;
		case 5:
			colorB--;
			if (colorB <= 0)
			{
				step = 0;
			}
			break;
		}
		break;
	}
}
void shoot()
{
	if (rightHanda[1] > -90)
	{
		rightHanda[1]--;
		leftHanda[1]--;
		cannonyd += 0.01;
	}
	else
	{
		bulletyd[0]--;
		bulletyd[1]--;
		bulletyd[2]--;
		if(bulletyd[0]<-100)
		  {
			bulletyd[0] = 0;
		  }
		if(bulletyd[1]<-100)
		  {
			bulletyd[1] = 10;
		  }
		if (bulletyd[2] < -100)
		{
			bulletyd[2] = 20;
		}
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
	if(rightHandyd> - bodyside*13/10 -limbside * 7 / 4)
	{
		rightHandyd-=0.1;
		rightHandxd -= 0.08;
		rightHanda[0] += 0.72;
	}
	rightHandx[0] = 0;
	rightHandy[0] = 0;
	rightHandz[0] = 1;
	if (leftHandyd > -bodyside*13/10)
	{
		leftHandyd -= 0.1;
	}
	if (destructz[0]<6)
	{
		destructy[0] -= 3.0 / 50;
		destructz[0]+=6.0/45;
		leftLega[1]+=2;
		rightLega[1]+=2;
	}
	else if(bodyrz<90)
	{
		destructy[1] = (-bodyside - limbside * 7 / 4+1.5)*sin(bodyry*3.14159 / 180);
		destructz[1] = - (-bodyside - limbside * 7 / 4 + 1.5)*sin(bodyrz*3.14159/180);
		bodya += 2;
		leftLega[0] += 2;
		leftLega[1] -= 2;
		bodyry+=2;
		bodyrz+=2;
	}
	leftLegx[0] = 1;
	leftLegy[0] = 0;
	leftLegz[0] = 0;
	leftLegx[1] = 1;
	leftLegy[1] = 0;
	leftLegz[1] = 0;
	rightLegx[1] = 1;
	rightLegy[1] = 0;
	rightLegz[1] = 0;
	bodyx = 1;
	bodyy = 0;
	bodyz = 0;
}
void transform()
{
	if (anklex >-0.3)
	{
		anklex -= 0.01;
	}
	else if (alla < 90)
	{
		alla+=3;
		limby[0] += 0.025;
		limby[1] += 0.1;
		leftHanda[0] -= 3;
		rightHanda[0] -= 3;
		leftLega[0] -= 3;
		rightLega[0] -= 3;
	}
	else if(headyd>-2.5)
	{
		headyd -= 0.05;
		bodyw += 0.016;
		leftLegxd += 0.04;
		rightLegxd -= 0.04;
	}
	else if (limbz[0] > -2.5)
	{
		limbz[0] -= 0.05;
	}
	allx = 1;
	ally = 0;
	allz = 0;
	leftHandx[0] = 1;
	leftHandy[0] = 0;
	leftHandz[0] = 0;
	rightHandx[0] = 1;
	rightHandy[0] = 0;
	rightHandz[0] = 0;
}
void test()
{
	if (leftHanda[0] >= -90)
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
	if (motivation == Shoot)
	{
		cannon();
		if(rightHanda[1]<=-90)
		{
			bullet(bulletyd[0]);
			bullet(bulletyd[1]);
			bullet(bulletyd[2]);
		}
	}
	glPopMatrix();

	glPushMatrix();																		//right hand
	rightHand();
	if (motivation == Shoot)
	{
		cannon();
		if (rightHanda[1] <= -90)
		{
			bullet(bulletyd[0]);
			bullet(bulletyd[1]);
			bullet(bulletyd[2]);
		}
	}
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
	gluPerspective(45.0f, 1.0f, 1.0f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 10, 30, 0, 0, 0, 0, 1, 0);
	DrawBackGround();
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

	Image* image = loadBMP("org1.bmp");
	_textureId[0] = loadTexture(image);
	delete image;

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
	standby();
	glutPostRedisplay();
}

void menuColor(int selection)
{
	motivation = ChangeColor;
	colorChoose = (Color)selection;
	glutPostRedisplay();
}

void timerFunction(int value)
{
	switch (motivation)
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
	int main_menu, action_menu, count_menu, color_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//glutInitWindowSize(1200,600);	//< Dual view
	glutInitWindowSize(WinWidth, WinHeight);	//< Single view

	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	init();

	color_menu = glutCreateMenu(menuColor);
	glutAddMenuEntry("White", White);
	glutAddMenuEntry("Black", Black);
	glutAddMenuEntry("Red", Red);
	glutAddMenuEntry("Green", Green);
	glutAddMenuEntry("Blue", Blue);
	glutAddMenuEntry("Turn", Turn);

	glutCreateMenu(menu);
	glutAddMenuEntry("Stand By", StandBy);
	glutAddMenuEntry("Walk", Walk);
	glutAddMenuEntry("Jump", Jump);
	glutAddSubMenu("Change Color",color_menu);
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
	gluCylinder(Ankle, 0.5, 0.5, 2, 15, 15);
	gluDeleteQuadric(Ankle);
	glEndList();
	glNewList(SPHERE, GL_COMPILE);
	GLUquadric *BULLET = gluNewQuadric();
	glColor3ub(0, 0, 0);
	glutSolidSphere(0.5, 12, 12);
	gluDeleteQuadric(BULLET);
	glEndList();
	glutTimerFunc(15, timerFunction, 1);
	glutMainLoop();
	return 0;

}