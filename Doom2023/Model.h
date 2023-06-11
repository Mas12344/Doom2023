#pragma once
#include <vector>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include "Resource_Manager.h"

struct Attributes {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
};

class Model {
public:
	Model(const char* filepath, const char* mtlpath);
	void Draw(const char* shader, glm::mat4 modelmatrix);

	~Model();
private:
	std::vector<tinyobj::material_t> m_materials;
	std::unordered_map<int, Attributes> m_meshes;
};