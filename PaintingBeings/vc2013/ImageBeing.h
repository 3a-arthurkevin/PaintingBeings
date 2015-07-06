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
		// Taille de l'image � g�n�rer
		int _imageSize;
		// Espace entre le being
		float _spacing;
		// R�solution des fomres d'un being
		int _resolution;
		// Being GPU
		gl::VboMesh _being;
		// Ficher shader
		gl::GlslProg _shader;

		// Pour l'interface et la d�formation
		float _angleMax;

	public:
		ImageBeing();
		~ImageBeing();

		//Set l'image de base � recr�er et Cr�er la forme du Being pour l'affichage de l'image dans draw()
		void setup(const int imageSize);
		//Cr�ation de l'image � partir de la meilleur image de la population de l'algo gen
		void draw(Surface bestImageBeing);

		//Binding des param�tres � passer aux shaders
		void bindShader(const int r, const int g, const int b, const Color surfaceColor, const int channelColor, const int x, const int y);

		// Setter Getter
		float& getSpacing();
		void setSpacing(const float value);

		int getImageSize() const;
		int& getResolution();

		gl::VboMesh getBeing();

		void setImageSize(const int imageSize);
		void setResolution(const unsigned int value);
		void setBeing(const gl::VboMesh being);

		float& getAngleMax();
};

