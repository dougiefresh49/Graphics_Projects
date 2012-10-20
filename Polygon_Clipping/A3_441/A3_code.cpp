/******************************************************************
	Notes:
	Image size is 400 by 400 by default.  You may adjust this if
		you want to.
	You can assume the window will NOT be resized.
	Call clearFramebuffer to clear the entire framebuffer.
	Call setFramebuffer to set a pixel.  This should be the only
		routine you use to set the color (other than clearing the
		entire framebuffer).  drawit() will cause the current
		framebuffer to be displayed.
	As is, your scan conversion should probably be called from
		within the display function.  There is a very short sample
		of code there now.
	You may add code to any of the subroutines here,  You probably
		want to leave the drawit, clearFramebuffer, and
		setFramebuffer commands alone, though.
  *****************************************************************/

#define ImageW 400
#define ImageH 400
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include "Polygon.h"

float framebuffer[ImageH][ImageW][3];

static int more_clicks_available = 1;
static int number_of_clicks = 0;
static int number_of_polygons = 0;

// storage lists 
static vector<Point> clicked_pixels;
static vector<int> clipping_corner_poitns;
static vector<Color> colors_used;
static vector<float> cur_x_values;
static vector<Edge> active_edge_list;
static vector<Polygon> polygon_list (10); 
static vector<Polygon> clipped_polygon_list (10); 


// defining colors for use below
static Color current_color;
static Color white(1.0f, 1.0f, 1.0f);
static Color black(0.0f, 0.0f, 0.0f);

// points of clipping square
static Point initial_clipping_point;
static Point end_clipping_point;
static Point ghost_point1;
static Point ghost_point2;

static int clipping_max_x;
static int clipping_max_y;
static int clipping_min_x;
static int clipping_min_y;

static bool draw_clipping_square = false;
static bool draw_clipped_poly = false;
static bool clipping_again = false;
void rand_color()
{
	float r = (float)(rand() % 256) / 255.0f;
	float g = (float)(rand() % 256) / 255.0f;
	float b = (float)(rand() % 256) / 255.0f;

	for(unsigned int i = 0; i < colors_used.size(); i++)
	{
		if(r == colors_used[i].r)
		{
			srand(time(NULL));
			r = (float)(rand() % 256) / 255.0f;
		}

		if(g == colors_used[i].g)
		{
			srand(time(NULL));
			g = (float)(rand() % 256) / 255.0f;
		}

		if(b == colors_used[i].b)
		{
			srand(time(NULL));
			b = (float)(rand() % 256) / 255.0f;
		}

	}

	current_color.r = r;
	current_color.g = g;
	current_color.b = b;

	colors_used.push_back(current_color);
}


// Clears framebuffer to black
void clearFramebuffer()
{
	int i,j;

	for(i=0;i<ImageH;i++) {
		for (j=0;j<ImageW;j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, Color clr)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;
	if (clr.r <= 1.0)
		if (clr.r >= 0.0)
			framebuffer[y][x][0] = clr.r;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (clr.g <= 1.0)
		if (clr.g >= 0.0)
			framebuffer[y][x][1] = clr.g;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (clr.b <=1.0)
		if (clr.b >= 0.0)
			framebuffer[y][x][2] = clr.b;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, ImageW, 0.0, ImageH);
	clearFramebuffer();
	
}
void undraw_clicked_pixels()
{
    for(unsigned int i = 0; i < clicked_pixels.size(); i++)
    {
        setFramebuffer(clicked_pixels[i].x-1, clicked_pixels[i].y, black);
        setFramebuffer(clicked_pixels[i].x-1, clicked_pixels[i].y, black);
        setFramebuffer(clicked_pixels[i].x-1, clicked_pixels[i].y-1, black);
        setFramebuffer(clicked_pixels[i].x, clicked_pixels[i].y-1, black);
    }
    clicked_pixels.clear();
}
void add_active_edges(Polygon p, int sc_line)
{
	for(unsigned int i = 0; i < p.edge_table[sc_line].size(); i++)
	{
		// adds edge to active edges
		if(p.edge_table[sc_line][i].vertex1.y != p.edge_table[sc_line][i].vertex2.y)
			active_edge_list.push_back(p.edge_table[sc_line][i]);
	}
}
void remove_edges(Polygon p, int sc_line)
{
	sc_line = sc_line + p.get_min_height();

	for(unsigned int i = 0; i < active_edge_list.size(); i++)
	{
		// checks if vertex1 or vertex 2 resides at current scanline with the other vertex < scanline, if so, edge gets removed
		if((active_edge_list[i].vertex2.y == sc_line && active_edge_list[i].vertex1.y < sc_line) || (active_edge_list[i].vertex1.y == sc_line && active_edge_list[i].vertex2.y < sc_line))
		{
			active_edge_list.erase(active_edge_list.begin() + i);
			i--;
		}

	}

	
}
void fill_pixels(Polygon p, int sc_line)
{
    // undraw the white pixels used to show user where they clicked
	undraw_clicked_pixels();
    
    sc_line = sc_line + p.get_min_height();

	// get a list of current x values (used for filling pixels)
	for(unsigned int j = 0; j < active_edge_list.size(); j++)
	{
        cur_x_values.push_back(active_edge_list[j].current_x);
    }

	// sort current x values 
	sort(cur_x_values.begin(), cur_x_values.end());

	// set pixel values to be drawn in frame buffer
	for(unsigned int i = 0; i < cur_x_values.size(); i++)
	{
		if(cur_x_values.size() - i > 1)
		{
			int filling_x_value = (int) ceil(cur_x_values[i]);
			
			while(filling_x_value <= (int) floor(cur_x_values[i + 1]))
			{
				if(filling_x_value != (int) ceil(cur_x_values[cur_x_values.size() - 1]))
				{
					setFramebuffer(filling_x_value, sc_line, p.get_poly_color());
					
				}

				filling_x_value++;
			}
		}
		i++;
	}
	
	glutPostRedisplay();
	cur_x_values.clear();
}
void increment_x_values()
{
	// increment x values on edges
	for(unsigned int j = 0; j < active_edge_list.size(); j++)
	{
		active_edge_list[j].current_x += active_edge_list[j].x_increment;
	}
}

void draw_polygon(Polygon poly)
{
	
	int scan_line = 0;

	while( scan_line < poly.edge_table.size())
	{
		add_active_edges(poly, scan_line);
		remove_edges(poly, scan_line);
		fill_pixels(poly, scan_line);
		increment_x_values();
		scan_line++;
	}
	
}
// Draws the scene
void drawit(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);

   // draws clipping square
	if(draw_clipping_square)
	{
		glColor3f(1.0,1.0,1.0);
		glBegin(GL_LINES);
			glVertex2f(initial_clipping_point.x, ImageH - initial_clipping_point.y);
			glVertex2f(ghost_point1.x,           ImageH - ghost_point1.y);
			
			glVertex2f(ghost_point1.x,           ImageH -  ghost_point1.y);
			glVertex2f(end_clipping_point.x,     ImageH - end_clipping_point.y);
			
			glVertex2f(end_clipping_point.x,     ImageH - end_clipping_point.y);
			glVertex2f(ghost_point2.x,           ImageH - ghost_point2.y);
			
			glVertex2f(ghost_point2.x,           ImageH - ghost_point2.y);
			glVertex2f(initial_clipping_point.x, ImageH - initial_clipping_point.y);
		glEnd();

	}

   glFlush();
}

void display(void)
{
	drawit();
	
}

void draw_clicked_pixels(int x, int y)
{
    setFramebuffer(x-1, y, white);
    setFramebuffer(x-1, y, white);
    setFramebuffer(x-1, y-1, white);
    setFramebuffer(x, y-1, white);

    Point temp_point(x,y);
    clicked_pixels.push_back(temp_point);
    
    glutPostRedisplay();

}

void draw_clipping_rectangle()
{
     
    if(initial_clipping_point.y < end_clipping_point.y)
    {
        ghost_point1.x = end_clipping_point.x;
        ghost_point1.y = initial_clipping_point.y;
        
        ghost_point2.x = initial_clipping_point.x;
        ghost_point2.y = end_clipping_point.y;
		
		// determine min and max values for clipping lines
		clipping_max_y = end_clipping_point.y;
		clipping_min_y = initial_clipping_point.y;
		
		if(initial_clipping_point.x < end_clipping_point.x)
		{
			clipping_max_x = end_clipping_point.x;
			clipping_min_x = initial_clipping_point.x;
		}
		else
		{
			clipping_max_x = initial_clipping_point.x; 
			clipping_min_x = end_clipping_point.x;
		}
    }
    
    else if(initial_clipping_point.y > end_clipping_point.y)
    {
        ghost_point1.x = initial_clipping_point.x;
        ghost_point1.y = end_clipping_point.y;
        
        ghost_point2.x = end_clipping_point.x;
        ghost_point2.y = initial_clipping_point.y;

		// determine min and max values for clipping lines
		clipping_min_y = end_clipping_point.y;
		clipping_max_y = initial_clipping_point.y;


		if(initial_clipping_point.x < end_clipping_point.x)
		{
			clipping_max_x = end_clipping_point.x;
			clipping_min_x = initial_clipping_point.x;
		}
		else
		{
			clipping_max_x = initial_clipping_point.x; 
			clipping_min_x = end_clipping_point.x;
		}
    }
    
   draw_clipping_square = true;
   
  
   
   
   
}

void clip_mouse_movement(int x, int y)
{
   end_clipping_point.x = x;
   end_clipping_point.y = y;

   draw_clipping_rectangle();
   glutPostRedisplay();
}

void mouse_movement(int x, int y)
{

}

void draw_polygons_mouse(int button, int state, int x, int y)
{
	
	if(more_clicks_available && (number_of_polygons < 10)) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN  && (number_of_clicks < 9))
			{
				// get the current x and y coordinate
				// have a counter so they can only click left 9 times (DONE)

				polygon_list[number_of_polygons].add_new_coordinate(x, y);
				number_of_clicks++;
                
                draw_clicked_pixels(x,y);
			}
			else if(state == GLUT_UP)
			{
				// not sure if I need this

			}
			break; 

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN  && (number_of_clicks >= 2 && number_of_clicks < 10))
			{
				// get the current x,y coordinate as the last vertex of the polygon
				polygon_list[number_of_polygons].add_new_coordinate(x, y, true);
                draw_clicked_pixels(x,y);
                
                // draw polygon with random color
				rand_color();
				polygon_list[number_of_polygons].set_poly_color(current_color);
				draw_polygon(polygon_list[number_of_polygons]);


				// reset number of clicks and increment number of polygons used
				number_of_clicks = 0;
				number_of_polygons++;
			}
			else if(state == GLUT_UP)
			{
				// not sure if I need this

			}
			break;

		default:
			break;

		}
	}

}

// 1 for x and 1 for Left and 1 for Top
Point determine_intersections(Point a, Point b, int clipping_x_or_y, int L_or_R = 0, int T_or_B  = 0)
{
	Point point_to_return;
	float slope = (float)(((float)b.y - (float)a.y) / ((float) b.x - (float) a.x));
	
	// right
	if(clipping_x_or_y && !L_or_R)
	{
		
		int y_to_return = a.y + slope*(clipping_max_x - a.x);
		point_to_return.x = clipping_max_x;
		point_to_return.y = y_to_return;
	
	}
	// bottom
	else if(!clipping_x_or_y && !T_or_B)
	{
		int x_to_rtn = a.x + (clipping_max_y - a.y)/slope;
		point_to_return.x = x_to_rtn;
		point_to_return.y = clipping_max_y;
	}

	// left
	else if(clipping_x_or_y && L_or_R)
	{
		int y_to_return = a.y + slope*(clipping_min_x - a.x);
		point_to_return.x = clipping_min_x;
		point_to_return.y = y_to_return;
	}

	else if(!clipping_x_or_y && T_or_B)
	{
		int x_to_rtn = a.x + (clipping_min_y - a.y)/slope;
		point_to_return.x = x_to_rtn;
		point_to_return.y = clipping_min_y;
	}

	return point_to_return;
}


void perform_clipping()
{
	clipped_polygon_list.clear();
	clipped_polygon_list.resize(number_of_polygons);
	
	for(unsigned int j = 0; j < number_of_polygons; j++)
	{
		vector<Point> clipped_point_vec = polygon_list[j].clicked_points;
		clipped_polygon_list[j] = polygon_list[j];  
	//	clipped_polygon_list[j].set_poly_color(white);
		for(unsigned int i = 0; i < clipping_corner_poitns.size(); i++)
		{
			vector<Point> temp_point_vec;
			
			for(unsigned int k = 0; k < clipped_point_vec.size(); k++)
			{
				
				Point temp_a = clipped_point_vec[k % clipped_point_vec.size()]; 
				Point temp_b = clipped_point_vec[(k + 1) % clipped_point_vec.size()];

				// compare to left clipping border
				if(i == 0)
				{
					// s outside e inside
					if(temp_a.x < clipping_corner_poitns[i] && temp_b.x > clipping_corner_poitns[i])
					{
						
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 1, 1));
						temp_point_vec.push_back(temp_b);										
					}

					// s inside e outside
					else if(temp_a.x > clipping_corner_poitns[i] && temp_b.x < clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 1, 1));
		
					}

					// s inside e inside
					else if(temp_a.x > clipping_corner_poitns[i] && temp_b.x > clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(temp_b);
					}

				}

				// compare to  right clipping border
				else if(i == 1)
				{
					// s outside e inside
					if(temp_a.x > clipping_corner_poitns[i] && temp_b.x < clipping_corner_poitns[i])
					{
						
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 1, 0));
						temp_point_vec.push_back(temp_b);										
					}

					// s inside e outside
					else if(temp_a.x < clipping_corner_poitns[i] && temp_b.x > clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 1, 0));
		
					}

					// s inside e inside
					else if(temp_a.x < clipping_corner_poitns[i] && temp_b.x < clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(temp_b);
					}

				}

				// compare to bottom clipping border
				else if(i == 2)
				{
					// s outside e inside
					if(temp_a.y > clipping_corner_poitns[i] && temp_b.y < clipping_corner_poitns[i])
					{
						
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 0, 0, 0));
						temp_point_vec.push_back(temp_b);										
					}

					// s inside e outside
					else if(temp_a.y < clipping_corner_poitns[i] && temp_b.y > clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 0, 0, 0));
		
					}

					// s inside e inside
					else if(temp_a.y < clipping_corner_poitns[i] && temp_b.y < clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(temp_b);
					}

				}

				// compare to top clipping border
				else if(i == 3)
				{
					// s outside e inside
					if(temp_a.y < clipping_corner_poitns[i] && temp_b.y > clipping_corner_poitns[i])
					{
						
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 0, 0, 1));
						temp_point_vec.push_back(temp_b);										
					}

					// s inside e outside
					else if(temp_a.y > clipping_corner_poitns[i] && temp_b.y < clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(determine_intersections(temp_a, temp_b, 0, 0, 1));
		
					}

					// s inside e inside
					else if(temp_a.y > clipping_corner_poitns[i] && temp_b.y > clipping_corner_poitns[i])
					{
						temp_point_vec.push_back(temp_b);
					}

					
				}
				
			}
			clipped_point_vec = temp_point_vec;
			
		}
		clipped_polygon_list[j].clicked_points = clipped_point_vec;
	}

	clearFramebuffer();
	//glutPostRedisplay();
		for(unsigned int i = 0; i < clipped_polygon_list.size(); i++)
		{
			if(clipped_polygon_list[i].get_max_x() > 0)
			{
				clipped_polygon_list[i].create_edge_table(true);
				draw_polygon(clipped_polygon_list[i]);
			}

		}
		clipping_corner_poitns.clear();
	
}

void clip_polygons_mouse(int button, int state, int x, int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN && number_of_clicks < 1)
        {
            // send clicked point to some function then keep redrawing lines for square
            initial_clipping_point.x = x;
            initial_clipping_point.y = y;
            
            number_of_clicks++;
            
           
        }
	
		else if(state == GLUT_UP && number_of_clicks < 2)
		{
			end_clipping_point.x = x;
			end_clipping_point.y = y;
			number_of_clicks++;

			clipping_corner_poitns.push_back(clipping_min_x);
			clipping_corner_poitns.push_back(clipping_max_x);
			clipping_corner_poitns.push_back(clipping_max_y);
			clipping_corner_poitns.push_back(clipping_min_y);
			
			perform_clipping();
			
			glutPostRedisplay();
			glutMotionFunc(mouse_movement);	
			
		}
		break;
    }
}

void key_board(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'c':
		// no more clicks and clipping now begins
        glutMouseFunc(clip_polygons_mouse);
        glutMotionFunc(clip_mouse_movement);
        number_of_clicks = 0;
		draw_clipping_square = false;
		if(clipping_again)
		{
			for(unsigned int j = 0; j < number_of_polygons; j++)
			{
				
				draw_polygon(polygon_list[j]);
			
			}
			
		}
		else
			clipping_again = true;
		glutPostRedisplay();
		break;
	case 'e':
		// clear screen and all lists so the user can draw again
		clearFramebuffer();

		glutMouseFunc(draw_polygons_mouse);
		glutMotionFunc(mouse_movement);
		more_clicks_available = 1;
		number_of_clicks = 0;
		number_of_polygons = 0;
		draw_clipping_square = false;
		colors_used.clear();
		polygon_list.clear();
		polygon_list.resize(10);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(ImageW,ImageH);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Doug Lasater - Homework 3");
	init();	
	glutDisplayFunc(display);

	glutMouseFunc(draw_polygons_mouse);
	glutMotionFunc(mouse_movement);
	glutKeyboardFunc(key_board);

	glutMainLoop();
	return 0;
}
