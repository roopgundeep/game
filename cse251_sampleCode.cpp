#include "sgl.h"

// Function Declarations
void drawScene();
void update(int value);
void drawBox(float len);
void drawBall(float rad);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y); 
void handleKeypress2(int key, int x, int y); 
void handleMouseclick(int button, int state, int x, int y);

// Global Variables
float ball_x = 1.0f;
float ball_y = 0.0f;
float ball_velx = 0.01f;
float ball_vely = 0.02f;
float ball_rad = 0.2f;
float box_len = 4.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float theta = 0.0f; 
FILE *fp;
int main(int argc, char **argv) {

    fp = fopen("output_file","w");
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    sglViewLoadIdentity();
    sglModLoadIdentity();

    sglProjOrtho(-5,5,-5,5,1,10);
//    sglProjFrustum(-5,5,-5,5,1,10);
    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("CSE251_sampleCode");  // Setup the window
    initRendering();
    // Register callbacks
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutMouseFunc(handleMouseclick);
    glutTimerFunc(10, update, 0);
    glutMainLoop();
    return 0;
}

// Function to draw objects on the screen
void drawScene() {
	akhil();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sglViewLoadIdentity();
    sglModLoadIdentity();
    sglModPushMatrix();
    sglModTranslate(0, 0, -5.0f);

    // Draw Triangle
    sglModPushMatrix();
    sglModTranslate(tri_x, tri_y, 0.0f);
    sglModRotate(theta, 0.0f, 0.0f, 1.0f);
    sglModScale(0.5f, 0.5f, 0.5f);
 /*   sglBegin();
    sglColour(0.0,1.0,0);
    sglVertex(3.2,-3.8,1);
    sglColour(1.0,0,0);
    sglVertex(0,0,1);
    sglColour(0,0,1.0);
    sglVertex(2.9,1.9,1);
    sglEnd();*/
    sglBegin();
    sglColour(0.0f, 0.0f, 1.0f);
    sglVertex(-2.0f, 1.0f, 0.0f);
    sglColour(0.0f, 1.0f, 0.0f);
    sglVertex(-3.0f, -1.0f, 0.0f);
    sglColour(1.0f, 0.0f, 0.0f);
    sglVertex(-1.0f, -1.0f, 0.0f);
    sglEnd();

    sglModPopMatrix();
    sglModPushMatrix();
    sglModTranslate(tri_x, tri_y, 0.0f);
    sglModRotate(theta, 0.0f, 0.0f, 1.0f);
    sglModScale(0.5f, 0.5f, 0.5f);
    /*sglBegin();
    sglColour(0.0,1.0,0);
    sglVertex(5,-5,1);
    sglColour(1.0,0,0);
    sglVertex(1,0,1);
    sglColour(0,0,1.0);
    sglVertex(2,1,1);
    sglEnd();*/
    sglBegin();
    sglColour(0.0f, 0.0f, 1.0f);
    sglVertex(0.0f, 2.0f, 0.0f);
    sglColour(0.0f, 1.0f, 0.0f);
    sglVertex(-1.0f, 3.0f, 0.0f);
    sglColour(1.0f, 0.0f, 0.0f);
    sglVertex(1.0f, 3.0f, 0.0f);
    sglEnd();


    sglshow();
//    sglshow();
    sglModPopMatrix();


    sglModPopMatrix();
    glutSwapBuffers();
}

void update(int value) {

	// Handle ball collisions with box
	if(ball_x + ball_rad > box_len/2 || ball_x - ball_rad < -box_len/2)
		ball_velx *= -1;
	if(ball_y + ball_rad > box_len/2 || ball_y - ball_rad < -box_len/2)
		ball_vely *= -1;

	// Update position of ball
	ball_x += ball_velx;
	ball_y += ball_vely;

	glutTimerFunc(10, update, 0);
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds

void drawBox(float len) {
   
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glVertex2f(-len / 2, -len / 2);
    glVertex2f(len / 2, -len / 2);
    glVertex2f(len / 2, len / 2);
    glVertex2f(-len / 2, len / 2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawTriangle() {

    sglBegin();
    sglVertex(3.2,-3.8,1);
    sglVertex(0,0,1);
    sglVertex(2.9,1.9,1);
    sglEnd();
}

// Initializing some openGL 3D rendering options
void initRendering() {

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
//   sglClear(0.0f, 0.0f, 0.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {
	sglViewport(0,0,w,h);
    glViewport(0,0,w,h);
}
void func()
{
	int c1=0;
	int i;
	for(i=0;i<counter;i++)
	{
		fprintf(fp,"%f %f %f\n",x_arr[i],y_arr[i],z_arr[i]);
	}
}
void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		func();
		exit(0);     // escape key is pressed
	}
}

void handleKeypress2(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT)
		tri_x -= 0.1;
	if (key == GLUT_KEY_RIGHT)
		tri_x += 0.1;
	if (key == GLUT_KEY_UP)
		tri_y += 0.1;
	if (key == GLUT_KEY_DOWN)
		tri_y -= 0.1;
}

void handleMouseclick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			theta += 15;
		else if (button == GLUT_RIGHT_BUTTON)
			theta -= 15;
	}
}

