#pragma once

#include "PrimitiveFactory.h"
#include "Image.h"

#include "cinder/app/App.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"

#include "cinder/gl/GlslProg.h"

#include "cinder/app/App.h"
#include "../include/Resources.h"

#include "Constants.h"

using namespace ci;

class ImageBeing
{
	private:
		Image _image;

		float _spacing;

		int _resolution;

		gl::VboMesh _being;

		gl::GlslProg _shader;

	public:
		ImageBeing();
		~ImageBeing();

		//Set l'image de base à recréer et Créer la forme du Being pour l'affichage de l'image dans draw()
		void setup(const Image image, const int resolution);
		//Création de l'image à partir de la meilleur image de la population de l'algo gen
		void draw(Surface bestImageBeing);

		//Binding des paramètres à passer aux shaders
		void bindShader(const int r, const int g, const int b, const Color surfaceColor, const int channelColor, const int x, const int y);

		float getSpacing();
		Image getImage() const;
		int getResolution();
		gl::VboMesh getBeing();

		float setSpacing(const float value);
		void setImage(const Image image);
		void setResolution(const unsigned int value);
		void setBeing(const gl::VboMesh being);
};

