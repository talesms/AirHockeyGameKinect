// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <windows.h>
#include "Vec2.h"
#include "Circle.h"

enum MODEL_TYPE
{
	BOARD,
	PADDLE_RED,
	PADDLE_BLUE,
	PUCK
};

class Model
{
private:
	void drawBoard();
	void drawPuck();
	void drawPaddle(bool blue);

public:
	Model(MODEL_TYPE type, Vec2 position, Vec2 velocity, Circle boundingCircle);
	~Model();
	void draw();
	void update(float dt);

	MODEL_TYPE mType;
	Vec2 mPosition;
	Vec2 mVelocity;
	Circle mBoundingCircle;
};

#endif // MODEL_H