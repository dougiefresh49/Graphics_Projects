#ifndef LEG_H
#define LEG_H

#include "Shoulder.h"
#include "Joints.h"
#include <math.h>

class Leg : public Body_Part
{
private:
    Shoulder leg_shoulder;
    Fore_Arm leg_forearm;

	Joints body_to_shoulder_joint;
	Joints shoulder_to_forearm_joint;

public:
	Leg(): Body_Part(0.0f,0.0f,0.0f) {}
	~Leg() {}

	void set_forearm_rotation(float angle) { leg_forearm.update_x_rotation(angle); }
	void update_leg_values(char L_or_R)
	{
		// setting values for shoulder joint
		body_to_shoulder_joint.update_width(1.85f);
		body_to_shoulder_joint.update_height(height + 0.5);
		body_to_shoulder_joint.update_depth(depth);

		body_to_shoulder_joint.set_x_loc(x_location - 1.75);
		body_to_shoulder_joint.set_y_loc(y_location);
		body_to_shoulder_joint.set_z_loc(z_location - 1.5);
		
		// setting the values for the shoulder of the leg
		leg_shoulder.update_width(width * 0.75);
		leg_shoulder.update_height(height);
		leg_shoulder.update_depth(depth/3);
		        
		
		leg_shoulder.set_y_loc(y_location);
		leg_shoulder.set_z_loc(z_location - 1.5);
        
		// setting values for forearm joint
		shoulder_to_forearm_joint.update_width(1.250f);
		shoulder_to_forearm_joint.update_height(height);
		shoulder_to_forearm_joint.update_depth(depth);

		
		shoulder_to_forearm_joint.set_y_loc(y_location);
		shoulder_to_forearm_joint.set_z_loc(z_location - 1.5);
		
		// setting the values for the forearm of the leg
		leg_forearm.update_width(depth / 2.5);
		leg_forearm.update_height(height);
		leg_forearm.update_depth(depth * 3);
    
			
		leg_forearm.set_y_loc(y_location);
		leg_forearm.set_z_loc(z_location + z_location + 1 - 1.5);

		// determines if leg is on left or right
		if(L_or_R == 'L')
		{
			leg_shoulder.set_x_loc(x_location + 0.25f);

			// set the forearm further down than the shoulder
			leg_forearm.set_x_loc(x_location - leg_shoulder.get_width() );

			shoulder_to_forearm_joint.set_x_loc(x_location - leg_shoulder.get_width() - 1.0f);
			
			// tells each body part that it is on the left side
			leg_forearm.set_is_on_left(true);
			leg_shoulder.set_is_on_left(true);

			body_to_shoulder_joint.set_is_on_left(true);
			//shoulder_to_forearm_joint.set_is_on_left(true);

		}
		else
		{
			leg_shoulder.set_x_loc(x_location - 1.0f);

			// set the elbow joint location 
			shoulder_to_forearm_joint.set_x_loc(x_location + leg_shoulder.get_width() - 1.0f);

			// set the forearm further down than the shoulder
			leg_forearm.set_x_loc(x_location + leg_shoulder.get_width() -0.85);
		}
    
	}

	void draw()
    {

		

		glPushMatrix();
		// rotates the entire leg (shoulder and arm) 
		glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		glRotatef( (GLfloat) rotation_angle_y, 0.0, 1.0, 0.0);
		glRotatef( (GLfloat) rotation_angle_x, 1.0, 0.0, 0.0);


		glPushMatrix();
		glColor4f(124.0f/255.0f, 1.0f , 0.0f, 1.0f);
		leg_forearm.draw();
		glPopMatrix();

		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        leg_shoulder.draw();
		glPopMatrix();

		glPushMatrix();
		glColor4f(124.0f/255.0f, 1.0f , 0.0f, 1.0f);
		shoulder_to_forearm_joint.draw();
		glPopMatrix();

		glPopMatrix();

		// draw the shoulder joint
		glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		body_to_shoulder_joint.draw();
		glPopMatrix();
		
    }



};

#endif
