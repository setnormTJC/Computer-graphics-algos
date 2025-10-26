#include "Texture.h"

#include<algorithm>

#include "ImageBMP.h"
#include "MyException.h"

Texture::Texture(const std::string& imageFilename)
{
	ImageBMP image(imageFilename);

	std::vector<std::vector<Color>> pixelMatrix = image.getPixelMatrix();

	flatten(pixelMatrix); //fills the `pixels` member var



}

Color Texture::sample(const Vec2& uv) const
{
    float u = std::clamp(uv.x, 0.0f, 1.0f); 
    float v = std::clamp(uv.y, 0.0f, 1.0f);


    int x = static_cast<int>(u * (textureWidth - 1));
    int y = static_cast<int>(v * (textureHeight - 1));

    size_t index = static_cast<size_t>(y) * textureWidth + x;

    if (index >= pixels.size())
    {
        throw MyException("Texture::sample index out of range", __LINE__, __FILE__);
    }
    
    Color color = pixels[index];
    
    return color;
}

void Texture::flatten(const std::vector<std::vector<Color>>& pixels2D)
{
    textureHeight = pixels2D.size();
    if (textureHeight == 0) throw MyException("Empty texture", __LINE__, __FILE__);

    textureWidth = pixels2D[0].size();

    pixels.reserve(textureWidth * textureHeight);

    for (const auto& row : pixels2D)
    {
        if (row.size() != textureWidth)
            throw MyException("Inconsistent row width in texture", __LINE__, __FILE__);
        pixels.insert(pixels.end(), row.begin(), row.end());
    }
}
