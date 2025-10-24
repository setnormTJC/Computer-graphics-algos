#define _USE_MATH_DEFINES
#include "Mat4.h"

#include<iomanip> 
#include "MyException.h"

Mat4::Mat4()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            elements[row][col] = 0;
        }
    }
}

void Mat4::set(const int row, const int column, float value)
{
    if (row < 0 || row > 3 || column < 0 || column > 3)
    {
        throw MyException("row and column must be between 0 and 3", __LINE__, __FILE__);
    }

    elements[row][column] = value; 
}

void Mat4::operator=(const std::array<std::array<float, 4>, 4>& clientElements)
{
    elements = clientElements; 
}

Mat4 Mat4::operator*(const Mat4& rhs) const
{
    Mat4 product; 

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            float sum{};
            for (int k = 0; k < 4; ++k)
            {
                sum += elements[row][k] * rhs.elements[k][col];
            }
            product.elements[row][col] = sum; 
        }
    }

    return product;
}

Mat4 Mat4::operator*(const float rhs) const
{
    Mat4 product; 

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            float newValue = elements[row][col] * rhs;
            product.elements[row][col] += newValue;
        }
    }

    return product;
}

Vec4 Mat4::operator*(const Vec4& rhs) const
{
    Vec4 newVec{};

    
    for (int row = 0; row < 4; ++row)
    {
        float sum{}; 

        for (int col = 0; col < 4; ++col)
        {
            sum += elements[row][col] * rhs[col];
        }
        newVec[row] = sum; 
    }

    return newVec;
}

Mat4 Mat4::getRotationMatrix(const Vec4& rot)
{
    Mat4 x; 

    x.elements =
    { {
        {1.0f,  0.0f,           0.0f,           0.0f},
        {0.0f,  cos(rot.x),     -sin(rot.x),    0.0f},
        {0.0f,  sin(rot.x),     cos(rot.x),     0.0f},
        {0.0f,  0.0f,           0.0f,           1.0f}
    } };

    Mat4 y; 
    y.elements =
    { {
        {cos(rot.y),    0.0f,           sin(rot.y),     0.0f},
        {0.0f,          1.0f,           0.0f,           0.0f},
        {-sin(rot.y),   0.0f,           cos(rot.y),     0.0f},
        {0.0f,          0.0f,           0.0f,           1.0f}
    } };

    Mat4 z;
    z.elements =
    { {
        { cos(rot.z),   -sin(rot.z),    0.0f,   0.0f },
        { sin(rot.z),    cos(rot.z),    0.0f,   0.0f },
        { 0.0f,          0.0f,          1.0f,   0.0f },
        { 0.0f,          0.0f,          0.0f,   1.0f }
    } };

    return z * y * x; //again, column-major order, I suppose
}

Mat4 Mat4::getModelMatrix(const Vec4& trans, const Vec4& rot, const Vec4& scale)
{
    //see (among others) figure 1 in: https://www.mdpi.com/2227-7390/10/11/1859
    Mat4 transMatrix;
    transMatrix.elements =
    { {
        {1.0f,  0.0f,   0.0f,   trans.x},
        {0.0f,  1.0f,   0.0f,   trans.y},
        {0.0f,  0.0f,   1.0f,   trans.z},
        {0.0f,  0.0f,   0.0f,   1.0f}
    } };

    float theta{}; // = ? (arccos(rot.y/rot.x)?
    
    Mat4 rotMatrix = getRotationMatrix(rot); 

    Mat4 scaleMatrix;
    scaleMatrix.elements =
    { {
        {scale.x,   0.0f,       0.0f,       0.0f},
        {0.0f,      scale.y,    0.0f,       0.0f},
        {0.0f,      0.0f,       scale.z,    0.0f},
        {0.0f,      0.0f,       0.0f,       1.0f}
    } };

    return transMatrix * rotMatrix * scaleMatrix;//TRS is COLUMN-MAJOR order 

}

Mat4 Mat4::getProjectionMatrix(const float zFar, const float zNear, float fovY, float aspectRatio)
{

    if (zNear <= 0.0f || zFar <= 0.0f || zNear >= zFar)
    {
        throw MyException("Invalid zNear/zFar values. Must satisfy 0 < zNear < zFar.", __LINE__, __FILE__);
    }

    if (fovY <= 0.0f || fovY >= M_PI) //tan goes to infinity at multiples of pi/2
    {
        throw MyException("Invalid field of view. fovY must be in the range (0, PI) radians.", __LINE__, __FILE__);
    }

    if (aspectRatio <= 0.0f)
    {
        throw MyException("Aspect ratio must be positive.", __LINE__, __FILE__);
    }

    Mat4 projectionMatrix;

    // Scale factors for x and y

    float denom = std::tan(fovY * 0.5f); //multiply by 0.5 because tan(pi/4) = 1 (pi/2) -> 90 degree vertical FOV
    float f = 1.0f / denom; // cotangent of half vertical FOV
    
    float a = f / aspectRatio;               // horizontal scaling
    float b = f;                             // vertical scaling

    // z-mapping to NDC [-1,1]
    float c = -(zFar + zNear) / (zFar - zNear); //NO change to c and d compared to the other overload of this function 
    float d = -(2.0f * zFar * zNear) / (zFar - zNear); //again, magic number 2 gives NDC -> [-1, 1] 

    float e = -1.0f;

    projectionMatrix = { {
        {a,     0.0f,   0.0f,   0.0f},
        {0.0f,  b,      0.0f,   0.0f},
        {0.0f,  0.0f,   c,       d},
        {0.0f,  0.0f,   e,       0.0f}
    } };

    return projectionMatrix;
}

Mat4 Mat4::getViewMatrix(const Vec4& eye, const Vec4& target, const Vec4& up)
{
    auto temp1 = (target - eye); 
    auto forward = temp1.normalize(); 
    
    auto temp2 = forward.cross(up); 
    auto right = temp2.normalize(); 

    auto trueUp = right.cross(forward); 

    Mat4 viewMatrix; //Mat4 is std::array<std::array<float, 4>, 4>>
    
    viewMatrix.elements =
    { {
        {right.x,       right.y,    right.z,        -right.dot(eye)},
        {trueUp.x,      trueUp.y,   trueUp.z,       -trueUp.dot(eye)},
        {-forward.x,    -forward.y, -forward.z,     forward.dot(eye)},
        {0.0f,          0.0f,       0.0f,           1.0f},
    } };

    return viewMatrix;
}

//Mat4 Mat4::getProjectionMatrix(const float zFar, const float zNear)
//{
//    Mat4 projectionMatrix;
//
//    float a = 1.0f; 
//    float b = 1.0f;
//
//    float c = -(zFar + zNear) / (zFar - zNear);
//    //float c = -(zFar) / (zFar - zNear);
//    float d = -(2.0f * zFar * zNear) / (zFar - zNear); //magic 2 has to do with NDC: [-1 to 1]
//    //float d = -(zFar * zNear) / (zFar - zNear);
//
//    float e = -1.0f;
//
//    projectionMatrix =    
//    { {
//        {a,         0.0f,       0.0f,       0.0f},
//        {0.0f,      b,          0.0f,       0.0f},
//        {0.0f,      0.0f,       c,          d},
//        {0.0f,      0.0f,       e,          0.0f}
//    } };
//
//    return projectionMatrix;
//}

std::ostream& operator<<(std::ostream& os, const Mat4& m)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            int COL_WIDTH = 10; 
            std::cout << std::fixed << std::setprecision(1); 
            std::cout << std::left << std::setw(COL_WIDTH) << m.elements[row][col]; 
        }
        std::cout << "\n";
    }
    return os; 
}
