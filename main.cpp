#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "freeglut.lib")
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Shader shader; // loads our vertex and fragment shaders
Shader shaderg; // loads our vertex and fragment shaders
Model *cylinderLL; //Left leg
Model *cylinderRL; //Right leg
Model *cylinderRA; //Right arm
Model *cylinderLA; //Left arm
Model *cylinderBD; //body
Model *cylinderHD; //head
Model *plane; //a plane
Model *sphere; //a sphere
Model *heli; //a sphere
Model *dragon; //a sphere
glm::mat4 projection; // projection matrix
glm::mat4 view; // where the camera is looking
glm::mat4 modelLL; // where the model (i.e., the myModel) is located wrt the camera, legs
glm::mat4 modelRL; // where the model (i.e., the myModel) is located wrt the camera, legs
glm::mat4 modelLA; // arms
glm::mat4 modelRA; // arms
glm::mat4 modelHB; //head and body
glm::vec3 moveCam = glm::vec3(0.0f, 4.0f, 0.0f);
glm::vec3 center = glm::vec3(0.0f, 4.0f, 1.0f);
glm::vec3 moveCamf = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 centerf = glm::vec3(0.0f, 0.0f, 0.0f);

int mode = 0;
float angle = 0;
float angleT = 0;
float moverZ = 0;
float moverX = 0;
//float angleX = 0;
//float angleZ = -1;
bool swingF = false;
bool swingB = false;
bool turnR = false;
bool turnL = false;
bool setup = true; 
bool DOOM = false; 
bool flyCam = false; 
bool flyTR = false;
bool flyTL = false;
bool flyTU = false;
bool flyTD = false;



/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	shaderg.InitializeFromFile("shaders/phong.vert", "shaders/green.frag");
	shaderg.AddAttribute("vertexPosition");
	shaderg.AddAttribute("vertexNormal");

	checkError ("initShader");
}

void initRendering(void)
{
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError ("initRendering");
}

void init(void) 
{	
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	
	// Load identity matrix into model matrix (no initial translation or rotation)
	

	initShader ();
	initRendering ();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{
	glm::vec3 x1 = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), center - moveCam));
	glm::vec3 lookDir = glm::normalize(center - moveCam);

	if (!DOOM && !flyCam) {
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(DOOM){
		view = glm::lookAt(moveCam, center, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(-angleT, 0.0f, 1.0f, 0.0f);
	}
	else if (flyCam) {
		view = glm::lookAt(moveCamf, centerf, glm::vec3(0.0f, 1.0f, 0.0f));
	}



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

	//left
	if (fmod(angleT, 360.0f) == 90 || fmod(angleT, -360.0f) == -270) {
		//screen right forward
		if (angle < 45 && swingF == true) {
			modelLL = glm::translate(moverX, -1.0f, moverZ + 1.5f) * glm::rotate(angle += 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move forward
		else if (angle > 0) {
			swingF = false;
			modelLL = glm::translate((moverX += .05f), -1.0f, moverZ + 1.5f) * glm::rotate(angle -= 1.0f, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			//view = glm::translate(-moverX, -4.0f, moverZ) * glm::rotate(-angleT, 0.0f, 1.0f, 0.0f) * glm::rotate(180.0f, 0.0f, 1.0f, 0.0f);
			moveCam += lookDir * .05f;
			center += lookDir * .05f;
		}
		//screen left backwards
		if (angle > -45 && swingB == true) {
			modelLL = glm::translate(moverX, -1.0f, moverZ + 1.5f) * glm::rotate(angle -= 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move backwards
		else if (angle < 0) {
			swingB = false;
			modelLL = glm::translate((moverX -= .05f), -1.0f, moverZ + 1.5f) * glm::rotate(angle += 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam -= lookDir * .05f;
			center -= lookDir * .05f;
		}
	}
	//right
	else if (fmod(angleT, 360) == 270 || fmod(angleT, -360) == -90) {
		//screen left
		if (angle > -45 && swingF == true) {
			modelLL = glm::translate(moverX, -1.0f, moverZ + 1.5f) * glm::rotate(angle -= 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move forward
		else if (angle < 0) {
			swingF = false;
			modelLL = glm::translate((moverX -= .05f), -1.0f, moverZ + 1.5f) * glm::rotate(angle += 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam += lookDir * .05f;
			center += lookDir * .05f;
		}
		//screen right backward
		if (angle < 45 && swingB == true) {
			modelLL = glm::translate(moverX, -1.0f, moverZ + 1.5f) * glm::rotate(angle += 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move backward
		else if (angle > 0) {
			swingB = false;
			modelLL = glm::translate((moverX += .05f), -1.0f, moverZ + 1.5f) * glm::rotate(angle -= 1, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX, -1.0f, moverZ - 1.5f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX, 2.0f, moverZ + 4.0f) * glm::rotate(-angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX, 2.0f, moverZ - 4.0f) * glm::rotate(angle, 0.0f, 0.0f, 1.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam -= lookDir * .05f;
			center -= lookDir * .05f;
		}
	}
	//faced towards
	else if (fmod(angleT, 360) == 0) {
		//toward camera
		if (angle < 45 && swingF == true) {
			modelLL = glm::translate(moverX - 1.5f, -1.0f, moverZ) * glm::rotate(angle += 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move forward
		else if (angle > 0) {
			swingF = false;
			modelLL = glm::translate(moverX - 1.5f, -1.0f, (moverZ += .05f)) * glm::rotate(angle -= 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam += lookDir*.05f;
			center += lookDir*.05f;
		}
		//away from camera
		if (angle > -45 && swingB == true) {
			modelLL = glm::translate(moverX - 1.5f, -1.0f, moverZ) * glm::rotate(angle -= 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move backward
		else if (angle < 0) {
			swingB = false;
			modelLL = glm::translate(moverX - 1.5f, -1.0f, (moverZ -= .05f)) * glm::rotate(angle += 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam -= lookDir * .05f;
			center -= lookDir * .05f;
		}
	}
	//faced away
	else if (fmod(angleT, 360) == 180|| fmod(angleT, -360) == -180) {
		//toward camera
		if (angle < 45 && swingB == true) {
			modelLL = glm::translate(moverX - 1.5f, -1.0f, moverZ) * glm::rotate(angle += 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move forward
		else if (angle > 0) {
			swingB = false;
			modelLL = glm::translate(moverX - 1.5f, -1.0f, (moverZ += .05f)) * glm::rotate(angle -= 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam -= lookDir * .05f;
			center -= lookDir * .05f;
		}
		//away from camera
		if (angle > -45 && swingF == true) {
			modelLL = glm::translate(moverX - 1.5f, -1.0f, moverZ) * glm::rotate(angle -= 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
		}
		//move legs back and move backward
		else if (angle < 0) {
			swingF = false;
			modelLL = glm::translate(moverX - 1.5f, -1.0f, (moverZ -= .05f)) * glm::rotate(angle += 1, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRL = glm::translate(moverX + 1.5f, -1.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelLA = glm::translate(moverX - 4.0f, 2.0f, moverZ) * glm::rotate(-angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelRA = glm::translate(moverX + 4.0f, 2.0f, moverZ) * glm::rotate(angle, 1.0f, 0.0f, 0.0f) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			modelHB = glm::translate(moverX, 0.0f, moverZ) * glm::rotate(angleT, 0.0f, 1.0f, 0.0f);
			moveCam += lookDir * .05f;
			center += lookDir * .05f;
		}
	}

	//turn right
	if (turnR == true) {
		modelHB = glm::translate(moverX + 0.0f, 0.0f, moverZ +0.0f) * glm::rotate(angleT -= 1, 0.0f, 1.0f, 0.0f);
		modelLL = modelHB * glm::translate(-1.5f, -1.0f, 0.0f); //DON'T TOUCH
		modelRL = modelHB * glm::translate(1.5f, -1.0f, 0.0f); //DON'T TOUCH
		modelLA = modelHB * glm::translate(-4.0f, 2.0f, 0.0f); //DON'T TOUCH
		modelRA = modelHB * glm::translate(4.0f, 2.0f, 0.0f); //DON'T TOUCH
		/*if(center.z != moverZ)
			center.z -= .0111;
		if (center.x != moverX-1)
			center.x -= .0111;*/

		if (fmod(angleT, 90.0f) == 0.0f)
			turnR = false;
	}
	//turn left
	if (turnL == true) {
		/*if (fmod(angleT, 360) == 0)
			faceF = true;
		if (fmod(angleT, 360.0f) == 90 || fmod(angleT, -360.0f) == -270)
			faceL = true;
		if (fmod(angleT, 360) == 270 || fmod(angleT, -360) == -90)
			faceR = true;
		if (fmod(angleT, 360) == 180 || fmod(angleT, -360) == -180)
			faceB = true;*/
		
		modelHB = glm::translate(moverX + 0.0f, 0.0f, moverZ + 0.0f) * glm::rotate(angleT += 1, 0.0f, 1.0f, 0.0f);
		modelLL = modelHB * glm::translate(-1.5f, -1.0f, 0.0f); //DON'T TOUCH
		modelRL = modelHB * glm::translate(+1.5f, -1.0f, 0.0f); //DON'T TOUCH
		modelLA = modelHB * glm::translate(-4.0f, 2.0f, 0.0f); //DON'T TOUCH
		modelRA = modelHB * glm::translate(4.0f, 2.0f, 0.0f); //DON'T TOUCH
		//center += x1;
		//view *= glm::rotate(-angleT, 0.0f, 1.0f, 0.0f);

		//if (faceF) {
		//	if (center.z != moverZ)
		//		center.z -= 1.0f;
		//	if (center.x != moverX + 1.0f)
		//		center.x += 1.0f;
		//	if (center.z == moverZ && center.x == moverX + 1.0f)
		//		faceF = false;
		//	//center += x1;
		//}
		//if (faceL) {
		//	if (center.z != moverZ - 1.0f)
		//		center.z -= 1.0f;
		//	if (center.x != moverX)
		//		center.x -= 1.0f;
		//	if (center.z == moverZ - 1.0f && center.x == moverX)
		//		faceL = false;
		//	//center += x1 * .01756f;
		//}
		//if (faceR) {
		//	if (center.z != moverZ + 1.0f)
		//		center.z += 1.0f;
		//	if (center.x != moverX)
		//		center.x += 1.0f;
		//	if (center.z == moverZ + 1.0f && center.x == moverX)
		//		faceR = false;
		//	//center += x1 * .01756f;
		//}
		//if (faceB) {
		//	if (center.z != moverZ)
		//		center.z += 1.0f;
		//	if (center.x != moverX - 1.0f)
		//		center.x += 1.0f;
		//	if (center.z == moverZ && center.x == moverX - 1.0f)
		//		faceB = false;
		//	//center += x1 * .01756f;
		//}

		if (fmod(angleT, 90.0f) == 0.0f)
			turnL = false;

	}



	cylinderLL->render(view * modelLL * glm::translate(0.0f,-2.0f,0.0f) * glm::scale(1.0f,2.0f,1.0f), projection); // Render current active model.
	cylinderRL->render(view * modelRL * glm::translate(0.0f,-2.0f,0.0f) * glm::scale(1.0f,2.0f,1.0f), projection);
	cylinderLA->render(view * modelLA * glm::translate(0.0f,-2.0f,0.0f) * glm::scale(1.0f,2.0f,1.0f), projection);
	cylinderRA->render(view * modelRA * glm::translate(0.0f,-2.0f,0.0f) * glm::scale(1.0f,2.0f,1.0f), projection);
	if (!DOOM) {
		cylinderHD->render(view* modelHB* glm::translate(0.0f, 3.0f, 0.0f)* glm::scale(1.0f, 1.0f, 1.0f), projection);
	}
	
		
	cylinderBD->render(view * modelHB * glm::translate(0.0f,1.0f,0.0f) * glm::scale(3.0f,2.0f,3.0f), projection);
	//move models to their place as a body
	if (setup == true) {
		modelLL = glm::translate(-1.5f, -1.0f, 0.0f);
		modelRL = glm::translate(1.5f, -1.0f, 0.0f);
		modelRA = glm::translate(4.0f, 2.0f, 0.0f);
		modelLA = glm::translate(-4.0f, 2.0f, 0.0f);
		modelHB = glm::translate(0.0f, 0.0f, 0.0f);

		setup = false;
	}
	
	// sphere is a child of the cylinder
	//sphere->render(view * model*glm::translate(2.0f, 0.0f,0.0f), projection);


	plane->render(view * glm::translate(0.0f,-5.0f,0.0f)*glm::scale(50.0f,1.0f,50.0f), projection);
	heli->render(view * glm::translate(20.0f,0.0f,0.0f)* glm::scale(.1f, .1f, .1f), projection);
	dragon->render(view * glm::translate(-20.0f,0.0f,0.0f)* glm::scale(.01f, .01f, .01f) * glm::rotate(90.0f, 0.0f, 1.0f, 0.0f), projection);
	
	glutSwapBuffers(); // Swap the buffers.
	checkError ("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}

/*Called when the window is resized*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	checkError ("reshape");
}

void specialkey(int key, int x, int y) {
	glm::vec3 x1f = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), centerf - moveCamf));
	glm::vec3 x1t = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), centerf - moveCamf));
	glm::vec3 lookDirf = glm::normalize(centerf - moveCamf);

	switch (key) {
	case GLUT_KEY_LEFT:
		centerf += x1f * .5f;
		printf("THIS IS %lf\n", centerf.x);
		printf("THIS IS %lf\n", moveCamf.x);
		
		break;
	case GLUT_KEY_RIGHT:
		centerf += x1f * -.5f;
		printf("THIS IS %lf\n", centerf.x);
		printf("THIS IS %lf\n", moveCamf.x);
		break;
	case GLUT_KEY_UP:
		centerf += x1t * .5f;
		printf("THIS IS %lf\n", centerf.y);
		printf("THIS IS %lf\n", moveCamf.y);
		break;
	case GLUT_KEY_DOWN:
		centerf += x1t * -.5f;
		printf("THIS IS %lf\n", centerf.y);
		printf("THIS IS %lf\n", moveCamf.y);
		break;
	}
	

}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	//glm::vec3 lookDir = glm::normalize(center - moveCam);
	glm::vec3 x1f = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), centerf - moveCamf));
	glm::vec3 lookDirf = glm::normalize(centerf - moveCamf);

	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 97://a - turn left
		turnL = true;
		//center = center + x1 *.5f;
		break;
	case 99://c - change camera mode
		mode++;
		if (mode == 1)
			DOOM = true;
		if (mode == 2) {
			DOOM = false;
			flyCam = true;
		}
		if (mode == 3) {
			mode = 1;
			flyCam = false;
			DOOM = true;
		}
		break;
	case 100://d - turn right
		turnR = true;
		//center += x1;
		break;
	case 102://f - fly forward
		moveCamf += lookDirf;
		center += lookDirf;
		break;
	case 114://r - reset all bools to false if stuck
		swingB = false;
		swingF = false;
		turnL = false;
		turnR = false;
		break;
	case 115://s - move backward
		swingB = true;
		//moveCam -= lookDir;
		//center -= lookDir;
		break;
	case 118://v - fly backward
		moveCamf -= lookDirf;
		center -= lookDirf;
		break;
	case 119://w - move forward
		swingF = true;
		//moveCam += lookDir;
		//center += lookDir;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glewInit();
	dumpInfo ();
	init ();
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glEnable(GL_DEPTH_TEST);
	glutSpecialFunc(specialkey);


	cylinderLL = new Model(&shader, "models/cylinder.obj");
	cylinderRL = new Model(&shader, "models/cylinder.obj");
	cylinderLA = new Model(&shader, "models/cylinder.obj");
	cylinderRA = new Model(&shader, "models/cylinder.obj");
	cylinderBD = new Model(&shader, "models/cylinder.obj");
	cylinderHD = new Model(&shaderg, "models/cylinder.obj");
	heli = new Model(&shader, "models/AW101.obj", "models/");
	dragon = new Model(&shader, "models/titan.obj", "models/");

	plane = new Model(&shaderg, "models/plane.obj");
	//sphere = new Model(&shader, "models/dodge-challenger_model.obj", "models/"); // you must specify the material path for this to load
	

	glutMainLoop();

	return 0;
}