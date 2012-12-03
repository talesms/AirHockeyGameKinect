#include "AirHockeyGame.h"
#include <GL/glut.h>
#include "Kinect.h"
#include "Camera.h"

static AirHockeyGame* game;
static Kinect* kinect;
static Camera* cam;

/* GLUT callback Handlers */

static void 
resize(int width, int height)
{
	cam->ajustPerspective(width, height, -1.0f, 1.0f, 2.0f, 100.0f);
}

static void 
display(void)
{
    static double t;
	static double timePrec;
	t = (glutGet(GLUT_ELAPSED_TIME));

	//if (t>0.01)
	//{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		kinect->Update();
		game->update(t-timePrec);
		game->draw();
		glutSwapBuffers();
	//}
    timePrec = t;
}


static void 
key(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 27 : 
        case 'q':
            exit(0);
            break;
		case 'p':
			game->pause();
			break;
		case ' ':
			game->unpause();
			break;

		case 'w':
			game->mCurrPlayerPos.mX += 0.5f;
			break;
		case 's':
			game->mCurrPlayerPos.mX -= 0.5f;
			break;
		case 'a':
			game->mCurrPlayerPos.mY += 0.5f;
			break;
		case 'd':
			game->mCurrPlayerPos.mY -= 0.5f;
			break;

		case 'z':
			cam->lock(true); break;
		case 'x':
			cam->lock(false); break;
    }

    glutPostRedisplay();
}

static void 
idle(void)
{
    glutPostRedisplay();
}


/* Program entry point */
int 
main(int argc, char *argv[])
{
	const int width		= 800;
	const int height	= 600;

	const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

	const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[] = { 100.0f };

	const Vec2 center(0.0f, 0.0f);
	float camEye[]	=	{-9.0f , 0.0f, -3.0f};
	float camCenter[] =	{0.0f, 0.0f, -6.0f};
	float camUp[]		=	{0.0f,0.0f,1.0f};
	float camPerspective[] = {width, height, -1.0f, 1.0f, 2.0f, 100.0f};

	game	= new AirHockeyGame(center);
	cam		= new Camera(camEye, camCenter, camUp, camPerspective);
	kinect	= new Kinect(game, cam);


    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Air Hockey Game");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	kinect->Start();

    glutMainLoop();

    return 0;
}

