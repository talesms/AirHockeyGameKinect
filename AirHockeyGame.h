#ifndef AIR_HOCKEY_GAME_H
#define AIR_HOCKEY_GAME_H

#include <windows.h>
#include "Rect.h"
#include "Circle.h"
#include "Model.h"

class AirHockeyGame
{
public:
	AirHockeyGame(Vec2 wndCenterPt);
	~AirHockeyGame();
	void pause();
	void unpause();
	void update(float dt);
	void draw();

	Vec2 mCurrPlayerPos;

private:
	void updateBluePaddle(float dt);
	void updateRedPaddle(float dt);
	void updatePuck(float dt);
	bool paddlePuckCollision(Model* paddle);
	void increaseScore(bool blue);

private:
	Vec2 mWndCenterPt;
	int mBlueScore;
	int mRedScore;
	bool mPaused;
	const float MAX_PUCK_SPEED;
	const float RED_SPEED;
	float mRedRecoverTime;
	Model* mGameBoard;
	Model* mBluePaddle;
	Model* mRedPaddle;
	Model* mPuck;
	Rect mBlueBounds;
	Rect mRedBounds;
	Rect mBoardBounds;
	Rect mBlueGoal;
	Rect mRedGoal;
	Vec2 mLastPlayerPos;
};

#endif // AIR_HOCKEY_GAME_H