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
		// Image original cot� CPU
		ci::Surface _surface;
		// Image original cot� GPU
		gl::Texture _texture;
		// Image original resize � atteindre par l'algo gene
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
		// Charger une image utilisateur lors d'un glisser/d�poser d'une image par l'utilisateur
		bool loadWithImagePath(boost::filesystem::path path);
		
		// Set l'image (utiliser pour le drag & drop d'image)
		void setImage(Surface surface);

		// G�n�re l'image miniature � reformer avec l'algo g�n�tique
		void generateMiniature();

		// Fait un copie de l'image original (surface)
		void copyImage(const Image image);
};

