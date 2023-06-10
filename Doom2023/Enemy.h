#pragma once
#include "Model.h"

class Enemy
{
private:
	Model model = Model("res/models/drone/Weatley.obj", "res/models/drone/");
	glm::vec3 pos;
	float attackDamage;
	int health;
	bool dead;
	void checkIfDead(int hp);
public:
	Enemy();
	~Enemy();
	void spawnEnemy();
	void reduceHealth();
};

