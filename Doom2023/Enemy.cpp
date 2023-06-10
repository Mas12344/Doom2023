#include "Enemy.h"
std::random_device Enemy::m_RandomDevice{};
std::mt19937 Enemy::m_Generator{ Enemy::m_RandomDevice() };

float randomNumber2(float a, float b) {
	std::uniform_real_distribution<float> randomNum(a, b);
	return randomNum(Enemy::m_Generator);
}

Enemy::Enemy(std::shared_ptr<Model> model) :
	GameObject(model),
    //pos(randomNumber2(0.f,70.f), 0.0f, randomNumber2(0.f,70.f)),
    attackDamage(25),
    health(100),
    dead(false)
{
	ApplyTransform(
		Transformation(
			TranformType::Translate,
			glm::vec3(),
			glm::vec3(randomNumber2(-2.f, 2.f), 2.0f, randomNumber2(-2.f, 2.f)),
			0.0f
		)
	);
	ApplyTransform(
		Transformation(
			TranformType::Scale,
			glm::vec3(),
			glm::vec3(0.016f),
			0.0f
		)
	);
}

glm::vec3 Enemy::getCoords() {
	return GetModelMatrix()[3];
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