#include "Raycaster.h"

Raycaster::Raycaster() :
	m_mouseX(0.0f),
	m_mouseY(0.0f),
	m_castRay(false),
	m_collision(false)
{}

Raycaster::~Raycaster()
{}

void Raycaster::Update(Camera& cam, float dt, void mouseCallback(GLFWwindow* window, double xposIn, double yposIn), Enemy* enemies[40])
{

	// Check if ray casting is true
	if (m_castRay)
	{
		// Cast ray and check for collision
		m_castRay = false;
		m_debugRayCastDraw = true;
		m_ray = CastRayFromWeapon(cam);
		CheckRaySphereCollision(cam, enemies);
	}
}

Ray Raycaster::CastRayFromMouse(Camera& cam)
{
	// screen space (viewport coordinates)
	float x = (2.0f * m_mouseX) / 640;
	float y = 1.0f - (2.0f * m_mouseY) / 480;
	float z = 1.0f;

	// normalised device space
	glm::vec3 mouseNdcCoords = glm::vec3(x, y, z);
	glm::vec4 mouseClipCoords = glm::vec4(mouseNdcCoords.x, mouseNdcCoords.y, -1.0f, 1.0f);
	glm::mat4 invProjMat = glm::inverse(cam.GetViewMatrix());
	glm::vec4 eyeCoords = invProjMat * mouseClipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	glm::mat4 invViewMat = glm::inverse(cam.GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

	Ray ray;
	ray.pos = cam.Position;
	ray.dir = rayDirection;

	return ray;
}

Ray Raycaster::CastRayFromWeapon(Camera& cam)
{
	Ray ray;
	ray.pos = cam.Position;
	ray.dir = cam.Front;
	return ray;
}

bool Raycaster::RaySphere(Camera& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z)
{
	// work out components of quadratic
	glm::vec3 v = glm::vec3(x, y, z) - cam.Position;
	long double a = glm::dot(RayDirWorld, RayDirWorld);
	long double b = 2.0 * glm::dot(v, RayDirWorld);
	long double c = glm::dot(v, v) - SphereRadius * SphereRadius;
	long double b_squared_minus_4ac = b * b + (-4.0) * a * c;

	if (b_squared_minus_4ac == 0)
	{
		// One real root 
		return true;
	}
	else if (b_squared_minus_4ac > 0)
	{
		// Two real roots 
		long double x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
		long double x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

		if (x1 >= 0.0 || x2 >= 0.0)
			return true;
		if (x1 < 0.0 || x2 >= 0.0)
			return true;
	}

	// No real roots
	return false;
}

void Raycaster::CheckRaySphereCollision(Camera& cam, Enemy* enemies[40])
{
	for (auto i = 0; i != 40; ++i)
	{
		m_collision = RaySphere(cam, m_ray.dir, 3.0f, enemies[i]->GetModelMatrix()[3].x, enemies[i]->GetModelMatrix()[3].y, enemies[i]->GetModelMatrix()[3].z);

		// Check if the ray is colliding with the sphere
		if (m_collision)
		{
			// Pass in the value the iterator is pointing to to the OnEnemyHit() function
			OnEnemyHit(enemies[i]);
		}
	}
}

inline void Raycaster::OnEnemyHit(Enemy* enemy)
{
	enemy->reduceHealth();
}

void Raycaster::OnPlayerHit(float damage)
{
	printf("YOU DIED");
}

bool Raycaster::PointInSphere(Camera& cam, glm::vec3& other, float radius)
{
	// Calculate distance between player and center of circle
	float distanceSq = std::pow(cam.Position.x - other.x, 2) + std::pow(cam.Position.y - other.y, 2) + std::pow(cam.Position.z - other.z, 2);

	// Check if the player is within the radius (if radius is bigger than point is inside circle) 
	if (distanceSq < (radius * radius))
	{
		return true;
	}

	return false;
}