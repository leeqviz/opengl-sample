#include <stdlib.h>

#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"

void utlPrintExtList() //The glGetString function returns a string describing the current OpenGL connection.
{

	//GL_VENDOR 
	//Returns the company responsible for this OpenGL implementation. 
	//This name does not change from release to release.
	const char* vendor = (const char*)glGetString(GL_VENDOR);

	//GL_RENDERER 
	//Returns the name of the renderer. This name is 
	//typically specific to a particular configuration of a hardware platform. 
	//It does not change from release to release.
	const char* renderer = (const char*)glGetString(GL_RENDERER);

	//GL_VERSION
	//Returns a version or release number.
	const char* version = (const char*)glGetString(GL_VERSION);

	//GL_EXTENSIONS
	//Returns a space-separated list of supported extensions to OpenGL.
	const char* extensions = (const char*)glGetString(GL_EXTENSIONS);

	printf(" Vendor: %s\n Renderer: %s\n Version: %s\n ", vendor, renderer, version);

	printf("Supported extensions:\n");
	char name[1024];
	int i = 0, j = 0;
	for(i = 0, j = 0; extensions[i] != '\0'; i++)
	{
		if(!isspace(extensions[i]))
			name[j++] = extensions[i];
		else
		{
			name[j] = '\0';
			printf("\t%s\n", name);
			j = 0;
		}
	}

	if(j > 0)
	{
		name[j] = '\0';	
		printf("\t%s\n", name);
	}
}

void usr_glClearMaterial() //clear material
{
	GLfloat blackColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, blackColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blackColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackColor);
	glMaterialfv(GL_FRONT, GL_EMISSION, blackColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
}

void glEnableTransparentTexture(GLboolean _enabled) //метод для отображения эффекта прозрачных текстур
{
	if (_enabled)
	{
		//метод, которым величины текстуры должны комбинироваться с обрабатываемыми фрагментами
		//GL_DECAL, GL_REPLACE, GL_MODULATE, GL_BLEND
		//OpenGL Red Book (с. 290 - 291)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//переопределим цвет и прозрачность материала
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		//включение режима смешивания цветов
		glEnable(GL_BLEND);

		//настройка режима смешивания
		//OpenGL Red Book (с. 160 - 161)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//делаем буфер глубины доступным только для чтения
		//решение проблемы прорисовки прозрачных объектов в нужном порядку
		glDepthMask(GL_FALSE); 
	}
	else
	{
		//используем исходные настройки материала
		glDisable(GL_COLOR_MATERIAL);

		//выключение режима смешивания цветов
		glDisable(GL_BLEND);

		//делаем буфер глубины доступным для записи
		glDepthMask(GL_TRUE); 
	}
}