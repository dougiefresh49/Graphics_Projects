#pragma once
#include <gl/glut.h>
#include <math.h>
#include <vector>
using namespace std;


class Axis
{
public:
	double x;
	double y;
};

struct Frame
{
	double basePosX; 
	double basePosY;
	double frameBodyAngles[4];
	double time;

	virtual ~Frame()
	{
		delete[] frameBodyAngles;
	}

	void setFrameValues(double bX, double bY, double a1, double a2, double a3, double a4, double t)
	{
		basePosX = bX;
		basePosY = bY;
		frameBodyAngles[0] = a1;
		frameBodyAngles[1] = a2;
		frameBodyAngles[2] = a3;
		frameBodyAngles[3] = a4;
		time = t;

	}

	//// dont need these, will get angles from chosen pose
	//friend std::istream& operator>>(std::istream& is, Frame& f)
	//{
	//	is >> f.time;
	//	is >> f.basePosX >> f.basePosY;

	//	for (int i = 0; i < 4; i++) 
	//		is >> f.frameBodyAngles[i];
	//	return is;
	//}

	//friend std::ostream& operator<<(std::ostream& os, const Frame& f)
	//{
	//	os << "  == Frame ==" << std::endl
	//	<< "  Time = " << f.time << std::endl
	//	<< "  (BaseX, BaseY) = (" << f.basePosX << ", " << f.basePosY << ")" << std::endl;
	//	for (int i = 0; i < 4; i++) os << "  Angle " << i << " = " << f.frameBodyAngles[i] << std::endl;
	//	os << "  ===========";
	//	return os;
	//}

};


class Lamp
{
private:
	int numberOFFrames;
	Frame *frames;


public:

	Lamp(double baseLength, double baseHeight, double length1, double height1, double length2, double height2, double length3, double height3);
	Lamp(double baseLength, double baseHeight, double length1, double height1, double length2, double height2, double length3, double height3, double length4, double height4);
	~Lamp(void);

	//0 -- baseLength, baseHeight
	//1 -- arm1 length, arm1 height
	//2 -- arm2 length, arm2 height
	//3 -- arm3 length, arm3 height
	double length[5], height[5];
	Axis localAxisX[5], localAxisY[5];

	//freedom which controls the lamp model
	//theta0 -- base rotation
	//theta1 -- amr1 rotation
	//theta2 -- arm2 rotation
	//theta3 -- arm3 rotation
	double theta0, theta1, theta2, theta3, theta4;
	//base position
	double xPos, yPos;

	//arm end position
	//0 -- start position of base
	//1 -- start position of arm1
	//2 -- start position of arm2
	//3 -- start position of arm3
	double armXPos[5], armYPos[5];
	
	Frame* getFrames() {return frames;}

	void setFrames();
	void showFrame(int i);

	void draw();
	void draw2();

	void setJointAngle(double theta0, double theta1, double theta2, double theta3, double theta4);
	void setJointAngle2(double theta0, double theta1, double theta2, double theta3);

	void setPostion(double x, double y);

	void CalculateLocalCoordinateForEachBone();
	void CalculateLocalCoordinateForEachBone2();

	void drawEllipse(double x, double y, bool p = false);
};

