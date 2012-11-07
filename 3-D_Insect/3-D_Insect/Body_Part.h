#ifndef BODY_PART_H
#define BODY_PART_H

#include <GL/glut.h>
class Body_Part
{
protected:
	float width;
	float height;
	float depth;

	float x_location;
	float y_location;
	float z_location;

	float rotation_angle_x;
	float rotation_angle_y;
	float rotation_angle_z;

	Body_Part(float w, float h, float d = 1.0f ) : width(w), height(h), depth(d)
	{
		rotation_angle_x = 0.0f;
	    rotation_angle_y = 0.0f;
	    rotation_angle_z = 0.0f;
	}
	~Body_Part(){}

public:

	virtual void draw() = 0;

	float get_width()	{return width;}
	float get_height() {return height;}
	float get_depth() {return depth;}

	float get_x_rotation() {return rotation_angle_x;}
	float get_y_rotation() {return rotation_angle_y;}
	float get_z_rotation() {return rotation_angle_z;}

	void set_x_loc(float x) {x_location = x;}
	void set_y_loc(float y) {y_location = y;}
	void set_z_loc(float z) {z_location = z;}

	float get_x_loc() { return x_location;}
	float get_y_loc() { return y_location;}
	float get_z_loc() { return z_location;}

	void update_width(float w) { width = w;}
	void update_height(float h) {height = h;}
	void update_depth(float d) {depth = d;}

	void update_x_rotation(float xx) { rotation_angle_x = xx;}
	void update_y_rotation(float yy) {rotation_angle_y = yy;}
	void update_z_rotation(float zz) {rotation_angle_z =  zz;}

};


#endif