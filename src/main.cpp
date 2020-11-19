#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>

// glut
#include <GL/glut.h>

#include "FrameAnimator.cpp";

#include "PhysicsSystem.cpp";

#include "RigidBody.cpp";

#include "SphereCollider.cpp";

#include "BoidSystem.cpp";

#include "Boid.cpp";

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;


//animation vars

FrameAnimator* animator;
float currentTime = 0;
float timeStep = 0.02;

static float PI = 3.1415926535897932384626432795;

PhysicsSystem * physics;


static float CATMULL_ROM_SPLINE [4][4] = {
	-0.5f,  1.0f, -0.5f,  0.0f,
	 1.5f, -2.5f,  0.0f,  1.0f,
	-1.5f,  2.0f,  0.5f,  0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f
};

static GLfloat B_SPLINE [4][4] = {
	-1.0f / 6.0f,  3.0f / 6.0f, -3.0f / 6.0f,  1.0f / 6.0f,
	 3.0f / 6.0f, -6.0f / 6.0f,  0.0f / 6.0f,  4.0f / 6.0f,
	-3.0f / 6.0f,  3.0f / 6.0f,  3.0f / 6.0f,  1.0f / 6.0f,
	 1.0f / 6.0f, 0.0f / 6.0f, 0.0f / 6.0f, 0.0f / 6.0f
};

void initializeBoids();
void initializeRigidBodies();
//================================
// init
//================================
void init( void ) {
	//This is where each frame is added, and this can be done with either quaternions or euler angles.  
	animator = new FrameAnimator(CATMULL_ROM_SPLINE, true);

	/*animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(2.5, 0, -5, 0, 0, PI));
	animator->addKeyFrame(new KeyFrame(-2.5, 5, -25, 0, PI, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -30, .5*PI, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));*/

	/*
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, PI / 4, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, PI / 4, 0));
	animator->addKeyFrame(new KeyFrame(-12.5, 0, -45, 0, -PI, 0));
	animator->addKeyFrame(new KeyFrame(7.5, 0, -45, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0));*/

	//animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 45, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 45, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-12.5, 0, -45, 45, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-12.5, 0, -45, -90, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(7.5, 0, -45, -90, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(7.5, 0, -45, -210, 0, 0, 0));
	animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, -210, 0, 0, 0));
	//animator->addKeyFrame(new KeyFrame(-2.5, 0, -25, 0, 0, 0, 0));

	initializeRigidBodies();
	initializeBoids();
}

//This method is responsible for actually taking the output of our interpolations and applying it to the object.
void animateArticulatedFigure()
{
	glLoadIdentity();

	KeyFrame* frame = animator->getInbetweenFrame(currentTime);
	glTranslatef(frame->posX, frame->posY, frame->posZ);

	//glRotatef(frame->getEulerX() * 180, 1, 0, 0);
	glRotatef(frame->quatW, 0, 1, 0);
	//glRotatef(frame->getEulerZ() * 180, 0, 0, 1);

	//float * m = frame->getMatrix();
	//glMultMatrixf(m);
		//float m[16] = { 2,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	//glMultMatrixf(m);

	// render objects

	// right leg
	glPushMatrix();
	glTranslatef(2, 0, 0);
	glRotatef(40 * sin(currentTime * 5), 1, 0, 0);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	// left leg
	glPushMatrix();
	glRotatef(40 * -sin(currentTime * 5), 1, 0, 0);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	// torso
	glPushMatrix();
	glTranslatef(1, 2.5, 0);
	glScalef(1, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1, 3, 0);
	glScalef(2, 2, 2);
	glutSolidCube(1.0);
	glPopMatrix();

	// head
	glPushMatrix();
	glTranslatef(1, 5.25, 0);
	glScalef(1.5, 1.5, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();

	// arms
	glPushMatrix();
	glTranslatef(1, 3, 0);
	glRotatef(40 * -sin(currentTime * 5), 0, 1, 0);
	glScalef(6, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	currentTime += timeStep;
}

Rigidbody* ball0_body, * ball1_body, * ball2_body, * ball3_body;
//std::shared_ptr<Rigidbody> ball0_body, ball1_body, ball2_body;
SphereCollider* sphere0, * sphere1, * sphere2, * sphere3;
void initializeRigidBodies()
{
	std::cout << "EG 111" << std::endl;
	/*ball0_body = new Rigidbody(new Vector3D(1, 5, -20), new Vector3D(0, 0, 0));
	ball2_body = new Rigidbody(new Vector3D(3, 5, -20), new Vector3D(-.01, 0, 0));
	ball1_body = new Rigidbody(new Vector3D(1, 7, -25), new Vector3D(0, 0, 0));
	ball3_body = new Rigidbody(new Vector3D(1.1, 7, -25.1), new Vector3D(0, 1, 0));*/

	ball0_body = new Rigidbody(new Vector3D(-2.5, 2.5, -20), new Vector3D(.02, 0, .02));
	ball2_body = new Rigidbody(new Vector3D(2.5, 6, -20), new Vector3D(-.02, 0, .02));
	ball1_body = new Rigidbody(new Vector3D(-2.5, 7, -15), new Vector3D(.02, 0, -.02));
	ball3_body = new Rigidbody(new Vector3D(2.5, 6.5, -15), new Vector3D(-.02, .1, -.02));

	sphere0 = new SphereCollider(.5);
	sphere1 = new SphereCollider(.5);
	sphere2 = new SphereCollider(.5);
	sphere3 = new SphereCollider(.5);

	physics = new PhysicsSystem();
	physics->addRigidSphere(ball0_body, sphere0);
	physics->addRigidSphere(ball1_body, sphere1);
	physics->addRigidSphere(ball2_body, sphere2);
	physics->addRigidSphere(ball3_body, sphere3);
}

void renderBallWithGravity(Rigidbody* rb, SphereCollider* sc)
{
	glPushMatrix();
	glTranslatef(rb->position->x, rb->position->y, rb->position->z);
	glutSolidSphere(sc->radius, 15, 15);
	rb->applyForce(0, -.01, 0);
	glPopMatrix();
}

void animateRigidBodies()
{
	glLoadIdentity();

	renderBallWithGravity(ball0_body, sphere0);
	renderBallWithGravity(ball1_body, sphere1);
	renderBallWithGravity(ball2_body, sphere2);
	renderBallWithGravity(ball3_body, sphere3);

	physics->tick();
}

BoidSystem* boidSys;
void initializeBoids()
{
	boidSys = new BoidSystem(*new Vector3D(0, 0, -20), 8, 1.0);

	//lol at how blatently i'm ignoring memory leaks
	for (int x = -3; x <= 3; x++ )
	{
		for (int y = -3; y <= 3; y++ )
		{
			for (int z = -3; z <= 3; z++)
			{
				Vector3D* dir = new Vector3D((rand() % 10) - 4, (rand() % 10) - 4, (rand() % 10) - 4);
				*dir = dir->normalize();
				boidSys->addBoid(*new Vector3D(x, y, z - 20), dir->scalarProd(0.1));
			}
		}
	}

	/*boidSys->addBoid(*new Vector3D(-2.5, -2.5, -20), *new Vector3D(0, 0, .1));
	boidSys->addBoid(*new Vector3D(2.5, -6, -20), *new Vector3D(0, 0, .1));
	boidSys->addBoid(*new Vector3D(-2.5, -7, -15), *new Vector3D(0, .1, 0));
	boidSys->addBoid(*new Vector3D(2.5, -6.5, -15), *new Vector3D(.1, 0, 0));

	boidSys->addBoid(*new Vector3D(-2.5, -3.5, -20), *new Vector3D(0, 0, -.1));
	boidSys->addBoid(*new Vector3D(2.5, -7, -21), *new Vector3D(0, 0, -.1));
	boidSys->addBoid(*new Vector3D(-3.5, -6, -15), *new Vector3D(0, -.1, 0));
	boidSys->addBoid(*new Vector3D(2.5, -7.5, -13), *new Vector3D(-.1, 0, 0));

	boidSys->addBoid(*new Vector3D(-2.5, -2.5, -20), *new Vector3D(0, 0, .1));
	boidSys->addBoid(*new Vector3D(2.5, -6, -20), *new Vector3D(0, 0, .1));
	boidSys->addBoid(*new Vector3D(-2.5, -7, -15), *new Vector3D(0, .1, 0));
	boidSys->addBoid(*new Vector3D(2.5, -6.5, -15), *new Vector3D(.1, 0, 0));

	boidSys->addBoid(*new Vector3D(-2.5, -3.5, -20), *new Vector3D(0, 0, -.1));
	boidSys->addBoid(*new Vector3D(2.5, -7, -21), *new Vector3D(0, 0, -.1));
	boidSys->addBoid(*new Vector3D(-3.5, -6, -15), *new Vector3D(0, -.1, 0));
	boidSys->addBoid(*new Vector3D(2.5, -7.5, -13), *new Vector3D(-.1, 0, 0));*/
}

void renderBoid(Boid* boid);
void animateBoids()
{
	for (Boid& boid : boidSys->boids)
	{
		renderBoid(& boid);
	}

	boidSys->tick();
}

void renderBoid(Boid * boid)
{
	glPushMatrix();
	glTranslatef(boid->position.x, boid->position.y, boid->position.z);
	glutSolidSphere(.1, 8, 8);
	glPopMatrix();
}

//================================
// update
//================================
void update( void ) {
	// do something before rendering...

	// rotation angle
	g_angle = ( g_angle + 5 ) % 360;
}

//================================
// render
//================================
void render( void ) {

	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se		= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	//glRotated(g_angle, 0.0, 1.0, 0.0);
	//animateRigidBodies();
	//animateArticulatedFigure();
	animateBoids();


	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;

	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}

//================================
// main
//================================
int main( int argc, char** argv ) {
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( 600, 600 ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );

	// init
	init();
	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 16, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}