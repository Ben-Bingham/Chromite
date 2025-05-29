#pragma once

#include "Component.h"

namespace Chromite {
	class Emitter : public Component {
	public:
		Emitter() = default;
		~Emitter() override = default;

		void Emit() {
			if (north != nullptr) north->SignalSouth();
			if (east != nullptr) east->SignalWest();
			if (south != nullptr) south->SignalNorth();
			if (west != nullptr) west->SignalEast();
		}
	};
}