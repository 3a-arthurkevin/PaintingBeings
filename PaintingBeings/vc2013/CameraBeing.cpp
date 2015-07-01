#include "CameraBeing.h"


CameraBeing::CameraBeing()
{
}


CameraBeing::~CameraBeing()
{
}


void CameraBeing::setup(float cameraDistance, Vec3f center, Vec3f up, float fieldOfView, float aspectRation, float nearCamera, float farCamera)
{
	this->_cameraDistance = cameraDistance;

	this->_eye = Vec3f(0.0f, 0.0f, this->_cameraDistance);	//Position de la camera
	this->_center = center;									//Point que la camera regardera
	this->_up = up;											//Définition du vecteur up de la camera

	this->_camera.setPerspective(fieldOfView, aspectRation, nearCamera, farCamera);
	this->_camera.lookAt(this->_eye, this->_center, this->_up);
	
	gl::setMatrices(this->_camera);
	gl::rotate(this->_cameraRotation);
}

void CameraBeing::update()
{
	_eye = Vec3f(0.0f, 0.0f, _cameraDistance);
	_camera.lookAt(_eye, _center, _up);
	gl::setMatrices(_camera);
	gl::rotate(this->_cameraRotation);

	//app::console() << "update camera" << std::endl;
}

void CameraBeing::draw()
{

}

CameraPersp CameraBeing::getCamera() const
{
	return this->_camera;
}

void CameraBeing::setCameraDistance(const float cameraDistance)
{
	this->_cameraDistance = cameraDistance;
}

void CameraBeing::setCameraRotation(const Quatf cameraRotation)
{
	this->_cameraRotation = cameraRotation;
}

float& CameraBeing::getCameraDistance()
{
	return this->_cameraDistance;
}

Quatf& CameraBeing::getCameraRotation()
{
	return this->_cameraRotation;
}
