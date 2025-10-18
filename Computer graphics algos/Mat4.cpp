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

Mat4 Mat4::getProjectionMatrix(const float zFar, const float zNear)
{
    Mat4 projectionMatrix;

    float a = 1.0f; 
    float b = 1.0f;

    float c = -(zFar + zNear) / (zFar - zNear);
    float d = -(2.0f * zFar * zNear) / (zFar - zNear);

    projectionMatrix =    
    { {
        {a,         0.0f,       0.0f,       0.0f},
        {0.0f,      b,          0.0f,       0.0f},
        {0.0f,      0.0f,       c,          d},
        {0.0f,      0.0f,       -1.0f,      0.0f}
    } };

    return projectionMatrix;
}

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
