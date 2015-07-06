#include "ImageBeing.h"

#include "cinder/app/App.h"


ImageBeing::ImageBeing()
{
	this->_resolution = 1;
	this->_spacing = 1;
	this->_angleMax = 50.0f;
	this->_resolution = 2;
}


ImageBeing::~ImageBeing()
{
}

void ImageBeing::setup(const int imageSize)
{
	this->_imageSize = imageSize;

	this->_being = gl::VboMesh(PrimitiveFactory::createCube(this->_resolution));

	try
	{
		_shader = gl::GlslProg(app::App::loadResource(RES_SHADER_VERT), app::App::loadResource(RES_SHADER_FRAG));
	}
	catch (gl::GlslProgCompileExc ex)
	{
		app::console() << ex.what() << std::endl;
	}
}

void ImageBeing::draw(Surface bestImageBeing)
{
	if (this->_imageSize > 0 && this->_shader)
	{
		int i = 0;
		int j = 0;

		int size = this->_imageSize;

		int index = 0;

		Surface::Iter iterMinSurface = bestImageBeing.getIter();
		
		Channel channelMiniature = Channel(bestImageBeing);
		Channel::Iter iterMinChannel = channelMiniature.getIter();

		size_t count = 1;

		while (iterMinSurface.line() && iterMinChannel.line())
		{
			i = 0;
			while (iterMinSurface.pixel() && iterMinChannel.pixel())
			{
				Color surfaceColor = Color(iterMinSurface.r(), iterMinSurface.g(), iterMinSurface.b());
				int channelColor = iterMinChannel.v();

				this->bindShader(iterMinSurface.r(), iterMinSurface.g(), iterMinSurface.b(), surfaceColor, channelColor, i, j);

				index = (j * size) + i;

				this->_being.enableClientStates();
				this->_being.bindAllData();

				glDrawElementsInstancedARB(this->_being.getPrimitiveType(), this->_being.getNumIndices(), GL_UNSIGNED_INT, (GLvoid*)(sizeof(uint32_t) * 0), count);

				gl::VboMesh::unbindBuffers();
				this->_being.disableClientStates();

				this->_shader.unbind();

				++i;
			}
			++j;
		}
	}
}

void ImageBeing::bindShader(const int r, const int g, const int b, const Color surfaceColor, const int channelColor, const int x, const int y)
{
	if (this->_shader) 
	{
		_shader.bind();
		
		int size = this->_imageSize;
		this->_shader.uniform("u_size", size);

		this->_shader.uniform("u_colorR", r);
		this->_shader.uniform("u_colorG", g);
		this->_shader.uniform("u_colorB", b);

		this->_shader.uniform("u_grayScale", channelColor);

		this->_shader.uniform("u_xPosition", x);
		this->_shader.uniform("u_yPosition", y);

		float random = RANDOM.randFloat(-1.0f, 1.0f);
		this->_shader.uniform("u_random", random);

		float time = static_cast<float>(ci::app::getElapsedSeconds());
		this->_shader.uniform("u_time", time);

		this->_shader.uniform("u_height", 1.0f);
		this->_shader.uniform("u_angleMax", _angleMax);

		this->_shader.uniform("u_spacing", _spacing);
	}
}

//------------------------------------

float& ImageBeing::getSpacing()
{
	return this->_spacing;
}

void ImageBeing::setSpacing(const float value)
{
	this->_spacing = value;
}

int ImageBeing::getImageSize() const
{
	return this->_imageSize;
}


int& ImageBeing::getResolution()
{
	return this->_resolution;
}


gl::VboMesh ImageBeing::getBeing()
{
	return this->_being;
}


void ImageBeing::setImageSize(const int imageSize)
{
	this->_imageSize = imageSize;;
}


void ImageBeing::setResolution(const unsigned int value)
{
	this->_resolution = value;
}


void ImageBeing::setBeing(const gl::VboMesh being)
{
	this->_being = being;
}

float& ImageBeing::getAngleMax()
{
	return this->_angleMax;
}