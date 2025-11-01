#include "MeshInstance.h"

MeshInstance::MeshInstance(const Vec4& trans, const Vec4& rot, const Vec4& scale)
    : trans(trans), scale(scale)
{
    this->rot.x = fmodf(rot.x, 2.0f * (float)M_PI); //wraparound 
    this->rot.y = fmodf(rot.y, 2.0f * (float)M_PI);
    this->rot.z = fmodf(rot.z, 2.0f * (float)M_PI);
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

/*@param vertexColors -> will be MODIFIED based on dot product of light and vertexNormals (a member of Mesh)*/
void MeshInstance::applyLight(const Mesh& mesh, std::vector<Color>& vertexColors, const Light& light)
{

    auto localVerts = mesh.getLocalVertices(); 
    auto localVertexNormals = mesh.getLocalVertexNormals(); 

    auto lightPosition = light.getPosition(); 
    float intensity = light.getIntensity(); 

    if (localVerts.size() != vertexColors.size()
        ||
        localVerts.size() != localVertexNormals.size())
    {
        throw MyException("Mesh vertices, colors, and normals must match in size.", __LINE__, __FILE__);
    }

    for (size_t i = 0; i < localVerts.size(); ++i)
    {
        Vec4 L = lightPosition - localVerts[i]; 
        //normalize L: 
        L = L.normalize(); 

        Vec4 N = localVertexNormals[i].normalize(); 


        float NdotL = N.dot(L);         //diffuse factor??

        float brightness = NdotL * intensity;

        vertexColors[i].setR(std::min(1.0f, vertexColors[i].getR8() * brightness));
        vertexColors[i].setG(std::min(1.0f, vertexColors[i].getG8() * brightness));
        vertexColors[i].setB(std::min(1.0f, vertexColors[i].getB8() * brightness));
    }
}



