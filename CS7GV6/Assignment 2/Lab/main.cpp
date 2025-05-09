
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include "particle.h"

#include <GL/freeglut.h>
#include <GL/glew.h>
#include <list>

using namespace std;

GenericMesh* particleMesh = new IcoSphere();
							//new Puff();

ParticlePortal* portals[] = {
	// ParticlePortal(stepCount, smallCorner, largeCorner)
	// BubblePortal
	new ParticlePortal(5, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)),
	// CloudPortal
	new ParticlePortal(30, vec3(-4.0f, 14.0f, -2.0f), vec3(4.0f, 16.0f, 2.0f)),
	// SmokePortal
	new ParticlePortal(30, vec3(-0.05f, 0.0f, -0.05f), vec3(0.05f, 0.0f, 0.05f))
};
ParticleGun* guns[] = {
	// ParticleGun(initialDirection, stepCount, vrotationRangeSize,
	//   meanSpeed, varSpeedFactor, speedStep)
	// BubbleGun
	new ParticleGun(vec3(0.0f, 1.0f, 0.0f), 360, vec3(30.0f, 360.0f, 0.0f), 0.015, 0.0001, 10),
	// CloudGun
	new ParticleGun(vec3(0.0f, 0.0f, 0.0f), 360, vec3(360.0f, 0.0f, 360.0f), 0.005, 0.0001, 10),
	// SmokeGun
	new ParticleGun(vec3(0.0f, 1.0f, 0.0f), 360, vec3(15.0f, 360.0f, 0.0f), 0.015, 0.0001, 10)
};
ParticleManager* managers[] = {
	// ParticleManager(shape, scale, cycleWait, meanNewParticles,
	//   varNewParticles, meanParticleLifetime, varParticleLifetime)
	// BubbleManager
	new ParticleManager(particleMesh, vec3(0.5f, 0.5f, 0.5f), 20, 2, 1, 1500, 500),
	// CloudManager
	new ParticleManager(particleMesh, vec3(1.0f, 1.0f, 1.0f), 10, 6, 3, 1500, 500),
	// SmokeManager
	new ParticleManager(particleMesh, vec3(0.5f, 0.5f, 0.5f), 20, 10, 3, 1500, 500)
};

list<ForceField>* forceFieldLists[] = {
	new list<ForceField>,
	new list<ForceField>,
	new list<ForceField>
};

int selectedType = 0;
bool paused = false;

vec3 cameraOffset = vec3(0.0f, 0.0f, 0.0f);
vec3 lightPosition = vec3(100.0f, 200.0f, -300.0f);
LightSource lightSource = LightSource(lightPosition, vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.1, 0.1, 0.1), 0.0001f, 0.000001f, 0.000000001f);

int totalWidth = 500;
int totalHeight = 720;
vec3 cameraPosition;
vec3 cameraFront;
vec3 cameraUp;

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Full-size
	mat4 view = look_at(cameraPosition, cameraPosition + cameraFront, cameraUp);
	mat4 projection = perspective(45.0f, (float)totalWidth / (float)totalHeight, 0.1f, 500.0f);

	glViewport(0, 0, totalWidth, totalHeight);

	if (!paused) {
		managers[selectedType]->advance(forceFieldLists[selectedType]);
		managers[selectedType]->add(portals[selectedType], guns[selectedType]);
	}
	
	managers[selectedType]->draw(view, projection);

    glutSwapBuffers();
}

void updateScene() {	
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

	// Draw the next frame
	glutPostRedisplay();
}

void updateTitle() {
	switch (selectedType) {
		case 0:
			glutSetWindowTitle("Bubbles from the Ocean Floor");
			break;
		case 1:
			glutSetWindowTitle("Clouds");
			break;
		case 2:
			glutSetWindowTitle("Smoke");
			break;
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
	int modKeys = glutGetModifiers();
	int direction = (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT ? -1 : 1;
	float cameraSpeed = 1.0f;
	vec3 temp;

	switch (key) {
		case '1':
		case '2':
		case '3':
			managers[selectedType]->clear();
			selectedType = (int)key - 49;
			updateTitle();
			
			break;
		case '\x18':
			lightPosition = vec3(-lightPosition.v[0], lightPosition.v[1], lightPosition.v[2]);
			lightSource.setLightPosition(lightPosition);
			break;
		case '\x19':
			lightPosition = vec3(lightPosition.v[0], -lightPosition.v[1], lightPosition.v[2]);
			lightSource.setLightPosition(lightPosition);
			break;
		case '\x1a':
			lightPosition = vec3(lightPosition.v[0], lightPosition.v[1], -lightPosition.v[2]);
			lightSource.setLightPosition(lightPosition);
			break;
		// Taken from: https://learnopengl.com/Getting-started/Camera
		case 'a':
			temp = normalise(cross(cameraFront, cameraUp)) * -cameraSpeed;
			cameraPosition += temp;
			
			break;
		case 'd':
			temp = normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
			cameraPosition += temp;
			
			break;
		case 's':
			temp = cameraFront * -cameraSpeed;
			cameraPosition += temp;

			break;
		case 'w':
			temp = cameraFront * cameraSpeed;
			cameraPosition += temp;

			break;
		case 'p':
			paused = !paused;

			break;
		case 'f':
			if (forceFieldLists[2]->empty()) {
				forceFieldLists[2]->push_back(ForceField(
					vec3(0.00002f, 0.0f, 0.0f),
					vec3(-10.0f, 5.0f, -10.0f),
					vec3(10.0f, 12.0f, 10.0f)));
				forceFieldLists[2]->push_back(ForceField(
					vec3(-0.00002f, -0.00001f, 0.0f),
					vec3(-10.0f, 10.0f, -10.0f),
					vec3(10.0f, 25.0f, 10.0f)));
			}
			else {
				forceFieldLists[2]->clear();
			}

			break;
	}
}

void init() {
	Shader* shader = new Shader("celShadedVertexShader.glsl", "celShadedFragmentShader.glsl");
	//Shader* shader = new Shader("pointLitVertexShader.glsl", "pointLitFragmentShader.glsl");

	shader->compile();

	particleMesh->init(shader, &lightSource);

	// ForceField(direction, smallCorner, largeCorner, inwardFactor)
	forceFieldLists[1]->push_back(ForceField(
		vec3(0.0f, 0.0f, 0.0f),
		vec3(-10.0f, 5.0f, -10.0f),
		vec3(10.0f, 25.0f, 10.0f), 0.0001f));

	// Camera
	cameraOffset = vec3(0.0f, 15.0f, -50.0f);
	cameraPosition = cameraOffset;
	cameraFront = vec3(0.0f, 0.0f, 1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(totalWidth, totalHeight);
    glutCreateWindow("Bubbles from the Ocean Floor");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	glutKeyboardFunc(processNormalKeys);

	 // A call to glewInit() must be done after glut is initialized!
	glewExperimental = GL_TRUE; //for non-lab machines, this line gives better modern GL support
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}