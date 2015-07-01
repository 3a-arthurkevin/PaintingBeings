#pragma once

#include "cinder/app/App.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"

#include "cinder/ImageIo.h"

#include "cinder/ip/Resize.h"

#include "cinder/gl/Texture.h"

#include "boost/filesystem/path.hpp"

using namespace ci;

class Image
{
	private:
		ci::ImageSourceRef _imageSource;
		ci::Surface _surface;
		gl::Texture _texture;
		ci::Surface _miniatureSurface;

		//Pour l'interface
		int _miniatureSize;

	public:
		Image();
		~Image();

		ci::ImageSourceRef getImageSource() const;
		ci::Surface getSurface() const;
		gl::Texture getTexture() const;
		ci::Surface getMiniatureSurface() const;

		void setMiniatureSize(const int value);
		int& getMiniatureSize();

		//Charger une image utilisateur lors d'un glisser/déposer d'une image par l'utilisateur
		void loadWithImagePath(boost::filesystem::path path);
		//Ouvre la boite de dialogue pour charger une image utilisateur
		void loadWithoutImagePath();
		
		//Set l'image (utiliser pour le drag & drop d'image)
		void setImage(std::string path);

		//Génère l'image miniature à reformer avec l'algo génétique
		void generateMiniature();
};

