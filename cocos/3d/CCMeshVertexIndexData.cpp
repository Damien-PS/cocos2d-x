/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <cocos/3d/CCMeshVertexIndexData.h>

#include <cocos/3d/CCBundle3D.h>
#include <cocos/renderer/CCVertexIndexBuffer.h>
#include <cocos/renderer/CCVertexIndexData.h>

#include <fstream>
#include <new>
#include <string>

using namespace std;

NS_CC_BEGIN

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshIndexData* MeshIndexData::create(const std::string& id, MeshVertexData* vertexData, IndexBuffer* indexbuffer, const AABB& aabb)
{
    auto meshindex = new (std::nothrow) MeshIndexData();

    meshindex->_id = id;
    meshindex->_indexBuffer = indexbuffer;
    meshindex->_vertexData = vertexData;
    indexbuffer->retain();
    meshindex->_aabb = aabb;

    meshindex->autorelease();
    return meshindex;
}

const VertexBuffer* MeshIndexData::getVertexBuffer() const
{
    return _vertexData->getVertexBuffer();
}

MeshIndexData::MeshIndexData()
: _indexBuffer(nullptr)
, _vertexData(nullptr)
, _primitiveType(GL_TRIANGLES)
{
}
MeshIndexData::~MeshIndexData()
{
    CC_SAFE_RELEASE(_indexBuffer);
}

MeshVertexData* MeshVertexData::create(const MeshData& meshdata)
{
    auto vertexdata = new (std::nothrow) MeshVertexData();
    int pervertexsize = meshdata.getPerVertexSize();
    vertexdata->_vertexBuffer = VertexBuffer::create(pervertexsize, static_cast<int>(meshdata.vertex.size() / (pervertexsize / 4)));
    vertexdata->_vertexData = VertexData::create();
    CC_SAFE_RETAIN(vertexdata->_vertexData);
    CC_SAFE_RETAIN(vertexdata->_vertexBuffer);

    int offset = 0;
    for (const auto& it : meshdata.attribs)
    {
        vertexdata->_vertexData->setStream(vertexdata->_vertexBuffer, VertexStreamAttribute(offset, it.vertexAttrib, it.type, it.size));
        offset += it.attribSizeBytes;
    }

    vertexdata->_attribs = meshdata.attribs;

    if (vertexdata->_vertexBuffer)
    {
        vertexdata->_vertexBuffer->updateVertices(reinterpret_cast<void const*>(&meshdata.vertex[0]),
                                                  static_cast<int>(meshdata.vertex.size()) * 4 / vertexdata->_vertexBuffer->getSizePerVertex(), 0);
    }

    bool needCalcAABB = (meshdata.subMeshAABB.size() != meshdata.subMeshIndices.size());
    for (size_t i = 0; i < meshdata.subMeshIndices.size(); i++)
    {
        auto& index = meshdata.subMeshIndices[i];
        auto indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::INDEX_TYPE_SHORT_16, static_cast<int>(index.size()));
        indexBuffer->updateIndices(&index[0], static_cast<int>(index.size()), 0);
        std::string id = (i < meshdata.subMeshIds.size() ? meshdata.subMeshIds[i] : "");
        MeshIndexData* indexdata = nullptr;
        if (needCalcAABB)
        {
            auto aabb = Bundle3D::calculateAABB(meshdata.vertex, meshdata.getPerVertexSize(), index);
            indexdata = MeshIndexData::create(id, vertexdata, indexBuffer, aabb);
        }
        else
            indexdata = MeshIndexData::create(id, vertexdata, indexBuffer, meshdata.subMeshAABB[i]);

        vertexdata->_indexs.pushBack(indexdata);
    }

    vertexdata->autorelease();
    return vertexdata;
}

MeshIndexData* MeshVertexData::getMeshIndexDataById(const std::string& id) const
{
    for (auto it : _indexs)
    {
        if (it->getId() == id)
            return it;
    }
    return nullptr;
}

bool MeshVertexData::hasVertexAttrib(int attrib) const
{
    for (const auto& it : _attribs)
    {
        if (it.vertexAttrib == attrib)
            return true;
    }
    return false;
}

MeshVertexData::MeshVertexData()
: _vertexData(nullptr)
, _vertexBuffer(nullptr)
, _vertexCount(0)
{
}
MeshVertexData::~MeshVertexData()
{
    CC_SAFE_RELEASE(_vertexData);
    CC_SAFE_RELEASE(_vertexBuffer);
    _indexs.clear();
}

NS_CC_END
