#include "Image.h"

Image::Image()
{
	this->_miniatureSize = 30;
}


Image::~Image()
{
}

void Image::setMiniatureSize(const int value)
{
	this->_miniatureSize = value;
}

int& Image::getMiniatureSize()
{
	return this->_miniatureSize;
}

ci::ImageSourceRef Image::getImageSource() const
{
	return this->_imageSource;
}

ci::Surface Image::getSurface() const
{
	return this->_surface;
}

gl::Texture Image::getTexture() const
{
	return this->_texture;
}

ci::Surface Image::getMiniatureSurface() const
{
	return this->_miniatureSurface;
}

bool Image::loadWithoutImagePath()
{
	try
	{
		fs::path imagePath = ci::app::getOpenFilePath("", ImageIo::getLoadExtensions());

		// Si string vide, l'utilisateur a annuler le choix de son image
		if (!imagePath.empty())
		{
			this->_imageSource = ci::loadImage(imagePath);
			this->_surface = this->_imageSource;
			this->_texture = gl::Texture(this->_surface);

			this->generateMiniature();

			return true;
		}
		else
		{
			app::console() << "Image path is empty !" << std::endl;
			return false;
		}
	}
	catch (...)
	{
		app::console() << "Unable to load the image." << std::endl;
		return false;
	}
}

bool Image::loadWithImagePath(boost::filesystem::path path)
{
	try
	{
		this->_imageSource = ci::loadImage(path);
		this->_surface = this->_imageSource;
		this->_texture = gl::Texture(this->_surface);

		this->generateMiniature();

		return true;
	}
	catch (...)
	{
		app::console() << "Unable to load the image." << std::endl;
		return false;
	}
}

void Image::setImage(Surface surface)
{
	this->_surface = surface;
	this->_texture = gl::Texture(this->_surface);

	this->generateMiniature();
}

void Image::generateMiniature()
{
	this->_miniatureSurface = Surface(this->_miniatureSize, this->_miniatureSize, false);

	ip::resize(this->_surface, &(this->_miniatureSurface));
}

void Image::copyImage(const Image image)
{
	this->_imageSource = image._imageSource;
	this->_miniatureSize = image._miniatureSize;
	this->_texture = image._texture.weakClone();//gl::Texture(image._texture);
	this->_surface = image._surface.clone();
	this->_miniatureSurface = image._miniatureSurface.clone();
}