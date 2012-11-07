#ifndef JOINTS_H
#define JOINTS_H

#include "Body_Part.h"

class Joints : public Body_Part
{
	bool is_on_left;
public:
	Joints(): Body_Part(0.0f, 0.0f, 0.0f) {is_on_left = false;}
	~Joints() {}

	void set_is_on_left(bool l) {is_on_left = l;}

	void draw()
	{
		glTranslatef(x_location, y_location, z_location);
		//glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		if(is_on_left)
			glTranslatef (2.0f, 0.0f, 0.0f);
		else
			 glTranslatef (width/2, 0.0f, 0.0f);
		glPushMatrix();
		glScalef(width, height, depth);
		glutSolidSphere(width/2, 20, 20);
		glPopMatrix();
	}

};

#endif