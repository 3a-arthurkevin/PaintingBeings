#include "AlgoGen.h"


AlgoGen::AlgoGen()
{
	this->_populationSize = 25;

	this->_scoreToAim = 0;
	this->_scoreThreshold = 20;

	this->_thresholdSurvivor = 10;
	this->_thresholdCrossover = 30;
	this->_thresholdMutation = 30;
	this->_thresholdLeftover = 30;
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
	float nbSurvivor = static_cast<float>(this->_populationSize * (this->_thresholdSurvivor / 100.0f));
	float nbCrossover = static_cast<float>(this->_populationSize * (this->_thresholdCrossover / 100.0f));
	float nbMutation = static_cast<float>(this->_populationSize * (this->_thresholdMutation / 100.0f));
	float nbLeftover = nbImage - (nbSurvivor + nbCrossover + nbMutation);

	//Filling the new population
	auto iteratorBegin = this->_imageMiniaturePopulation.begin();
	newPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + static_cast<int>(nbSurvivor)));

	iteratorBegin = this->_imageMiniaturePopulation.begin();
	auto crossOverPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + static_cast<int>(nbCrossover)));

	iteratorBegin = this->_imageMiniaturePopulation.begin();
	auto mutationPopulation = std::vector<ImageMiniature>(iteratorBegin, (iteratorBegin + static_cast<int>(nbMutation)));

	this->generateCrossover(newPopulation, this->_imageMiniaturePopulation, crossOverPopulation, static_cast<int>(nbCrossover));
	this->generateMutation(newPopulation, mutationPopulation, static_cast<int>(nbMutation));
	this->generateLeftover(newPopulation, static_cast<int>(nbLeftover));

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
	int total = this->_thresholdSurvivor + this->_thresholdCrossover + this->_thresholdMutation + this->_thresholdLeftover;

	if (total != 100)
	{
		this->_thresholdSurvivor = 10;
		this->_thresholdCrossover = 30;
		this->_thresholdMutation = 30;
		this->_thresholdLeftover = 100 - (this->_thresholdSurvivor + this->_thresholdCrossover + this->_thresholdMutation);
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


void AlgoGen::setThresholdSurvivor(const int value)
{
	this->_thresholdSurvivor = value;
}

void AlgoGen::setThresholdCrossover(const int value)
{
	this->_thresholdCrossover = value;
}

void AlgoGen::setThresholdMutation(const int value)
{
	this->_thresholdMutation = value;
}

void AlgoGen::setThresholdNewPopulation(const int value)
{
	this->_thresholdLeftover = value;
}

int& AlgoGen::getThresholdSurvivor()
{
	return this->_thresholdSurvivor;
}

int& AlgoGen::getThresholdCrossover()
{
	return this->_thresholdCrossover;
}

int& AlgoGen::getThresholdMutation()
{
	return this->_thresholdMutation;
}

int& AlgoGen::getThresholdNewPopulation()
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

