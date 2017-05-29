/*
 * Utils.cpp
 *
 *  Created on: 20.03.2014
 *      Author: parojtbe
 */

#include <GeoShape/Utils.h>

namespace GeoShape {

namespace {
Vector3 computeNormal(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
    return (p1 - p0).crossProduct(p2 - p0);
}
}

void RescaleTexcoords(const Vector2& scale, std::vector<Vector2>& coords) {
    for(size_t i = 0; i < coords.size(); i++) {
        coords[i] *= scale;
    }
}

void GenerateNormals(const std::vector<uint32_t>& index, const std::vector<Vector3>& points, std::vector<Vector3>& normals) {
    normals.resize(points.size(), Vector3(0, 0, 0));

    for(size_t f = 0; f < index.size()/3; f++) {
        const Vector3& p0 = points[index[3*f + 0]];
        const Vector3& p1 = points[index[3*f + 1]];
        const Vector3& p2 = points[index[3*f + 2]];
        const Vector3 normal = computeNormal(p0, p1, p2);

        for(size_t i = 0; i < 3; i++) {
            normals[index[f*3 + i]] += normal;
        }
    }

    for(size_t i = 0; i < normals.size(); i++) {
        normals[i].normalise();
    }
}

void GenerateNormals(const std::vector<Vector3>& points, std::vector<Vector3>& normals) {
    normals.resize(points.size());

    for(size_t f = 0; f < points.size()/3; f++) {
        const Vector3& p0 = points[3*f + 0];
        const Vector3& p1 = points[3*f + 1];
        const Vector3& p2 = points[3*f + 2];
        const Vector3 normal = computeNormal(p0, p1, p2).normalisedCopy();

        for(size_t i = 0; i < 3; i++) {
            normals[f*3 + i] = normal;
        }
    }
}

template<class T>
void DuplicateByIndex(const std::vector<uint32_t>& index, const std::vector<T>& pointsIn, std::vector<T>& pointsOut) {
    pointsOut.reserve(index.size());

    for(size_t i = 0; i < index.size(); i++) {
        pointsOut.push_back(pointsIn[index[i]]);
    }
}

void Triangulate(const std::vector<int>& index, std::vector<uint32_t>& tri_index) {
    // we will have approximately the same amount of indices
    // depending on the order of the polygons
    tri_index.reserve(index.size());

    size_t polyStart = 0; // start of the current polygon

    for(size_t i = 0; i < index.size(); i++) {
        if(index[i] > -1 and (i + 1) != index.size()) {
            continue;
        }

        if((i - polyStart) < 3) {
            continue; // not a supported polygon
        }

        // triangulate by fan out
        for(size_t p = polyStart + 1; p < i - 1; p++) {
            tri_index.push_back(index[polyStart]);
            tri_index.push_back(index[p]);
            tri_index.push_back(index[p + 1]);
        }

        polyStart = i + 1;
    }
}

void LineStripToList(const std::vector<int>& index, std::vector<uint32_t>& lst_index) {
    for (size_t i = 1; i < index.size(); i++) {
        if(index[i - 1] > -1 and  index[i] > -1) {
            lst_index.push_back(index[i - 1]);
            lst_index.push_back(index[i]);
        }
    }
}

void SortVectorByIndex(const std::vector<int>& index, const std::vector<Vector3>& vector, std::vector<Vector3> outVector) {
    outVector.reserve(index.size());
	for(int k : index) {
		if (k >= 0 and size_t(k) < vector.size()) {
			outVector.push_back(vector[k]);
		} else {
			//TODO IndexOutOfBoundException
			//throw std::exception;
		}
	}
}

// instantiations
template void DuplicateByIndex<Vector3>(const std::vector<uint32_t>& index, const std::vector<Vector3>& pointsIn, std::vector<Vector3>& pointsOut);
template void DuplicateByIndex<Vector2>(const std::vector<uint32_t>& index, const std::vector<Vector2>& pointsIn, std::vector<Vector2>& pointsOut);
} /* namespace GeoShape */

