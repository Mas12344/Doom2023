#include "Enemy.h"
#include <random>

float randomNumber2(float a, float b) {
	std::mt19937 m_mtRandom;
	std::uniform_real_distribution<float> randomNum(a, b);
	return randomNum(m_mtRandom);
}

Enemy::Enemy() :
	GameObject(std::make_shared<Model>("res/models/drone/Weatley.obj", "res/models/drone/")),
    pos(randomNumber2(0.f,70.f), 0.0f, randomNumber2(0.f,70.f)),
    attackDamage(25),
    health(100),
    dead(false)
{}

glm::mat4 Enemy::getCoords() {
	enemy.ApplyTransform(
		Transformation(
			TranformType::Translate,
			pos,
			glm::vec3(0.f, 2.f, 0.f),
			0.0f
		)
	);

	glm::mat4 Menemy = enemy.ApplyTransform(
		Transformation(
			TranformType::Scale,
			glm::vec3(0.f),
			glm::vec3(0.016f),
			0.0f
		)
	);
	return Menemy;
}

void Enemy::spawnEnemy() {
	if (dead) return;
	enemy.GetModel()->Draw("simple", getCoords());
}

void Enemy::reduceHealth() {
	health -= attackDamage;
	checkIfDead(health);
}

void Enemy::checkIfDead(int hp) {
	if (hp <= 0) {
		dead = true;
	}
}