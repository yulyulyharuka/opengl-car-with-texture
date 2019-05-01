#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Rain {
public:
	glm::vec4 offset;
	glm::vec4 speed;
	Rain(glm::vec4 offset) {
		this->offset = offset;
	}
	Rain(glm::vec4 offset, glm::vec4 speed) {
		this->offset = offset;
		this->speed = speed;
	}

	void update() {
		this->offset += this->speed;
		if (this->offset.y <= -1.0f) {
			this->offset.y = 1.0f;
		}
	}
};

class Smoke {
public:
	glm::vec4 origin;
	glm::vec4 offset;
	glm::vec4 speed;
	glm::vec4 start;
	glm::vec4 end;
	Smoke(glm::vec4 offset) {
		this->offset = offset;
	}
	Smoke(glm::vec4 offset, glm::vec4 speed) {
		this->offset = offset;
		this->speed = speed;
	}
	Smoke(glm::vec4 offset, glm::vec4 speed, glm::vec4 start, glm::vec4 end) {
		this->offset = offset;
		this->origin = offset;
		this->speed = speed;
		this->start = start;
		this->end = end;
	}

	void update() {
		this->offset += this->speed;
		if (this->offset.x < this->start.x || this->offset.y < this->start.y || this->start.z < this->start.z ||
			this->offset.x > this->end.x || this->offset.y > this->end.y || this->end.z > this->end.z) {
			this->offset = this->origin;
		}
	}
};

#endif

