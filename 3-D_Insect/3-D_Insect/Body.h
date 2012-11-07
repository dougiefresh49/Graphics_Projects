#ifndef BODY_H
#define BODY_H

#include "Leg.h"
#include "Head.h"
#include <string>
using namespace std;

class Body : public Body_Part
{
private: 
	Head body_head;

	Leg bottom_left;
	Leg middle_left;
	Leg top_left;

	Leg bottom_right;
	Leg middle_right;
	Leg top_right;


public:
	Body(float w, float h, float d = 0.0f) : Body_Part(w, h, d)
	{
		// setting the values for head
		body_head.update_width(w * (0.50));
		body_head.update_height(h * (0.15));
		body_head.update_depth(d * (0.8));

		body_head.set_x_loc(0.0f);
		body_head.set_y_loc(h/2);
		body_head.set_z_loc(d-1);


		// setting the values for each leg
        
        // ######### Top Left Leg #############
        top_left.update_width(w);
        top_left.update_height(h/8);
        top_left.update_depth(d);
        
        top_left.set_x_loc(-w/2 - 1);
        top_left.set_y_loc(h/2 - 0.5);
        top_left.set_z_loc(d/2);

		top_left.update_leg_values('L');
        
         //  ######### Middle Left Leg #############
        middle_left.update_width(w);
        middle_left.update_height(h/8);
        middle_left.update_depth(d );
        
        middle_left.set_x_loc(-w/2 - 1);
        middle_left.set_y_loc(0.0f);   
        middle_left.set_z_loc(d/2);

		middle_left.update_leg_values('L');
        
        // ########## Bottom Left Leg ###########
        bottom_left.update_width(w);
        bottom_left.update_height(h/8);
        bottom_left.update_depth(d);
        
        bottom_left.set_x_loc(-w/2 - 1);
		bottom_left.set_y_loc(-h/2 + 0.5);   
        bottom_left.set_z_loc(d/2);
        
		bottom_left.update_leg_values('L');

        //  ######### Top Right Leg #############
        top_right.update_width(w);
        top_right.update_height(h/8);
        top_right.update_depth(d);
        
        top_right.set_x_loc(w/2 + 1);
        top_right.set_y_loc(h/2 - 0.5);   
        top_right.set_z_loc(d/2);
        
		top_right.update_leg_values('R');
         //  ######### Middle Right Leg #############
        middle_right.update_width(w);
        middle_right.update_height(h/8);
        middle_right.update_depth(d);
        
        middle_right.set_x_loc(w/2 + 1.0f);
        middle_right.set_y_loc(0.0f);   
        middle_right.set_z_loc(d/2);
        
		middle_right.update_leg_values('R');
        // ########## Bottom Right Leg ###########
        bottom_right.update_width(w);
        bottom_right.update_height(h/8);
        bottom_right.update_depth(d);
        
        bottom_right.set_x_loc(w/2 + 1);
        bottom_right.set_y_loc(-h/2 + 0.5);    
        bottom_right.set_z_loc(d/2);
        
		bottom_right.update_leg_values('R');
	}
    
    void draw()
    {
		

		glPushMatrix();
		glRotatef((GLfloat) rotation_angle_z, 0.0, 0.0, 1.0);
		glRotatef( (GLfloat) rotation_angle_y, 0.0, 1.0, 0.0);
		glRotatef( (GLfloat) rotation_angle_x, 1.0, 0.0, 0.0);

		glPushMatrix();

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		draw_body();




		bottom_left.draw();
        middle_left.draw();
        top_left.draw();
        
        bottom_right.draw();
	    middle_right.draw();
        top_right.draw();
		
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        body_head.draw();

		glPopMatrix();
		
		glPopMatrix();

		
		
    }

	void draw_body()
	{
		//glTranslatef(x_location, y_location, z_location);
		

		glPushMatrix();
		glScalef(width, height, depth);
		glutSolidCube(1.0);
		glPopMatrix();
	
	}
void set_rotation_value_right_click(int selected_body_part,float rotation_angle)
{
        switch(selected_body_part) 
		{
		case 1:
			// top left shoulder
			if((5* rotation_angle) < 15)
				 top_left.update_z_rotation(5 * rotation_angle);
			break;
		case 2:
			// top left forearm
			if((5* rotation_angle) < 25)
				top_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 3:
			// middle left shoulder
			if((5* rotation_angle) < 15)
				middle_left.update_z_rotation(5 * rotation_angle);
		case 4:
			// middle left forearm
			if((5* rotation_angle) < 25)
				middle_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 5:
			// bottom left shoulder
			if((5* rotation_angle) < 15)
				bottom_left.update_z_rotation(5*rotation_angle);
			break;
		case 6:
			// bottom left forearm
			if((5* rotation_angle) < 25)
				bottom_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 7:
			// top right shoulder
			if((-5* rotation_angle) > -15)
				top_right.update_z_rotation(-5 *rotation_angle);
			break;
		case 8:
			// top right forearm
			if((5* rotation_angle) < 25)
				top_right.set_forearm_rotation(5 * rotation_angle);
			break;
		case 9:
			// middle right shoulder
			if((-5* rotation_angle) > -15)
				middle_right.update_z_rotation(-5 * rotation_angle);
		case 10:
			// middle right forearm
			if((-5* rotation_angle) > -25)
				middle_right.set_forearm_rotation(-5 * rotation_angle);
			break;
		case 11:
			// bottom right shoulder
			if((-5* rotation_angle) > -15)
				bottom_right.update_z_rotation(-5 * rotation_angle);
			break;
		case 12:
			// bottom right forearm
			if((-5* rotation_angle) > -25)
				bottom_right.set_forearm_rotation(- 5 * rotation_angle);
			break;
		case 13:
			// head movement
			if((-2* rotation_angle) > -25)
				body_head.update_z_rotation(-2 * rotation_angle);
			break;
			
		default:
			break;

		}

}

	void set_rotation_value_left_click(int selected_body_part,float rotation_angle)
	{

		switch(selected_body_part) 
		{
		case 1:
			// top left shoulder
			if((5* rotation_angle) < 15)
				 top_left.update_y_rotation(5 * rotation_angle);
			break;
		case 2:
			// top left forearm
			if((5* rotation_angle) < 25)
				top_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 3:
			// middle left shoulder
			if((5* rotation_angle) < 15)
				middle_left.update_y_rotation(5 * rotation_angle);
		case 4:
			// middle left forearm
			if((5* rotation_angle) < 25)
				middle_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 5:
			// bottom left shoulder
			if((5* rotation_angle) < 15)
				bottom_left.update_y_rotation(5*rotation_angle);
			break;
		case 6:
			// bottom left forearm
			if((5* rotation_angle) < 25)
				bottom_left.set_forearm_rotation(5 * rotation_angle);
			break;
		case 7:
			// top right shoulder
			if((-5* rotation_angle) > -15)
				top_right.update_y_rotation(-5 *rotation_angle);
			break;
		case 8:
			// top right forearm
			if((5* rotation_angle) < 25)
				top_right.set_forearm_rotation(5 * rotation_angle);
			break;
		case 9:
			// middle right shoulder
			if((-5* rotation_angle) > -15)
				middle_right.update_y_rotation(-5 * rotation_angle);
		case 10:
			// middle right forearm
			if((-5* rotation_angle) > -25)
				middle_right.set_forearm_rotation(-5 * rotation_angle);
			break;
		case 11:
			// bottom right shoulder
			if((-5* rotation_angle) > -15)
				bottom_right.update_y_rotation(-5 * rotation_angle);
			break;
		case 12:
			// bottom right forearm
			if((-5* rotation_angle) > -25)
				bottom_right.set_forearm_rotation(- 5 * rotation_angle);
			break;
		case 13:
			// head movement
			if((-2* rotation_angle) > -25)
				body_head.update_x_rotation(-2 * rotation_angle);
			break;
			
		default:
			break;
		}
	}

};



#endif