
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void initWindow(void);

float angle = 0.0;
int left, right;
int leftTime, rightTime;
int thrust, thrustTime;
float x, y, xf, yf, xv, yv, v;
int shield = 0, joyShield = 0, cursor = 1;
int lastTime;
int paused = 0;
int resuming = 1;
int originalWindow = 0, currentWindow;


#ifndef maxw
#define maxw 150
#endif

int wx[maxw];
int wy[maxw];
float wd[maxw];
int tx, ty;

int i, j; float l;


int pixel; 
int px, py; 
bool touch=false; 
int h, hf;

bool fullscreen=false, win=false, crt=false;

void *font = GLUT_BITMAP_9_BY_15;

//GLUT_BITMAP_TIMES_ROMAN_24;
//GLUT_BITMAP_9_BY_15;
//GLUT_BITMAP_TIMES_ROMAN_10
//GLUT_BITMAP_HELVETICA_18

void
output(int x, int y, char *string)
{
  int len;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void
drawShip(float angle)
{
  float rad;

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_CULL_FACE);

  glPushMatrix();
  glTranslatef(x, y, 0.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  if (thrust) {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.75, -0.5);
    glVertex2f(-1.75, 0);
    glVertex2f(-0.75, 0.5);
    glEnd();
  }
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(2.0, 0.0);
  glVertex2f(-1.0, -1.0);
  glVertex2f(-0.5, 0.0);
  glVertex2f(-1.0, 1.0);
  glVertex2f(2.0, 0.0);
  glEnd();
  if (shield) {
    glColor3f(0.1, 0.1, 1.0);
    glBegin(GL_LINE_LOOP);
    for (rad=0.0; rad<12.0; rad += 1.0) {
      glVertex2f(2.3 * cos(2*rad/M_PI)+0.2, 2.0 * sin(2*rad/M_PI));
    }
    glEnd();
  }
  glPopMatrix();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

}

void map()
{

//MAP

for (i=0; i<(maxw); i++)
{

glPushMatrix();
 
	glColor3f(0.5, 0.5, 0.5);

  glTranslated(wx[i]*5+5,wy[i]*5+10,0);

//glRotatef(angle*i/100, 0, 0, 1);
  
  glutSolidSphere(wd[i], 8, 5);

	if (((wx[i]*5+5)>x) || ((wy[i]*5+10)>y))
	{
		l=sqrt(((wx[i]*5+5)-x)*((wx[i]*5+5)-x)+((wy[i]*5+10)-y)*((wy[i]*5+10)-y));
	}

	if ((x>(wx[i]*5+5)) || (y>(wy[i]*5+10)))
	{
		l=sqrt((x-(wx[i]*5+5))*(x-(wx[i]*5+5))+(y-(wy[i]*5+10))*(y-(wy[i]*5+10)));
	}

	glColor3f(0.5/l*(wx[i]*5+5)*0.3, 0.5/l*y*0.3, 0.5/l*(wy[i]*5+10)*0.3);

  glutSolidSphere(wd[i]*0.67, 7, 5);

glPopMatrix();

}

glColor3f(1.2, 0.7, 0.8); output(tx*5+4.7,ty*5+10,"\4");


}

void ground()
{

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_CULL_FACE);

//GROUND

glColor3f(0.2, 0.2, 0.2);

for (i=1; i<21; i++)
{

glPushMatrix();

  glTranslated(0,i*5,0);	
  glutSolidSphere(4, 8, 4);

glPopMatrix();

glPushMatrix();

  glTranslated(100,i*5,0);	
  glutSolidSphere(4, 8, 4);

glPopMatrix();

}

for (i=1; i<20; i++)
{

glPushMatrix();

  glTranslated(i*5,100,0);	
  glutSolidSphere(4, 8, 4);

glPopMatrix();

glPushMatrix();

  glTranslated(i*5,5,0);	
  glutSolidSphere(4, 8, 4);

glPopMatrix();

}

glPushMatrix();

glBegin(GL_POLYGON);
  glVertex2f(0, 5);
  glVertex2f(100, 5);
  glVertex2f(100, 0);
  glVertex2f(0, 0);
  glVertex2f(0, 5);
  glEnd();

glPopMatrix();
  
  		glColor3f(0.9/h*15, 0.4*h/5, 0); output(4,3,"Voyager");

  	for (j=0; j<h; j++)
	{
  		glColor3f(0.9/j*15, 0.4*j/5, 0);
		output(15+j*2,3,"+");
	}

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

}

void genmap()
{

for (i=0; i<(maxw); i++)
{

	wd[i] = 0;

}

hf = 15; h = hf;

bool b=false; 

for (i=1; i<(maxw+1); i++)
{
    wx[i]=(int) (rand() % 19);
	wy[i]=(int) (rand() % 18);

    do
	{

	b=false;

	for (j=1; j<i; j++)
		if ((wx[i]==wx[j]) && (wy[i]==wy[j]))
			b=true;

		if (b==true)
	{
		wx[i]=(int) (rand() % 19);
		wy[i]=(int) (rand() % 18);
	}

	} while (b!=false);

	}

    tx=(int) (rand() % 19);
	ty=(int) (rand() % 18);

    do
	{

	b=false;

	for (j=1; j<maxw; j++)
		if ((tx==wx[j]) && (ty==wy[j]))
			b=true;

		if (b==true)
	{
		tx=(int) (rand() % 19);
		ty=(int) (rand() % 18);
	}

	} while (b!=false);

    xf=(int) (rand() % 19);
	yf=(int) (rand() % 18);

    do
	{

	b=false;

	for (j=1; j<maxw; j++)
		if ((xf==wx[j]) && (yf==wy[j]) || (xf==tx) && (yf==ty))
			b=true;

		if (b==true)
	{
		xf=(int) (rand() % 19);
		yf=(int) (rand() % 18);
	}

	} while (b!=false);

	xf = xf * 5 + 5; yf = yf * 5 + 10;

	x = xf; y = yf; xv = 0; yv = 0;

	crt = false;
}

void crtmap()
{

	if (crt==false)
	{
		for (j=0; j<maxw; j++)
			if (wd[j]<3)
			{
				wd[j]+=0.2; 
			}

		if ((int) wd[maxw-1] == 3) crt=true;
	}

}

void
readShip(float x,float y, int time)
{

	if (fullscreen==false)
	{
		px=6*x; 
		py=6.2*y;
	} else {
		px=7.31*(x+20);
		py=7.68*y;
	}

  glReadPixels( px, py, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

  if (pixel<1000000) touch=false;

  if ((pixel>1000000) && (touch==false))
  {
	xv = -xv;
    yv = -yv;

	touch=true; 
	
	if (win==false) h--;

	if (crt==true)
	{

	if (win==false)
	{

	for (i=0; i<(maxw); i++)
	{
		if (wd[i]>0)
		{

	    if ((x>((wx[i]*5+5)-(wd[i]/1.025))) && 
			(x<((wx[i]*5+5)+(wd[i]/1.025))) && 
			(y>((wy[i]*5+10)-(wd[i]/1.025))) && 
			(y<((wy[i]*5+10)+(wd[i]/1.025))))
		{
			wd[i]-=0.1;
		}

		}
	}

	}

	}

  }

}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

		if (crt==true)
		{

			for (i=0; i<(maxw); i++)
			{
				if (wd[i]<3) wd[i]-=0.1;
			}

		} else crtmap();
 
		map();

		ground();

    if (x>(tx*5+3.6) && (x<(tx*5+5.8)) && (y>(ty*5+8.9)) && (y<(ty*5+11.1))) win=true;

	if (win==true)
	{
		glColor3f(1.2, 0.7, 0.8); thrust = 0;

		output(40,50,"LEVEL COMPLITE");
	}
	
	else drawShip(angle);

  glutSwapBuffers();
}


void
idle(void)
{

  int time, delta;

  time = glutGet(GLUT_ELAPSED_TIME);
  if (resuming) {
    lastTime = time;
    resuming = 0;
  }
  if (left) {
    delta = time - leftTime;
    angle = angle + delta * 0.4;
    leftTime = time;
  }
  if (right) {
    delta = time - rightTime;
    angle = angle - delta * 0.4;
    rightTime = time;
  }
  if (thrust) {
    delta = time - thrustTime;
    v = delta * 0.00004; 
	xv = xv + cos(angle*M_PI/180.0) * v;
    yv = yv + sin(angle*M_PI/180.0) * v;
    thrustTime = time; 
  }
  delta = time - lastTime; 

  readShip(x, y, time); 

  x = x + xv * delta;
  y = y + yv * delta; 

	if (h==0)
	{
		genmap();
	}

  lastTime = time; 
  glutPostWindowRedisplay(currentWindow);
}

void
visible(int vis)
{
  if (vis == GLUT_VISIBLE) {
    if (!paused) {
      glutIdleFunc(idle);
    }
  } else {
    glutIdleFunc(NULL);
  }
}

/* ARGSUSED1 */
void
key(unsigned char key, int px, int py)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'A':
  case 'a':
    thrust = 1; 
    thrustTime = glutGet(GLUT_ELAPSED_TIME);
    break;
  case 'S':
  case 's':
    shield = 1;
    break;
  case 'C':
  case 'c':
    cursor = !cursor;
    glutSetCursor(
      cursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
    break;
  case 'n':
  case 'N':
	genmap();
    x = xf;
    y = yf;
	h = hf;
	xv = 0;
    yv = 0;
	win=false;
	thrust=0;
  case 'z':
  case 'Z':
	if (win==false)
{
    x = xf;
    y = yf;
	h = hf;
	xv = 0;
    yv = 0;
}
    break;
  case 'g':
	fullscreen = true;
    glutGameModeString("1024x768:16@60");
    glutEnterGameMode();
    initWindow();
    break;
  case 'l':
	fullscreen = false;
    if (originalWindow != 0 && currentWindow != originalWindow) {
      glutLeaveGameMode();
      currentWindow = originalWindow;
    }
    break;
  case 'P':
  case 'p':
    paused = !paused; 
    if (paused) {
      glutIdleFunc(NULL);
    } else {
      resuming = 1;
	  glutIdleFunc(idle);
    }
    break;
  }
}

/* ARGSUSED1 */
void
keyup(unsigned char key, int x, int y)
{
  switch (key) {
  case 'A':
  case 'a':
    thrust = 0;
    break;
  case 'S':
  case 's':
    shield = 0;
    break;
  }
}

/* ARGSUSED1 */
void
special(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_F1:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    break;
  case GLUT_KEY_F2:
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    break;
  case GLUT_KEY_UP:
	  	if (win==false)
	{
    thrust = 1;
    thrustTime = glutGet(GLUT_ELAPSED_TIME);
	}
    break;
  case GLUT_KEY_LEFT:
		if (win==false)
	{
    left = 1;
    leftTime = glutGet(GLUT_ELAPSED_TIME);
	}
    break;
  case GLUT_KEY_RIGHT:
		if (win==false)
	{
    right = 1;
    rightTime = glutGet(GLUT_ELAPSED_TIME);
	}
    break;
  }
}

/* ARGSUSED1 */
void
specialup(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    thrust = 0;
    break;
  case GLUT_KEY_LEFT:
    left = 0;
    break;
  case GLUT_KEY_RIGHT:
    right = 0;
    break;
  }
}

void
initWindow(void)
{
  glutIgnoreKeyRepeat(1);

  glutDisplayFunc(display);
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(special);
  glutSpecialUpFunc(specialup);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (fullscreen==false) glOrtho(0, 100, 0, 100, -10, 10); 
		else glOrtho(-20, 120, 0, 100, -10, 10);

  glMatrixMode(GL_MODELVIEW); 
  glPointSize(3.0);

    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);

  static GLfloat lightpos[] =
  {10.f, 50.f, 50.f, 0.f};
	
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

	currentWindow = glutGetWindow();
}

int
main(int argc, char **argv)
{

	genmap();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(600, 620);
/*
  if (argc > 1 && !strcmp(argv[1], "-fullscreen")) {
    glutGameModeString("640x480:16@60");
    glutEnterGameMode();
  } else {
    originalWindow = glutCreateWindow("Voyager");
  }
*/
  originalWindow = glutCreateWindow("Voyager");
  initWindow();



	//fullscreen = true;
    //glutGameModeString("1680x1050:16@60");
    //glutEnterGameMode();
	//initWindow();

    cursor = !cursor;
    glutSetCursor(
      cursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);



  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
