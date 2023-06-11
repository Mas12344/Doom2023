#pragma once
#include "Model.h"
#include <glm/glm.hpp>
#include <memory>

enum class TranformType
{
	Translate,
	Scale,
	Rotate
};

struct Transformation {
	TranformType tt;
	glm::vec3 axis;
	glm::vec3 val;
	float angle;
};


class GameObject {
public:
	GameObject(std::shared_ptr<Model> model);
	std::shared_ptr<Model> GetModel();
	void SetModel(std::shared_ptr<Model> model);
	glm::mat4 ApplyTransform(Transformation t);
	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; };

protected:
	std::shared_ptr<Model> m_Model;
	glm::mat4 m_ModelMatrix;

};