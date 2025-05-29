#pragma once

#include <iostream>

#include "Component.h"

namespace Chromite {
	class Printer : public Component {
	public:
		Printer() = default;
		~Printer() override = default;

		void SignalNorth() override {
			std::cout << "Received from North" << std::endl;
		}

		void SignalEast() override {
			std::cout << "Received from East" << std::endl;
		}

		void SignalSouth() override {
			std::cout << "Received from South" << std::endl;
		}

		void SignalWest() override {
			std::cout << "Received from West" << std::endl;
		}
	};
}