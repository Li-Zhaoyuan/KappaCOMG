#include "Scene2.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}


void Scene2::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);//dark blue
	//enable depth test 
	glEnable(GL_DEPTH_TEST);

	rotateAngle = 1;
	translateX = 1;
	scaleAll = 1;

	//Generate a default VAO for now 
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	//An aaray of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data[] =
	{
		
		0.0f, 1.0f, 0.0f, // 0 of triangle
		1.0f, -1.0f, 0.0f, // 1 of triangle
		-1.0f, -1.0f, 0.0f, // 2 of traingle 

	};

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);

	// Transfer vertices to openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
		vertex_buffer_data, GL_STATIC_DRAW);


	// An array of 3 vectors which represents the colors the 3 vertices
	static const GLfloat color_buffer_data[] = {
		1.0f, 1.0f, 0.0f, // color of vertex 0
		1.0f, 1.0f, 0.0f, // color of vertex 1
		1.0f, 0.0f, 0.0f, // color of vertex 2 

	};


	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);


	glBufferData(GL_ARRAY_BUFFER, sizeof (color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);
	

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

void Scene2::Update(double dt)
{
	if (rotateAngle < 180 && rotateAngle > 0)
	{
		rotateAngle += (float)(10 * dt);
		if (rotateAngle > 180)
		{
			rotateAngle = -2;
		}
	}
	else if (rotateAngle < 0 && rotateAngle > -180)
	{
		rotateAngle -= (float)(10 * dt);
		if (rotateAngle < -180)
		{
			rotateAngle = 2;
		}
	}
	translateX += (float)(10 * dt);
	if (translateX > 25)
	{
		translateX = 0;
	}
	scaleAll += (float)(2 * dt);
	if (scaleAll > 2)
	{
		scaleAll = 0;
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateAngle = 2;
		translateX = 2;
		scaleAll = 2;
	}
}

void Scene2::Render()
{
	// Render VBO here
	//glClear(GL_COLOR_BUFFER_BIT);// clear color buffer every frame

	//Clear color & depth buffer every frame
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

	projection.SetToOrtho(-40,40, -30,30, -10,10);

	glEnableVertexAttribArray(0);// 1st attribute buffer: vertices 
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	
	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToIdentity();
	model = translate * rotate * scale; //scale, followed byrotate, then lastly translate
	MVP = projection * view * model; // Remember, matrixmultiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0,	3,	GL_FLOAT,	GL_FALSE,	0,	0	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // starting from vertex 0; 3vertices = 1 triangle

	model.SetToIdentity();

	scale.SetToIdentity();
	rotate.SetToRotation(45, 0, 0, 1);
	translate.SetToTranslation(translateX, 5, 0);
	model = translate * rotate * scale; //scale, followed byrotate, then lastly translate
	MVP = projection * view * model; // Remember, matrixmultiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // starting from vertex 0; 3vertices = 1 triangle

	model.SetToIdentity();

	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToIdentity();
	translate.SetToTranslation(25,10,0);
	model = translate * rotate * scale; //scale, followed byrotate, then lastly translate
	MVP = projection * view * model; // Remember, matrixmultiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // starting from vertex 0; 3vertices = 1 triangle



	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here

	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}