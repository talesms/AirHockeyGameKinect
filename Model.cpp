#include "Model.h"
#include <GL/glut.h>

Model::Model(MODEL_TYPE type, Vec2 position, Vec2 velocity, Circle boundingCircle)
	:mType(type), mPosition(position), mVelocity(velocity), mBoundingCircle(boundingCircle)
{

}


Model::~Model()
{

}

void Model::draw()
{
	switch(mType)
	{
		case BOARD:
			    drawBoard();
			break;

		case PADDLE_RED:
			    drawPaddle(false);
			break;

		case PADDLE_BLUE:
				drawPaddle(true);
			break;

		case PUCK:
			    drawPuck();
			break;
	}

}

void Model::update(float dt)
{
	// Update the sprites position.
	mPosition += (mVelocity * dt);
	// Update bounding circle, too. That is, the bounding
	// circle moves with the sprite.
	mBoundingCircle.c = mPosition;
}

void Model::drawBoard()
{
	glColor3f(0.0f,0.2f,0.0f);
	glPushMatrix();
		glTranslatef(mPosition.mX, mPosition.mY, -6.0f);
		glPushMatrix();
			glScalef(10.0f, 5.0f, 0.1);
			glutSolidCube(1);
		glPopMatrix();

		glColor3f(0.0f,0.0f,1.0f);
		glPushMatrix();
			glScalef(0.01f, 5.0f, 0.15);
			glutSolidCube(1);
		glPopMatrix();

		glColor3f(2.0f,0.0f,0.0f);
		glPushMatrix();
			glTranslatef(5.0f, 0.0f, 0.0f);
			glScalef(0.1f, 1.5f, 0.15);
			glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-5.0f, 0.0f, 0.0f);
			glScalef(0.1f, 1.5f, 0.15);
			glutSolidCube(1);
		glPopMatrix();

	glPopMatrix();
}

void Model::drawPaddle(bool blue)
{
	GLUquadric *myQuad;

	if (blue)
		glColor3f(0.0f,0.0f,1.0f);
	else
		glColor3f(1.0f,0.0f,0.0f);
	
	myQuad = gluNewQuadric();

	glPushMatrix();
		glTranslatef(mPosition.mX, mPosition.mY, -5.95f);
		gluQuadricOrientation(myQuad, GLU_OUTSIDE);
		gluCylinder(myQuad, mBoundingCircle.r, mBoundingCircle.r, 0.12f, 15, 15);
		glTranslatef(0.0f, 0.0f, 0.12f);
		gluDisk(myQuad, mBoundingCircle.r*0.75,  mBoundingCircle.r, 15, 15);
		glTranslatef(0.0f, 0.0f, -0.10f);
		gluQuadricOrientation(myQuad, GLU_INSIDE);
		gluCylinder(myQuad, mBoundingCircle.r*0.75, mBoundingCircle.r*0.75, 0.15f, 15, 15);
		gluQuadricOrientation(myQuad, GLU_OUTSIDE);
		gluDisk(myQuad, mBoundingCircle.r*0.45,  mBoundingCircle.r*0.75, 15, 15);
		gluCylinder(myQuad, mBoundingCircle.r*0.45, mBoundingCircle.r*0.45, 0.20f, 15, 15);
		glTranslatef(0.0f, 0.0f, 0.20f);
		gluDisk(myQuad, 0.0f,  mBoundingCircle.r*0.45, 15, 15);

	glPopMatrix();
}

void Model::drawPuck()
{
	glColor3f(1.0f,1.0f,1.0f);

	glPushMatrix();
		glTranslatef(mPosition.mX, mPosition.mY, -5.95f);
		gluCylinder(gluNewQuadric(), mBoundingCircle.r, mBoundingCircle.r, 0.05f, 15, 15);
		glTranslatef(0.0f, 0.0f, 0.05f);
		gluDisk(gluNewQuadric(), 0.0f,  mBoundingCircle.r, 15, 15);
	glPopMatrix();
}