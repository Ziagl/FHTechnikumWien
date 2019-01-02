#include <vector>
#include "globals.h"
#include "mesh.h"
#include "kdTree.h"
#include "ray.h"

using namespace std;

//vector<Mesh*> vMeshes;
vector<TRIANGLE*> vec;
KDTree<TRIANGLE> tree;
Ray r;
Vector camerapos(0.0f, -2.0f, -20.0f);

bool drawkdTree = true;

float _angle = 0;            //The rotation of the box

void update(int value);
void updateRay(float rotY, float transX, float transY, float transZ);
void createPyramid(Point pTop);

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:	//Escape key
		exit(0);
	case 'w': glPolygonMode(GL_FRONT, GL_LINE); glPolygonMode(GL_BACK, GL_LINE); break;
	case 'f': glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_FILL); break;
	case 't': drawkdTree = !drawkdTree; break;
	case 'a': update(+1); break;
	case 's': update(-1); break;
	case 'y': updateRay(+1.0f, 0.0f, 0.0f, 0.0f); break;
	case 'x': updateRay(-1.0f, 0.0f, 0.0f, 0.0f); break;
	case 'j': updateRay(0.0f, -0.5f, 0.0f, 0.0f); break;
	case 'l': updateRay(0.0f, 0.5f, 0.0f, 0.0f); break;
	case 'i': updateRay(0.0f, 0.0f, 0.5f, 0.0f); break;
	case 'k': updateRay(0.0f, 0.0f, -0.5f, 0.0f); break;
	default: std::cout << key;
	}
	glutPostRedisplay();
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// define some triangles that are divided by kdTree algorithm
	vec.clear();
	vec.push_back(new TRIANGLE(0.0f, 0.0f, 0.0f));
	vec.push_back(new TRIANGLE(4.0f, 4.0f, 0.0f));
	vec.push_back(new TRIANGLE(2.0f, 0.0f, -6.0f));
	vec.push_back(new TRIANGLE(0.0f, 5.0f, -6.0f));
	vec.push_back(new TRIANGLE(-3.0f, 4.0f, 3.0f));
	vec.push_back(new TRIANGLE(-5.0f, -3.0f, 1.0f));
	vec.push_back(new TRIANGLE(4.0f, -4.0f, 0.0f));

	// create kdTree
	tree.buildTree(vec);

	// create ray for intersection tests
	r.orig = Point(0.0f, 0.0f, 0.0f);
	r.dir = Point(0.0f, 1.0f, -1.0f);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(camerapos.x, camerapos.y, camerapos.z);

	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat lightPos[] = { -20.0f, 10.0f, 40.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	Ray rR = r;

	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	// draw ray
	glColor3f(1.0f, 0.9f, 0.0f);
	glBegin(GL_LINES);
	int spreadNeg = 20;
	int spreadPos = 20;
	glVertex3f(rR.orig.x - rR.dir.x*spreadNeg, rR.orig.y - rR.dir.y*spreadNeg, rR.orig.z - rR.dir.z*spreadNeg);
	glVertex3f(rR.orig.x + rR.dir.x*spreadPos, rR.orig.y + rR.dir.y*spreadPos, rR.orig.z + rR.dir.z*spreadPos);
	glEnd();

	bool in = tree.intersect(rR);

	// draw koordinate system
	glBegin(GL_LINES);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	// draw all triangles
	glColor3f(0.0f, 0.0f, 1.0f);
	for (vector<TRIANGLE*>::iterator it = vec.begin();
		it != vec.end();
		++it)
	{
		(**it).intersect(rR);	// do intersection test with ray (changes color of triangle)
		(**it).draw();
	}

	// draw kdTree
	if (drawkdTree)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		int i = tree.mNodes.size();
		for (int j = 0; j < i; ++j)
		{
			glBegin(GL_LINES);
			Point min = tree.mNodes[j].boundingBox.mMin;
			Point max = tree.mNodes[j].boundingBox.mMax;

			glVertex3f(min.x, min.y, min.z);
			glVertex3f(min.x, min.y, max.z);

			glVertex3f(min.x, min.y, min.z);
			glVertex3f(max.x, min.y, min.z);

			glVertex3f(min.x, min.y, min.z);
			glVertex3f(min.x, max.y, min.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(max.x, min.y, max.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(min.x, max.y, max.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(max.x, max.y, min.z);
			
			glVertex3f(min.x, min.y, max.z);
			glVertex3f(max.x, min.y, max.z);

			glVertex3f(min.x, min.y, max.z);
			glVertex3f(min.x, max.y, max.z);

			glVertex3f(max.x, min.y, min.z);
			glVertex3f(max.x, min.y, max.z);

			glVertex3f(max.x, min.y, min.z);
			glVertex3f(max.x, max.y, min.z);

			glVertex3f(min.x, max.y, min.z);
			glVertex3f(min.x, max.y, max.z);

			glVertex3f(min.x, max.y, min.z);
			glVertex3f(max.x, max.y, min.z);
			glEnd();
		}
	}

	glutSwapBuffers();
}

// called every 25 milliseconds
void update(int value) {
	_angle += value;
	if (_angle > 360) {
		_angle -= 360;
	}
	glutPostRedisplay();
}

void updateRay(float rotY, float transX, float transY, float transZ) {
	Matrix mTrans;
	Matrix mRot;
	mTrans.translation(transX, transY, transZ);
	mRot.rotY(rotY);
	r.dir = mTrans * mRot*r.dir;
	r.orig = mTrans * r.orig;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow("kdTree");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}