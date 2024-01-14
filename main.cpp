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

CameraMotionHelper cameraMotionHelper; //объект-помощник для поворота сцены с помощью курсора мыши

GLfloat animationParam = 0.0f; //значение параметра для анимации
GLuint particleTexture; //текстура стекла

void setupLight() //метод настройки источников освещения
{
	glEnable(GL_LIGHTING); //включение освещения
	glEnable(GL_LIGHT0); //включение точечного источника света 0 (нулевой лампы) всего может быть 8 источников света

	//установка фоновой освещенности белый цвет
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//настройка точечного источника света:
	//в модели освещения Фонга свет представляется в виде трех составляющих: 
	//зеркальная (отвечает за блики) 
	GLfloat specularLight[] = {1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	//фоновая (равномерный серый цвет)
	GLfloat ambientLight0[] = {0.3f, 0.3f, 0.3f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);

	//рассеянная (белый цвет)
	GLfloat diffuseLight[] = {1.0, 1.0, 1.0}; 	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //установка общего фона (черный цвет)

	glEnable(GL_DEPTH_TEST); //активирование функции глубины

	//автоматическое привидение нормалей к единичной длине
	//затратная процедура, нужна при изменении размеров моделей
	//и для правильного расчета освещения
	glEnable(GL_NORMALIZE);

	//отключаем задание материалов объектов одним параметром
	//glColor3f 
	//для задания материала объектов мы будем использовать
	//команду glMaterialfv
	glDisable(GL_COLOR_MATERIAL);

	setupLight(); //настройка источников освещения
	modelInitSprites(); //настройка модели
}

void axis(GLfloat size) //метод прорисовки координатных осей параметр задает размер осей
{
	//включаем задание материалов объектов одним параметром glColor3f 
	//для непосредственного задания цвета линий
	glEnable(GL_COLOR_MATERIAL);

	//сбрасываем настройки материала, чтобы материал
	//не оказывал воздействие на цвет линий
	GLfloat emissionColor[] = {0.0, 0.0, 0.0, 1.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor);

	//устанавливаем толщину линий
	glLineWidth(1.0f);

	//прорисовка оси Х с помощью отрезка
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);	
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(-size, 0.0f, 0.0f);
	glEnd();

	//прорисовка названия начала оси Х - "-Х"
	glRasterPos3f(-size, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'-');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)'X');

	//прорисовка названия конца оси Х - "+Х"
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

	//отключаем использование непосредственного задания цвета
	glDisable(GL_COLOR_MATERIAL);
}

void renderLamp() //метод прорисовки модели "лампочки"
{
	//Когда речь идет о свойствах материала в приложении к освещению, 
	//то имеется в виду его способность воспринимать каждую из трех 
	//компонент цвета каждой составляющей освещенности.

	//собственное свечение объекта
	//наша "лампочка" светится белым цветом
	GLfloat emissionColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	//остальные составляющие материала оставляем "черными"
	//так как они в нашем случае не влияют на конечный (белый)
	//цвет модели 
	GLfloat ambientColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat diffuseColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat specularColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	
	//в библиотеке GLUT существует ряд методов 
	//для прорисовки простых примитивов
	//в данном случае сферы
	glutSolidSphere(0.2f, 32, 32);
}

void renderSprites() //рисуем каждую частицу
{
	for (SPRITES::iterator i = modelSprites.begin(); i != modelSprites.end(); i++)	
	{
		glPushMatrix(); //сохраняем модельную матрицу в стек

		glTranslatef(i->p[0], i->p[1], i->p[2]); //устанавливаем положение частицы

		//создаем billboard матрицу из текущей. особенность данного преобразования в том, что поверхность спрайта всегда повернута к наблюдателю
		double MV[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, MV);
		utlMat4x4(MV, MV);
		glLoadMatrixd(MV);

		glScalef(i->s, i->s, i->s);	//изменяем размер частицы

		glBegin(GL_QUADS); //рисуем спрайт как квадрат с заданным цветом
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

		glPopMatrix(); //извлекаем модельную матрицу из стека
	}
}

void renderModel() //метод отрисовки системы частиц
{	
	usr_glClearMaterial(); //сброс всех параметров материала
	glDepthMask(GL_FALSE); //делаем буфер глубины доступным только для чтения решение проблемы прорисовки прозрачных объектов в нужном порядке
	glEnable(GL_COLOR_MATERIAL); //включаем режим задания цвета материала
	glDisable(GL_LIGHTING); //отключим освещение
	glEnable(GL_TEXTURE_2D); //включаем текстурирование
	glEnable(GL_BLEND); //включаем смешивае
	glEnable(GL_CULL_FACE);

	//задаем параметры смешивания таким образом, чтобы на спрайте отрисовалась только "белая" часть текстуры с прозрачностью
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, particleTexture); //задаем непосредственно текстуру
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	renderSprites(); //рисуем непосредственно сами спрайты

	//отключаем выбраные режимы
	glDisable(GL_BLEND); 
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE); 

	glEnable(GL_LIGHTING); //включим освещение
}

void display() //Функция управляет всем выводом на экран
{
	//Команда glClear очищает буферы, а параметр buf определяет буферы, 
	//которые нужно очистить. Типичная программа вызывает команду
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) 
	//для очистки буферов цвета и глубины. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glMatrixMode(GL_MODELVIEW); //выбираем видовую матрицу

	glLoadIdentity(); //сбрасываем матрицу			

	//устанавливаем наблюдателя относительно нулевой
	//координаты в пространстве
	//положение наблюдателя получаем от объекта-помощника
	gluLookAt(cameraMotionHelper.getDistance(), cameraMotionHelper.getDistance(), cameraMotionHelper.getDistance(),
		      0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	//поворачиваем всю наблюдаемую систему по вертикали
	//(управление сценой курсором мыши)
	//угол поворота получаем от объекта-помощника
	glRotatef(cameraMotionHelper.getRotationXZ(), 1.0f, 0.0f, -1.0f);

	//поворачиваем всю наблюдаемую систему по горизонтали
	//(управление сценой курсором мыши)
	//угол поворота получаем от объекта-помощника
	glRotatef(cameraMotionHelper.getRotationY(), 0.0f, 1.0f, 0.0f);

	//управление положением источника света
	glPushMatrix(); //сохраняем текущую видовую матрицу в стек

	//установка позиции источника освещения
	GLfloat lightPos[] = {3.0f, 7.0f, 7.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//установка позиции модельки "лампочки"
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]); 

	//renderLamp(); //прорисовка "лампочки"

	glPopMatrix(); //извлекаем видовую матрицу из стека

	//axis(15.0f); //прорисовка глобальной системы координат

	renderModel(); //прорисовка модели

	glutSwapBuffers(); //команда смены кадров (для двойной буфферизации)
}

void reshape(GLint w, GLint h) //Функция вызывается при изменении размеров окна
{ 
	glViewport(0, 0, w, h); //устанавливаем размеры области отображения

	glMatrixMode(GL_PROJECTION); //выбираем для настройки матрицу проекций

	glLoadIdentity(); //сбросим выбранную матрицу (матрицу проекций) заменяет текущую матрицу на единичную

	//настройка перспективной проекции
	GLfloat zNear = 1;
	GLfloat zFar = 200;
	//Параметры zFar и zNear задают расстояние от наблюдателя 
	//до плоскостей отсечения по глубине и должны быть положительными
	gluPerspective(45.0, (float)w / (float)h, zNear, zFar);
} 

void keyboardFunc( unsigned char key, int x, int y ) //Функция обрабатывает сообщения от клавиатуры
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

void updateAnimation(int value) //метод получения событий таймера
{
	//анимация частиц (изменение положения каждой частицы)
	for (SPRITES::iterator i = modelSprites.begin(); i != modelSprites.end(); i++) 
	{
		
		i->step();
	}

	glutPostRedisplay(); //посылаем сообщение для перерисовки сцены
	glutTimerFunc(25, updateAnimation, 0); //запускаем таймер заново
}

void mouseFunc(int button, int state, int x, int y) //метод получения сообщений от мыши (нажатие клавиш) будем использовать его для управления отображением сцены
{	
	if (button == GLUT_LEFT_BUTTON) //поступило событие от левой кнопки мыши
	{		
		if (state == GLUT_DOWN) //нажатие кнопки мыши
		{
			//запоминаем начальные координаты в которой
			//нажали левую кнопку мыши
			//сообщаем объекту-помощнику что начат поворот сцены
			cameraMotionHelper.startMove(x, y);
		}
		else			
		{
			//кнопка мыши была отжата,
			//поэтому заканчиваем поворот сцены
			//сообщаем объекту-помощнику что поворот сцены окончен
			cameraMotionHelper.endMove();
		}
	}
}

void mouseMotionFunc(int x, int y) //метод получения событий перемещения курсора мыши при любой нажатой кнопке мыши
{
	cameraMotionHelper.move(x, y); //сообщаем объекту-помощнику новые координаты курсора
}

void mouseWheelFunc(int button, int dir, int x, int y) //метод получения событий от колеса мыши используется для приближения и отдаления камеры наблюдателя
{
	//в зависимости от направления прокрутки колеса
	//сообщаем объекту-помощнику либо увеличить дистанцию
	//до наблюдаемой точки - либо уменьшить
	if (dir < 0) cameraMotionHelper.increaseDistance(); 
	else cameraMotionHelper.decreaseDistance();
}

int main(int argc, char** argv)
{
	//Функция glutInit(&argc, argv) производит начальную инициализацию 
	//самой библиотеки GLUT.
	glutInit(&argc, argv);

	//Команда glutInitDisplayMode(GLUT_RGB) инициализирует буфер кадра и 
	//настраивает полноцветный(непалитровый) режим RGB. 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//glutInitWindowSize(Width, Height) используется для 
	//задания начальных размеров окна.
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//задать положение создаваемого окна относительно 
	//верхнего левого угла экрана
	glutInitWindowPosition(100, 100);

	//Эта команда создаёт окно с заголовком, который вы укажете в качестве 
	//параметра и возвращает HANDLER окна в виде числа int. 
	//Этот HANDLER обычно используется для последующих операций 
	//над этим окном, таких как изменение параметров окна и закрытие окна.
	glutCreateWindow("Particles");

	utlPrintExtList(); //вывод в консоль доступных расширений для текущей версии OpenGL

	init();

	glutDisplayFunc(display); //регистрируем функцию перерисовки сцены

	glutReshapeFunc(reshape); //регистрируем функцию изменения размера окна

	glutKeyboardFunc(keyboardFunc); //регистрируем функцию получения событий от клавиатуры

	glutMouseWheelFunc(mouseWheelFunc); //регистрируем функцию получения событий от колеса мыши

	glutTimerFunc(25, updateAnimation, 0); //регистрируем функцию получения событий от таймера для реализации анимаций

	glutMouseFunc(mouseFunc); //регистрируем функцию получения событий от кнопок мыши

	glutMotionFunc(mouseMotionFunc); //регистрируем функцию получения событий от движения курсора

	utlLoadTexture(&particleTexture, "sprite.jpg");
	
	//Контроль всех событий и вызов нужных функций происходит внутри цикла
	//обработки сообщений в функции glutMainLoop().
	glutMainLoop();

	return 0;
}