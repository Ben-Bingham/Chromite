#pragma once

#include "Component.h"

namespace Chromite {
	class StraightWire : public Component {
	public:
		StraightWire() = default;
		~StraightWire() override = default;

		void SignalNorth() override {
			if (m_Horizontal) return;

			if (south != nullptr) south->SignalNorth();
		}

		void SignalEast() override {
			if (!m_Horizontal) return;

			if (west != nullptr) west->SignalEast();
		}

		void SignalSouth() override {
			if (m_Horizontal) return;

			if (north != nullptr) north->SignalSouth();
		}

		void SignalWest() override {
			if (!m_Horizontal) return;

			if (east != nullptr) east->SignalWest();
		}

		void RotateClockwise() override {
			if (m_Horizontal) {
				m_Horizontal = false;
			}
			else {
				m_Horizontal = true;
			}
		}

		void RotateCounterClockwise() override {
			if (m_Horizontal) {
				m_Horizontal = false;
			}
			else {
				m_Horizontal = true;
			}
		}

	private:
		bool m_Horizontal{ true };
	};
}