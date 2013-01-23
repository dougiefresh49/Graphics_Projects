#include <GL/glut.h>
#include "Lamp.h"
#include "levmar.h"
#include "Matrix.h"
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

using namespace std;

#define WIDTH 1200
#define HEIGHT 800

Lamp *lamp;

struct Point
{
	double x, y;
	Point(double xx , double yy) { x = xx; y = yy;}
	Point() { x = 0; y = 0;}
};


bool rightMouseClicked = true;
bool drawFixedPoint = false;
bool drawMovePoint = false;

//fixed constraints	
vector<int> fixIdx;						//belongs to which arm or base
vector<Point> fixedPoints;				//global position of each fix constraint
vector<Point> fixedLocalPoint;			//local position of each fixed constraint in the corresponding arm's local coordinate.	

//moving constraints
bool movPoint = false;
int movIdx = -1;						//belongs to which arm or base
double movX, movY;						//global position of the moving constraint
double movLocalX, movLocalY;			//local position of the moving constraint in the corresponding arm's local coordinate.

int fixedX;
int fixedY;

bool drawLamp1 = true;
bool drawLamp2 = false;
bool drawTower = false;

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	if(drawTower)
	{
		glColor3f (139.0/250.0, 69.0/250.0, 19.0/205.0);
		glRectf(700.0, 0.0, 1200, 400); 
		glColor3f (0.0, 0.0, 0.0);
		glRectf(825.0, 0.0, 1100, 300);
	}


	if(drawLamp1)
		lamp->draw();
	if(drawLamp2)
		lamp->draw2();


	if(drawFixedPoint)
	{
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			for(unsigned int i = 0; i < fixedPoints.size(); i++)
				glVertex3d(fixedPoints[i].x, fixedPoints[i].y + 200, 0);
		glEnd();
	}
	
	if(drawMovePoint )
	{
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glColor3f(1.0f,  1.0f, 0.0f);
			glVertex3d(movX, movY + 200, 0);
		glEnd();
	}

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

double perform_catmullRom(double t, double p0, double p1, double p2, double p3, int m1, int m2)
{
  vector<vector< double> > t_vec(1, vector<double>(4, 0.0));
  t_vec[0][0] = t * t * t;
  t_vec[0][1] = t * t;
  t_vec[0][2] = t;
  t_vec[0][3] = 1;
  Matrix t_matrix(1, 4, t_vec);

  vector< vector<double> > hermite_vec(4, vector<double>(4, 0.0));

  hermite_vec[0][0] = 2; 
  hermite_vec[0][1] = -2; 
  hermite_vec[0][2] = 1;
  hermite_vec[0][3] = 1;


  hermite_vec[1][0] = -3;
  hermite_vec[1][1] = 3; 
  hermite_vec[1][2] = -2;
  hermite_vec[1][3] = -1;


  hermite_vec[2][0] = 0;
  hermite_vec[2][1] = 0;
  hermite_vec[2][2] = 1; 
  hermite_vec[2][3] = 0;


  hermite_vec[3][0] = 1;
  hermite_vec[3][1] = 0;
  hermite_vec[3][2] = 0;
  hermite_vec[3][3] = 0;

  Matrix hermite_matrix(4, 4, hermite_vec);

  vector< vector<double> > catmull_vec(4, vector<double>(4, 0.0));

  catmull_vec[0][m1] = 1;
  catmull_vec[1][m2] = 1;

  catmull_vec[2][max(m1 - 1, 0)] = -1;
  catmull_vec[2][min(m1 + 1, 3)] = 1;

  catmull_vec[3][max(m2 - 1, 0)] = -1; 
  catmull_vec[3][min(m2 + 1, 3)] = 1;

  Matrix catmull_matrix(4, 4, catmull_vec);

  vector<vector<double> > point_vec(4, vector<double>(1, 0.0));
  point_vec[0][0] = p0;
  point_vec[1][0] = p1;
  point_vec[2][0] = p2;
  point_vec[3][0] = p3;
  Matrix point_matrix(4, 1, point_vec);

  Matrix result = t_matrix * hermite_matrix * catmull_matrix * point_matrix;

  return result.GetValue(0, 0);
}


void interpolate()
{
  Frame* frames = lamp->getFrames();

  for (int j = 0; j < 3; j++)
  {
  
    double rate = 1.0 / ((frames[j + 1].time - frames[j].time) / 0.01);
	
    for (double i = 0.0; i < 1.0; i += rate)
	{
      double dof[6] = 
	  {
        perform_catmullRom(i, frames[0].basePosX, frames[1].basePosX, frames[2].basePosX, frames[3].basePosX, j, j + 1),
        perform_catmullRom(i, frames[0].basePosY, frames[1].basePosY, frames[2].basePosY, frames[3].basePosY, j, j + 1),
        perform_catmullRom(i, frames[0].frameBodyAngles[0], frames[1].frameBodyAngles[0], frames[2].frameBodyAngles[0], frames[3].frameBodyAngles[0], j, j + 1),
        perform_catmullRom(i, frames[0].frameBodyAngles[1], frames[1].frameBodyAngles[1], frames[2].frameBodyAngles[1], frames[3].frameBodyAngles[1], j, j + 1),
        perform_catmullRom(i, frames[0].frameBodyAngles[2], frames[1].frameBodyAngles[2], frames[2].frameBodyAngles[2], frames[3].frameBodyAngles[2], j, j + 1),
        perform_catmullRom(i, frames[0].frameBodyAngles[3], frames[1].frameBodyAngles[3], frames[2].frameBodyAngles[3], frames[3].frameBodyAngles[3], j, j + 1)
      };
	  

		lamp-> setPostion(dof[0], dof[1]);
		if(drawLamp2)
			lamp-> setJointAngle2(dof[2], dof[3], dof[4], dof[5]);
		else
			lamp-> setJointAngle(dof[2], dof[3], dof[4], -dof[3], dof[5]);
	    display();
    }

  }

}


Point calculateMoveGlobalPoints()
{
	Point globalPos;

	double convrToRadians = 3.14 / 180.0;
	double tempX = movLocalX;
	double tempY = movLocalY;
	double tempX1 = 0.0;
	double tempY1 = 0.0;
	double tempX2 = 0.0;
	double tempY2 = 0.0;
	
	if(movIdx == 4)
	{
		tempX1 = (tempX * cos(lamp->theta3 * convrToRadians)) - (tempY * sin(lamp->theta3 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta3 * convrToRadians)) + (tempY * cos(lamp->theta3 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;

	}

	if(movIdx == 3)
	{
		// only relevant if we have a 4th piece
		if(movIdx != 3)
			tempX += lamp->length[3];

		tempX1 = (tempX * cos(lamp->theta3 * convrToRadians)) - (tempY * sin(lamp->theta3 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta3 * convrToRadians)) + (tempY * cos(lamp->theta3 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;
	}
	
	if( movIdx == 2)
	{
		// only add in this leg if the higher leg has been clicked
		if(movIdx != 2)
			tempX += lamp->length[2];

		tempX1 = (tempX * cos(lamp->theta2 * convrToRadians)) - (tempY * sin(lamp->theta2 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta2 * convrToRadians)) + (tempY * cos(lamp->theta2 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;

	}
	
	if(movIdx == 1)
	{
		// only add in this leg if the higher leg has been clicked
		if(movIdx != 1)
			tempX += lamp->length[1];

		tempX1 = (tempX * cos(lamp->theta1 * convrToRadians)) - (tempY * sin(lamp->theta1 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta1 * convrToRadians)) + (tempY * cos(lamp->theta1 * convrToRadians));

	}
	

	tempY1 += lamp->height[0];

	tempX2 = (tempX1 * cos(lamp->theta0 * convrToRadians)) - (tempY1 * sin(lamp->theta0 * convrToRadians));
	tempY2 = (tempX1 * sin(lamp->theta0 * convrToRadians)) + (tempY1 * cos(lamp->theta0 * convrToRadians));

	tempX2 += lamp->xPos;
	tempY2 += lamp->yPos;

	globalPos.x = tempX2;
	globalPos.y = tempY2;

	cout << "____Test_____ \n" 
		 << "Move Global: X,Y: " << globalPos.x << " , " << globalPos.y << endl
	     << "Actual Move: X,Y: " << movX << " , " << movY << endl << endl;

	return globalPos;

}
Point calculateFixedGlobalPoints(int i)
{
	Point globalPos;

	double convrToRadians = 3.14 / 180.0;
	double tempTheata = 0;
	double tempX = fixedLocalPoint[i].x;
	double tempY = fixedLocalPoint[i].y;
	double tempX1 = 0.0;
	double tempY1 = 0.0;
	double tempX2 = 0.0;
	double tempY2 = 0.0;

	if(fixIdx[i] == 4)
	{
		tempX1 = (tempX * cos(lamp->theta4 * convrToRadians)) - (tempY * sin(lamp->theta4 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta4 * convrToRadians)) + (tempY * cos(lamp->theta4 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;
	}
	
	if(fixIdx[i] >= 3)
	{
		// only relevant if we have a 4th piece
		if(fixIdx[i] != 3)
			tempX += lamp->length[3];

		tempX1 = (tempX * cos(lamp->theta3 * convrToRadians)) - (tempY * sin(lamp->theta3 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta3 * convrToRadians)) + (tempY * cos(lamp->theta3 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;
	}
	
	if( fixIdx[i] >= 2)
	{
		// only add in this leg if the higher leg has been clicked
		if(fixIdx[i] != 2)
			tempX += lamp->length[2];

		tempX1 = (tempX * cos(lamp->theta2 * convrToRadians)) - (tempY * sin(lamp->theta2 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta2 * convrToRadians)) + (tempY * cos(lamp->theta2 * convrToRadians));

		tempX = tempX1;
		tempY = tempY1;

	}
	
	if(fixIdx[i] >= 1)
	{
		// only add in this leg if the higher leg has been clicked
		if(fixIdx[i] != 1)
			tempX += lamp->length[1];

		std::cout << "(" << tempX1 << ", " << tempY1 << ")" << std::endl;
		tempX1 = (tempX * cos(lamp->theta1 * convrToRadians)) - (tempY * sin(lamp->theta1 * convrToRadians));
		tempY1 = (tempX * sin(lamp->theta1 * convrToRadians)) + (tempY * cos(lamp->theta1 * convrToRadians));
		std::cout << "(" << tempX1 << ", " << tempY1 << ")" << std::endl;
	}
	

	tempY1 += lamp->height[0];

	tempX2 = (tempX1 * cos(lamp->theta0 * convrToRadians)) - (tempY1 * sin(lamp->theta0 * convrToRadians));
	tempY2 = (tempX1 * sin(lamp->theta0 * convrToRadians)) + (tempY1 * cos(lamp->theta0 * convrToRadians));

	tempX2 += lamp->xPos;
	tempY2 += lamp->yPos;

	globalPos.x = tempX2;
	globalPos.y = tempY2;


	cout << "____Test_____ \n" 
		 << "Fixed Global: X,Y: " << globalPos.x << " , " << globalPos.y << endl
	     << "Actual Fixed: X,Y: " << movX << " , " << movY << endl << endl;


	return globalPos;

}

Point calculateGlobal(double localX, double localY, double *p, int idxVal)
{
	//###############################
	// should be Point not void
	// #################


	GLfloat transform[16];
    Point global;
	int idx = idxVal;


    if(idx == 0)
	{ // base
        glPushMatrix();
                glLoadIdentity();

                glTranslated(p[0], p[1], 0);
                glRotated(p[2],0,0,1);

                glTranslated(localX, localY, 0);
                glGetFloatv(GL_MODELVIEW_MATRIX, transform);

        glPopMatrix();
	}

	else if(idx == 1)
	{ // bone 1
        glPushMatrix();
                glLoadIdentity();

                glTranslated(p[0], p[1], 0);		     //translate by base coord
                glRotated(p[2],0,0,1);				    //rotate by base theta 

                glTranslated(0,lamp->height[0], 0);     //translate by base height
                glRotated(p[3], 0, 0, 1);

                glTranslated(localX, localY, 0);        //translate by p
                glGetFloatv(GL_MODELVIEW_MATRIX, transform);

        glPopMatrix();

	}
	else if(idx == 2)
	{
		glPushMatrix();
                glLoadIdentity();

                glTranslated(p[0], p[1], 0);		     //translate by base coord
                glRotated(p[2],0,0,1);				    //rotate by base theta 

                glTranslated(0,lamp->height[0], 0);     //translate by base height
                glRotated(p[3], 0, 0, 1);

				glTranslated(0,lamp->height[1], 0);     //translate by base height
                glRotated(p[4], 0, 0, 1);

                glTranslated(localX, localY, 0);        //translate by p
                glGetFloatv(GL_MODELVIEW_MATRIX, transform);

        glPopMatrix();

	}

	else if(idx == 3)
	{
		glPushMatrix();
                glLoadIdentity();

                glTranslated(p[0], p[1], 0);		     //translate by base coord
                glRotated(p[2],0,0,1);				    //rotate by base theta 

                glTranslated(0,lamp->height[0], 0);     //translate by base height
                glRotated(p[3], 0, 0, 1);

				glTranslated(0,lamp->height[1], 0);     //translate by base height
                glRotated(p[4], 0, 0, 1);

			    glTranslated(0,lamp->height[2], 0);     //translate by base height
                glRotated(p[5], 0, 0, 1);

                glTranslated(localX, localY, 0);        //translate by p
                glGetFloatv(GL_MODELVIEW_MATRIX, transform);

        glPopMatrix();

	}

	return (Point(transform[12] , transform[13]));

}

void costEvaluation(double *p, double *x, int m, int n, void *data)
{
	//You need implement the cost function here
	vector<Point> fixedGlobalPoint;
	Point tempFixdGlobalPos;
	Point tempMovGlobalPos;

	//First Step: You should implement Forward Kinematics Here to Calculate the Global Position of each constraint based on the current P.

	// determining fixed global positions
	for(unsigned int i = 0; i < fixIdx.size(); i ++)
	{
		// will eventually use calculateGlobal not calculateFixedGlobalPoints
		//tempFixdGlobalPos = calculateFixedGlobalPoints(i);	

		tempFixdGlobalPos = calculateGlobal(fixedLocalPoint[i].x,fixedLocalPoint[i].y, p, fixIdx[i]);
		fixedGlobalPoint.push_back(tempFixdGlobalPos);
	}
	
	// determining move global position
	tempMovGlobalPos = calculateGlobal(movLocalX, movLocalY, p, movIdx);

	//Second Step: Make the cost function here ....... If the ith term is (f(theta)-C)^2, you can assign x[i] = f(theta)-C;

	if(fixedPoints.size() > 0)
	{
		x[2] = (fixedPoints[0].x - tempFixdGlobalPos.x)*.1 ;
		x[3] = (fixedPoints[0].y - tempFixdGlobalPos.y)*.1 ;
	}
	x[0] = (movX - tempMovGlobalPos.x)*.1;
	x[1] = (movY - tempMovGlobalPos.y)*.1;
	x[4] = (p[0] - lamp->xPos)*.1;
	x[5] = (p[1] - lamp->yPos)*.1;
	x[6] = (p[2] - lamp->theta0)*.1;
	x[7] = (p[3] - lamp->theta1)*.1;
	x[8] = (p[4] - lamp->theta2)*.1;
	x[9] = (p[5] - lamp->theta3)*.1;
	x[10] = (p[6] - lamp->theta4)*.1;


	cout << "____In CostEval___ " << endl
		 << "x[0]: " << x[0] << endl
		 << "x[1]: " << x[1] << endl
		 << "x[2]: " << x[2] << endl
		 << "x[3]: " << x[3] << endl
		 << "x[4]: " << x[4] << endl
		 << "x[5]: " << x[5] << endl
		 << "x[6]: " << x[6] << endl
		 << "x[7]: " << x[7] << endl
		 << "x[8]: " << x[8] << endl
		 << "x[9]: " << x[9] << endl
		 << "x[10]: " << x[10] << endl <<endl;
	

}

int processHits (GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	printf ("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}

		ptr += names+2;
	}
	printf ("The closest hit names are ");
	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++,ptr++) {
		printf ("%d ", *ptr);
		
	}
	printf ("\n");
	ptr--;
	return *ptr;
}

/************************************************************************/
/* selection help determines which object is selected                   */
/************************************************************************/
//mouse_x, mouse_y is the current mouse position 

int selection(int mouse_x, int mouse_y)
{
	GLuint	buffer[512];										// Set Up A Selection Buffer
	GLint	hits;												// The Number Of Objects That We Selected

	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
	GLint	viewport[4];

	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// Tell OpenGL To Use Our Array For Selection

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	(void) glRenderMode(GL_SELECT);

	glInitNames();												// Initializes The Name Stack
	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
	glPushMatrix();												// Push The Projection Matrix
	glLoadIdentity();											// Resets The Matrix

	gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]-mouse_y), 1.0f, 1.0f, viewport);

	// Apply The Perspective Matrix
	gluOrtho2D (0.0, (GLdouble) viewport[2], 0.0, (GLdouble) viewport[3]);
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	lamp->draw();
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	hits=glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many

	if (hits != 0)
		return(processHits(hits,buffer));
	else
		return -1;
}


void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN)
		{
			rightMouseClicked = true; 
			drawFixedPoint = true;
			fixedX = x;
			fixedY = HEIGHT - y;

			double tmpX = x;
			double tmpY = 600-y;

			// get the index of point
			// 0-base, 1- arm1, 2-arm2, 3-arm3
			int idx = selection(x,y);
			
			cout<<"we have chosen #"<<idx <<" object"<<endl;

			if(idx != -1)
			{
				fixIdx.push_back(idx);

				//calculate position in the local coordinate
				lamp->CalculateLocalCoordinateForEachBone();

				Point tempPoint(tmpX, tmpY);

				fixedPoints.push_back(tempPoint);
				

				double start2fix[2] = {tmpX-lamp->armXPos[idx],tmpY - lamp->armYPos[idx]};
				double localX = start2fix[0]*lamp->localAxisX[idx].x + start2fix[1]*lamp->localAxisX[idx].y;
				double localY = start2fix[0]*lamp->localAxisY[idx].x + start2fix[1]*lamp->localAxisY[idx].y;

				tempPoint.x = localX;
				tempPoint.y = localY;

				fixedLocalPoint.push_back(tempPoint);

				if(idx == -1)
					cout<<"We will not use the fixed constraints"<<endl;
				else
					cout<<"Bone: "<< idx <<" local Coordinate: "<< localX <<" "<< localY << endl;
				 
			}
			
			
		}
		break;

	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{
			
			movX = x;
			movY = 600-y;
			movPoint = true;
			drawMovePoint = true;
			rightMouseClicked = false;
			


			movIdx = selection(x,y); 
			cout<<"we have chosen #"<<movIdx <<" object"<<endl;

			if(movIdx != -1)
			{
				//calculate position in the local coordinate
				lamp->CalculateLocalCoordinateForEachBone();
				double start2mov[2] = {movX-lamp->armXPos[movIdx],movY - lamp->armYPos[movIdx]};
				movLocalX = start2mov[0]*lamp->localAxisX[movIdx].x + start2mov[1]*lamp->localAxisX[movIdx].y;
				movLocalY = start2mov[0]*lamp->localAxisY[movIdx].x + start2mov[1]*lamp->localAxisY[movIdx].y;

				cout << "Bone: "<< movIdx << " local Coordinate: " << movLocalX <<" " << movLocalY << endl;
			}
			else
				cout<< "Please choose a point on the arm!!" << endl;

			
		}
		else if(state == GLUT_UP)
		{
			drawMovePoint = false;

			// after dragging, restore
			fixIdx.clear();
			fixedLocalPoint.clear();
			fixedPoints.clear();

			
			movIdx = -1;
			
			
		}
		break;

	}

	glutPostRedisplay();
	

}

// You should Implement IK here. Then during dragging
// the pose will keep changing.
void mouseMotion(int xx, int yy)
{
	if(!rightMouseClicked)
	{
		movX = xx;
		movY = 600-yy;


		//do IK here
		double p[7];  //for degrees of freedom, p[0],p[1] for root position, p[2] for root rotation ,p[3],p[4],p[5] for joint angle
		double *x;  // x's size is based on the number of fixed constraints.
		x = new double [2 + fixIdx.size() * 2 + 6 + 1];
	
		//1. assign initial joint angle and root position to p 
		//You need implement here
		p[0] = lamp->xPos;
		p[1] = lamp->yPos;
		p[2] = lamp->theta0;
		p[3] = lamp->theta1;
		p[4] = lamp->theta2;
		p[5] = lamp->theta3;
		p[6] = lamp->theta4;

		//2. for each measurement or cost term, we expected it to be 0.
		for(int i=0;i<2+fixIdx.size()*2+6;i++)
			x[i] = 0;


		//3. do IK
		//http://www.ics.forth.gr/~lourakis/levmar/ , see this link about usage of dlevmar_dif function
		// the last 0 is for extra data that you may need to use in the costEvaluation
		// If you don't use, you can set it to 0.
		dlevmar_dif(costEvaluation, p, x, 6, 2 + fixIdx.size() * 2 + 6 + 1, 200, 0, 0, 0, 0, 0);
		cout << "IK done" << endl;


		//4. assign result back to the lamp
		//You should implement here. The optimized result is now stored in p.
		lamp-> setPostion(p[0], p[1]);
		lamp-> setJointAngle(p[2], p[3], p[4], p[5], p[6]);

		//cout << " _____In Mouse_Motion____" << endl
		// << "xPos: " << lamp->xPos << endl
		// << "yPos: " << lamp->yPos << endl
		// << "theta0: " << lamp->theta0 << endl
		// << "theta1: " << lamp->theta1 << endl
		// << "theta2: " << lamp->theta2 << endl
		// << "theta3: " << lamp->theta3 << endl 
		// << "theta4: " << lamp->theta4 << endl << endl;

		delete []x;
		glutPostRedisplay();

	}
}

void animate()
{
	drawTower = true;
	lamp->setFrames();

	interpolate();

}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'a':
		drawFixedPoint = false;
		animate();
		drawFixedPoint = true;
		break;
	case 'u':
		drawLamp2 = false;
		drawLamp1 = true;
		display();
		break;
	case 'k':
		drawLamp2 = true;
		drawLamp1 = false;
		glutPostRedisplay();
		break;
	case ' ':
		// addFrame();
		break;
	case '1':
		lamp->showFrame(0);
		glutPostRedisplay();
		break;
	case '2':
		lamp->showFrame(1);
		glutPostRedisplay();
		break;
	case '3':
		lamp->showFrame(2);
		glutPostRedisplay();
		break;
	case '4':
		lamp->showFrame(3);
		glutPostRedisplay();
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (WIDTH, HEIGHT); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Doug Lasater - Programming Assignment 6");
	init ();

	lamp = new Lamp(100,40,150,30,140,20,120,20, 90, 40);
	lamp->setJointAngle(0.0, 45, 45, -45, 75);
	lamp->setPostion(600, 100);

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMotion);
	glutMainLoop();
	return 0;
}