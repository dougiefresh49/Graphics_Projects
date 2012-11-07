
#include "Body.h"

static Body insect_body(5.0,10.0,2.0);
static int shoulder = 0, elbow = 0;
static int current_body_part_selected = 0;

static int WIDTH = 500;
static int HEIGHT = 500;

static int center_x = WIDTH/2;
static int center_y = HEIGHT/2;

static int initial_x_clicked;
static int initial_y_clicked;

static int zoom = 65;

static float y_view = 0.0f;
static float x_view = 0.0f;

// true for left false for right
static bool left_or_right_clicked = true;

struct vec{
	float x,y;
};

void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	
	glViewport (0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective((double)zoom, (GLfloat) WIDTH/(GLfloat) HEIGHT, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (x_view, y_view, -21.0);
	
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();

   insect_body.draw();
   glPopMatrix();
   glutSwapBuffers();
}


void function_keys(int key, int x, int y)
{
	 switch (key) 
	 {
	 case GLUT_KEY_UP:
		 // zoom in 
		 insect_body.update_x_rotation(insect_body.get_x_rotation() + 5);
		 glutPostRedisplay();
		 break;
	 case GLUT_KEY_DOWN:
		 // zoom out
		  insect_body.update_x_rotation(insect_body.get_x_rotation() - 5);
		glutPostRedisplay();
	 case GLUT_KEY_LEFT:
		 insect_body.update_y_rotation(insect_body.get_y_rotation() - 5);
		 glutPostRedisplay();
		 break;
	 case GLUT_KEY_RIGHT:
		 insect_body.update_y_rotation(insect_body.get_y_rotation() + 5);
		 glutPostRedisplay();
		 break;
	 case GLUT_KEY_PAGE_UP:
		 // scroll up to look at bug
		 y_view += 5;
		 glutPostRedisplay();
		 break;
	 case GLUT_KEY_PAGE_DOWN:
		 // scroll down 
		  y_view -= 5;
		// insect_body.update_z_rotation(insect_body.get_z_rotation() - 5);
		 glutPostRedisplay();
		 break;
	 case GLUT_KEY_F1:
	   // top left shoulder
	   current_body_part_selected = 1;
	   break;
	case GLUT_KEY_F2:
	   // top left forearm
	   current_body_part_selected = 2;
	   break;
   case GLUT_KEY_F3:
	   // middle left shoulder
	   current_body_part_selected = 3;
	   break;
	case GLUT_KEY_F4:
	   // middle left forearm
		current_body_part_selected = 4;
	   break;
   case GLUT_KEY_F5:
	   // bottom left shoulder
	   current_body_part_selected = 5;
	   break;
	case GLUT_KEY_F6:
	   // bottom  left forearm
		current_body_part_selected = 6;
	   break;
	case GLUT_KEY_F7:
	   // top right shoulder
		current_body_part_selected = 7;
	   break;
	case GLUT_KEY_F8:
	   // top right forearm
		current_body_part_selected = 8;
	   break;
   case GLUT_KEY_F9:
	   // middle right shoulder
	   current_body_part_selected = 9;
	   break;
	case GLUT_KEY_F10:
	   // middle right forearm
		current_body_part_selected = 10;
	   break;
   case GLUT_KEY_F11:
	   // bottom right shoulder
	   current_body_part_selected = 11;
	   break;

	 }
}
void keyboard (unsigned char key, int x, int y)
{
   switch (key) 
   {
   case '+':
	   zoom -= 5;
	   glutPostRedisplay();
	   break;
   case '-':
	   zoom += 5;
	   glutPostRedisplay();
	   break;
   case '/':
	   x_view -= 1;
	   glutPostRedisplay();
	   break;
   case '*':
	   x_view += 1;
	   glutPostRedisplay();
	   break;
   case 'e':
	   // unselect any part that is currently selected
	   current_body_part_selected = 0;
	   break;
   case '1':
	// bottom right forearm
	current_body_part_selected = 12;
	   break;

	case 'h':
		// rotate the head
		current_body_part_selected = 13;
		break;
      default:
         break;
   }
}

void mouse_clicked(int button, int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{
			left_or_right_clicked = true;
			initial_x_clicked = x;
			initial_y_clicked = y;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN)
		{
			left_or_right_clicked = false;
			initial_x_clicked = x;
			initial_y_clicked = y;
		}
	}
	
	

}

float dot_product(vec a, vec b)
{
  return (a.x*b.x + a.y*b.y );
}

vec normalize(vec a) 
{
  double term = 1.0/sqrt( pow((a.x), 2) + pow((a.y), 2) );
  vec b = {a.x*term , a.y*term};
  return b;
}


void mouse_movement(int x, int y)
{

	vec vecA;
	vec vecB;

	vecA.x = initial_x_clicked;
	vecA.y = initial_y_clicked;

	vecB.x = x;
	vecB.y = y;

	float rotation_angle =   acos( dot_product( normalize(vecA), normalize(vecB)) );
	rotation_angle *= 10;

	if(left_or_right_clicked)
		insect_body.set_rotation_value_left_click(current_body_part_selected, rotation_angle );
	else
		insect_body.set_rotation_value_right_click(current_body_part_selected, rotation_angle );

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (WIDTH, HEIGHT); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutMouseFunc(mouse_clicked);
   glutMotionFunc(mouse_movement);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(function_keys);
   glutMainLoop();
   return 0;
}