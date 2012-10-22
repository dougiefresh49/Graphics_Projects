
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>

static GLfloat spin = 0.0;
static float spin_constant = 0.5f;

static int window_size_h = 250;
static int window_size_w = 250;

static float temp_R; 
static float temp_G ;
static	float temp_B;

static int right_spin = 0;
static int left_spin = 0;

// line options for triangle
static int line_style = GL_POLYGON;

// mouse position variables
static int home_click_position_x = 0;
static int home_click_position_y = 0;
static int last_x_pos = 0;
static int last_y_pos = 0;

static int drag_or_drop = 0;
static int current_color = 0;
// color array
static float color_change_constant = 0.0f;
static float rgb_values[3] = {0.83, 0.83, 0.83};
static float current_values[9] = {0.0,0.0,0.0,   0.5,0.5,0.5,   0.82,0.82,0.82};


// first three values are min colors, next three are middle colors, max color values 
static int black[9]   = {0,0,0,			 128,128,128,	 211,211,211}; 
static int cyan[9]    = {72,209,204,     224,255,255,    0,255,255};
static int magenta[9] = {139,102,139,    238,130,238,    255,0,255};
static int white[9]   = {139,137,137,    255,250,250,    255,255,255};
static int yellow[9]  = {184,134,11,     255,255,224,    255,255,0};	


void init (void)
{
	glClearColor(rgb_values[0], rgb_values[1], rgb_values[2], 0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	if(drag_or_drop == 1)
		glTranslatef(((float)last_x_pos - (float)window_size_w/2.0f), ((float)window_size_h/2.0f - (float)last_y_pos), 1.0f);
	glRotatef(spin, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0f);
	glBegin(line_style);
		glVertex2f(0.0, 40.0);
		glVertex2f(-25.0, -25.0);	
		glVertex2f(-25.0, -25.0);
		glVertex2f(25.0, -25.0);
		glVertex2f(25.0, -25.0);
		glVertex2f(0.0, 40.0);		
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

void spinDisplay_Left(void)
{
	spin = spin + .2*spin_constant;

	if(spin > 360.0)
		spin = spin - 360.0;

	glutPostRedisplay();
}
void spinDisplay_Right(void)
{
	spin = spin - .2* spin_constant;

	if(spin < -360.0f)
		spin = spin + 360.0f;
	
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void change_colors(float r, float g, float b, int color_pressed)
{
	if(color_pressed == 0)
	{
		int i = 0;
		for(i; i< 9; i++)
			current_values[i] = (float)black[i]/255.0f;
	}
	else if(color_pressed == 1)
	{
		int i = 0;
		for(i; i< 9; i++)
			current_values[i] = (float)cyan[i]/255.0f;
	}
	else if(color_pressed == 2)
	{
		int i = 0;
		for(i; i< 9; i++)
			current_values[i] = (float)magenta[i]/255.0f;
	}
	else if(color_pressed == 3)
	{
		int i = 0;
		for(i; i< 9; i++)
			current_values[i] = (float)white[i]/255.0f;
	}
	else if(color_pressed == 4)
	{
		int i = 0;
		for(i; i< 9; i++)
			current_values[i] = (float)yellow[i]/255.0f;
	}
	
	current_color = color_pressed;

	rgb_values[0] = r;
	rgb_values[1] = g;
	rgb_values[2] = b;

	glClearColor(rgb_values[0], rgb_values[1], rgb_values[2], 0.0);
}

void key_board(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '1':
		// the polygon should be displayed as a set of points marking the vertices only.
		line_style = GL_POINTS; 
		break;
	case '2':
		//  the polygon should be displayed as a set of lines showing the outline of the polygon only
		line_style = GL_LINES;
		break;
	case '3':
		// the polygon should be filled in completely.
		line_style = GL_POLYGON;
		break;
	case 'b':
		change_colors((float)black[3]/255.0f, (float)black[4]/255.0f, (float)black[5]/255.0f, 0);
		break;
	case 'c':
		change_colors((float)cyan[3]/255.0f, (float)cyan[4]/255.0f, (float)cyan[5]/255.0f, 1);
		break;
	case 'm':
		change_colors((float)magenta[3]/255.0f, (float)magenta[4]/255.0f, (float)magenta[5]/255.0f, 2);
		break;
	case 'w':
		change_colors((float)white[3]/255.0f, (float)white[4]/255.0f, (float)white[5]/255.0f, 3);
		break;
	case 'y':
		change_colors((float)yellow[3]/255.0f, (float)yellow[4]/255.0f,(float) yellow[5]/255.0f, 4);
	default:
		break;

	}
	glutPostRedisplay();
}

void mouse_movement(int x, int y)
{
	spin_constant = (1.0f + ((float)home_click_position_y - (float)y)/(float)window_size_h);
	color_change_constant = ((float)x /(float)window_size_w);

	if(spin_constant < 0.0f )
		spin_constant = 0.0f;

	// adjust colors based on max and min colors chosen
	temp_R =  current_values[0] + (color_change_constant * ((current_values[6] - current_values[0])));
	temp_G =  current_values[1] + (color_change_constant * ((current_values[7] - current_values[1])));
	temp_B =  current_values[2] + (color_change_constant * ((current_values[8] - current_values[2])));
	
	change_colors(temp_R, temp_G, temp_B, current_color);

	last_y_pos = y;
	last_x_pos = x;	
}
void drag_drop(void)
{
	if(left_spin == 1)
	{
		//glutIdleFunc(NULL);
		glutIdleFunc(spinDisplay_Left);
	}
	else if(right_spin == 1)
	{
		//glutIdleFunc(NULL);
		glutIdleFunc(spinDisplay_Right);
	}
	else
		glutPostRedisplay();

}
void mouse(int button, int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{
			left_spin = 1;
			spin_constant = 0.5f;
			glutIdleFunc(spinDisplay_Left);

			home_click_position_x = x;
			home_click_position_y = y;

			last_x_pos = home_click_position_x;
			last_y_pos = home_click_position_y;
		}
		else if(state == GLUT_UP)
		{
			left_spin = 0;
			glutIdleFunc(NULL);
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN)
		{
			right_spin = 1;
			spin_constant = 0.5f;
			glutIdleFunc(spinDisplay_Right);
			
			home_click_position_x = x;
			home_click_position_y = y;

			last_x_pos = home_click_position_x;
			last_y_pos = home_click_position_y;
		}
		else if(state == GLUT_UP)
		{
			right_spin = 0;
			glutIdleFunc(NULL);
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			drag_or_drop = 1;
			glutIdleFunc(drag_drop);
		}
		else if(state == GLUT_UP)
			drag_or_drop = 0;
		break;
      default:
         break;
	}
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (window_size_w, window_size_h); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Doug Lasater - Assignment 2");	// Window titled with name - assignment 2
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 

   glutMouseFunc(mouse);
   glutMotionFunc(mouse_movement);

   glutKeyboardFunc(key_board);
   glutMainLoop();
   return 0;
}
