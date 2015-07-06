#include "AlgoGen.h"


AlgoGen::AlgoGen()
{
	this->_populationSize = 25;

	this->_scoreToAim = 0;
	this->_scoreThreshold = 20;

	this->_thresholdSurvivor = 0.1f;
	this->_thresholdCrossover = 0.3f;
	this->_thresholdMutation = 0.3f;
	this->_thresholdLeftover = 0.3f;
}


AlgoGen::~AlgoGen()
{
}

void AlgoGen::setup(const ci::Surface miniatureSurface)
{
	capThreshold();

	this->_surfaceMiniatureOriginal = miniatureSurface.clone();
	
	this->_imageMiniaturePopulation.clear();

	for (int i = 0; i < this->_populationSize; ++i)
		this->_imageMiniaturePopulation.push_back(this->createRandomImage());
}

void AlgoGen::update()
{
	int nbImage = this->_populationSize;

	//Rate the population before create a new one
	this->ratePopulation();

	//Sorting
	std::sort(this->_imageMiniaturePopulation.begin(), this->_imageMiniaturePopulation.end());

	//Creating new population
	std::vector<ImageMiniature> newPopulation;

	//Getting number of survivor / crossed / mutated / leftover ImageMiniature
	int nbSurvivor = static_cast<int>(this->_populationSize * this->_thresholdSurvivor);
	int nbCrossover = static_cast<int>(this->_populationSize * this->_thresholdCrossover);
	int nbMutation = static_cast<int>(this->_populationSize * this->_thresholdMutation);
	int nbLeftover = nbImage - (nbSurvivor + nbCrossover + nbMutation);

	//Filling the new population
	auto iteratorBegin = this->_imageMiniaturePopulation.begin();
	newPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + nbSurvivor));

	iteratorBegin = this->_imageMiniaturePopulation.begin();
	auto crossOverPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + nbCrossover));

	iteratorBegin = this->_imageMiniaturePopulation.begin();
	auto mutationPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + nbMutation));

	this->generateCrossover(newPopulation, this->_imageMiniaturePopulation, crossOverPopulation, nbCrossover);
	this->generateMutation(newPopulation, mutationPopulation, nbMutation);
	this->generateLeftover(newPopulation, nbLeftover);

	this->_imageMiniaturePopulation = newPopulation;
}

std::vector<ImageMiniature> AlgoGen::getImagePopulation()
{
	return this->_imageMiniaturePopulation;
}

Surface AlgoGen::getSurfaceMiniatureOriginal() const
{
	return this->_surfaceMiniatureOriginal;
}

Surface AlgoGen::getBestImage()
{
	return this->_imageMiniaturePopulation[0].getSurface();
}

void AlgoGen::capThreshold()
{
	if (this->_thresholdSurvivor + this->_thresholdCrossover + this->_thresholdMutation > 1.0f)
	{
		this->_thresholdSurvivor = 0.1f;
		this->_thresholdCrossover = 0.3f;
		this->_thresholdMutation = 0.3f;
		this->_thresholdLeftover = 1.0f - (this->_thresholdSurvivor + this->_thresholdCrossover + this->_thresholdMutation);
	}
}

void AlgoGen::setPopulation(const int value)
{
	this->_populationSize = value;
}

int& AlgoGen::getPopulationSize()
{
	return this->_populationSize;
}


void AlgoGen::setScoreToAim(const int value)
{
	this->_scoreToAim = value;
}

void AlgoGen::setScoreThreshold(const int value)
{
	this->_scoreThreshold = value;
}

int& AlgoGen::getScoreToAim()
{
	return this->_scoreToAim;
}

int& AlgoGen::getScoreThreshold()
{
	return this->_scoreThreshold;
}


void AlgoGen::setThresholdSurvivor(const float value)
{
	this->_thresholdSurvivor = value;
}

void AlgoGen::setThresholdCrossover(const float value)
{
	this->_thresholdCrossover = value;
}

void AlgoGen::setThresholdMutation(const float value)
{
	this->_thresholdMutation = value;
}

void AlgoGen::setThresholdNewPopulation(const float value)
{
	this->_thresholdLeftover = value;
}

float& AlgoGen::getThresholdSurvivor()
{
	return this->_thresholdSurvivor;
}

float& AlgoGen::getThresholdCrossover()
{
	return this->_thresholdCrossover;
}

float& AlgoGen::getThresholdMutation()
{
	return this->_thresholdMutation;
}

float& AlgoGen::getThresholdNewPopulation()
{
	return this->_thresholdLeftover;
}


ImageMiniature AlgoGen::crossover(ImageMiniature father, ImageMiniature mother)
{
	ImageMiniature crossedImage = mother;

	Surface::Iter iterFather = father.getSurface().getIter();
	Surface::Iter iterCrossedImage = crossedImage.getSurface().getIter();

	while (iterFather.line() && iterCrossedImage.line())
	{
		while (iterFather.pixel() && iterCrossedImage.pixel())
		{
			if (RANDOM.randBool())
			{
				iterCrossedImage.r() = iterFather.r();
				iterCrossedImage.g() = iterFather.g();
				iterCrossedImage.b() = iterFather.b();
			}
		}
	}
	return crossedImage;
}

ImageMiniature AlgoGen::mutation(ImageMiniature image)
{
	ImageMiniature mutatedImage = image;
	int width = mutatedImage.getSurface().getWidth();
	int height = mutatedImage.getSurface().getHeight();

	int randomWidthPosition = RANDOM.randInt(0, width);
	int randomHeightPosition = RANDOM.randInt(0, height);

	int r =  RANDOM.randInt(0, 256);
	int g = RANDOM.randInt(0, 256);
	int b = RANDOM.randInt(0, 256);

	mutatedImage.getSurface().setPixel(Vec2i(randomWidthPosition, randomHeightPosition), Color8u(r, g, b));

	return mutatedImage;
}

ImageMiniature AlgoGen::createRandomImage()
{
	ImageMiniature generatedImage(this->_surfaceMiniatureOriginal.getWidth(), this->_surfaceMiniatureOriginal.getHeight());

	Surface::Iter iterImage = generatedImage.getSurface().getIter();

	while (iterImage.line())
	{
		while (iterImage.pixel())
		{
			iterImage.r() = RANDOM.randInt(0, 256);
			iterImage.g() = RANDOM.randInt(0, 256);
			iterImage.b() = RANDOM.randInt(0, 256);
		}
	}
	return generatedImage;
}

void AlgoGen::rateImage(ImageMiniature& imageToRate)
{
	//originalImage and imageToRate has the same resolution
	Surface::Iter iterOriginalImage = this->_surfaceMiniatureOriginal.getIter();
	Surface::Iter iterImageToRate = imageToRate.getSurface().getIter();

	int ratingPixel = 0;
	int scoreTotal = 0;

	while (iterOriginalImage.line() && iterImageToRate.line())
	{
		while (iterOriginalImage.pixel() && iterImageToRate.pixel())
		{
			ratingPixel = 0;

			ratingPixel += abs(iterOriginalImage.r() - iterImageToRate.r());
			ratingPixel += abs(iterOriginalImage.g() - iterImageToRate.g());
			ratingPixel += abs(iterOriginalImage.b() - iterImageToRate.b());
			
			if (ratingPixel >= this->_scoreThreshold * 6)
				scoreTotal += 80;
			else if (ratingPixel >= this->_scoreThreshold * 5)
				scoreTotal += 40;
			else if (ratingPixel >= this->_scoreThreshold * 4)
				scoreTotal += 20;
			else if (ratingPixel >= this->_scoreThreshold * 3)
				scoreTotal += 10;
			else if (ratingPixel >= this->_scoreThreshold * 2)
				scoreTotal += 5;
			else if (ratingPixel >= this->_scoreThreshold)
				scoreTotal += 2;
			else if (ratingPixel >= (this->_scoreThreshold/2))
				scoreTotal += 1;
			else
				scoreTotal += 0;
			
		}
	}
	imageToRate.setScore(scoreTotal);
}

void AlgoGen::ratePopulation()
{
	for (auto it = this->_imageMiniaturePopulation.begin(); it != this->_imageMiniaturePopulation.end(); ++it)
		this->rateImage(*it);
}

void AlgoGen::generateCrossover(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> originalPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate)
{
	unsigned int nbElementOriginal = survivorPopulation.size();
	unsigned int nbElementCross = originalPopulation.size();

	ImageMiniature father;
	ImageMiniature mother;

	while (nbElementToGenerate > 0)
	{
		mother = originalPopulation[RANDOM.randInt(0, nbElementCross)];
		father = survivorPopulation[RANDOM.randInt(0, nbElementOriginal)];

		newPopulation.push_back(this->crossover(father, mother));

		--nbElementToGenerate;
	}
}

void AlgoGen::generateMutation(std::vector<ImageMiniature>& newPopulation, const std::vector<ImageMiniature> survivorPopulation, int nbElementToGenerate)
{
	unsigned int nbElement = survivorPopulation.size();

	ImageMiniature imageToMutate;

	while (nbElementToGenerate > 0)
	{
		imageToMutate = survivorPopulation[RANDOM.randInt(0, nbElement)];

		newPopulation.push_back(this->mutation(imageToMutate));

		--nbElementToGenerate;
	}
}

void AlgoGen::generateLeftover(std::vector<ImageMiniature>& newPopulation, int nbElementToGenerate)
{
	while (nbElementToGenerate > 0)
	{
		newPopulation.push_back(this->createRandomImage());

		--nbElementToGenerate;
	}
}

