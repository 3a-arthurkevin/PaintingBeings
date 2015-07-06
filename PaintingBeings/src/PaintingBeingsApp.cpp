/*
Copyright(c) 2015, Kevin WATHTHUHEWA - Pierre Bloquet
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

// Include des classes de la solution
#include "AlgoGen.h"
#include "Constants.h"
#include "Image.h"
#include "ImageBeing.h"
#include "PrimitiveFactory.h"

// Include classique de Cinder
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

// Include Cinder
	// Camera
#include "cinder/Arcball.h"
#include "cinder/Camera.h"
	// Capture Camera
#include "cinder/Capture.h"
	// Création Image
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
	// Interface
#include "cinder/params/Params.h"
	// Affichage des textures sur des Rectangles
#include "cinder/Rect.h"

// Include Windows (pour avoir le temps lors du nommage de l'image Screen Shot)
#include <time.h>

using namespace ci;
using namespace ci::app;
using namespace std;

// Classe application
class PaintingBeingsApp : public AppNative 
{
	public:
		//----------------------------------------
		// Fonctions de l'App
		//----------------------------------------

		// Set les valeurs des booléen pour savoir dans quel état est l'App
		void setPlay();
		void setPause();
		void setStop();

		// Set affichage des images ou des Beings
		void setDisplayBeing();

		// Récupére une image venant de la web cam
		void captureCamera();

		// Reset Position de la camera
		void resetCamera();

		// Créer une image à l'endroit ou se trouve l'exe de l'App de la meilleur image de la population de l'AlgoGen
		void screenShot();
		
		// Event souris permettant de déplacer la camera (rotation + zoom)
		void mouseDown(MouseEvent event);
		void mouseDrag(MouseEvent event);
		void mouseWheel(MouseEvent event);

		// Event clavier pour les interactions quelconque (par exemple : Fullscreen)
		void keyDown(KeyEvent event);

		// Event permettant de charger une image en faisant un glisser/déposer
		void fileDrop(FileDropEvent event);

		// Setting des paramètres de l'interface (fonction appelée par Cinder automatiquement ?)
		void prepareSettings(Settings* settings);
		
		// Update interface
		void updateInterface(bool readOnlyAlgoGenParams);


		// Fonction de base de Cinder pour le bon déroulement de l'App 
			// Ordre d'appel [en boucle] par l'App : --> Setup --> Update --> Draw --> Setup --> Update --> ...)
		void setup();	
		void update();
		void draw();

		// Switch pour afficher 
		void setShowWireFrame();

		// Ferme l'application
		void closeApp();
		//----------------------------------------
		// Variables de l'Application
		//----------------------------------------

	private:

		//Algo
		AlgoGen _algoGen;

		//Camera
		ci::Arcball	_arcball;
		ci::CameraPersp	_camera;
		
		//Webcam
		Capture _capture;

		//Image
		Image _image;
		ImageBeing _imageBeing;
		
		//Interface
		params::InterfaceGl _params;

		float _frameRate = 0.0;
		bool _launchAlgoGen = false;
		bool _play = false;
		bool _pause = false;
		bool _stop = true;

		//Option interface
		bool _showWireFrame = false;
		bool _showImageBeing = true;
		
		//Rectangle pour afficher les images
		Rectf _rectangleTextutre;
		Rectf _rectangleAlgoGen;

};



//-------------------------------------------------------------
// Définition de fonction de l'App
//-------------------------------------------------------------

void PaintingBeingsApp::setPlay()
{
	if (_launchAlgoGen)
	{
		_play = true;
		_pause = false;
		_stop = false;
	}
}

void PaintingBeingsApp::setPause()
{
	if (_play)
	{
		_play = false;
		_pause = true;
		_stop = false;
	}
}

void PaintingBeingsApp::setStop()
{
	if (_launchAlgoGen)
	{
		_play = false;
		_pause = false;
		_stop = false;

		_launchAlgoGen = false;

		updateInterface(false);
	}
}

void PaintingBeingsApp::setDisplayBeing()
{
	_showImageBeing = !_showImageBeing;

	float eyeZ = static_cast<float>(_rectangleTextutre.getWidth()) * 2.5f;

	if (_showImageBeing)
		eyeZ = static_cast<float>(_image.getMiniatureSize()) * 2.5f;

	resetCamera();
}

void PaintingBeingsApp::captureCamera()
{
	if (_launchAlgoGen)
	{
		_launchAlgoGen = false;
		setStop();
	}

	if (_capture && _capture.checkNewFrame())
	{
		_image.setImage( _capture.getSurface());
		_algoGen.setup(_image.getMiniatureSurface());
		_imageBeing.setup(_image.getMiniatureSize());

		resetCamera();

		_launchAlgoGen = true;
		_showImageBeing = false;

		setPlay();
	}
}

void PaintingBeingsApp::resetCamera()
{
	float eyeZ = static_cast<float>(_rectangleTextutre.getWidth()) * 2.5f;

	if (_showImageBeing)
		eyeZ = static_cast<float>(_image.getMiniatureSize()) * 2.5f;

	float radiusArcBall = static_cast<float>(getWindowHeight() * 0.5f);

	float fieldOfView = 75.0f;
	float nearCamera = 0.1f;
	float farCamera = 1000000.0f;
	_camera = CameraPersp(getWindowWidth(), getWindowHeight(), fieldOfView, nearCamera, farCamera);

	_camera.lookAt(Vec3f(0.0f, 0.0f, eyeZ), Vec3f::zero());
	_arcball = Arcball(getWindowSize());
	_arcball.setRadius(radiusArcBall);
}

void PaintingBeingsApp::screenShot()
{
	if (_launchAlgoGen)
	{
		time_t  nameFile;
		time(&nameFile);
		writeImage(getAppPath() / ("pb_" + toString(nameFile) + ".png"), _algoGen.getBestImage().clone());
	}
}

void PaintingBeingsApp::mouseDown(MouseEvent event)
{
	_arcball.mouseDown(event.getPos());
}

void PaintingBeingsApp::mouseDrag(MouseEvent event)
{
	_arcball.mouseDrag(event.getPos());
}

void PaintingBeingsApp::mouseWheel(MouseEvent event)
{
	if (_launchAlgoGen)
	{
		Vec3f eye = _camera.getEyePoint();
		eye.z -= event.getWheelIncrement() * 1.5f;

		float eyeMaxZ = static_cast<float>(_image.getMiniatureSize()) * 10.0f;

		if (eye.z > 1.0f && eye.z < eyeMaxZ)
			_camera.setEyePoint(eye);
	}
}

void PaintingBeingsApp::keyDown(KeyEvent event)
{
	//Appuiyer sur f pour mettre l'App en plein écran pour quitter le plein écran
	if (event.getChar() == 'f')
	{
		setFullScreen(!isFullScreen());
	}

	if (event.getChar() == 'r')
	{
		resetCamera();
	}
	/*
	if (event.getCode() == KeyEvent::KEY_RIGHT) 

	if (event.getCode() == KeyEvent::KEY_LEFT) 

	if (event.getCode() == KeyEvent::KEY_UP)

	if (event.getCode() == KeyEvent::KEY_DOWN)
	*/
}

void PaintingBeingsApp::fileDrop(FileDropEvent event)
{		
	if (_image.loadWithImagePath(event.getFile(0)))
	{
		updateInterface(true);

		_algoGen.setup(_image.getMiniatureSurface());
		_imageBeing.setup(_image.getMiniatureSize());

		resetCamera();

		_launchAlgoGen = true;
		_showImageBeing = true;

		setPlay();
	}
	else
		app::console() << "Erreur lors du chargement de l'image" << std::endl;
}

void PaintingBeingsApp::prepareSettings(Settings* settings)
{
	//Setting de la taille de la fenetre
	settings->setWindowSize(640, 480);

	//Setting du frame rate attendu
	settings->setFrameRate(24.0f);
}

void PaintingBeingsApp::updateInterface(bool readOnlyAlgoGenParams)
{
	_params.clear();

	// FrameRate
	_params.addParam("Frame rate", &_frameRate, "", true);

	// Taille de l'image miniature à recréer
	_params.addParam("Image size", &(_image.getMiniatureSize()), "min=10 max=100 step=5", readOnlyAlgoGenParams);

	// Taille de la population de l'algo gen
	_params.addParam("Population size", &(_algoGen.getPopulationSize()), "min=10 max=100 step=5", readOnlyAlgoGenParams);

	// Pourcentage pour l'algo gen
	_params.addParam("% Survivor", &(_algoGen.getThresholdSurvivor()), "min=0.05 max=0.50 step=0.05", readOnlyAlgoGenParams);
	_params.addParam("% Crossover", &(_algoGen.getThresholdCrossover()), "min=0.05 max=0.50 step=0.05", readOnlyAlgoGenParams);
	_params.addParam("% Mutation", &(_algoGen.getThresholdMutation()), "min=0.05 max=0.50 step=0.05", readOnlyAlgoGenParams);
	_params.addParam("% Leftover", &(_algoGen.getThresholdNewPopulation()), "min=0.05 max=0.50 step=0.05", readOnlyAlgoGenParams);

	// Resolution des Beings
	_params.addParam("Resolution", &(_imageBeing.getResolution()), "min=1 max=10 step=1", readOnlyAlgoGenParams);


	// Espacement entre cube de l'image Being
	_params.addParam("Spacing", &(_imageBeing.getSpacing()), "min=0 max=20 step=0.1", false);

	// Frequence du twist pour la déformation GLSL
	_params.addParam("Angle Max Twist", &(_imageBeing.getAngleMax()), "min=1.0 max=360.0 step=1.0", false);


	// Bouton capturant une image de la webCam
	_params.addButton("Webcam Shot", bind(&PaintingBeingsApp::captureCamera, this));
	// Bouton pour faire un screen shot de l'image Being
	_params.addButton("Save image Being", bind(&PaintingBeingsApp::screenShot, this));

	// Bouton pour modifier l'état de l'application
	_params.addButton("Play", bind(&PaintingBeingsApp::setPlay, this));
	_params.addButton("Pause", bind(&PaintingBeingsApp::setPause, this));
	_params.addButton("Stop", bind(&PaintingBeingsApp::setStop, this));

	// Option interface
	_params.addButton("Switch Image/Being", bind(&PaintingBeingsApp::setDisplayBeing, this));
	_params.addButton("Wireframe", bind(&PaintingBeingsApp::setShowWireFrame, this));

	// Reset position camera
	_params.addButton("Reset Camera", bind(&PaintingBeingsApp::resetCamera, this));

	_params.addButton("Quit", bind(&PaintingBeingsApp::closeApp, this));
}

//-------------------------------------------------------------

void PaintingBeingsApp::setup()
{	
	setStop();

	// Setup des paramètres de OpenGL
	glShadeModel(GL_SMOOTH);
	gl::enable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// Setup Capture webcam
	_capture = Capture(640, 480);
	_capture.start();

	// Setup camera
	float fieldOfView = 75.0f;
	float nearCamera = 0.1f;
	float farCamera = 1000000.0f;
	_camera = CameraPersp(getWindowWidth(), getWindowHeight(), fieldOfView, nearCamera, farCamera);
	_camera.lookAt(Vec3f(0.0f, 0.0f, 50.0f), Vec3f::zero());

	// Setup arcball (camera rotation)
	float radiusArcBall = static_cast<float>(getWindowHeight() * 0.5f);
	_arcball = Arcball(getWindowSize());
	_arcball.setRadius(radiusArcBall);


	// Génération de la population d'ImageSphere
	_imageBeing = ImageBeing();

	_rectangleAlgoGen.set(-75.0f, 25.0f, -25.0f, -25.0f);
	_rectangleTextutre.set(25.0f, 25.0f, 75.0f, -25.0f);


	// Setup de l'interface
	_params = params::InterfaceGl("Options", Vec2i(200, 400));
	updateInterface(false);
}

void PaintingBeingsApp::update()
{
	_frameRate = getAverageFps();
	
	if (_play && _launchAlgoGen)
	{
		_algoGen.update();
	}
	
}

void PaintingBeingsApp::draw()
{
	gl::clear(ColorAf::gray(0.6f));

	gl::setViewport(getWindowBounds());
	gl::setMatrices(_camera);

	glMultMatrixf(_arcball.getQuat());

	if (_showWireFrame)
		gl::enableWireframe();
	else
		gl::disableWireframe();

	if (_launchAlgoGen)
	{
		if (!_showImageBeing)
		{
			_image.getTexture().enableAndBind();
			gl::draw(_image.getTexture(), _rectangleTextutre);
			_image.getTexture().unbind();

			gl::Texture textureAlgoGen = _algoGen.getBestImage();
			textureAlgoGen.enableAndBind();
			gl::draw(textureAlgoGen, _rectangleAlgoGen);
			textureAlgoGen.unbind();
		}
		else
		{
			_imageBeing.draw(_algoGen.getBestImage());
		}
	}	

	_params.draw();
}

void PaintingBeingsApp::setShowWireFrame()
{
	_showWireFrame = !_showWireFrame;
}

void PaintingBeingsApp::closeApp()
{
	_capture.stop();
	PaintingBeingsApp::quit();
}

CINDER_APP_NATIVE(PaintingBeingsApp, RendererGl)