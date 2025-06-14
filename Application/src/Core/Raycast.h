#pragma once

#include <glm/vec3.hpp>

class Raycast {
	// Represents a ray in 3D space defined by an origin and a direction.
	// The ray is parameterized as: P(t) = orig + t * dir, where t >= 0.
public:
	Raycast() {}
	Raycast(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

	const glm::vec3& origin() const { return orig; }
	const glm::vec3& direction() const { return dir; }
	const glm::vec3 at(float t) const { return orig + t * dir; }
	float length() const { return dir.length(); }
private:
	glm::vec3 orig;
	glm::vec3 dir;
};
