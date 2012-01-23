#include "GLMethods.h"

void GLMethods::clearGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLMethods::clearGL(double r, double g, double b, double a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLMethods::setViewport(int left, int bottom, int width, int height)
{
	GLint* params = new GLint[2];

	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, params);
	if ((GLint)width <= params[0] && (GLint)height <= params[1])
	{
		glViewport((GLint)left,(GLint)bottom,(GLsizei)width,(GLsizei)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-width/2, width/2, -height/2, height/2);
		glMatrixMode(GL_MODELVIEW);
	}
	else
	{
		cerr << "Error: width or height to viewport exceeded the limits.\n";
	}
}

bool GLMethods::checkSizeTile(int size)
{
	GLuint texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA, (GLint)size, (GLint)size, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	GLint width;

	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	if (width == 0)
		return false;// Can't use that texture
	else
		return true;
}

bool GLMethods::createTexture(unsigned int* texture, int format, int width, int height, void* pixels)
{
	//Criamos um objeto de textura e obtemos seu id
	glGenTextures(1, (GLuint*)texture);

	//Dizemos ao OpenGL que iremos trabalhar com o objeto.
	glBindTexture(GL_TEXTURE_2D, (GLuint)*texture);

	//Filtro. Veremos isso nos próximos artigos.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Interpretamos o formato passado
	GLint f;
	if (format == 1)
		f = GL_LUMINANCE;
	else if (format == 3)
		f = GL_RGB;
	else
		f = GL_RGBA;

	//Carregamos a imagem do disco
	glTexImage2D(GL_TEXTURE_2D, 0, f, width, height, 0,
				 f, GL_UNSIGNED_BYTE, (GLvoid*)pixels);

	//Caso tenha havido um erro no processo, lançamos o erro. E caso contrário retornamos positivamente.
	GLenum err = glGetError ();
	if (GL_NO_ERROR != err)
		cerr << "Error: error when creating texture!\n";
	else
		return true;
	return false;
}

void GLMethods::deleteTexture(unsigned int* texture)
{
	glDeleteTextures(1, (GLuint*)texture);
}

/*
void GLMethods::rotateTexture(unsigned int &texture, double ang)
{

 if (!glIsTexture((GLuint)texture))
 {
  return;
 }
 glEnable(GL_TEXTURE_2D);
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
 glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glTranslatef(0.5,0.5,0.0);
 glRotatef(ang,0.0,0.0,1.0);
 glTranslatef(-0.5,-0.5,0.0);
 glMatrixMode(GL_MODELVIEW);
 glDisable(GL_TEXTURE_2D);
 glRotatef(0.0,0.0,0.0,1.0);

}
*/

void GLMethods::drawImage(unsigned int texture, double left, double right, double top, double bottom, bool useAlpha)
{
	//Retornamos imediatamente se o identificador da textura criada n~ao ´e v´alido
	if (!glIsTexture((GLuint)texture))
	{
		return;
	}

	if (useAlpha)
	{
		glEnable (GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
	glBegin (GL_QUADS);
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(left, top);

		glTexCoord2f(1.0, 1.0);
		glVertex2f(right, top);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(right, bottom);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(left, bottom);
	}
	glEnd ();
	glDisable(GL_TEXTURE_2D);

	if (useAlpha)
		glDisable(GL_BLEND);
}

void GLMethods::getMousePos(double x, double y, double result[2])
{
	//Esse método precisa de revisão quanto a nome, localização e até mesmo de funcionamento.
	GLdouble objCoord[3];
	glLoadIdentity();
	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	GLdouble projMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	gluUnProject(x, y, 0.0, modelMatrix, projMatrix, viewport, &objCoord[0], &objCoord[1], &objCoord[2]);

	result[0] = objCoord[0];
	result[1] = objCoord[1];
}
