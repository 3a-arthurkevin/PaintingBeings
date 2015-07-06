#pragma once

#include "cinder/app/App.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"

#include "cinder/gl/Texture.h"

using namespace ci;

class ImageMiniature
{
	private:
		Surface _surface;
		int _score;

	public:
		ImageMiniature();
		ImageMiniature(int width, int height);
		~ImageMiniature();

		void setSurface(const Surface surface);
		void setScore(const int value);

		Surface getSurface();
		int getScore() const;

		void rateImage();

		void operator = (const ImageMiniature & imageMiniature);

		/*
		Overinding of the comparisons operators for ordering
		*/
		bool operator < (const ImageMiniature & imageMiniature) const;
		bool operator > (const ImageMiniature & imageMiniature) const;
		bool operator >= (const ImageMiniature & imageMiniature) const;
		bool operator <= (const ImageMiniature & imageMiniature)const;
		bool operator == (const ImageMiniature & imageMiniature) const;
		bool operator != (const ImageMiniature & imageMiniature) const;

		void copyImageMiniature(const ImageMiniature imageMiniature);
};

