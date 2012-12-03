#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

class Camera
{
private:
	bool mLock;
	float mEye[3];
	float mUp[3];
	float mCenter[3];
	float mPerspective[6];

public:
	Camera(float eye[3], float center[3], float up[3],
		float perspective[6]);
	~Camera(void);
	void reload();
	void lock(bool k);
	bool isLocked();
	void ajustEye(float x, float y, float z);
	void ajustPerspective(int width, int height, 
		float bottom, float top, float zNear, float zFar);
};

#endif // CAMERA_H

