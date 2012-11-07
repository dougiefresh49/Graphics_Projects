#ifndef HEAD_H
#define HEAD_H

#include "Body_Part.h"

class Head : public Body_Part
{	

public:
	Head(float w, float h, float d = 1.0f) : Body_Part(w, h, d){}
	Head() : Body_Part(0.0f,0.0f,0.0f) {}
	~Head() {}

	void draw()
	{

		 glTranslatef(x_location, y_location, z_location);
		 glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		 glRotatef((GLfloat) rotation_angle_x, 1.0, 0.0, 0.0);
		 glTranslatef (0.0, height/2, 0.0);
		 glPushMatrix();
		 glScalef(width, height, depth);
		 glutSolidCube (1.0);
		 glPopMatrix();

	}



};



#endif