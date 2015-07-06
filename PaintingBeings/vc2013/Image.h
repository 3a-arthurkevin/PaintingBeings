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
		// Path image original
		ci::ImageSourceRef _imageSource;
		// Image original coté CPU
		ci::Surface _surface;
		// Image original coté GPU
		gl::Texture _texture;
		// Image original resize à atteindre par l'algo gene
		ci::Surface _miniatureSurface;

		//Pour l'interface
		int _miniatureSize;

	public:
		Image();
		~Image();

		// Getter Setter
		ci::ImageSourceRef getImageSource() const;
		ci::Surface getSurface() const;
		gl::Texture getTexture() const;
		ci::Surface getMiniatureSurface() const;

		void setMiniatureSize(const int value);
		int& getMiniatureSize();

		// Ouvre la boite de dialogue pour charger une image utilisateur
		bool loadWithoutImagePath();
		// Charger une image utilisateur lors d'un glisser/déposer d'une image par l'utilisateur
		bool loadWithImagePath(boost::filesystem::path path);
		
		// Set l'image (utiliser pour le drag & drop d'image)
		void setImage(Surface surface);

		// Génère l'image miniature à reformer avec l'algo génétique
		void generateMiniature();

		// Fait un copie de l'image original (surface)
		void copyImage(const Image image);
};

