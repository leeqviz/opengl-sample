#ifndef TEXTURE_UTILS
#define TEXTURE_UTILS

#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"

#include "FreeImage.h"

void utlLoadTexture(GLuint* texture, const char* imageFileName)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imageFileName,0); //определяем формат файла изображения

	FIBITMAP* img = FreeImage_Load(format, imageFileName); //загрузка файла текстуры

	BYTE* bDataPointer = FreeImage_GetBits(img); //получение массива байт текстуры

	int iWidth = FreeImage_GetWidth(img); 
	int iHeight = FreeImage_GetHeight(img); 
	int iBPP = FreeImage_GetBPP(img); 
	
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	int iFormat = iBPP == 24 ? GL_BGR_EXT : iBPP == 8 ? GL_LUMINANCE : 0;
	printf("utlLoadTexture: %02d %02d %02d\n", iWidth, iHeight, iBPP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //режим фильтрации текстуры при расширении
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //режим фильтрации текстуры при уменьшении

	FreeImage_Unload(img); //выгрузка данных изображения из памяти
}

#endif