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

		// Taille de la poulation pour l'algo génétique
		int _populationSize;
		
		// Score pour le fitting des pixels à atteindre
		int _scoreToAim;
		int _scoreThreshold;

		// Seuil servant à créer les nouvelles populations de l'algo génétique
		int _thresholdSurvivor;
		int _thresholdCrossover;
		int _thresholdMutation;
		int _thresholdLeftover;

		// Population d'image
		std::vector<ImageMiniature> _imageMiniaturePopulation;

	public:
		AlgoGen();
		~AlgoGen();

		// Set les paramètres pour lancer l'algo gen en créant la population initial
		void setup(const ci::Surface miniatureSurface);

		// Note la population, trie la liste et crée la nouvelle population
		void update();

		// Fonction cappant les seuils pour avoir un cumul egal à 100% (pas plus par moins) <--- fonction vide pour le moment
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

		// Créer une image par crossover
		ImageMiniature crossover(ImageMiniature father, ImageMiniature mother);
		// Créer une image par mutation
		ImageMiniature mutation(ImageMiniature image);
		// Créer une image aléatoirement
		ImageMiniature createRandomImage();

		// Créer le pourentage de la nouvelle population pour le
			// Crossover
			// Mutation
			// Les nouveaux à générer aléatoirement
		void generateCrossover(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> originalPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateMutation(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate);
		void generateLeftover(std::vector<ImageMiniature>& newPopulation, int nbElementToGenerate);
};

