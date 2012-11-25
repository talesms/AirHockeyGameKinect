#include "AirHockeyGame.h"
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>

AirHockeyGame::AirHockeyGame(Vec2 wndCenterPt)
: MAX_PUCK_SPEED(0.006f), RED_SPEED(0.004f), mBlueScore(0), mRedScore(0)
{
	// Save input parameter.
	mWndCenterPt = wndCenterPt;

	// The game is initially paused.
	mPaused = true;

	// No recovery time for red to start.
	mRedRecoverTime = 0.0f;

	// Create the models:
	Circle bc;
	Vec2 p0(0.0f, 0.0f);
	Vec2 v0(0.0f, 0.0f);
	mGameBoard = new Model(BOARD, p0, v0, bc);
	p0.mX = -1.5f;
	v0.mX = -0.0001f;
	bc.c = p0;
	bc.r = 0.3f; // Puck radius = 0.3
	mPuck = new Model(PUCK, p0, v0, bc);
	p0.mX = 2.5f;
	p0.mY = 0.0f;
	v0.mX = 0.0f;
	bc.c = p0;
	bc.r = 0.35f; // Paddle radius = 0.35
	mRedPaddle = new Model(PADDLE_RED, p0, v0, bc);
	p0.mX = -2.5f;
	p0.mY = 0.0f;
	bc.c = p0;
	bc.r = 0.35f; // Paddle radius = 0.35
	mBluePaddle = new Model(PADDLE_BLUE, p0, v0, bc);

	// Initialize the rectangles.
	mBlueBounds = Rect(-5.0f, -2.5f, 0.0f, 2.5f);
	mRedBounds = Rect(0.0f, -2.5f, 5.0f, 2.5f);
	mBoardBounds = Rect(-5.0f, -2.5f, 5.0f, 2.5f);
	mBlueGoal = Rect(-5.0f, -0.5f, -4.6f, 0.5f);
	mRedGoal = Rect(4.6f, -0.5f, 5.0f, 0.5f);
}

AirHockeyGame::~AirHockeyGame()
{
	delete mGameBoard;
	delete mBluePaddle;
	delete mRedPaddle;
	delete mPuck;
}

void AirHockeyGame::pause()
{
	mPaused = true;
}

void AirHockeyGame::unpause()
{
	mPaused = false;
}

void AirHockeyGame::update(float dt)
{
	// Only update the game if the game is not paused.
	if( !mPaused )
	{
		updateBluePaddle(dt);
		updateRedPaddle(dt);
		updatePuck(dt);

		// Decrease recovery time as time passes.
		if( mRedRecoverTime > 0.0f )
			mRedRecoverTime -= dt/1000.0f;
	}
}

void AirHockeyGame::draw()
{
	// Draw the sprites.
	mGameBoard->draw();
	mBluePaddle->draw();
	mRedPaddle->draw();
	mPuck->draw();

	// Draw the player scores.
	//TODO: colocar aqui para desenhar as pontuações
}

void AirHockeyGame::updateBluePaddle(float dt)
{

	// Change in position.
	Vec2 dp;
	dp = mCurrPlayerPos - mLastPlayerPos;

	// Velocity is change in position with respect to time.
	mBluePaddle->mVelocity = dp / dt;

	// Update the blue paddle's position.
	mBluePaddle->update(dt);

	// Make sure the blue paddle stays inbounds.
	mBlueBounds.forceInside(mBluePaddle->mBoundingCircle);
	mBluePaddle->mPosition = mBluePaddle->mBoundingCircle.c;

	// The current position is now the last position.
	mLastPlayerPos = mCurrPlayerPos;
}

void AirHockeyGame::updateRedPaddle(float dt)
{
	// The red paddle's AI is overly simplistic: When the
	// puck moves into red's boundary, the red paddle
	// simply moves directly towards the puck to hit it.
	// When the puck leaves red's boundaries, the red
	// paddle returns to the center of its boundary.
	if( mRedRecoverTime <= 0.0f )
	{

		// Is the puck in red's boundary? If yes, then
		// move the red paddle directly toward the puck.
		if( mRedBounds.isPtInside(mPuck->mPosition) )
		{
			// Vector directed from paddle to puck.
			Vec2 redVel = mPuck->mPosition - mRedPaddle->mPosition;
			redVel = redVel.normalize();
			redVel *= RED_SPEED;
			mRedPaddle->mVelocity = redVel;
		}

		// If no, then move the red paddle to the point (4.5, 0).
		else
		{
			Vec2 redVel = Vec2(4.55f, 0.0f) - mRedPaddle->mPosition;
			if(redVel.length() > 0.6f)
			{
				redVel = redVel.normalize();
				redVel *= RED_SPEED;
				mRedPaddle->mVelocity = redVel;
			}

			// Within 5 units--close enough.
			else
				mRedPaddle->mVelocity = Vec2(0.0f, 0.0f);
		}

		// Update the red paddle's position.
		mRedPaddle->update(dt);

		// Make sure the red paddle stays inbounds.
		mRedBounds.forceInside(mRedPaddle->mBoundingCircle);
		mRedPaddle->mPosition = mRedPaddle->mBoundingCircle.c;
	}
}

void AirHockeyGame::updatePuck(float dt)
{
	paddlePuckCollision(mBluePaddle);

	// If red hits the puck then make a small 1/10th of a second
	// delay before the red paddle can move away as sort of a
	// "recovery period" after the hit. This is to model the
	// fact that when a human player hits something, it takes
	// a short period of time to recover from the collision.
	if(paddlePuckCollision(mRedPaddle))
		mRedRecoverTime = 1.0f;

	// Clamp puck speed to some maximum velocity; this provides
	// good stability.
	if( mPuck->mVelocity.length() >= MAX_PUCK_SPEED )
	{
		mPuck->mVelocity = mPuck->mVelocity.normalize();
		mPuck->mVelocity *= MAX_PUCK_SPEED;
	}

	// Did the puck hit a wall? If so, reflect about edge.
	Circle puckCircle = mPuck->mBoundingCircle;
	if( puckCircle.c.mX - puckCircle.r < mBoardBounds.minPt.mX )
		mPuck->mVelocity.mX *= -1.0f;
	if( puckCircle.c.mX + puckCircle.r > mBoardBounds.maxPt.mX )
		mPuck->mVelocity.mX *= -1.0f;
	if( puckCircle.c.mY - puckCircle.r < mBoardBounds.minPt.mY )
		mPuck->mVelocity.mY *= -1.0f;
	if( puckCircle.c.mY + puckCircle.r > mBoardBounds.maxPt.mY )
		mPuck->mVelocity.mY *= -1.0f;

	// Make sure puck stays inbounds of the gameboard.
	mBoardBounds.forceInside(mPuck->mBoundingCircle);
	mPuck->mPosition = mPuck->mBoundingCircle.c;
	mPuck->update(dt);
	if( mBlueGoal.isPtInside(mPuck->mPosition) )
	{
		pause();
		increaseScore(false);
		printf("\nScore\tAzul:%d\t\tVermelho:%d", mBlueScore, mRedScore);
	}
	if( mRedGoal.isPtInside(mPuck->mPosition) )
	{
		pause();
		increaseScore(true);
		printf("\nScore\tAzul:%d\t\tVermelho:%d", mBlueScore, mRedScore);
	}
}

bool AirHockeyGame::paddlePuckCollision(Model* paddle)
{
	Vec2 normal;
	if(paddle->mBoundingCircle.hits(mPuck->mBoundingCircle, normal))
	{

		// Hit updates cirle's position. So update pucks
		// position as well since the two correspond.
		mPuck->mPosition = mPuck->mBoundingCircle.c;

		//*******************
		// Apply equation (7)
		//*******************
		// Compute the paddle's velocity relative to the puck's
		// velocity.
		Vec2 relVel = paddle->mVelocity - mPuck->mVelocity;

		// Get the component of the relative velocity along
		// the normal.
		float impulseMag = relVel * normal; // dot product

		// Are the objects getting closer together?
		if( impulseMag >= 0.0f )
		{
			// Project the relative velocity onto the normal.
			Vec2 impulse = normal * impulseMag;

			// Add the impulse to the puck.
			mPuck->mVelocity += impulse * 2.0f;
			return true;
		}
	}
	return false;
}

void AirHockeyGame::increaseScore(bool blue)
{
	if( blue )
		++mBlueScore;
	else
		++mRedScore;

	// A point was just scored, so reset puck to center and
	// pause game.
	mPuck->mPosition = Vec2(0.0f, 0.0f);
	mPuck->mVelocity = Vec2(0.0f, 0.0f);
	mPuck->mBoundingCircle.c = Vec2(0.0f, 0.0f);

	// After score, pause the game so player can prepare for
	// next round.

	pause();
}