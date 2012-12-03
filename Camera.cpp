#include "Camera.h"


Camera::Camera(float eye[3], float center[3], float up[3], float perspective[6])
{
	mEye[0] = eye[0];
	mEye[1] = eye[1];
	mEye[2] = eye[2];

	mCenter[0] = center[0];
	mCenter[1] = center[1];
	mCenter[2] = center[2];

	mUp[0] = up[0];
	mUp[1] = up[1];
	mUp[2] = up[2];

	mPerspective[0] = perspective[0];
	mPerspective[1] = perspective[1];
	mPerspective[2] = perspective[2];
	mPerspective[3] = perspective[3];
	mPerspective[4] = perspective[4];
	mPerspective[5] = perspective[5];

	mLock = true;
}


Camera::~Camera(void)
{
}

void Camera::reload()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(mPerspective[0], mPerspective[1], mPerspective[2], 
		mPerspective[3], mPerspective[4], mPerspective[5]);
	gluLookAt(mEye[0] , mEye[1], mEye[2], mCenter[0], mCenter[1], 
		mCenter[2], mUp[0], mUp[1], mUp[2]);
	glMatrixMode(GL_MODELVIEW);
}

void Camera::lock(bool k)
{
	if(!mLock && k)
	{
		mEye[0] = -9.0f;
		mEye[1] = 0.0f;
		mEye[2] = -3.0f;

		reload();
	}

	mLock = k;
}

bool Camera::isLocked()
{
	return mLock;
}

void Camera::ajustEye(float x, float y, float z)
{
	mEye[0] = -9.0f + y*5;
	mEye[1] = 1.0f + x*6;
	mEye[2] = -3.0f + z*5;

	if(!mLock)
		reload();
}

void Camera::ajustPerspective(int width, int height, 
		float bottom, float top, float zNear, float zFar)
{
	const float ar = (float) width / (float) height;

	mPerspective[0] = -ar;
	mPerspective[1] = ar;
	mPerspective[2] = bottom;
	mPerspective[3] = top;
	mPerspective[4] = zNear;
	mPerspective[5] = zFar;
    
    glViewport(0, 0, width, height);
	reload();

    glLoadIdentity();

}