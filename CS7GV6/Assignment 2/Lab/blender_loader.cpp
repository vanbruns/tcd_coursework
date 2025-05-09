// Created from:
//  https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
//  https://www.keithlantz.net/2011/10/a-preliminary-wavefront-obj-loader-in-c/

#include "blender_loader.h"
#include "maths_funcs.h" //Anton's math class

#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void BlenderObjLoader::load(const char* filename)
{
	vector<vec3> vertices;
	vector<vec3> textCoords;
	vector<vec3> normals;
	vector<BlenderFace> faces;

	ifstream in(filename, ios::in);

	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}

	string line;

	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ") {
			istringstream s(line.substr(2));
			vec3 v;

			s >> v.v[0];
			s >> v.v[1];
			s >> v.v[2];
			vertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vt ") {
			istringstream s(line.substr(3));
			vec3 v;

			s >> v.v[0];
			s >> v.v[1];
			s >> v.v[2];
			textCoords.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ") {
			istringstream s(line.substr(3));
			vec3 v;

			s >> v.v[0];
			s >> v.v[1];
			s >> v.v[2];
			normals.push_back(v);
		}
		else if (line.substr(0, 2) == "f ") {
			istringstream s(line.substr(2));
			int vert, tex, norm;
			BlenderFace face = BlenderFace();

			while (!s.eof()) {
				s >> vert >> std::ws;
				face.vertIndices.push_back(vert - 1);

				if (s.peek() == '/') {
					s.get();

					if (s.peek() == '/') {
						s.get();
						s >> norm >> std::ws;
						face.normIndices.push_back(norm - 1);
					}
					else {
						s >> tex >> std::ws;
						face.textCoordIndices.push_back(tex - 1);

						if (s.peek() == '/') {
							s.get();
							s >> norm >> std::ws;
							face.normIndices.push_back(norm - 1);
						}
					}
				}
			}

			faces.push_back(face);
		}
		else {

		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		// Triangular faces
		if (faces[i].vertIndices.size() == 3) {
			verts.push_back(vertices[faces[i].vertIndices[0]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[2]);
			vert_count += 3;

			if (!faces[i].textCoordIndices.empty()) {
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[2]);
			}

			if (!faces[i].normIndices.empty()) {
				norms.push_back(normals[faces[i].normIndices[0]].v[0]);
				norms.push_back(normals[faces[i].normIndices[0]].v[1]);
				norms.push_back(normals[faces[i].normIndices[0]].v[2]);
				norms.push_back(normals[faces[i].normIndices[1]].v[0]);
				norms.push_back(normals[faces[i].normIndices[1]].v[1]);
				norms.push_back(normals[faces[i].normIndices[1]].v[2]);
				norms.push_back(normals[faces[i].normIndices[2]].v[0]);
				norms.push_back(normals[faces[i].normIndices[2]].v[1]);
				norms.push_back(normals[faces[i].normIndices[2]].v[2]);
			}
		}
		// Quadular faces
		else
		{
			verts.push_back(vertices[faces[i].vertIndices[0]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[1]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[0]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[2]].v[2]);
			verts.push_back(vertices[faces[i].vertIndices[3]].v[0]);
			verts.push_back(vertices[faces[i].vertIndices[3]].v[1]);
			verts.push_back(vertices[faces[i].vertIndices[3]].v[2]);
			vert_count += 6;

			if (!faces[i].textCoordIndices.empty()) {
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[1]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[0]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[2]].v[2]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[3]].v[0]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[3]].v[1]);
				text_coords.push_back(textCoords[faces[i].textCoordIndices[3]].v[2]);
			}

			if (!faces[i].normIndices.empty()) {
				norms.push_back(normals[faces[i].normIndices[0]].v[0]);
				norms.push_back(normals[faces[i].normIndices[0]].v[1]);
				norms.push_back(normals[faces[i].normIndices[0]].v[2]);
				norms.push_back(normals[faces[i].normIndices[1]].v[0]);
				norms.push_back(normals[faces[i].normIndices[1]].v[1]);
				norms.push_back(normals[faces[i].normIndices[1]].v[2]);
				norms.push_back(normals[faces[i].normIndices[2]].v[0]);
				norms.push_back(normals[faces[i].normIndices[2]].v[1]);
				norms.push_back(normals[faces[i].normIndices[2]].v[2]);
				norms.push_back(normals[faces[i].normIndices[0]].v[0]);
				norms.push_back(normals[faces[i].normIndices[0]].v[1]);
				norms.push_back(normals[faces[i].normIndices[0]].v[2]);
				norms.push_back(normals[faces[i].normIndices[2]].v[0]);
				norms.push_back(normals[faces[i].normIndices[2]].v[1]);
				norms.push_back(normals[faces[i].normIndices[2]].v[2]);
				norms.push_back(normals[faces[i].normIndices[3]].v[0]);
				norms.push_back(normals[faces[i].normIndices[3]].v[1]);
				norms.push_back(normals[faces[i].normIndices[3]].v[2]);
			}
		}
	}
}

float* BlenderObjLoader::vertices() {
	return &verts[0];
}

float* BlenderObjLoader::textureCoordinates() {
	return &text_coords[0];
}

float* BlenderObjLoader::normals() {
	return &norms[0];
}

int BlenderObjLoader::verticesCount() {
	return vert_count;
}