#include "Mesh.h"

Mesh::Mesh(const std::vector<Vec4>& localSpaceVerts, const Vec4& trans, const Vec4& rot, const Vec4& scale)
    :localSpaceVerts(localSpaceVerts), trans(trans), scale(scale)
{
    this->rot.x = fmod(rot.x, 2.0f * M_PI); //wraparound 
    this->rot.y = fmod(rot.y, 2.0f * M_PI);
    this->rot.z = fmod(rot.z, 2.0f * M_PI);
}

std::vector<Vec4> Mesh::applyModelMatrix() const
{
    Mat4 modelMatrix = Mat4::getModelMatrix(trans, rot, scale); 

    std::vector<Vec4> worldSpaceVerts;
    worldSpaceVerts.reserve(localSpaceVerts.size());

    for (const auto& vert : localSpaceVerts)
    {
        Vec4 temp = modelMatrix * vert;
        worldSpaceVerts.push_back(temp); 
    }

    return worldSpaceVerts;
}

void Mesh::logTransformationInfo(const std::string& logFilename)
{
}

