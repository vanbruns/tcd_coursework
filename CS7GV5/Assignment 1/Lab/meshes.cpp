#include "meshes.h"
#include "blender_loader.h"

vec3 rgbToNorm(vec3 rgb) {
	return rgb / 255.0;
}

void Table::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("table.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(255, 255, 255));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;
	specularExponent = 100.0f;
	k = 0.0f;

	GenericMesh::init(shader, lightSource);
}

void AirplaneMesh::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("plane.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(109, 135, 100));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;
	specularExponent = 100.0f;
	k = 0.0f;

	GenericMesh::init(shader, lightSource);
}

void PropellerMesh::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("propeller.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(0, 80, 239));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 2.0f;
	shininess = 8000.0f;
	specularExponent = 100.0f;
	k = 0.0f;

	GenericMesh::init(shader, lightSource);
}

void WheelsMesh::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("wheels.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(255, 255, 255));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;
	specularExponent = 100.0f;
	k = 0.0f;

	GenericMesh::init(shader, lightSource);
}

void Landscape::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("landscape.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(255, 255, 255));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;
	specularExponent = 100.0f;
	k = 0.0f;

	GenericMesh::init(shader, lightSource);
}