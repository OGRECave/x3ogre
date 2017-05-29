/*
 * OgreMeshAdapter.cpp
 *
 *  Created on: 26.03.2014
 *      Author: parojtbe
 */

#include "OgreMeshAdapter.h"

#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>
#include <OgreVector2.h>

using namespace Ogre;

namespace X3D {
template<class T>
void OgreMeshAdapter(Ogre::Mesh* mesh, RenderOperation::OperationType rop,
		const std::vector<T>& idx,const std::vector<Vector3>& coord, const std::vector<Vector3>& normal,
		const std::vector<Vector2>& texCoord, const std::vector<Vector3>& color) {
    HardwareBufferManager& hbm = HardwareBufferManager::getSingleton();
    HardwareVertexBufferSharedPtr hwbuf;

    SubMesh* sub = mesh->createSubMesh();
    sub->useSharedVertices = true;
    sub->operationType = rop;

    mesh->sharedVertexData = new VertexData;
    mesh->sharedVertexData->vertexCount = coord.size();
    VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;

    uint16_t source = 0;

    // position
    decl->addElement(source, 0, VET_FLOAT3, VES_POSITION);
    hwbuf = hbm.createVertexBuffer(VertexElement::getTypeSize(VET_FLOAT3), coord.size(), HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    hwbuf->writeData(0, hwbuf->getSizeInBytes(), coord[0].ptr(), true);
    mesh->sharedVertexData->vertexBufferBinding->setBinding(source, hwbuf);

    // normal
    if(not normal.empty()) {
        decl->addElement(++source, 0, VET_FLOAT3, VES_NORMAL);
        hwbuf = hbm.createVertexBuffer(VertexElement::getTypeSize(VET_FLOAT3), coord.size(), HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        hwbuf->writeData(0, hwbuf->getSizeInBytes(), normal[0].ptr(), true);
        mesh->sharedVertexData->vertexBufferBinding->setBinding(source, hwbuf);
    }

    // texCoords
    if(not texCoord.empty()) {
        decl->addElement(++source, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        hwbuf = hbm.createVertexBuffer(VertexElement::getTypeSize(VET_FLOAT2), coord.size(), HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        hwbuf->writeData(0, hwbuf->getSizeInBytes(), texCoord[0].ptr(), true);
        mesh->sharedVertexData->vertexBufferBinding->setBinding(source, hwbuf);
    }

    // Color
    if(not color.empty()) {
        decl->addElement(++source, 0, VET_FLOAT3, VES_DIFFUSE);
        hwbuf = hbm.createVertexBuffer(VertexElement::getTypeSize(VET_FLOAT3), coord.size(), HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        hwbuf->writeData(0, hwbuf->getSizeInBytes(), color[0].ptr(), true);
        mesh->sharedVertexData->vertexBufferBinding->setBinding(source, hwbuf);
    }

    if(not idx.empty()) {
        HardwareIndexBuffer::IndexType it = HardwareIndexBuffer::IT_16BIT;

        if(sizeof(T) != sizeof(uint16_t) and coord.size() >= std::numeric_limits<uint16_t>::max()) {
            it = HardwareIndexBuffer::IT_32BIT;
        }

        HardwareIndexBufferSharedPtr idxbuf = hbm.createIndexBuffer(it, idx.size(), HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        sub->indexData->indexBuffer = idxbuf;
        sub->indexData->indexCount = idx.size();
        sub->indexData->indexStart = 0;

        if(it == HardwareIndexBuffer::IT_16BIT and sizeof(T) != sizeof(uint16_t)) {
            std::vector<uint16_t> tmp;
            tmp.insert(tmp.end(), idx.begin(), idx.end());

            idxbuf->writeData(0, idxbuf->getSizeInBytes(), &tmp[0], true);
        } else {
            idxbuf->writeData(0, idxbuf->getSizeInBytes(), &idx[0], true);
        }
    }

    AxisAlignedBox bbox;
    bbox.setNull();

    for(const Vector3& p : coord) {
        bbox.merge(p);
    }

    mesh->_setBounds(bbox);
}

// some instantiations
template void OgreMeshAdapter<uint16_t>(Ogre::Mesh* mesh, RenderOperation::OperationType rop,
		const std::vector<uint16_t>& idx, const std::vector<Vector3>& coord, const std::vector<Vector3>& normal,
        const std::vector<Vector2>& texCoord, const std::vector<Vector3>& color);

template void OgreMeshAdapter<uint32_t>(Ogre::Mesh* mesh, RenderOperation::OperationType rop,
		const std::vector<uint32_t>& idx, const std::vector<Vector3>& coord, const std::vector<Vector3>& normal,
        const std::vector<Vector2>& texCoord, const std::vector<Vector3>& color);
} /* namespace X3D */

