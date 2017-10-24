#include "glew.h"
#include "freeglut.h"
#include "Angel.h"
#include "mat.h"
#define NUM_OF_TRIANGLE_VERTECIES  3
#define NUM_OF_TRIANGLE 4
#define NUM_OF_CIRCLE 5
#define NUM_OF_TRIANGLE_FOR_CIRCLE 360
#define CIRCLE_RADIUS 0.1
#define DOUBLE_PI 2*M_PI

//shuriken color   : vec4(1.0, 1.0, 0.0, 1.0);
//background color : vec4(0.5, 0.5, 0.5, 1.0); 

static const int numOfTrianglesVertices = NUM_OF_TRIANGLE * NUM_OF_TRIANGLE_VERTECIES;
static const int numOfCirclesVertices = (NUM_OF_TRIANGLE_FOR_CIRCLE + 2) * NUM_OF_CIRCLE;

void makeCircle(vec2 centerPos, float radius, vec2* circleVertecies, int count) {
	//center vertex
	circleVertecies[(NUM_OF_TRIANGLE_FOR_CIRCLE + 2) * count] = centerPos;
	
	for (int i = 1; i <= NUM_OF_TRIANGLE_FOR_CIRCLE + 1; i++) {
		GLfloat COS = cos(i * DOUBLE_PI / NUM_OF_TRIANGLE_FOR_CIRCLE);
		GLfloat SIN = sin(i * DOUBLE_PI / NUM_OF_TRIANGLE_FOR_CIRCLE);

		GLfloat xPos = centerPos.x + CIRCLE_RADIUS * COS;
		GLfloat yPos = centerPos.y + CIRCLE_RADIUS * SIN;

		circleVertecies[i + (NUM_OF_TRIANGLE_FOR_CIRCLE + 2) * count] = vec2(xPos, yPos);
	}
}


void init() {
	vec2 bufferArr[numOfTrianglesVertices + numOfCirclesVertices];

	vec2 triangles[numOfTrianglesVertices] = {
		vec2(-1.0, 1.0), vec2(-1.0, -1.0), vec2(-0.5, 0.0),
		vec2(-1.0, 1.0), vec2( 0.0,  0.5), vec2( 1.0, 1.0),
		vec2( 1.0, 1.0), vec2( 0.5,  0.0), vec2( 1.0,-1.0),
		vec2( 1.0,-1.0), vec2( 0.0, -0.5), vec2(-1.0, -1.0)
	};
	
	vec2 circleCenterPos[NUM_OF_CIRCLE] = {
		vec2( 0.0, 0.0),
		vec2( 0.5, 0.0),
		vec2(-0.5, 0.0),
		vec2( 0.0, 0.5),
		vec2( 0.0,-0.5)
	};

	vec2 circleArr[numOfCirclesVertices];

	for (int i = 0; i < NUM_OF_CIRCLE; i++) {
		makeCircle(circleCenterPos[i], CIRCLE_RADIUS, circleArr, i);
	}

	for (int i = 0; i < numOfCirclesVertices; i++) {
		bufferArr[i] = circleArr[i];
	}

	for (int i = 0; i < numOfTrianglesVertices; i++) {
		bufferArr[i + numOfCirclesVertices] = triangles[i];
	}

	//Create vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Create and initalize vertex buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(bufferArr), bufferArr, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleArr), circleArr, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
	//Load shaders and use resulting shader program
	const char* vertexShader = "myShader.vert";
	const char* fragmentShader = "myFragment.frag";

	GLuint program = InitShader(vertexShader, fragmentShader);
	glUseProgram(program);
	//set up vertex array
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void display()
{
	glClearColor(0.5, 0.5, 0.5, 1.0); // gray background
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	//Drawing circles
	for (int i = 0; i < NUM_OF_CIRCLE; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, (NUM_OF_TRIANGLE_FOR_CIRCLE + 2)*i, (NUM_OF_TRIANGLE_FOR_CIRCLE + 2));
	}
	//Drawing triangles
	//glDrawArrays(GL_TRIANGLES, numOfCirclesVertices, numOfTrianglesVertices); 
	glFlush();
	//glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(800, 0);

	// If you are using freeglut, the next two lines will check if 
	// the code is truly 3.1. Otherwise, comment them out
	glutInitContextVersion(3, 1);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Ninja Star");
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}

