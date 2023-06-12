#pragma once
#include "Model.h"
#include "GameObject.h"
#include <random>

class Enemy: public GameObject
{
private:
	//glm::vec3 pos;
	float attackDamage;
	int health;
	void checkIfDead(int hp);
	glm::vec3 getCoords();
	static std::random_device m_RandomDevice;
public:
	static std::mt19937 m_Generator;
	bool dead;
	Enemy(std::shared_ptr<Model> model);
	~Enemy();
	void reduceHealth();
};

