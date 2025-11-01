#include "Light.h"

Light::Light(const Vec4& position, float intensity)
	:position(position), intensity(intensity)
{

}

const Vec4 Light::getPosition() const
{
	return position;
}

const float Light::getIntensity() const
{
	return intensity;
}
