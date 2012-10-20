#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include<vector>
#include <utility>

using namespace std;

struct Color 
{
	float r, g, b;		// Color (R,G,B values)
	Color(){}
	Color(float rr, float gg, float bb) : r(rr), g(gg), b(bb){}
	~Color(){}
};

struct Point
{
	int x;
	int y;

	Point() {}
	Point(int xx, int yy) : x(xx), y(yy) {}
	~Point(){}
};

struct Edge
{
	Point vertex1;
	Point vertex2;
	
	int max_y_val;
	float current_x;		// x-value of endpoint with lowest y-value
	float x_increment;  // 1/slope

	Edge() {}
	Edge(Point a, Point b) 
	{
		vertex1 = a;
		vertex2 = b;

		float slope = 0.0f;

		// set currentx, max_y, and x increment
		if(a.y > b.y)
		{
			max_y_val = a.y;
			current_x = (float)b.x;
			slope = (float)( (float)(b.y - a.y)/ (float)(b.x - a.x));
			x_increment = (float)(1.0f /slope);
		}
		else if(a.y == b.y)
		{
			x_increment = 0.0f;
		}
		else
		{
			max_y_val = b.y;
			current_x = (float)a.x;
			slope = (float)( (float)(b.y - a.y)/ (float)(b.x - a.x));
			x_increment = (float)(1.0f /slope);
		}
		


	}

	~Edge(){}
	void change_value(Point a, Point b)
	{
		vertex1 = a;
		vertex2 = b;

		float slope = 0.0f;

		// set currentx, max_y, and x increment
		if(a.y > b.y)
		{
			max_y_val = a.y;
			current_x = (float)b.x;
			slope = (float)( (float)(b.y - a.y)/ (float)(b.x - a.x));
			x_increment = (float)(1.0f /slope);
		}
		else if(a.y == b.y)
		{
			x_increment = 0.0f;
		}
		else
		{
			max_y_val = b.y;
			current_x = (float)a.x;
			slope = (float)( (float)(b.y - a.y)/ (float)(b.x - a.x));
			x_increment = (float)(1.0f /slope);
		}
		
	}
};

class Polygon
{
private: 
	
	vector<Edge> edge_list;
	
	Color poly_color;

	int max_height;
	int max_width;
	int min_height;

public:
	vector<vector<Edge> > edge_table;
	vector<Point> clicked_points;
	
	
	 Polygon() { min_height = ImageH; }	   // used to set the min_height > 0 to begin so it can correctly find the actual min y value
	~Polygon(){}

	void add_new_coordinate(int x, int y, bool right_click = false)
	{ 
		// ads new pint to point list
		clicked_points.push_back(Point(x,y) ); 

			// sets the max width and height for each polygon
		if(y > max_height)
			max_height = y;
		if(y < min_height)
			min_height = y;
		if(x > max_width)
			max_width = x;

		// make the edge list
		if(clicked_points.size() > 1 && !right_click)
		{
			Edge temp_edge(clicked_points[clicked_points.size()- 2], clicked_points[clicked_points.size()- 1]);
			edge_list.push_back(temp_edge);
		}
		else if(right_click)
		{
			Edge temp_edge(clicked_points[clicked_points.size()- 2], clicked_points[clicked_points.size()- 1]);
			edge_list.push_back(temp_edge);

			temp_edge.change_value(clicked_points[clicked_points.size()- 1], clicked_points[0]);
			edge_list.push_back(temp_edge);

			create_edge_table();
		}
	}

	void set_poly_color(Color clr)	{poly_color = clr;}
	Color get_poly_color(){	return poly_color;	}
	int get_min_height(){ return min_height;}
	int get_max_x() {return max_width; }

	void create_edge_table(bool clipping = false)
	{	
		//set the size of the vector
		edge_table.resize(max_height - min_height + 1);

		

		if(clipping)
		{
			edge_table.clear();
			edge_table.resize(max_height - min_height + 1);
			for(unsigned int i = 0; i < clicked_points.size(); i++)
			{
				Edge temp_edge(clicked_points[i % clicked_points.size()], clicked_points[(i+1) % clicked_points.size()]);
				edge_list.push_back(temp_edge);
			}
		}

		

		for(int cur_y = min_height; cur_y <= max_height; cur_y++)
		{
			for(int i = 0; i < edge_list.size(); i++)
			{
				// checks if either (but not both) y value's of the endpoints of the edge are the same as current scanline value
				// ignore horizontal lines
				if((edge_list[i].vertex1.y == cur_y || edge_list[i].vertex2.y == cur_y ) && !(edge_list[i].vertex1.y == cur_y && edge_list[i].vertex2.y == cur_y))
				{
					// adds edge to active edge list
					edge_table[cur_y - min_height].push_back(edge_list[i]);

					// removes the edge that was added to the table and subtracts 1 from i for consistency
					edge_list.erase(edge_list.begin()+i);
					i--;
				}
			}
		}
	}

	
	
	
	
	
	
};

#endif