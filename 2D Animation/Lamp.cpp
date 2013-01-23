#include "Lamp.h"
#include <iostream>

using namespace std;

Lamp::Lamp(double baseLength,double baseHeight,double length1,double height1,double length2,double height2, double length3,double height3, double length4, double height4)
{
	this->length[0] = baseLength;
	this->height[0] = baseHeight;
	this->length[1] = length1;
	this->height[1] = height1;
	this->length[2] = length2;
	this->height[2] = height2;
	this->length[3] = length3;
	this->height[3] = height3;
	this->length[4] = length4;
	this->height[4] = height4;

}

Lamp::Lamp(double baseLength,double baseHeight,double length1,double height1,double length2,double height2, double length3,double height3)
{
	this->length[0] = baseLength;
	this->height[0] = baseHeight;
	this->length[1] = length1;
	this->height[1] = height1;
	this->length[2] = length2;
	this->height[2] = height2;
	this->length[3] = length3;
	this->height[3] = height3;

}


Lamp::~Lamp(void)
{
}

void Lamp::setJointAngle(double theta0,double theta1,double theta2,double theta3, double theta4 = 0.0)
{
	this->theta0 = theta0;
	this->theta1 = theta1;
	this->theta2 = theta2;
	this->theta3 = theta3;
	this->theta4 = theta4;
}

void Lamp::setJointAngle2(double theta0,double theta1,double theta2,double theta3)
{
	this->theta0 = theta0;
	this->theta1 = theta1;
	this->theta2 = theta2;
	this->theta4 = theta3;
}

void Lamp::setPostion(double x, double y)
{
	xPos = x;
	yPos = y;
}

void Lamp::setFrames()
{
	this->frames = new Frame[4];
	frames[0].setFrameValues(1000.0, 0.0, 0.0, 45.0, 135.0, 45.0, 0.25);
	frames[1].setFrameValues(500.0, 150.0, 45.0, 75.0, 120.0, 30.0, 0.80);
	frames[2].setFrameValues(10.0, 300.0, -90.0, 120.0, 140.0, 110.0, 5.3);
	frames[3].setFrameValues(800.0, 400.0, 0.0, 90.0, -45.0, 75.0, 6.5);
	
}

void Lamp::showFrame(int i)
{
	this->setJointAngle2(frames[i].frameBodyAngles[0], frames[i].frameBodyAngles[1], frames[i].frameBodyAngles[2], frames[i].frameBodyAngles[3]);  
	this->setPostion(frames[i].basePosX, frames[i].basePosY);

}

void Lamp::CalculateLocalCoordinateForEachBone()
{
	GLfloat transform[16];
	glPushMatrix();
	glLoadIdentity();
	glTranslated(xPos,yPos,0);
	glRotated(theta0,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[0] = transform[12];
	armYPos[0] = transform[13];
	localAxisX[0].x = transform[0];
	localAxisX[0].y = transform[1];
	localAxisY[0].x = transform[4]; 
	localAxisY[0].y = transform[5];

	glTranslated(0,height[0],0);
	//get the start position of arm1;
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[1] = transform[12]; armYPos[1] = transform[13];

	glRotated(theta1,0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[1].x = transform[0]; 
	localAxisX[1].y = transform[1];
	localAxisY[1].x = transform[4];
	localAxisY[1].y = transform[5];


	glTranslated(length[1],0,0);
	//get the start position of arm2
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[2] = transform[12]; armYPos[2] = transform[13];

	glRotated(theta2,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[2].x = transform[0]; 
	localAxisX[2].y = transform[1];
	localAxisY[2].x = transform[4]; 
	localAxisY[2].y = transform[5];


	glTranslated(length[2],0,0);
	//get the start position of arm3
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[3] = transform[12]; armYPos[3] = transform[13];

	glRotated(theta3,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[3].x = transform[0];
	localAxisX[3].y = transform[1];
	localAxisY[3].x = transform[4];
	localAxisY[3].y = transform[5];

	glTranslated(length[3],0,0);
	//get the start position of arm4
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[4] = transform[12]; armYPos[4] = transform[13];

	glRotated(theta4,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[4].x = transform[0];
	localAxisX[4].y = transform[1];
	localAxisY[4].x = transform[4];
	localAxisY[4].y = transform[5];

	glPopMatrix();
}

void Lamp::CalculateLocalCoordinateForEachBone2()
{
	GLfloat transform[16];
	glPushMatrix();
	glLoadIdentity();
	glTranslated(xPos,yPos,0);
	glRotated(theta0,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[0] = transform[12];
	armYPos[0] = transform[13];
	localAxisX[0].x = transform[0];
	localAxisX[0].y = transform[1];
	localAxisY[0].x = transform[4]; 
	localAxisY[0].y = transform[5];

	glTranslated(0,height[0],0);
	//get the start position of arm1;
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[1] = transform[12]; armYPos[1] = transform[13];

	glRotated(theta1,0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[1].x = transform[0]; 
	localAxisX[1].y = transform[1];
	localAxisY[1].x = transform[4];
	localAxisY[1].y = transform[5];


	glTranslated(length[1],0,0);
	//get the start position of arm2
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[2] = transform[12]; armYPos[2] = transform[13];

	glRotated(theta2,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[2].x = transform[0]; 
	localAxisX[2].y = transform[1];
	localAxisY[2].x = transform[4]; 
	localAxisY[2].y = transform[5];


	glTranslated(length[2],0,0);
	//get the start position of arm3
	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	armXPos[3] = transform[12]; armYPos[3] = transform[13];

	glRotated(theta3,0,0,1);

	glGetFloatv(GL_MODELVIEW_MATRIX,transform);
	localAxisX[3].x = transform[0];
	localAxisX[3].y = transform[1];
	localAxisY[3].x = transform[4];
	localAxisY[3].y = transform[5];

	glPopMatrix();
}


void Lamp::draw()
{
	glPushMatrix();

	glTranslated(xPos ,yPos, 0);
	
	
		//draw base
		glColor3f(.5,.5,.5);
		glRotated(theta0,0,0,1);
		glTranslated(0,height[0]/2,0);
		glLoadName(0);  // This command is for arm selection. If you want to add a new arm, you need to give a new name before drawing it.
		glRectf(-length[0]/2,-height[0]/2, length[0]/2, height[0]/2);

		// go to the center of arm1
		glTranslated(0,height[0]/2,0);
		glRotated(theta1,0,0,1);
		glTranslated(length[1]/2,0,0);

		//draw arm1
		
		glLoadName(1);
		glColor3f(0.0,0.0,1.0);
		drawEllipse(length[1]/2,height[1]/2);
		glColor3f(0.0,0.0,0.0);
		glRectf(-length[1]/2 + length[1]/18, -height[0]/6, length[1]/2 - length[1]/18, height[1]/6); 


		//go to the center of arm2
		glTranslated(length[1]/2,0,0);
		glRotated(theta2,0,0,1);
		glTranslated(length[2]/2,0,0);

		//draw arm2
		glColor3f(1.0,0.0,1.0);
		glLoadName(2);
		drawEllipse(length[2]/2,height[2]/2);
		glColor3f(0.0,0.0,0.0);
		glRectf(-length[2]/2 + length[2]/18, -height[1]/6, length[2]/2 - length[2]/18, height[2]/6); 

		//go to the center of arm3
		glTranslated(length[2]/2,0,0);
		glRotated(theta3,0,0,1);
		glTranslated(length[3]/2,0,0);

		//draw arm3
		glColor3f(0.0,1.0,0.0);
		glLoadName(3);
		drawEllipse(length[3]/2,height[3]/2);
		glColor3f(0.0,0.0,0.0);
		glRectf(-length[3]/2 + length[3]/18, -height[2]/6, length[3]/2 - length[3]/18, height[3]/6); 

		//go to the center of arm4
		glTranslated(length[3]/2,0,0);
		glRotated(theta3,0,0,1);
		glTranslated(length[4]/2,0,0);

		//draw arm4
		glColor3f(0.0,1.0,1.0);
		glLoadName(4);
		drawEllipse(length[4]/2,height[4]/2, true);


	glPopMatrix();
}

void Lamp::draw2()
{
	glPushMatrix();

	glTranslated(xPos ,yPos, 0);
	
	
		//draw base
		glColor3f(.5,.5,.5);
		glRotated(theta0,0,0,1);
		glTranslated(0,height[0]/2,0);
		glLoadName(0);  // This command is for arm selection. If you want to add a new arm, you need to give a new name before drawing it.
		glRectf(-length[0]/2,-height[0]/2, length[0]/2, height[0]/2);

		// go to the center of arm1
		glTranslated(0,height[0]/2,0);
		glRotated(theta1,0,0,1);
		glTranslated(length[1]/2,0,0);

		//draw arm1
		glColor3f(0.0,0.0,1.0);
		glLoadName(1);
		drawEllipse(length[1]/2,height[1]/2);
		glColor3f(0.0,0.0,0.0);
		glRectf(-length[1]/2 + length[1]/18, -height[0]/6, length[1]/2 - length[1]/18, height[1]/6); 

		//go to the center of arm2
		glTranslated(length[1]/2,0,0);
		glRotated(theta2,0,0,1);
		glTranslated(length[2]/2,0,0);

		//draw arm2
		glColor3f(1.0,0.0,1.0);
		glLoadName(2);
		drawEllipse(length[2]/2,height[2]/2);
		glColor3f(0.0,0.0,0.0);
		glRectf(-length[2]/2 + length[2]/18, -height[1]/6, length[2]/2 - length[2]/18, height[2]/6); 

		//go to the center of arm3
		glTranslated(length[2]/2,0,0);
		glRotated(theta4 + 195,0,0,1);
		glTranslated(length[4]/2,0,0);

		//draw arm3
		glColor3f(0.0,1.0,0.0);
		glLoadName(3);
		drawEllipse(length[4]/2,height[4]/2, true);


	glPopMatrix();
}

void Lamp::drawEllipse(double x, double y, bool p )
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d( 0 , 0 , 0 );

	if(p)
	{
		for(double i = 0 ; i < 3.84 ; i += 0.01)
		{
			glVertex3d( x * cos(i) , y * sin(i) , 0 );
		}
	}
	else
	{
		for(double i = 0 ; i < 6.4 ; i += 0.01)
		{
			glVertex3d( x * cos(i) , y * sin(i) , 0 );
		}
	}


	glEnd();
}