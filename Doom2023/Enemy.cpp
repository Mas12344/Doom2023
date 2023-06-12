#include "Enemy.h"
std::random_device Enemy::m_RandomDevice{};
std::mt19937 Enemy::m_Generator{ Enemy::m_RandomDevice() };

float randomNumber2(float a, float b) {
	std::uniform_real_distribution<float> randomNum(a, b);
	return randomNum(Enemy::m_Generator);
}

Enemy::Enemy(std::shared_ptr<Model> model) :
	GameObject(model),
    attackDamage(10),
    health(100),
    dead(false)
{
	if (dead) return;
	ApplyTransform(
		Transformation(
			TranformType::Translate,
			glm::vec3(),
			glm::vec3(randomNumber2(-18.f, 18.f), 2.0f, randomNumber2(-18.f, 18.f)),
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
	ApplyTransform(
		Transformation(
			TranformType::Rotate,
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(),
			glm::radians(randomNumber2(0.0, 720.))
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
		//glm::mat4 m(1.f);
		//SetModelMatrix(m);
		ApplyTransform(
			Transformation(
				TranformType::Translate,
				glm::vec3(),
				glm::vec3(0.f, -100.f, 0.f),
				0.0f
			)
		);
	}
}