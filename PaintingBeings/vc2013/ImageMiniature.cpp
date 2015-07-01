#include "ImageMiniature.h"


ImageMiniature::ImageMiniature()
{
	this->_score = 0;
}

ImageMiniature::ImageMiniature(int width, int height)
{
	this->_score = 0;
	this->_surface = Surface(width, height, false);
}

ImageMiniature::~ImageMiniature()
{
}

void ImageMiniature::setSurface(const Surface image)
{
	this->_surface = image;
}

void ImageMiniature::setScore(const int value)
{
	this->_score = value;
}


Surface ImageMiniature::getSurface()
{
	return this->_surface;
}

int ImageMiniature::getScore() const
{
	return this->_score;
}

void ImageMiniature::operator = (const ImageMiniature & imageMiniature)
{
	this->_surface = imageMiniature._surface.clone();
	this->_score = imageMiniature._score;
}

bool ImageMiniature::operator < (const ImageMiniature & imageMiniature) const
{
	return (this->_score < imageMiniature._score);
}
bool ImageMiniature::operator > (const ImageMiniature & imageMiniature) const
{
	return (imageMiniature < *this);
}
bool ImageMiniature::operator >= (const ImageMiniature & imageMiniature) const
{
	return !(*this < imageMiniature);
}
bool ImageMiniature::operator <= (const ImageMiniature & imageMiniature)const
{
	return !(*this > imageMiniature);
}
bool ImageMiniature::operator == (const ImageMiniature & imageMiniature) const
{
	return !(*this < imageMiniature || *this > imageMiniature);
}
bool ImageMiniature::operator != (const ImageMiniature & imageMiniature) const
{
	return (*this < imageMiniature || *this > imageMiniature);
}