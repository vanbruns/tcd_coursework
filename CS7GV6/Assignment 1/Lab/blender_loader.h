#include <vector>

using namespace std;

class BlenderFace
{
public:
	vector<int> vertIndices;
	vector<int> textCoordIndices;
	vector<int> normIndices;
};

class BlenderObjLoader
{
private:
	vector<float> verts;
	vector<float> text_coords;
	vector<float> norms;
	int vert_count = 0;
public:
	void load(const char* filename);
	float* vertices();
	float* textureCoordinates();
	float* normals();
	int verticesCount();
};