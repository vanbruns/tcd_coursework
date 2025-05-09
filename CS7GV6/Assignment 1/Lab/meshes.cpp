#include "teapot_model.h"
#include "meshes.h"
#include "blender_loader.h"

vec3 rgbToNorm(vec3 rgb) {
	return rgb / 255.0;
}

void Teapot::init(Shader* shader, LightSource* lightSource) {
	vertexCount = teapot_vertex_count;
	vertices = teapot_vertex_points;
	normals = teapot_normals;

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(100, 118, 135));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Cylinder::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("cylinder.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(27, 161, 226));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void IcoSphere::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("ico_sphere.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(118, 96, 138));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 8000.0f;

	GenericMesh::init(shader, lightSource);
}

void Cone::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("cone.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(240, 163, 10));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Cube::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("cube.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(0, 138, 0));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Dodecahedron::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("dodecahedron.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(162, 0, 37));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 2.0f;
	shininess = 100.0f;

	GenericMesh::init(shader, lightSource);
}

void Icosahedron::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("icosahedron.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(130, 90, 44));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Octahedron::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("octahedron.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(244, 114, 208));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Tetrahedron::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("tetrahedron.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(250, 104, 0));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Torus::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("torus.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(109, 135, 100));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 8.0f;
	shininess = 2000.0f;

	GenericMesh::init(shader, lightSource);
}

void Merkaba::init(Shader* shader, LightSource* lightSource) {
	BlenderObjLoader blender;

	blender.load("merkaba.obj");
	vertexCount = blender.verticesCount();
	vertices = blender.vertices();
	normals = blender.normals();

	reflectSpecular = vec3(0.5f, 0.5f, 0.5f);
	reflectDiffuse = rgbToNorm(vec3(0, 80, 239));
	reflectAmbient = vec3(1.0f, 1.0f, 1.0f);
	spotSize = 2.0f;
	shininess = 8000.0f;

	GenericMesh::init(shader, lightSource);
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

	GenericMesh::init(shader, lightSource);
}