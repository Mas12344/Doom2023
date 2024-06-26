#pragma once
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Camera.h"
#include <vector>
#include "Enemy.h"
#include <GLFW/glfw3.h>

struct Ray
{
	glm::vec3 pos;
	glm::vec3 dir;
};

class Raycaster
{
public:
	~Raycaster();

	static Raycaster& GetInstance()
	{
		static Raycaster instance;
		return instance;
	}

	Raycaster(Raycaster const&) = delete;

	void operator=(Raycaster const&) = delete;

	void ProcessInput(Camera& cam, float dt, void mouseCallback(GLFWwindow* window, double xposIn, double yposIn));
	void Update(Camera& cam, float dt, int width, int height, Enemy *enemies[40]);

	void CastRay() { m_castRay = true; }
	Ray& GetRay() { return m_ray; }
	bool GetDebugRayCastDraw() { return m_debugRayCastDraw; }
	void OnEnemyHit(Enemy* enemy);
	void OnPlayerHit(float damage);
	bool PointInSphere(Camera& cam, glm::vec3&, float radius);
	Raycaster();

private:

	Ray m_ray;
	float m_mouseX, m_mouseY;
	bool m_debugRayCastDraw;
	bool m_collision;
	bool m_castRay;
	int m_screenWidth;
	int m_screenHeight;

	// Private functions
	Ray CastRayFromMouse(Camera& cam);
	Ray CastRayFromWeapon(Camera& cam);
	void CheckRaySphereCollision(Camera& cam, Enemy *enemies[40]);
	bool RaySphere(Camera& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z);
};

#endif // !__PHYSICS_H__