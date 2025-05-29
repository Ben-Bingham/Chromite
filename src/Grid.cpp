#include "Grid.h"

namespace Chromite {
	Grid::MeshData Grid::GenerateMeshData() {
        std::vector<float> gridVertices{ };

        // TODO this could be done with instanced rendering if needed for performance
        for (int x = 0; x <= gridSize.x; ++x) {
            float x0 = gridOrigin.x - n + x * gridLength;
            float y0 = gridOrigin.y - n;

            float x1 = gridOrigin.x - n + x * gridLength;
            float y1 = gridOrigin.y + gridSize.y * gridLength + n;

            float x2 = gridOrigin.x + n + x * gridLength;
            float y2 = gridOrigin.y + gridSize.y * gridLength + n;

            float x3 = gridOrigin.x + n + x * gridLength;
            float y3 = gridOrigin.y - n;

            gridVertices.push_back(x0);
            gridVertices.push_back(y0);
            gridVertices.push_back(0.0f);

            gridVertices.push_back(x1);
            gridVertices.push_back(y1);
            gridVertices.push_back(0.0f);

            gridVertices.push_back(x2);
            gridVertices.push_back(y2);
            gridVertices.push_back(0.0f);

            gridVertices.push_back(x3);
            gridVertices.push_back(y3);
            gridVertices.push_back(0.0f);
        }

        for (int y = 0; y <= gridSize.y; ++y) {
            for (int x = 0; x < gridSize.x; ++x) {
                float x0 = gridOrigin.x + n + x * gridLength;
                float y0 = gridOrigin.y - n + y * gridLength;

                float x1 = gridOrigin.x + n + x * gridLength;
                float y1 = gridOrigin.y + n + y * gridLength;

                float x2 = gridOrigin.x + gridLength - n + x * gridLength;
                float y2 = gridOrigin.y + n + y * gridLength;

                float x3 = gridOrigin.x + gridLength - n + x * gridLength;
                float y3 = gridOrigin.y - n + y * gridLength;

                gridVertices.push_back(x0);
                gridVertices.push_back(y0);
                gridVertices.push_back(0.0f);

                gridVertices.push_back(x1);
                gridVertices.push_back(y1);
                gridVertices.push_back(0.0f);

                gridVertices.push_back(x2);
                gridVertices.push_back(y2);
                gridVertices.push_back(0.0f);

                gridVertices.push_back(x3);
                gridVertices.push_back(y3);
                gridVertices.push_back(0.0f);
            }
        }

        std::vector<unsigned int> gridIndices{ };
        unsigned int j = 0;
        for (int i = 0; i < gridVertices.size() / 12; ++i) {
            gridIndices.push_back(0 + j);
            gridIndices.push_back(1 + j);
            gridIndices.push_back(3 + j);

            gridIndices.push_back(1 + j);
            gridIndices.push_back(2 + j);
            gridIndices.push_back(3 + j);

            j += 4;
        }

        indexCount = gridIndices.size();

        return MeshData{ gridVertices, gridIndices };
	}
}