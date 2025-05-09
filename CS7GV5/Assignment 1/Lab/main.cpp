
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include "airplane.h"

#include <GL/freeglut.h>
#include <GL/glew.h>

using namespace std;

enum transformOperations { Combined, NonUniformScale, UniformScale, TranslateX, TranslateY, TranslateZ, RotateX, RotateY, RotateZ };
enum shaders { BlinnPhong, FlawedBlinnPhong, NonPhotoRealistic, Minnaert, PointLit };

Shader* blinnPhongShader;
Shader* flawedBlinnPhongShader;
Shader* nonPhotoRealisticShader;
Shader* minnaertShader;
Shader* pointLitShader;
shaders selectedShader = PointLit;
bool shadersInitialized = false;

mat4 planeModel;
Airplane plane = Airplane();
vec3 planeLocation = vec3(0.0f, 0.0f, 0.0f);

GenericMesh* table = new Table();
mat4 tableModel;

vec3 cameraOffset = vec3(0.0f, 0.0f, 0.0f);
vec3 lightPosition = vec3(100.0f, 200.0f, -300.0f);
LightSource lightSource = LightSource(lightPosition, vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.1, 0.1, 0.1), 0.0001f, 0.000001f, 0.000000001f);

int totalWidth = 1280;
int totalHeight = 720;
int viewportWidth = 256;
int viewportHeight = 196;
bool viewportVisible[] = { true, true, true, true };
bool firstPerson = true;
vec3 cameraPosition;
vec3 cameraFront;
vec3 cameraUp;
float lastX = totalWidth / 2.0f;
float lastY = totalHeight / 2.0f;
float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;
float fov = 45.0f;

float translationRate = 2.0f;
float rotationRate = 5.0f;
float scalingRate = 1.0625f;

void initSelectedShader() {
	Shader* shader;

	switch (selectedShader) {
		default:
		case BlinnPhong:
			shader = blinnPhongShader;

			break;
		case FlawedBlinnPhong:
			shader = flawedBlinnPhongShader;

			break;
		case NonPhotoRealistic:
			shader = nonPhotoRealisticShader;

			break;
		case Minnaert:
			shader = minnaertShader;

			break;
		case PointLit:
			shader = pointLitShader;

			break;
	}

	// Plane
	plane.init(shader, &lightSource);

	// Table
	table->init(shader, &lightSource);
}

void toggleCameraPosition() {
	firstPerson = !firstPerson;

	if (firstPerson) {
		mat4 view = rotate_y_deg(rotate_x_deg(rotate_z_deg(identity_mat4(), roll), -pitch), -yaw);
		cameraOffset = view * vec4(0.0f, 10.0f, 0.0f, 1.0f);
		cameraFront = normalise(view * vec4(0.0f, 0.0f, 1.0f, 1.0f));
		cameraUp = normalise(view * vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else {
		cameraOffset = vec3(0.0f, -7.0f, -180.0f);
		cameraFront = vec3(0.0f, 0.0f, 1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
	}
	
	cameraPosition = planeLocation + cameraOffset;
}

void updatePlaneModel() {
	planeModel = scale(identity_mat4(), vec3(0.3f, 0.3f, 0.3f));
	planeModel = translateFirst(rotate_y_deg(rotate_x_deg(rotate_z_deg(planeModel, roll), -pitch), yaw), planeLocation);
}

void drawScene(mat4 view, mat4 projection) {
	plane.draw(planeModel, view, projection);

	table->draw(tableModel, view, projection);
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

	updatePlaneModel();

	// Rotate propeller and wheels
	plane.propellerRotate(15.0f);
	plane.wheelsRotate(5.0f);

	// Full-size
	mat4 view = look_at(cameraPosition, cameraPosition + cameraFront, cameraUp);
	mat4 projection = perspective(45.0f, (float)totalWidth / (float)totalHeight, 0.1f, 500.0f);

	glViewport(0, 0, totalWidth, totalHeight);
	drawScene(view, projection);

	// Top-left
	if (viewportVisible[0]) {
		view = identity_mat4();
		projection = orthogonal(-100.0f + planeLocation.v[0], 100.0f + planeLocation.v[0],
			-100.0f - planeLocation.v[1], 100.0f - planeLocation.v[1],
			-100.0f - planeLocation.v[2], 100.0f - planeLocation.v[2]);

		setViewportWithBackground(0, totalHeight - viewportWidth, viewportHeight, viewportWidth, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Top-right
	if (viewportVisible[1]) {
		view = rotate_x_deg(identity_mat4(), 90.0f);
		projection = orthogonal(-100.0f + planeLocation.v[0], 100.0f + planeLocation.v[0],
			-100.0f - planeLocation.v[2], 100.0f - planeLocation.v[2],
			-100.0f + planeLocation.v[1], 100.0f + planeLocation.v[1]);

		setViewportWithBackground(totalWidth - viewportWidth, totalHeight - viewportHeight, viewportWidth, viewportHeight, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Bottom-left
	if (viewportVisible[2]) {
		view = rotate_y_deg(identity_mat4(), 90.0f);
		projection = orthogonal(-100.0f + planeLocation.v[2], 100.0f + planeLocation.v[2],
			-100.0f - planeLocation.v[1], 100.0f - planeLocation.v[1],
			-100.0f + planeLocation.v[0], 100.0f + planeLocation.v[0]);

		setViewportWithBackground(0, 0, viewportHeight, viewportWidth, vec4(0.0, 0.0, 0.0, 1.0));
		drawScene(view, projection);
	}

	// Bottom-right
	if (viewportVisible[3]) {
		view = rotate_y_deg(identity_mat4(), 270.0f);
		projection = orthogonal(-100.0f - planeLocation.v[2], 100.0f - planeLocation.v[2],
			-100.0f - planeLocation.v[1], 100.0f - planeLocation.v[1],
			-100.0f - planeLocation.v[0], 100.0f - planeLocation.v[0]);

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

void processNormalKeys(unsigned char key, int x, int y) {
	int modKeys = glutGetModifiers();
	int objectPressed = (int)key - 48;
	int direction = (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT ? -1 : 1;
	float cameraSpeed = 1.0f;
	float rollSpeed = 5.0f;
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
			//for (int i = 0; i < sizeof(objectSelected) / sizeof(objectSelected[0]); i++) {
			//	objectSelected[i] = false;
			//}

			//objectSelected[objectPressed] = !objectSelected[objectPressed];
			break;
		case 'U':
		case 'u':
			// Alt key scales it smaller (reverse)
			//transform(UniformScale, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'X':
			// Alt key translates in negative direction
			//transform(TranslateX, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'Y':
			// Alt key translates in negative direction
			//transform(TranslateY, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'Z':
			// Alt key translates in negative direction
			//transform(TranslateZ, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'x':
			// Alt key rotates clockwise on x-axis (pointing right)
			//transform(RotateX, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'y':
			// Alt key rotates clockwise on y-axis (pointing up)
			//transform(RotateY, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'z':
			// Alt key rotates clockwise on z-axis (pointing out)
			//transform(RotateZ, (modKeys & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
			break;
		case 'c':
			//person.forwardLeftShoulderRotate(direction * 1.0f);
			break;
		case 'v':
			//person.sideLeftShoulderRotate(direction * 1.0f);
			break;
		case 'b':
			//person.upperLeftArmRotate(direction * 1.0f);
			break;
		case 'n':
			//person.leftElbowRotate(direction * 1.0f);
			break;
		case 'm':
			//person.lowerLeftArmRotate(direction * 1.0f);
			break;
		case 'f':
			//person.forwardRightShoulderRotate(direction * 1.0f);
			break;
		case 'g':
			//person.sideRightShoulderRotate(direction * 1.0f);
			break;
		case 'h':
			//person.upperRightArmRotate(direction * 1.0f);
			break;
		case 'j':
			//person.rightElbowRotate(direction * 1.0f);
			break;
		case 'k':
			//person.lowerRightArmRotate(direction * 1.0f);
			break;
		case '\x1':
			//for (int i = 0; i < sizeof(objectSelected) / sizeof(objectSelected[0]); i++) {
			//	objectSelected[i] = true;
			//}

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
			roll -= rollSpeed;
			
			break;
		case 'd':
			roll += rollSpeed;
			
			break;
		case 's':
			//temp = cameraFront * -cameraSpeed;
			//cameraPosition += temp;
			//planeModel = translate(planeModel, temp);
			//planeLocation += temp;
			break;
		case 'w':
			//temp = cameraFront * cameraSpeed;
			//cameraPosition += temp;
			// TODO: Adjust for head difference.
			//planeModel = translate(planeModel, temp);
			//planeLocation += temp;
			break;
		case 'B':
			selectedShader = BlinnPhong;
			initSelectedShader();
			break;
		case 'N':
			selectedShader = NonPhotoRealistic;
			initSelectedShader();
			break;
		case 'M':
			selectedShader = Minnaert;
			initSelectedShader();
			break;
		case 'P':
			selectedShader = PointLit;
			initSelectedShader();
			break;
		case 'F':
			selectedShader = FlawedBlinnPhong;
			initSelectedShader();
			break;
		case 'p':
			toggleCameraPosition();

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
	float sensitivity = 0.3f; // change this value to your liking
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw -= xoffset;
	pitch -= yoffset;
}

void init() {
	if (!shadersInitialized) {
		blinnPhongShader = new Shader("reflectionCommonVertexShader.glsl", "blinnPhongFragmentShader.glsl");
		flawedBlinnPhongShader = new Shader("reflectionCommonVertexShader.glsl", "flawedBlinnPhongFragmentShader.glsl");
		nonPhotoRealisticShader = new Shader("celShadedVertexShader.glsl", "celShadedFragmentShader.glsl");
		minnaertShader = new Shader("reflectionCommonVertexShader.glsl", "minnaertFragmentShader.glsl");
		pointLitShader = new Shader("pointLitVertexShader.glsl", "pointLitFragmentShader.glsl");

		blinnPhongShader->compile();
		flawedBlinnPhongShader->compile();
		nonPhotoRealisticShader->compile();
		minnaertShader->compile();
		pointLitShader->compile();

		shadersInitialized = true;
	}

	initSelectedShader();

	// Plane
	planeModel = scale(identity_mat4(), vec3(0.3f, 0.3f, 0.3f));
	planeModel = translate(planeModel, planeLocation);

	// Table
	tableModel = scale(identity_mat4(), vec3(3.0f, 3.0f, 3.0f));
	tableModel = translate(tableModel, vec3(7.0f, -100.0f, 40.0f));

	// Camera
	toggleCameraPosition();
	cameraFront = vec3(0.0f, 0.0f, 1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(totalWidth, totalHeight);
    glutCreateWindow("Plane Rotation");
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