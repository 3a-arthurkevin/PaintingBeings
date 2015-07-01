#pragma once

#include <vector>
#include <algorithm> 

#include "cinder/app/App.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include "Image.h"
#include "ImageMiniature.h"

#include "Constants.h"

using namespace ci;

class AlgoGen
{
	private:
		Image _image;

		int _populationSize;
		
		int _scoreToAim;
		int _scoreThreshold;

		float _thresholdSurvivor;
		float _thresholdCrossover;
		float _thresholdMutation;
		float _thresholdLeftover;

		std::vector<ImageMiniature> _imageMiniaturePopulation;

		Surface _imageToDisplay;

	public:
		AlgoGen();
		~AlgoGen();

		void setup(const Image image);
		void update();

		std::vector<ImageMiniature> getImagePopulation()
		{
			return _imageMiniaturePopulation;
		}

		Surface getImageToDisplay()
		{
			return _imageToDisplay;
		}

		void capThreshold();

		void setImage(const Image image);
		Image getImage() const;

		void setPopulation(const int value);
		int& getPopulationSize();

		void setScoreToAim(const int value);
		void setScoreThreshold(const int value);
		int& getScoreToAim();
		int& getScoreThreshold();

		void setThresholdSurvivor(const float value);
		void setThresholdCrossover(const float value);
		void setThresholdMutation(const float value);
		void setThresholdNewPopulation(const float value);
		float& getThresholdSurvivor();
		float& getThresholdCrossover();
		float& getThresholdMutation();
		float& getThresholdNewPopulation();

		void rateImage(ImageMiniature& imageToRate);
		void ratePopulation();

		ImageMiniature crossover(ImageMiniature father, ImageMiniature mother);
		ImageMiniature mutation(ImageMiniature image);
		ImageMiniature createRandomImage();

		void generateCrossover(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> originalPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateMutation(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateLeftover(std::vector<ImageMiniature>& newPopulation, int nbElementToGenerate);
};

