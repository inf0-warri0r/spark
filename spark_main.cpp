/*
*Author :Tharindra Galahena
*Project:SPARK - Crawling Robote A.I.
*Date   :11/11/2012
*License:
* 
*     Copyright 2012 Tharindra Galahena
*
* This program is free software: you can redistribute it and/or modifood_y it under the 
* terms of the GNU General Public License as published by the Free Software Foundation, 
* either version 3 of the License, or (at your option) any later version. This program is 
* distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even 
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with This program. 
* If not, see http://www.gnu.org/licenses/.
*
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <fstream>

#include "spark.cpp"

using namespace std;

bool finish = false;
spark *s;
int steps = 0;
bool p = false;

void par(float x1, float x2, float y1, float y2, float r, float b, float g){
	glColor3f(r, b, g);
	glBegin(GL_QUADS);
	
	glVertex3f(x1, y1*2, 0.0);
	glVertex3f(x2, y1*2, 0.0);
	glVertex3f(x2, y2*2, 0.0);
	glVertex3f(x1, y2*2, 0.0);

	glEnd();
}
void line(int x1, int y1, int x2, int y2, float r, float b, float g){
	glColor3f(r, b, g);
	glBegin(GL_LINES);
    glVertex3f(x1, y1*2,0.0);
    glVertex3f(x2, y2*2,0.0);
    glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.0, 0.0, -22.0);
	
	point_i  body  = s -> get_body();
	point_i  foot  = s -> get_foot();
	point_d joint = s -> get_joint();
	
	int x = body.x;
	int y = body.y;
	
	if(x > 25 || x < -5) x = x % 30 - 5;
	
	double fx = x + foot.x;
	double fy = y + foot.y;
	double jx = (double)x + joint.x;
	double jy = (double)y + joint.y;
	
	
	par((fx - 0.1 - 10), (fx + 0.1 - 10), (fy - 0.1), (fy + 0.1), 0, 3, 0);
	
	line((x  - 10.0),  y, (jx - 10.0), jy, 0, 0, 3);
	line((jx - 10.0), jy, (fx - 10.0), fy, 0, 0, 3);
	line((-10 - 10.0), 0, (35 - 10.0),  0, 3, 0, 0);
	
	par((double)(x - 2 - 10) , (x - 10) / 1.0, (y + 1), y, 0, 3, 0);
	glutSwapBuffers();
}
void init(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);	
	glLoadIdentity ();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GLfloat acolor[] = {1.4, 1.4, 1.4, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, acolor);
}
void timer_func(int a) {
	s -> try_and_learn();
	
	glutPostRedisplay();
	if(!p) glutTimerFunc(400, timer_func, 0);
}
void keyboard(unsigned char key, int x, int y){
	
	if(key == 27) {		
		exit(0);	
	}else if((char)key == 's' || (char)key == 'S'){
		for(int i = 0; i < 1000; i++){
			s -> try_and_learn();
		}
	}else if((char)key == 'p' || (char)key == 'P'){
		p = true;
	}else if((char)key == 'r' || (char)key == 'R'){
		p = false;
		glutTimerFunc(400, timer_func, 0);
	}
}
void Reshape(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(45.0, (float)w/(float)h, 0.1, 200.0);
}
int main(int argc, char** argv){
	
	srand(time(NULL));
	s = new spark(0, 0, 7, 4, 0.9, 0.01, 10);
	cout << "-----------------------------------------------" << endl;
	cout << endl;
	cout << "       --- SPARK - CRAWLING ROBORT A.I. ---     " << endl;
	cout << endl;
	cout << "created by : tharindra galahena (inf0_warri0r) " << endl;
	cout << "blog       : www.inf0warri0r.blogspot.com      " << endl;
	cout << endl;
	cout << "-----------------------------------------------" << endl;
	cout << endl;
	cout << endl;
	
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,400);
	glutInitWindowPosition(500,0);
	glutCreateWindow("SPARK");
	
	init();
	glutTimerFunc(400, timer_func, 0);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	
	glutMainLoop();
	return 0;
}
