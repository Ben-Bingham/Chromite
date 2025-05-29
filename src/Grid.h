#pragma once

#include <glm/glm.hpp>

namespace Chromite {
	struct Grid {
		glm::ivec2 gridSize{ 100, 100 }; // Number of cells in the grid
		glm::vec2 gridOrigin{ 0.0f, 0.0f }; // Top left corner of the grid
		float gridLength = 0.1f; // The length of one of the squares of the grid
		float n = 0.002f; // half the width of the grid walls

		size_t indexCount = 0;

		struct MeshData {
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
		};

		MeshData GenerateMeshData();
	};
}