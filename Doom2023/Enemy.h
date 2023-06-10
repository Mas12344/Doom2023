#pragma once
#include "Model.h"
#include "GameObject.h"

class Enemy: public GameObject
{
private:
	GameObject enemy = GameObject(std::make_shared<Model>("res/models/drone/Weatley.obj", "res/models/drone/"));
	glm::vec3 pos;
	float attackDamage;
	int health;
	bool dead;
	void checkIfDead(int hp);
	glm::mat4 getCoords();
public:
	Enemy();
	~Enemy();
	void spawnEnemy();
	void reduceHealth();
};

