#include "Image.h"

Image::Image()
{
	this->_miniatureSize = 30;

	this->_surface = ci::Surface();

	this->_miniatureSurface = ci::Surface();
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

void Image::loadWithoutImagePath()
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
		}
		else
		{
			app::console() << "Image path is empty !" << std::endl;
		}
	}
	catch (...)
	{
		app::console() << "Unable to load the image." << std::endl;
	}
}

void Image::loadWithImagePath(boost::filesystem::path path)
{
	try
	{
		this->_imageSource = ci::loadImage(path);
		this->_surface = this->_imageSource;
		this->_texture = gl::Texture(this->_surface);

		this->generateMiniature();
	}
	catch (...)
	{
		app::console() << "Unable to load the image." << std::endl;
	}
}

void Image::generateMiniature()
{
	this->_miniatureSurface = Surface(this->_miniatureSize, this->_miniatureSize, false);

	ip::resize(this->_surface, &(this->_miniatureSurface));
}
