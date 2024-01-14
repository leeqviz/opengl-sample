#include <stdlib.h>
#include <stdio.h>

#include "GL/glew.h"

#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"

#include "CameraMotionHelper.h"
#include "ModelDescription.h"
#include "GLUtils.h"
#include "TextureUtils.h"
#include "MathUtils.h"

using namespace glm;

#define ESCAPE '\033'

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

CameraMotionHelper cameraMotionHelper; //������-�������� ��� �������� ����� � ������� ������� ����

GLfloat animationParam = 0.0f; //�������� ��������� ��� ��������
GLuint particleTexture; //�������� ������

void setupLight() //����� ��������� ���������� ���������
{
	glEnable(GL_LIGHTING); //��������� ���������
	glEnable(GL_LIGHT0); //��������� ��������� ��������� ����� 0 (������� �����) ����� ����� ���� 8 ���������� �����

	//��������� ������� ������������ ����� ����
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//��������� ��������� ��������� �����:
	//� ������ ��������� ����� ���� �������������� � ���� ���� ������������: 
	//���������� (�������� �� �����) 
	GLfloat specularLight[] = {1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	//������� (����������� ����� ����)
	GLfloat ambientLight0[] = {0.3f, 0.3f, 0.3f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);

	//���������� (����� ����)
	GLfloat diffuseLight[] = {1.0, 1.0, 1.0}; 	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //��������� ������ ���� (������ ����)

	glEnable(GL_DEPTH_TEST); //������������� ������� �������

	//�������������� ���������� �������� � ��������� �����
	//��������� ���������, ����� ��� ��������� �������� �������
	//� ��� ����������� ������� ���������
	glEnable(GL_NORMALIZE);

	//��������� ������� ���������� �������� ����� ����������
	//glColor3f 
	//��� ������� ��������� �������� �� ����� ������������
	//������� glMaterialfv
	glDisable(GL_COLOR_MATERIAL);

	setupLight(); //��������� ���������� ���������
	modelInitSprites(); //��������� ������
}

void axis(GLfloat size) //����� ���������� ������������ ���� �������� ������ ������ ����
{
	//�������� ������� ���������� �������� ����� ���������� glColor3f 
	//��� ����������������� ������� ����� �����
	glEnable(GL_COLOR_MATERIAL);

	//���������� ��������� ���������, ����� ��������
	//�� �������� ����������� �� ���� �����
	GLfloat emissionColor[] = {0.0, 0.0, 0.0, 1.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor);

	//������������� ������� �����
	glLineWidth(1.0f);

	//���������� ��� � � ������� �������
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);	
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(-size, 0.0f, 0.0f);
	glEnd();

	//���������� �������� ������ ��� � - "-�"
	glRasterPos3f(-size, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'-');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'X');

	//���������� �������� ����� ��� � - "+�"
	glRasterPos3f(size, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'+');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'X');

	//glBegin(GL_LINES);
	//glColor3f(0.0f, 1.0f, 0.0f);	
	//glVertex3f(0.0f, size, 0.0f);
	//glVertex3f(0.0f, -size, 0.0f);
	//glEnd();

	//glRasterPos3f(0.0f, -size, 0.0f);
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'-');
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'Y');

	//glRasterPos3f(0.0f, size, 0.0f);
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'+');
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'Y');

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);	
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, 0.0f, -size);
	glEnd();

	glRasterPos3f(0.0f, 0.0f, -size);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'-');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'Z');

	glRasterPos3f(0.0f, 0.0f, size);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'+');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'Z');

	//��������� ������������� ����������������� ������� �����
	glDisable(GL_COLOR_MATERIAL);
}

void renderLamp() //����� ���������� ������ "��������"
{
	//����� ���� ���� � ��������� ��������� � ���������� � ���������, 
	//�� ������� � ���� ��� ����������� ������������ ������ �� ���� 
	//��������� ����� ������ ������������ ������������.

	//����������� �������� �������
	//���� "��������" �������� ����� ������
	GLfloat emissionColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	//��������� ������������ ��������� ��������� "�������"
	//��� ��� ��� � ����� ������ �� ������ �� �������� (�����)
	//���� ������ 
	GLfloat ambientColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat diffuseColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat specularColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	
	//� ���������� GLUT ���������� ��� ������� 
	//��� ���������� ������� ����������
	//� ������ ������ �����
	glutSolidSphere(0.2f, 32, 32);
}

void renderSprites() //������ ������ �������
{
	for (SPRITES::iterator i = modelSprites.begin(); i != modelSprites.end(); i++)	
	{
		glPushMatrix(); //��������� ��������� ������� � ����

		glTranslatef(i->p[0], i->p[1], i->p[2]); //������������� ��������� �������

		//������� billboard ������� �� �������. ����������� ������� �������������� � ���, ��� ����������� ������� ������ ��������� � �����������
		double MV[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, MV);
		utlMat4x4(MV, MV);
		glLoadMatrixd(MV);

		glScalef(i->s, i->s, i->s);	//�������� ������ �������

		glBegin(GL_QUADS); //������ ������ ��� ������� � �������� ������
			glColor4f(i->c[0], i->c[1], i->c[2], i->c[3]);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(-1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(-1.0f,-1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f( 1.0f,-1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f( 1.0f, 1.0f);
		glEnd();

		glPopMatrix(); //��������� ��������� ������� �� �����
	}
}

void renderModel() //����� ��������� ������� ������
{	
	usr_glClearMaterial(); //����� ���� ���������� ���������
	glDepthMask(GL_FALSE); //������ ����� ������� ��������� ������ ��� ������ ������� �������� ���������� ���������� �������� � ������ �������
	glEnable(GL_COLOR_MATERIAL); //�������� ����� ������� ����� ���������
	glDisable(GL_LIGHTING); //�������� ���������
	glEnable(GL_TEXTURE_2D); //�������� ���������������
	glEnable(GL_BLEND); //�������� ��������
	glEnable(GL_CULL_FACE);

	//������ ��������� ���������� ����� �������, ����� �� ������� ������������ ������ "�����" ����� �������� � �������������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, particleTexture); //������ ��������������� ��������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	renderSprites(); //������ ��������������� ���� �������

	//��������� �������� ������
	glDisable(GL_BLEND); 
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE); 

	glEnable(GL_LIGHTING); //������� ���������
}

void display() //������� ��������� ���� ������� �� �����
{
	//������� glClear ������� ������, � �������� buf ���������� ������, 
	//������� ����� ��������. �������� ��������� �������� �������
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) 
	//��� ������� ������� ����� � �������. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glMatrixMode(GL_MODELVIEW); //�������� ������� �������

	glLoadIdentity(); //���������� �������			

	//������������� ����������� ������������ �������
	//���������� � ������������
	//��������� ����������� �������� �� �������-���������
	gluLookAt(cameraMotionHelper.getDistance(), cameraMotionHelper.getDistance(), cameraMotionHelper.getDistance(),
		      0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	//������������ ��� ����������� ������� �� ���������
	//(���������� ������ �������� ����)
	//���� �������� �������� �� �������-���������
	glRotatef(cameraMotionHelper.getRotationXZ(), 1.0f, 0.0f, -1.0f);

	//������������ ��� ����������� ������� �� �����������
	//(���������� ������ �������� ����)
	//���� �������� �������� �� �������-���������
	glRotatef(cameraMotionHelper.getRotationY(), 0.0f, 1.0f, 0.0f);

	//���������� ���������� ��������� �����
	glPushMatrix(); //��������� ������� ������� ������� � ����

	//��������� ������� ��������� ���������
	GLfloat lightPos[] = {3.0f, 7.0f, 7.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//��������� ������� �������� "��������"
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]); 

	//renderLamp(); //���������� "��������"

	glPopMatrix(); //��������� ������� ������� �� �����

	//axis(15.0f); //���������� ���������� ������� ���������

	renderModel(); //���������� ������

	glutSwapBuffers(); //������� ����� ������ (��� ������� ������������)
}

void reshape(GLint w, GLint h) //������� ���������� ��� ��������� �������� ����
{ 
	glViewport(0, 0, w, h); //������������� ������� ������� �����������

	glMatrixMode(GL_PROJECTION); //�������� ��� ��������� ������� ��������

	glLoadIdentity(); //������� ��������� ������� (������� ��������) �������� ������� ������� �� ���������

	//��������� ������������� ��������
	GLfloat zNear = 1;
	GLfloat zFar = 200;
	//��������� zFar � zNear ������ ���������� �� ����������� 
	//�� ���������� ��������� �� ������� � ������ ���� ��������������
	gluPerspective(45.0, (float)w / (float)h, zNear, zFar);
} 

void keyboardFunc( unsigned char key, int x, int y ) //������� ������������ ��������� �� ����������
{
	if (key == ESCAPE) exit(0); 

	if (key == '1') spriteMode = 1;

	if (key == '2') spriteMode = 2;

	if (key == '3') spriteMode = 3;

	if (key == '4') spriteMode = 4; //task

	if (key == 'w')
	{
		for (int i=0; i<50; i++)
			modelSprites.push_back(SPRITE());

		printf("sprite count = %d\n", modelSprites.size());
	}

	if (key == 's')
	{
		for (int i=0; i<50 && modelSprites.size() > 0; i++)
			modelSprites.erase(modelSprites.begin());

		printf("sprite count = %d\n", modelSprites.size());
	}

	if (key == 'd')
	{
		vXZ0 += 0.01f;

		printf("vXZ0 = %.03f\n", vXZ0);
	}

	if (key == 'a')
	{
		if (vXZ0-0.02f > 0.0f)
			vXZ0 -= 0.01f;

		printf("vXZ0 = %.03f\n", vXZ0);
	}

	if (key == 'r')
	{
		vY0 += 0.01f;

		printf("vY0 = %.03f\n", vY0);
	}

	if (key == 'f')
	{
		vY0 -= 0.01f;

		if (vY0 <= 0.0f)
			vY0 = 0.01f;

		printf("vY0 = %.03f\n", vY0);
	}
} 

void updateAnimation(int value) //����� ��������� ������� �������
{
	//�������� ������ (��������� ��������� ������ �������)
	for (SPRITES::iterator i = modelSprites.begin(); i != modelSprites.end(); i++) 
	{
		
		i->step();
	}

	glutPostRedisplay(); //�������� ��������� ��� ����������� �����
	glutTimerFunc(25, updateAnimation, 0); //��������� ������ ������
}

void mouseFunc(int button, int state, int x, int y) //����� ��������� ��������� �� ���� (������� ������) ����� ������������ ��� ��� ���������� ������������ �����
{	
	if (button == GLUT_LEFT_BUTTON) //��������� ������� �� ����� ������ ����
	{		
		if (state == GLUT_DOWN) //������� ������ ����
		{
			//���������� ��������� ���������� � �������
			//������ ����� ������ ����
			//�������� �������-��������� ��� ����� ������� �����
			cameraMotionHelper.startMove(x, y);
		}
		else			
		{
			//������ ���� ���� ������,
			//������� ����������� ������� �����
			//�������� �������-��������� ��� ������� ����� �������
			cameraMotionHelper.endMove();
		}
	}
}

void mouseMotionFunc(int x, int y) //����� ��������� ������� ����������� ������� ���� ��� ����� ������� ������ ����
{
	cameraMotionHelper.move(x, y); //�������� �������-��������� ����� ���������� �������
}

void mouseWheelFunc(int button, int dir, int x, int y) //����� ��������� ������� �� ������ ���� ������������ ��� ����������� � ��������� ������ �����������
{
	//� ����������� �� ����������� ��������� ������
	//�������� �������-��������� ���� ��������� ���������
	//�� ����������� ����� - ���� ���������
	if (dir < 0) cameraMotionHelper.increaseDistance(); 
	else cameraMotionHelper.decreaseDistance();
}

int main(int argc, char** argv)
{
	//������� glutInit(&argc, argv) ���������� ��������� ������������� 
	//����� ���������� GLUT.
	glutInit(&argc, argv);

	//������� glutInitDisplayMode(GLUT_RGB) �������������� ����� ����� � 
	//����������� ������������(������������) ����� RGB. 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//glutInitWindowSize(Width, Height) ������������ ��� 
	//������� ��������� �������� ����.
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//������ ��������� ������������ ���� ������������ 
	//�������� ������ ���� ������
	glutInitWindowPosition(100, 100);

	//��� ������� ������ ���� � ����������, ������� �� ������� � �������� 
	//��������� � ���������� HANDLER ���� � ���� ����� int. 
	//���� HANDLER ������ ������������ ��� ����������� �������� 
	//��� ���� �����, ����� ��� ��������� ���������� ���� � �������� ����.
	glutCreateWindow("Particles");

	utlPrintExtList(); //����� � ������� ��������� ���������� ��� ������� ������ OpenGL

	init();

	glutDisplayFunc(display); //������������ ������� ����������� �����

	glutReshapeFunc(reshape); //������������ ������� ��������� ������� ����

	glutKeyboardFunc(keyboardFunc); //������������ ������� ��������� ������� �� ����������

	glutMouseWheelFunc(mouseWheelFunc); //������������ ������� ��������� ������� �� ������ ����

	glutTimerFunc(25, updateAnimation, 0); //������������ ������� ��������� ������� �� ������� ��� ���������� ��������

	glutMouseFunc(mouseFunc); //������������ ������� ��������� ������� �� ������ ����

	glutMotionFunc(mouseMotionFunc); //������������ ������� ��������� ������� �� �������� �������

	utlLoadTexture(&particleTexture, "sprite.jpg");
	
	//�������� ���� ������� � ����� ������ ������� ���������� ������ �����
	//��������� ��������� � ������� glutMainLoop().
	glutMainLoop();

	return 0;
}