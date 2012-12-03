// Kinect.h
#ifndef KINECT_H
#define KINECT_H

#include <windows.h>
#include <NuiApi.h>
#include "AirHockeyGame.h"
#include "Camera.h"

class Kinect
{
private:
    INuiSensor*            m_pNuiSensor;
    BSTR                    m_instanceId;
	HANDLE					m_hNextSkeletonEvent;
	HRESULT					FindKinectSensor();
	void					SkeletonFrameReady(NUI_SKELETON_FRAME * skeletonFrame);
	AirHockeyGame*			m_game;
	Camera*					m_cam;


public:
							Kinect(AirHockeyGame* game, Camera* cam);
							~Kinect(void);
	HRESULT					Start();
	void					Update();
};

#endif // KINECT_H