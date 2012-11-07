#ifndef SHOULDER_H
#define SHOULDER_H

#include "Fore_Arm.h"

class Shoulder : public Body_Part
{
private: 
	bool is_on_left;
public:
    Shoulder(): Body_Part(0.0f,0.0f,0.0f) {is_on_left = false;}
    Shoulder(float w, float h, float d = 0.0f) : Body_Part(w,h,d) {}
    ~Shoulder() {}
    
    void draw()
    {
         glTranslatef(x_location, y_location, z_location);
		 glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		 if(is_on_left)
			glTranslatef (-width/2, 0.0f, 0.0f);
		 else
			 glTranslatef (width/2, 0.0f, 0.0f);
		 glPushMatrix();
		 glScalef(width, height, depth);
		 glutSolidCube (1.0);
		 glPopMatrix();
    
    
    }

	void set_is_on_left(bool l) {is_on_left = l;}
};

#endif
