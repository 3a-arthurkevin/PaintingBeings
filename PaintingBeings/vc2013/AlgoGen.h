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
		Surface _surfaceMiniatureOriginal;

		// Taille de la poulation pour l'algo g�n�tique
		int _populationSize;
		
		// Score pour le fitting des pixels � atteindre
		int _scoreToAim;
		int _scoreThreshold;

		// Seuil servant � cr�er les nouvelles populations de l'algo g�n�tique
		int _thresholdSurvivor;
		int _thresholdCrossover;
		int _thresholdMutation;
		int _thresholdLeftover;

		// Population d'image
		std::vector<ImageMiniature> _imageMiniaturePopulation;

	public:
		AlgoGen();
		~AlgoGen();

		// Set les param�tres pour lancer l'algo gen en cr�ant la population initial
		void setup(const ci::Surface miniatureSurface);

		// Note la population, trie la liste et cr�e la nouvelle population
		void update();

		// Fonction cappant les seuils pour avoir un cumul egal � 100% (pas plus par moins) <--- fonction vide pour le moment
		void capThreshold();

		// Getter et Setter
		std::vector<ImageMiniature> getImagePopulation();
		Surface getSurfaceMiniatureOriginal() const;
		Surface getBestImage();

		void setPopulation(const int value);
		int& getPopulationSize();

		void setScoreToAim(const int value);
		void setScoreThreshold(const int value);
		int& getScoreToAim();
		int& getScoreThreshold();

		void setThresholdSurvivor(const int value);
		void setThresholdCrossover(const int value);
		void setThresholdMutation(const int value);
		void setThresholdNewPopulation(const int value);
		int& getThresholdSurvivor();
		int& getThresholdCrossover();
		int& getThresholdMutation();
		int& getThresholdNewPopulation();

		// Note l'image
		void rateImage(ImageMiniature& imageToRate);
		// Note la population
		void ratePopulation();

		// Cr�er une image par crossover
		ImageMiniature crossover(ImageMiniature father, ImageMiniature mother);
		// Cr�er une image par mutation
		ImageMiniature mutation(ImageMiniature image);
		// Cr�er une image al�atoirement
		ImageMiniature createRandomImage();

		// Cr�er le pourentage de la nouvelle population pour le
			// Crossover
			// Mutation
			// Les nouveaux � g�n�rer al�atoirement
		void generateCrossover(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> originalPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateMutation(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateLeftover(std::vector<ImageMiniature>& newPopulation, int nbElementToGenerate);
};

