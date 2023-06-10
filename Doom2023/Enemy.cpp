#include "Enemy.h"
#include <random>

float randomNumber2(float a, float b) {
	std::mt19937 m_mtRandom;
	std::uniform_real_distribution<float> randomNum(a, b);
	return randomNum(m_mtRandom);
}

Enemy::Enemy(): 
	pos(randomNumber2(10.f, 10.f), 0.0f, randomNumber2(10.f, 10.f)),
	health(100),
	dead(false),
	attackDamage(25)
{}

void Enemy::spawnEnemy() {
	if (dead) return;
	model.m_modelmatrix = glm::scale(model.m_modelmatrix, glm::vec3(0.01f));
	//glTranslatef(pos.x,pos.y,pos.z);
	model.Draw("simple");
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