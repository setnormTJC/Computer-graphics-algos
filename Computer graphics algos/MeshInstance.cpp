#include "MeshInstance.h"

MeshInstance::MeshInstance(const Vec4& trans, const Vec4& rot, const Vec4& scale)
    : trans(trans), scale(scale)
{
    this->rot.x = fmod(rot.x, 2.0f * M_PI); //wraparound 
    this->rot.y = fmod(rot.y, 2.0f * M_PI);
    this->rot.z = fmod(rot.z, 2.0f * M_PI);
}

void MeshInstance::applyTransformation(const std::vector<Vec4>& localVerts, 
    std::vector<Vec4>& worldVerts) const
{
    Mat4 modelMatrix = Mat4::getModelMatrix(trans, rot, scale); 

    worldVerts.reserve(localVerts.size());

    for (const auto& vert : localVerts)
    {
        Vec4 temp = modelMatrix * vert;
        worldVerts.push_back(temp); 
    }
}

void MeshInstance::logTransformationInfo(const std::string& logFilename) const
{
}

void MeshInstance::setRotation(const Vec4& newRotation)
{
    rot = newRotation; 
}

const Vec4 MeshInstance::getRotation() const
{
    return rot; 
}

void MeshInstance::setTranslation(const Vec4& newTranslation)
{
    trans = newTranslation;
}

const Vec4 MeshInstance::getTranslation() const
{
    return trans; 
}



