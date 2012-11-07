#ifndef FORE_ARM_H
#define FORE_ARM_H

#include "Body_Part.h"

class Fore_Arm : public Body_Part
{
private:
	bool is_on_left;
public:
	Fore_Arm() : Body_Part(0.0f, 0.0f, 0.0f) {is_on_left = false;}
	Fore_Arm(float w, float h, float d = 1.0f) : Body_Part(w,h,d) {}
	~Fore_Arm() {}

	void draw()
	{
		glTranslatef(x_location, y_location, z_location);
		glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		glRotatef( (GLfloat) rotation_angle_y, 0.0, 1.0, 0.0);
		glRotatef( (GLfloat) rotation_angle_x, 1.0, 0.0, 0.0);
		if(is_on_left)
			glTranslatef (-width/2, 0.0f, 0.0f);
		else
			 glTranslatef (width/2, 0.0f, 0.0f);
		glPushMatrix();
		glScalef(width, height, depth);
		glutSolidCube(1.0);
		glPopMatrix();
		
	}

	void set_is_on_left(bool l) {is_on_left = l;}

};

#endif
