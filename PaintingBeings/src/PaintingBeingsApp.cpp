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
	// Screen Shot
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
	// Interface
#include "cinder/params/Params.h"
	// Affichage des textures sur des Rectangles
#include "cinder/Rect.h"
	// Pour threader l'algo gen
#include "cinder/Thread.h"
#include "cinder/ConcurrentCircularBuffer.h"

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
		
		// Fonction de base de Cinder pour le bon déroulement de l'App 
			// Ordre d'appel [en boucle] par l'App : --> Setup --> Update --> Draw --> Setup --> Update --> ...)
		void setup();	
		void update();
		void draw();


		//----------------------------------------
		// Variables de l'Application
		//----------------------------------------

		//Thread
		//bool _stopThread;
		//shared_ptr<thread> _threadAlgoGen;
		//ConcurrentCircularBuffer<Surface> *_bestImage;

		//Algo
		AlgoGen _algoGen;

		//Camera
		ci::Arcball	_arcball;
		ci::CameraPersp	_camera;
		
		//Image
		Image _image;
		ImageBeing _imageBeing;
		
		//Interface
		params::InterfaceGl _params;

		//params::InterfaceGl _paramsAlgoGen;

		float _frameRate = 0.0;
		bool _launchAlgoGen = false;
		bool _play = false;
		bool _pause = false;
		bool _stop = true;
		
		//Rectangle pour afficher les images
		Rectf rectangleTextutre;
		Rectf rectangleAlgoGen;

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
	}
}

void PaintingBeingsApp::screenShot()
{
	if (_launchAlgoGen)
	{
		time_t  nameFile;
		time(&nameFile);
		writeImage(getAppPath() / ("pb_" + toString(nameFile) + ".png"), _algoGen.getImageToDisplay().clone());
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
		eye.z -= event.getWheelIncrement() * 10.0f;

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

	/*
	if (event.getCode() == KeyEvent::KEY_RIGHT) 

	if (event.getCode() == KeyEvent::KEY_LEFT) 

	if (event.getCode() == KeyEvent::KEY_UP)

	if (event.getCode() == KeyEvent::KEY_DOWN)
	*/
}

void PaintingBeingsApp::fileDrop(FileDropEvent event)
{
	_image.loadWithImagePath(event.getFile(0));

	_algoGen.setup(_image);
	
	float eyeZ = static_cast<float>(_image.getMiniatureSize()) * 2.5f;
	float radiusArcBall = static_cast<float>(getWindowHeight() * 0.5f);
	
	_camera.lookAt(Vec3f(0.0f, 0.0f, eyeZ), Vec3f::zero());
	_arcball = Arcball(getWindowSize());
	_arcball.setRadius(radiusArcBall);

	_launchAlgoGen = true;
	setPlay();

	_imageBeing.setup(_image, 5);
}

void PaintingBeingsApp::prepareSettings(Settings* settings)
{
	//Setting de la taille de la fenetre
	settings->setWindowSize(640, 480);

	//Setting du frame rate attendu
	settings->setFrameRate(60.0f);
}

//-------------------------------------------------------------

void PaintingBeingsApp::setup()
{	
	// Setup des paramètres de OpenGL
	glShadeModel(GL_SMOOTH);
	gl::enable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// Setup camera
	float fieldOfView = 75.0f;
	float nearCamera = 0.01f;
	float farCamera = 1000000.0f;
	_camera = CameraPersp(getWindowWidth(), getWindowHeight(), fieldOfView, nearCamera, farCamera);
	_camera.lookAt(Vec3f(0.0f, 0.0f, 50.0f), Vec3f::zero());

	// Setup arcball (camera rotation)
	float radiusArcBall = static_cast<float>(getWindowHeight() * 0.5f);
	_arcball = Arcball(getWindowSize());
	_arcball.setRadius(radiusArcBall);


	// Génération de la population d'ImageSphere
	_imageBeing = ImageBeing();

	rectangleAlgoGen.set(-75.0f, 25.0f, -25.0f, -25.0f);
	rectangleTextutre.set(25.0f, 25.0f, 75.0f, -25.0f);


	// Setup de l'interface
	
	bool readOnly = true;

	_params = params::InterfaceGl("Options", Vec2i(200, 300));

	// FrameRate
	_params.addParam("Frame rate", &_frameRate, "", readOnly);
	
	// Taille de l'image miniature à recréer
	_params.addParam("Image size", &(_image.getMiniatureSize()), "min=10 max=200 step=10", !readOnly);
	
	// Taille de la population de l'algo gen
	_params.addParam("Population size", &(_algoGen.getPopulationSize()), "min=5 max=100 step=5", !readOnly);

	// Pourcentage pour l'algo gen
	_params.addParam("% Survivor", &(_algoGen.getThresholdSurvivor()), "min=0.05 max=0.50 step=0.05", !readOnly);
	_params.addParam("% Crossover", &(_algoGen.getThresholdCrossover()), "min=0.05 max=0.50 step=0.05", !readOnly);
	_params.addParam("% Mutation", &(_algoGen.getThresholdMutation()), "min=0.05 max=0.50 step=0.05", !readOnly);
	_params.addParam("% Leftover", &(_algoGen.getThresholdNewPopulation()), "min=0.05 max=0.50 step=0.05", !readOnly);

	// Bouton pour faire un screen shot
	_params.addButton("Screen shot", bind(&PaintingBeingsApp::screenShot, this), "key=space");

	// Bouton pour modifier l'état de l'application
	_params.addButton("Play", bind(&PaintingBeingsApp::setPlay, this), "key=a");
	_params.addButton("Pause", bind(&PaintingBeingsApp::setPause, this), "key=z");
	_params.addButton("Stop", bind(&PaintingBeingsApp::setStop, this), "key=e");


	_params.addButton("Quit", bind(&PaintingBeingsApp::quit, this), "key=q");
	
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
	gl::enableWireframe();

	gl::clear(ColorAf::gray(0.6f));

	gl::setViewport(getWindowBounds());
	gl::setMatrices(_camera);

	glMultMatrixf(_arcball.getQuat());

	if (_launchAlgoGen)
	{
		_image.getTexture().enableAndBind();
		gl::draw(_image.getTexture(), rectangleTextutre);
		_image.getTexture().unbind();

		gl::Texture textureAlgoGen = _algoGen.getImagePopulation()[0].getSurface();
		textureAlgoGen.enableAndBind();
		gl::draw(textureAlgoGen, rectangleAlgoGen);
		textureAlgoGen.unbind();

		_imageBeing.draw(_algoGen.getImageToDisplay());
	}	

	_params.draw();
}

CINDER_APP_NATIVE(PaintingBeingsApp, RendererGl)
