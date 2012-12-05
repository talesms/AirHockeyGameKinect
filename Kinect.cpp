#include "Kinect.h"

Kinect::Kinect(AirHockeyGame* game, Camera* cam)
	:m_game(game), m_cam(cam)
{
}


Kinect::~Kinect(void)
{
}

// Call Start once at application start.
HRESULT Kinect::Start()
{
	m_hNextSkeletonEvent = NULL;

	// Initialize m_pNuiSensor
	HRESULT hr = FindKinectSensor();

	if (SUCCEEDED(hr))
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
		}
	}

	return hr;
}

// Call Update on each iteration of the application's update loop.
void Kinect::Update()
{
	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0) )
	{
		NUI_SKELETON_FRAME skeletonFrame = {0};

		// Get the skeleton frame that is ready
		if (SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
		{
			// Process the skeleton frame
			SkeletonFrameReady(&skeletonFrame);
		}
	}
}

HRESULT	Kinect::FindKinectSensor()
{
	return NuiCreateSensorByIndex(0, &m_pNuiSensor);
}

void Kinect::SkeletonFrameReady(NUI_SKELETON_FRAME* pSkeletonFrame)
{
	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		const NUI_SKELETON_DATA & skeleton = pSkeletonFrame->SkeletonData[i];
		NUI_SKELETON_POSITION_TRACKING_STATE jointRHand;
		NUI_SKELETON_POSITION_TRACKING_STATE jointLHand;
		NUI_SKELETON_POSITION_TRACKING_STATE jointHead;

		switch (skeleton.eTrackingState)
		{
			case NUI_SKELETON_TRACKED:
			
				jointRHand = skeleton.eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_HAND_RIGHT];
				jointLHand = skeleton.eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_HAND_LEFT];
				jointHead = skeleton.eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_HEAD];

				if (jointRHand == NUI_SKELETON_POSITION_NOT_TRACKED)
				{
					m_game->pause();
					return; // nothing to do,  right hand arent tracked
				}
				
				if (jointRHand == NUI_SKELETON_POSITION_TRACKED)
				{
					m_game->unpause();

					const Vector4& jointPosition = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];

					m_game->mCurrPlayerPos.mX = jointPosition.y * 4.0f - 2.0f;
					m_game->mCurrPlayerPos.mY = jointPosition.x * -4.0f + 0.5f;
				}

				if (!m_cam->isLocked() && jointHead == NUI_SKELETON_POSITION_TRACKED)
				{
					const Vector4& jointPosition = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD];
					m_cam->ajustEye(jointPosition.x, jointPosition.y, jointPosition.z);
				}

				if (jointLHand == NUI_SKELETON_POSITION_TRACKED)
				{
					static boolean bWait = false;
					const Vector4& jointPosition = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];

					if (jointPosition.y > 0.0f)
					{
						if (!bWait)
						{
							bWait = true;
							m_cam->lock(!m_cam->isLocked());
						}
					}
					else
						bWait = false;

				}
			
			break;
 
			case NUI_SKELETON_POSITION_ONLY:
				m_game->pause();
			break;
		}
	}
}
