
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include "person.h"

#include <GL/freeglut.h>
#include <GL/glew.h>

using namespace std;

enum transformOperations { Combined, NonUniformScale, UniformScale, TranslateX, TranslateY, TranslateZ, RotateX, RotateY, RotateZ };

GenericMesh* table = new Table();
mat4 tableModel;
GenericMesh* teapot = new Teapot();
mat4 leftTeapotModel;
mat4 rightTeapotModel;
GenericMesh* objects[] = { new Cone(), new Cube(), new Cylinder(), new Dodecahedron(), new IcoSphere(), new Icosahedron(), new Merkaba(), new Octahedron(), new Tetrahedron(), new Torus() };
mat4 objectModels[] = { identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4(), identity_mat4() };
vec3 objectLocations[] = { vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
							vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) };
bool objectSelected[] = { true, false, false, false, false, false, false, false, false, false };
GenericMesh* personMeshes[] = { new Teapot(), new IcoSphere() };
mat4 personModel;
GenericMesh* selectedPersonMesh;
int selectedPersonMeshIndex = 0;
Person person;
vec3 personLocation = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraOffset = vec3(0.0f, 0.0f, 0.0f);
vec3 lightPosition = vec3(100.0f, 200.0f, -300.0f);
LightSource lightSource = LightSource(lightPosition, vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.1, 0.1, 0.1), 0.0001f, 0.000001f, 0.000000001f);

int totalWidth = 1280;
int totalHeight = 720;
int viewportWidth = 256;
int viewportHeight = 196;
bool viewportVisible[] = { false, true, false, false };
vec3 cameraPosition;
vec3 cameraFront;
vec3 cameraUp;
float lastX = totalWidth / 2.0f;
float lastY = totalHeight / 2.0f;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 45.0f;

float translationRate = 2.0f;
float rotationRate = 5.0f;
float scalingRate = 1.0625f;

void drawScene(mat4 view, mat4 projection) {
	person.draw(personModel, view, projection);

	for (int i = 0; i < sizeof(objects) / sizeof(objects[0]); i++) {
		objects[i]->draw(objectModels[i], view, projection);
	}

	table->draw(tableModel, view, projection);
	teapot->draw(leftTeapotModel, view, projection);
	teapot->draw(rightTeapotModel, view, projection);
}

void setViewportWithBackground(GLint x, GLint y, GLsizei width, GLsizei height, vec4 color) {
	glScissor(x, y, width, height);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(color.v[0], color.v[1], color.v[2], color.v[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glViewport(x, y, width, height);
}

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Full-size
	mat4 view = look_at(cameraPosition, cameraPosition + cameraFront, cameraUp);
	mat4 projection = perspective(45.0f, (float)totalWidth / (float)totalHeight, 0.1f, 500.0f);

	glViewport(0, 0, totalWidth, totalHeight);
	drawScene(view, projection);

	// Top-left
	if (viewportVisible[0]) {
		view = identity_mat4();
		projection = orthogonal(-100.0f + personLocation.v[0], 100.0f + personLocation.v[0],
			-100.0f - personLocation.v[1], 100.0f - personLocation.v[1],
			-100.0f - personLocation.v[2], 100.0f - personLocation.v[2]);

		setViewportWithBackground(0, totalHeight - viewportWidth, viewportHeight, viewportWidth, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Top-right
	if (viewportVisible[1]) {
		view = rotate_x_deg(identity_mat4(), 90.0f);
		projection = orthogonal(-100.0f + personLocation.v[0], 100.0f + personLocation.v[0],
			-100.0f - personLocation.v[2], 100.0f - personLocation.v[2],
			-100.0f + personLocation.v[1], 100.0f + personLocation.v[1]);

		setViewportWithBackground(totalWidth - viewportWidth, totalHeight - viewportHeight, viewportWidth, viewportHeight, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Bottom-left
	if (viewportVisible[2]) {
		view = rotate_y_deg(identity_mat4(), 90.0f);
		projection = orthogonal(-100.0f + personLocation.v[2], 100.0f + personLocation.v[2],
			-100.0f - personLocation.v[1], 100.0f - personLocation.v[1],
			-100.0f + personLocation.v[0], 100.0f + personLocation.v[0]);

		setViewportWithBackground(0, 0, viewportHeight, viewportWidth, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Bottom-right
	if (viewportVisible[3]) {
		view = rotate_y_deg(identity_mat4(), 270.0f);
		projection = orthogonal(-100.0f - personLocation.v[2], 100.0f - personLocation.v[2],
			-100.0f - personLocation.v[1], 100.0f - personLocation.v[1],
			-100.0f - personLocation.v[0], 100.0f - personLocation.v[0]);

		setViewportWithBackground(totalWidth - viewportHeight, 0, viewportHeight, viewportWidth, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

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

vec4 locationToVec4(vec3 location) {
	return vec4(location.v[0], location.v[1], location.v[2], 1.0f);
}

void transform(transformOperations operation, bool reverse = false) {
	for (int i = 0; i < sizeof(objectSelected) / sizeof(objectSelected[0]); i++) {
		mat4 transformation = identity_mat4();

		if (!objectSelected[i]) {
			continue;
		}

		switch (operation) {
			case Combined:
				transformation = translateFirst(scale(rotate_x_deg(transformation, rotationRate), vec3(scalingRate, scalingRate, scalingRate)), objectLocations[i]);
				break;
			case NonUniformScale:
				transformation = translateFirst(scale(transformation, vec3(scalingRate, scalingRate * scalingRate, scalingRate * scalingRate * scalingRate)), objectLocations[i]);
				break;
			case UniformScale:
				transformation = translateFirst(scale(transformation, vec3(scalingRate, scalingRate, scalingRate)), objectLocations[i]);
				break;
			case TranslateX:
				transformation = translate(transformation, vec3(translationRate, 0.0f, 0.0f));
				break;
			case TranslateY:
				transformation = translate(transformation, vec3(0.0f, translationRate, 0.0f));
				break;
			case TranslateZ:
				transformation = translate(transformation, vec3(0.0f, 0.0f, translationRate));
				break;
			case RotateX:
				transformation = translateFirst(rotate_x_deg(transformation, rotationRate), objectLocations[i]);
				break;
			case RotateY:
				transformation = translateFirst(rotate_y_deg(transformation, rotationRate), objectLocations[i]);
				break;
			case RotateZ:
				transformation = translateFirst(rotate_z_deg(transformation, rotationRate), objectLocations[i]);
				break;
		}

		if (reverse) {
			transformation = inverse(transformation);
		}

		switch (operation) {
			case TranslateX:
			case TranslateY:
			case TranslateZ:
				objectLocations[i] = transformation * locationToVec4(objectLocations[i]);
				break;
		}

		objectModels[i] = transformation * objectModels[i];
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
	int modKeys = glutGetModifiers();
	int objectPressed = (int)key - 48;
	int direction = (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT ? -1 : 1;
	float cameraSpeed = 1.0f;
	vec3 temp;

	switch (key) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			for (int i = 0; i < sizeof(objectSelected) / sizeof(objectSelected[0]); i++) {
				objectSelected[i] = false;
			}

			objectSelected[objectPressed] = !objectSelected[objectPressed];
			break;
		case 'U':
		case 'u':
			// Alt key scales it smaller (reverse)
			transform(UniformScale, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'X':
			// Alt key translates in negative direction
			transform(TranslateX, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'Y':
			// Alt key translates in negative direction
			transform(TranslateY, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'Z':
			// Alt key translates in negative direction
			transform(TranslateZ, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'x':
			// Alt key rotates clockwise on x-axis (pointing right)
			transform(RotateX, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'y':
			// Alt key rotates clockwise on y-axis (pointing up)
			transform(RotateY, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'z':
			// Alt key rotates clockwise on z-axis (pointing out)
			transform(RotateZ, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'c':
			person.forwardLeftShoulderRotate(direction * 1.0f);
			break;
		case 'v':
			person.sideLeftShoulderRotate(direction * 1.0f);
			break;
		case 'b':
			person.upperLeftArmRotate(direction * 1.0f);
			break;
		case 'n':
			person.leftElbowRotate(direction * 1.0f);
			break;
		case 'm':
			person.lowerLeftArmRotate(direction * 1.0f);
			break;
		case 'f':
			person.forwardRightShoulderRotate(direction * 1.0f);
			break;
		case 'g':
			person.sideRightShoulderRotate(direction * 1.0f);
			break;
		case 'h':
			person.upperRightArmRotate(direction * 1.0f);
			break;
		case 'j':
			person.rightElbowRotate(direction * 1.0f);
			break;
		case 'k':
			person.lowerRightArmRotate(direction * 1.0f);
			break;
		case '\x1':
			for (int i = 0; i < sizeof(objectSelected) / sizeof(objectSelected[0]); i++) {
				objectSelected[i] = true;
			}

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
			personModel = translate(personModel, temp);
			personLocation += temp;
			
			break;
		case 'd':
			temp = normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
			cameraPosition += temp;
			personModel = translate(personModel, temp);
			personLocation += temp;
			
			break;
		case 's':
			temp = cameraFront * -cameraSpeed;
			cameraPosition += temp;
			personModel = translate(personModel, temp);
			personLocation += temp;
			break;
		case 'w':
			temp = cameraFront * cameraSpeed;
			cameraPosition += temp;
			// TODO: Adjust for head difference.
			personModel = translate(personModel, temp);
			personLocation += temp;
			break;
	}
}

void processSpecialKeys(int key, int x, int y) {
	int viewportSelected = key - 5;

	switch (key) {
		case 5:
		case 6:
		case 7:
		case 8:
			viewportVisible[viewportSelected] = !viewportVisible[viewportSelected];
			break;
	}
}

void processMouseWheel(int wheel, int direction, int x, int y) {
	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= y;
	}

	if (fov <= 1.0f) {
		fov = 1.0f;
	}

	if (fov >= 45.0f) {
		fov = 45.0f;
	}
}

// Taken from: https://learnopengl.com/Getting-started/Camera

void processPassiveMouse(int xpos, int ypos) {
	float sensitivity = 0.1f; // change this value to your liking
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw -= xoffset;
	pitch -= yoffset;

	vec3 front;

	front.v[0] = cos(radians(yaw)) * cos(radians(pitch));
	front.v[1] = sin(radians(pitch));
	front.v[2] = -sin(radians(yaw)) * cos(radians(pitch));

	cameraFront = normalise(front);

	person.headRotate(-pitch, yaw + 90.0f);
}


void init() {
	Shader* shader = new Shader("pointLitVertexShader.glsl", "pointLitFragmentShader.glsl");

	shader->compile();

	// Person
	for (int i = 0; i < sizeof(personMeshes) / sizeof(personMeshes[0]); i++) {
		personMeshes[i]->init(shader , &lightSource);
	}
	
	selectedPersonMesh = personMeshes[selectedPersonMeshIndex];
	person = Person(selectedPersonMesh, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
	personModel = translate(identity_mat4(), personLocation);

	// Camera
	cameraOffset = vec3(0.0f, selectedPersonMesh->getLength(), selectedPersonMesh->getHeight() * 3.0f);
	cameraPosition = personLocation + cameraOffset;
	cameraFront = vec3(0.0f, 0.0f, 1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);

	// Objects
	for (int i = 0; i < sizeof(objectLocations) / sizeof(objectLocations[0]); i++) {
		objectLocations[i] = cameraPosition + vec3(75.0f - 15.0f * i, 0.0f, 40.0f);
		objectModels[i] = scale(objectModels[i], vec3(5.0f, 5.0f, 5.0f));
		objectModels[i] = translate(objectModels[i], objectLocations[i]);
	}

	for (int i = 0; i < sizeof(objects) / sizeof(objects[0]); i++) {
		objects[i]->init(shader , &lightSource);
	}

	// Table
	table->init(shader , &lightSource);
	tableModel = translate(identity_mat4(), cameraPosition + vec3(7.0f, -10.0f, 40.0f));

	// Teapot
	teapot->init(shader, &lightSource);
	leftTeapotModel = translate(identity_mat4(), cameraPosition + vec3(100.0f, -1.5f, 40.0f));
	rightTeapotModel = rotate_y_deg(identity_mat4(), 180.0f);
	rightTeapotModel = translate(rightTeapotModel, cameraPosition + vec3(-85.0f, -1.5f, 40.0f));
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(totalWidth, totalHeight);
    glutCreateWindow("Teapot Person Explores Lighting");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glutSetCursor(GLUT_CURSOR_NONE);

	glutPassiveMotionFunc(processPassiveMouse);
	glutMouseWheelFunc(processMouseWheel);

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