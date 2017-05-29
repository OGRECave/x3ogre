/*
 * Primitive.cpp
 *
 *  Created on: 03.03.2014
 *      Author: parojtbe
 */

#include <GeoShape/Primitive.h>
#include <limits>

#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006

using namespace std;

namespace GeoShape {

void Disk2D(std::vector<Vector2>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texCoord, uint32_t& mode,
        float outr, float inr, size_t res) {
    uint16_t seg = 0;

    if (inr <= numeric_limits<float>::epsilon()) { // inr == zero
        mode = GL_TRIANGLE_FAN;
        vertex.resize(res + 2);
        texCoord.resize(res + 2);
        normal.resize(res + 2);

        // Solid disk
        vertex[seg] = Vector2(0, 0);
        texCoord[seg] = Vector2(0.5, 0.5);
        normal[seg] = Vector3(0, 0, 1);
        seg++;

        vertex[seg] = Vector2(0, outr);
        texCoord[seg] = Vector2(0.5, 1);
        normal[seg] = Vector3(0, 0, 1);
        seg++;

        for (size_t i = 1; i < res; ++i) {
            float angle = 2.f * M_PI * i / res;
            float x = -sin(angle);
            float y = cos(angle);

            vertex[seg] = Vector2(x, y) * outr;
            texCoord[seg] = Vector2(0.5 + x / 2, 0.5 + y / 2);
            normal[seg] = Vector3(0, 0, 1);
            seg++;
        }

        vertex[seg] = Vector2(0, outr);
        texCoord[seg] = Vector2(0.5, 1);
        normal[seg] = Vector3(0, 0, 1);
    } else {
        mode = GL_TRIANGLE_STRIP;
        vertex.resize(2 * res + 2);
        texCoord.resize(2 * res + 2);
        normal.resize(2 * res + 2);

        // Disk with a hole
        float f = inr / outr;

        vertex[seg] = Vector2(0, inr);
        texCoord[seg][0] = 0.5;
        texCoord[seg][1] = float(0.5) + f / float(2.f);
        normal[seg] = Vector3(0, 0, 1);
        seg++;

        vertex[seg] = Vector2(0, outr);
        texCoord[seg] = Vector2(0.5, 1);
        normal[seg] = Vector3(0, 0, 1);
        seg++;

        for (size_t i = 1; i < res; ++i) {
            float angle = 2.f * M_PI / res * i;
            float x = -sin(angle);
            float y = cos(angle);

            vertex[seg] = Vector2(x, y) * inr;
            texCoord[seg] = Vector2(0.5 + x / 2, 0.5 + y / 2)*f;
            normal[seg] = Vector3(0, 0, 1);
            seg++;

            vertex[seg] = Vector2(x, y) * outr;
            texCoord[seg] = Vector2(0.5 + x / 2, 0.5 + y / 2);
            normal[seg] = Vector3(0, 0, 1);
            seg++;
        }

        vertex[seg] = Vector2(0, inr);
        texCoord[seg][0] = 0.5;
        texCoord[seg][1] = float(0.5) + f / float(2.f);
        normal[seg] = Vector3(0, 0, 1);
        seg++;

        vertex[seg] = Vector2(0, outr);
        texCoord[seg] = Vector2(0.5, 1);
        normal[seg] = Vector3(0, 0, 1);
        seg++;
    }

}

void Box(std::vector<uint16_t>& idx, std::vector<Vector3>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texCoord, const Vector3& size) {
    float px = size[0], py = size[1], pz = size[2];
    float nx = -size[0], ny = -size[1], nz = -size[2];

    Vector2 texOneFace[] = {Vector2(1, 0), Vector2(1, 1), Vector2(0, 1), Vector2(0, 0)};

    vertex.resize(4*6);
    normal.reserve(4*6);
    texCoord.reserve(4*6);

    // Front
    vertex[0] = Vector3(px, ny, pz);
    vertex[1] = Vector3(px, py, pz);
    vertex[2] = Vector3(nx, py, pz);
    vertex[3] = Vector3(nx, ny, pz);
    normal.insert(normal.end(), 4, Vector3(0, 0, 1));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Back
    vertex[4] = Vector3(nx, ny, nz);
    vertex[5] = Vector3(nx, py, nz);
    vertex[6] = Vector3(px, py, nz);
    vertex[7] = Vector3(px, ny, nz);
    normal.insert(normal.end(), 4, Vector3(0, 0, -1));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Left
    vertex[8] = Vector3(nx, ny, pz);
    vertex[9] = Vector3(nx, py, pz);
    vertex[10] = Vector3(nx, py, nz);
    vertex[11] = Vector3(nx, ny, nz);
    normal.insert(normal.end(), 4, Vector3(-1, 0, 0));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Right
    vertex[12] = Vector3(px, ny, nz);
    vertex[13] = Vector3(px, py, nz);
    vertex[14] = Vector3(px, py, pz);
    vertex[15] = Vector3(px, ny, pz);
    normal.insert(normal.end(), 4, Vector3(1, 0, 0));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Bottom
    vertex[16] = Vector3(px, ny, nz);
    vertex[17] = Vector3(px, ny, pz);
    vertex[18] = Vector3(nx, ny, pz);
    vertex[19] = Vector3(nx, ny, nz);
    normal.insert(normal.end(), 4, Vector3(0, -1, 0));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Top
    vertex[20] = Vector3(px, py, pz);
    vertex[21] = Vector3(px, py, nz);
    vertex[22] = Vector3(nx, py, nz);
    vertex[23] = Vector3(nx, py, pz);
    normal.insert(normal.end(), 4, Vector3(0, 1, 0));
    texCoord.insert(texCoord.end(), texOneFace, texOneFace + 4);

    // Faces
    idx.reserve(6*3*2);
    for(uint16_t i = 0; i < 6; i++) {
        idx.push_back(i*4 + 0);
        idx.push_back(i*4 + 1);
        idx.push_back(i*4 + 2);

        idx.push_back(i*4 + 0);
        idx.push_back(i*4 + 2);
        idx.push_back(i*4 + 3);
    }
}

void Rectangle(std::vector<Vector2>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texCoord, const Vector2& size) {
    float px = size[0], py = size[1];
    float nx = -size[0], ny = -size[1];

    // the following has to be consistent with PolygonBackground and with Rectangle2D
    vertex.resize(4);
    vertex[0] = Vector2(px, ny);
    vertex[1] = Vector2(px, py);
    vertex[2] = Vector2(nx, py);
    vertex[3] = Vector2(nx, ny);

    texCoord.resize(4);
    texCoord[0] = Vector2(1, 0);
    texCoord[1] = Vector2(1, 1);
    texCoord[2] = Vector2(0, 1);
    texCoord[3] = Vector2(0, 0);

    normal.insert(normal.end(), 4, Vector3(0, 0, 1));
}

void Sphere(std::vector<uint16_t>& idx, std::vector<Vector3>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texCoord, const Vector3& size,
        size_t res) {
    size_t width = res*2;
    size_t height = res;

    const float r = 1;

    vertex.reserve(width*height);
    normal.reserve(width*height);
    texCoord.reserve(width*height);

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float el = float(j) / (height - 1) * M_PI;
            float az = float(i) / (width - 1) * M_PI * 2;

            Vector3 pos(r * sin(el) * cos(az), r * cos(el), r * sin(el) * sin(az));
            vertex.push_back(Vector3(size[0]*pos[0], size[1]*pos[1], size[2]*pos[2]));
            normal.push_back(pos);

            Vector2 tpos(1 - float(i) / (width - 1), 1 - float(j) / (height - 1));
            //Vector2 tpos(0.5 - atan2(pos[2], pos[0])/(2*M_PI), 0.5 - 2*asin(pos[1])/(2*M_PI));

            texCoord.push_back(tpos);
        }
    }

    idx.reserve((width-1)*(height-1)*2*3);

    for (size_t j = 0; j < height - 1; j++) {
        for (size_t i = 0; i < width - 1; i++) {
            idx.push_back(j * width + i + 1);
            idx.push_back((j + 1) * width + i);
            idx.push_back(j * width + i);

            idx.push_back(j * width + i + 1);
            idx.push_back((j + 1) * width + i + 1);
            idx.push_back((j + 1) * width + i);
        }
    }
}

void Cylinder(std::vector<uint16_t>& idx, std::vector<Vector3>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texture, const Vector3& size, size_t res) {
    float sx = size[0], sz = size[2], height = size[1] * 2;
    float da = 2*M_PI / res;

    size_t slices = res;

    // sides(2x), top, bottom all have different normals
    vertex.reserve(slices * 4+2);
    normal.reserve(slices * 4+2);
    texture.reserve(slices * 4+2);
    vertex.resize(slices*2+2);
    normal.resize(slices*2+2);
    texture.resize(slices*4+2);


    // Sides
    for (size_t i = 0; i <= slices; i++) {
        float ds = da * i;
        float x = -sin(ds);
        float z = -cos(ds);
        vertex[i] = Vector3(sx*x, -height / 2, sz*z);
        vertex[slices + 1 + i] = Vector3(sx*x, height / 2, sz*z);

        normal[i] = Vector3(x, 0, z);
        normal[slices + 1 + i] = normal[i];

        texture[i] = Vector2((double)i/slices,0);
        texture[slices + 1 + i] = Vector2((double)i/slices,1);

        // Bottom
        if (i<slices) {
			texture[2*slices + 2 + i] = Vector2(x/2+0.5, z/2+0.5);
			texture[3*slices + 2 + i] = Vector2(x/2+0.5, -z/2+0.5);
        }
    }

    // Top/Bottom
    vertex.insert(vertex.end(), vertex.begin(), vertex.end()-slices-2); // copy vertices for top/ bottom
    vertex.insert(vertex.end(), vertex.begin()+slices+1, vertex.end()-slices-1);

    normal.insert(normal.end(), slices, Vector3(0, -1, 0));
    normal.insert(normal.end(), slices, Vector3(0, 1, 0));

    idx.reserve(slices*2*3 + 2*(slices-2)*3);

    // sides

    idx.push_back(slices - 1);
    idx.push_back(slices);
    idx.push_back(2*slices + 1);

    idx.push_back(slices - 1);
    idx.push_back(2*slices + 1);
    idx.push_back(2*slices);

    for (size_t i = 1; i < slices; i++) {
        idx.push_back(i - 1);
        idx.push_back(i);
        idx.push_back(slices + 1 + i);

        idx.push_back(i - 1);
        idx.push_back(slices + 1 + i);
        idx.push_back(slices + 1 + i - 1);
    }

    // top/ bottom
    for (size_t i = 0; i < slices - 2; i++) {
        idx.push_back(2*slices + 2);
        idx.push_back(2*slices + 2 + i + 2);
        idx.push_back(2*slices + 2 + i + 1);

        idx.push_back(3*slices + 2);
        idx.push_back(3*slices + 2 + i + 1);
        idx.push_back(3*slices + 2 + i + 2);
    }

}

void Cone(std::vector<uint16_t>& idx, std::vector<Vector3>& vertex, std::vector<Vector3>& normal, std::vector<Vector2>& texture, const Vector3& size, size_t res) {
    float sx = size[0], sz = size[2], height = size[1] * 2;
    float da = 2*M_PI / res;

    size_t slices = res;

    vertex.reserve(3*slices + 1);
    vertex.resize(3*slices + 1);

    normal.reserve(3*slices + 1);
    normal.resize(2*slices + 1);

    texture.reserve(3*slices + 1);
    texture.resize(3*slices + 1);

    // tip
    for (size_t i = 0; i < slices; i++) {
		//float ds = da * i;
		//float x = -sin(ds);
		//float z = -cos(ds);
		normal[i] = Vector3(0, 1, 0);
		vertex[i] = Vector3(0, height / 2, 0);
		texture[i] = Vector2((double)i/slices,1);
	}

    for (size_t i = 0; i < slices; i++) {
        float ds = da * i;
        float x = -sin(ds);
        float z = -cos(ds);
        vertex[slices + i] = Vector3(sx * x, -height / 2, sz * z);
        normal[slices + i] = Vector3(x, 0, z);
        texture[slices + i] = Vector2((double)i/slices,0);
    }
    vertex[2*slices] = Vector3(0, -height / 2, -sz);
	normal[2*slices] = Vector3(0, 0, -1);
	texture[2*slices] = Vector2(1,0);

    //vertex.insert(vertex.end(), vertex.begin() + slices, vertex.end() - 1);
    normal.insert(normal.end(), slices, Vector3(0, -1 , 0)); // bottom
    for (size_t i = 0; i < slices; i++) {
		float ds = da * i;
		float x = sin(ds);
		float z = cos(ds);
		vertex[2*slices + 1 + i] = Vector3(sx * x, -height / 2, sz * z);
		texture[2*slices + 1 + i] = Vector2(x/2+0.5, z/2+0.5);
	}

    idx.reserve(3*(slices - 2 + slices));

    // sides
    for (size_t i = 0; i < slices; i++) {
        idx.push_back(i);
        idx.push_back(slices + i);
        idx.push_back(slices + i + 1);
    }

    // bottom
    for (size_t i = 1; i < slices - 1; i++) {
        idx.push_back(2*slices + 1);
        idx.push_back(2*slices + i + 2);
        idx.push_back(2*slices + i + 1);
    }

    /*
    vertex.reserve(2*slices + 1);
    vertex.resize(slices + 1);

    normal.reserve(2*slices + 1);
    normal.resize(slices + 1);

    texture.reserve(2*slices +1);
    texture.resize(slices + 1);

    // tip
    normal[0] = Vector3(0, 1, 0);
    vertex[0] = Vector3(0, height / 2, 0);
    texture[0] = Vector2(0, 0);

    for (size_t i = 0; i < slices; i++) {
        float ds = da * i;
        float x = sin(ds);
        float z = cos(ds);
        vertex[i + 1] = Vector3(sx * x, -height / 2, sz * z);
        normal[i + 1] = Vector3(x, 0, z);
        texture[i + 1] = Vector2((double)i/slices,1);
    }

    vertex.insert(vertex.end(), vertex.begin() + 1, vertex.end());
    normal.insert(normal.end(), slices, Vector3(0, -1 , 0)); // bottom
    for (size_t i = 0; i < slices; i++) {
		float ds = da * i;
		float x = sin(ds)/2;
		float y = cos(ds)/2;
		texture[slices + i] = Vector2(x+0.5, y+0.5);
	}

    idx.reserve(slices - 2 + slices);


    // sides
    idx.push_back(0);
    idx.push_back(slices);
    idx.push_back(1);

    for (size_t i = 1; i < slices; i++) {
        idx.push_back(0);
        idx.push_back(i);
        idx.push_back(i + 1);
    }

    // bottom
    for (size_t i = 0; i < slices - 2; i++) {
        idx.push_back(1 + slices);
        idx.push_back(1 + slices + i + 2);
        idx.push_back(1 + slices + i + 1);
    }*/
}

}

/* namespace GeoShape */

