#pragma once
#include "Model.h"
#include "GameObject.h"
#include <random>

class Enemy: public GameObject
{
private:
	float attackDamage;
	int health;
	void checkIfDead(int hp);
	static std::random_device m_RandomDevice;
public:
	glm::vec3 targetpos;
	glm::vec3 getCoords();
	static std::mt19937 m_Generator;
	bool dead;
	Enemy(std::shared_ptr<Model> model);
	~Enemy();
	void reduceHealth();
};

