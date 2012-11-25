#include "AirHockeyGame.h"
#include <GL/glut.h>
#include "Kinect.h"

static AirHockeyGame* game;
static Kinect* kinect;

/* GLUT callback Handlers */

static void 
resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	gluLookAt(-9.0f , 0.0f, -3.0f, 0.0f, 0.0f, -6.0f, 0.0f,0.0f,1.0f);
	//gluLookAt(-6.0f, 0.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity() ;

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
		case 'o':
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
    }

    glutPostRedisplay();
}

static void 
idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int 
main(int argc, char *argv[])
{
	Vec2 center;
	center.mX = 0.0f;
	center.mY = 0.0f;
	game = new AirHockeyGame(center);
	kinect = new Kinect(game);


    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
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

