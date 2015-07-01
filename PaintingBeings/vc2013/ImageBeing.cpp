#include "ImageBeing.h"

#include "cinder/app/App.h"


ImageBeing::ImageBeing()
{
	this->_resolution = 1;
}


ImageBeing::~ImageBeing()
{
}

void ImageBeing::setup(const Image image, const int resolution)
{
	this->_resolution = resolution;
	this->_image = image;

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
	if (this->_image.getMiniatureSize() > 0 && this->_shader)
	{
		int i = 0;
		int j = 0;

		int size = this->_image.getMiniatureSize();

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
		
		int size = this->_image.getMiniatureSize();
		this->_shader.uniform("u_size", size);

		//this->_shader.uniform("u_spacing", spacing);

		this->_shader.uniform("u_colorR", r);
		this->_shader.uniform("u_colorG", g);
		this->_shader.uniform("u_colorB", b);

		this->_shader.uniform("u_grayScale", channelColor);

		this->_shader.uniform("u_xPosition", x);
		this->_shader.uniform("u_yPosition", y);

		float random = RANDOM.randFloat(-10.0f, 10.0f);
		this->_shader.uniform("u_random", random);
	}
}

//------------------------------------

Image ImageBeing::getImage() const
{
	return this->_image;
}


int ImageBeing::getResolution()
{
	return this->_resolution;
}


gl::VboMesh ImageBeing::getBeing()
{
	return this->_being;
}


void ImageBeing::setImage(const Image image)
{
	this->_image = image;
}


void ImageBeing::setResolution(const unsigned int value)
{
	this->_resolution = value;
}


void ImageBeing::setBeing(const gl::VboMesh being)
{
	this->_being = being;
}

