#include "Assignment1.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"


Assignment1::Assignment1()
{
}

Assignment1::~Assignment1()
{
}


void Assignment1::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.7f, 1.0f, 0.0f);
	//enable depth test 
	glEnable(GL_DEPTH_TEST);

	rotateAngle = 1;
	translateX = 1;
	scaleAll = 1;
	flyBack = 1;
	cloudMovement = 1;
	explosionRotation = 1;
	explosionSize = 1;
	sizeValue;
	//Generate a default VAO for now 
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	landScape();
	clouds();
	sun();
	explosion();
	cactus();

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		//"Shader//SimpleVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader"
		);

	//Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	// Use our shader
	glUseProgram(m_programID);

}

void Assignment1::Update(double dt)
{
	
	rotateAngle -= (float)(20 * dt);
	translateX += (float)(10 * dt);
	scaleAll += (float)(2 * dt);
	if (flyBack < 30 && flyBack > 0)
	{
		cloudMovement += (float)(10 * dt);
		flyBack += (float)(10 * dt);
	}
	else if (flyBack > 30)
	{
		flyBack = -1;
	}
	if (flyBack > -30 && flyBack < 0)
	{
		cloudMovement -= (float)(10 * dt);
		flyBack -= (float)(10 * dt);
	}
	else if (flyBack < -30)
	{
		flyBack = 1;
	}
	explosionRotation += (float)(10 * dt);
	if (sizeValue < 10 && sizeValue > 0)
	{
		explosionSize += (float)(2 * dt);
		sizeValue += (float)(2 * dt);
	}
	else if (sizeValue > 10)
	{
		sizeValue = -1;
	}
	if (sizeValue > -10 && sizeValue < 0)
	{
		explosionSize -= (float)(2 * dt);
		sizeValue -= (float)(2 * dt);
	}
	else if (sizeValue < -10)
	{
		sizeValue = 1;
	}
}

void Assignment1::Render()
{
	// Render VBO here
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();

	projection.SetToOrtho(-40, 40, -30, 30, -10, 10);

	glEnableVertexAttribArray(0);// 1st attribute buffer: vertices 
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	//<landscape>
	scale.SetToScale(125, 125, 125);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0,-125,1);
	model = translate * rotate * scale; 
	MVP = projection * view * model; 
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	model.SetToIdentity();

	scale.SetToScale(20, 20, 20);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(25, 0, 0);
	model = translate * rotate * scale; 
	MVP = projection * view * model; 
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	model.SetToIdentity();

	scale.SetToScale(50, 20, 20);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(10, -5, -0.5);
	model = translate * rotate * scale; 
	MVP = projection * view * model; 
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[LANDSCAPE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
	//</landscape>

	//<cloud>
	scale.SetToScale(10, 10, 10);
	rotate.SetToRotation(25, 0, 0, 1);
	translate.SetToTranslation(cloudMovement , 25, 0.5f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CLOUDS_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CLOUDS_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

	//</cloud>
	//<sun>
	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(-25, 20, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[SUN_3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[SUN_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
	
	//</sun>
	//<explosion>
	scale.SetToScale(explosionSize, explosionSize, explosionSize);
	rotate.SetToRotation(explosionRotation, 0, 0, 1);
	translate.SetToTranslation(-25, 20, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[EXPLOSION_4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[EXPLOSION_4]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	//</explosion>
	//<cactus>
	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-25, 0, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);

	model.SetToIdentity();

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(15, -5, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);

	model.SetToIdentity();

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(7, 5, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);

	model.SetToIdentity();
	
	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(180, 0, 1, 0);
	translate.SetToTranslation(25, 0, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);
	

	model.SetToIdentity();

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(180, 0, 1, 0);
	translate.SetToTranslation(-15, -5, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);

	model.SetToIdentity();

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(180, 0, 1, 0);
	translate.SetToTranslation(-7, 5, 2);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 13);
	//</cactus>
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Assignment1::Exit()
{
	// Cleanup VBO here

	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}

void Assignment1::landScape()
{
	static const GLfloat landscape_data[] =
	{

		0.0f, -0.5f , 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[LANDSCAPE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(landscape_data),
		landscape_data, GL_STATIC_DRAW);

	static const GLfloat landscape_color_data[] = {
		1.0f, 0.7f, 0.0f, 
		1.0f, 0.7f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.7f, 0.0f, 
		1.0f, 0.7f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[LANDSCAPE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (landscape_color_data),
		landscape_color_data, GL_STATIC_DRAW);
}

void Assignment1::clouds()
{
	static const GLfloat cloud_data[] =
	{
 
		/*-1.0f, 0.1f, 0.0f,
		-0.6f, 0.8f, 0.0f,
		0.1f, 1.0f, 0.0f,
		0.8f, 0.6f, 0.0f,
		1.0f, -0.1f, 0.0f,
		0.6f, -0.8f, 0.0f,
		-0.1f, -1.0f, 0.0f,
		-0.8f, -0.6f, 0.0f,*/
		-1.0f, 0.2f, 0.0f,
		-1.0f, -0.2f, 0.0f,
		-0.7f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.3f, -0.8f, 0.0f,
		0.7f,-0.8f, 0.0f,
		1.0f, -0.5f, 0.0f,
		1.0f, -0.1f, 0.0f,
		0.7f, 0.2f, 0.0f,
		0.0f, 0.2f, 0.0f,
		-0.3f, 0.5f, 0.0f,
		-0.7f, 0.5f, 0.0f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CLOUDS_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cloud_data),
		cloud_data, GL_STATIC_DRAW);

	static const GLfloat cloud_color_data[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CLOUDS_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (cloud_color_data),
		cloud_color_data, GL_STATIC_DRAW);
}

void Assignment1::sun()
{
	static const GLfloat sun_data[] =
	{
 
		-1.0f, 0.1f, 0.5f,
		-0.6f, 0.8f, 0.5f,
		 0.1f, 1.0f, 0.5f,
		 0.8f, 0.6f, 0.5f,
		 1.0f, -0.1f, 0.5f,
		 0.6f, -0.8f, 0.5f,
		-0.1f, -1.0f, 0.5f,
		-0.8f, -0.6f, 0.5f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[SUN_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sun_data),
		sun_data, GL_STATIC_DRAW);

	static const GLfloat sun_color_data[] = {
		1.0f, 0.9f, 0.5f,
		1.0f, 0.9f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.9f, 0.5f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.9f, 0.0f,
		1.0f, 0.9f, 0.0f,
		1.0f, 0.9f, 0.9f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[SUN_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (sun_color_data),
		sun_color_data, GL_STATIC_DRAW);
}

void Assignment1::explosion()
{
	static const GLfloat explosion_data[] =
	{
		0.0f, 0.8f, -0.5f,
		0.8f, -0.3f, -0.5f,
		-0.8f, -0.3f, -0.5f,

		0.0f, -0.8f, -0.5f,
		-0.8f, 0.3f, -0.5f,
		0.8f, 0.3f, -0.5f,

		0.0f, 1.0f, -0.5f,
		1.0f, -0.5f, -0.5f,
		-1.0f, -0.5f, -0.5f,

		0.0f, -1.0f, -0.5f,
		-1.0f, 0.5f, -0.5f,
		1.0f, 0.5f, -0.5f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[EXPLOSION_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(explosion_data),
		explosion_data, GL_STATIC_DRAW);

	static const GLfloat explosion_color_data[] = 
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[EXPLOSION_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (explosion_color_data),
		explosion_color_data, GL_STATIC_DRAW);
}

void Assignment1::cactus()
{
	static const GLfloat cactus_data[] =
	{

		-0.5f, 0.0f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.8f, -0.5, 0.0f,
		1.0f, -0.2f, 0.0f,
		1.0f, 0.2f, 0.0f,
		0.8f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.8f, 0.0f,
		0.2f, 1.0f, 0.0f,
		-0.2f, 1.0f, 0.0f,
		-0.5f, 0.8f, 0.0f,

	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[CACTUS_5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cactus_data),
		cactus_data, GL_STATIC_DRAW);

	static const GLfloat cactus_color_data[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[CACTUS_5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (cactus_color_data),
		cactus_color_data, GL_STATIC_DRAW);
}