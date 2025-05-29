#pragma once

#include <glm/glm.hpp>

namespace Chromite {
	class Component {
	public:
		Component() = default;
		virtual ~Component() { }

		virtual void SignalNorth() { }
		virtual void SignalEast() { }
		virtual void SignalSouth() { }
		virtual void SignalWest() { }

		virtual void RotateClockwise() { }
		virtual void RotateCounterClockwise() { }

		Component* north;
		Component* east;
		Component* south;
		Component* west;

		glm::ivec2 position{ };
	};
}