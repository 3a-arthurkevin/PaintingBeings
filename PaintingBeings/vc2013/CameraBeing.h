#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"

using namespace ci;

class CameraBeing
{
	private:
		CameraPersp _camera;
		Vec3f _eye;
		Vec3f _center;
		Vec3f _up;

		//Pour interface
		float _cameraDistance;
		Quatf _cameraRotation;

	public:
		CameraBeing();
		~CameraBeing();

		void setup(float cameraDistance, Vec3f center, Vec3f up, float fieldOfView, float aspectRation, float nearCamera, float farCamera);
		void update();
		void draw();

		CameraPersp getCamera() const;

		void setCameraDistance(const float cameraDistance);
		void setCameraRotation(const Quatf cameraRotation);
		float& getCameraDistance();
		Quatf& getCameraRotation();
};

