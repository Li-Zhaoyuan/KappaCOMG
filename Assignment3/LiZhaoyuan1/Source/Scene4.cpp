#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"


Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//variable to rotate geometry
	rotateAngle = 0;
	planet1RevAngle = 0; 
	planet1RotAngle = 0; 
	moon1RotAngle = 0;

	mercuryRevAngle = 0;
	mercuryRotAngle = 0;

	venusRevAngle = 0;
	venusRotAngle = 0;

	earthRevAngle = 0;
	earthRotAngle = 0;

	marsRevAngle = 0;
	marsRotAngle = 0;

	jupiterRevAngle = 0;
	jupiterRotAngle = 0;

	saturnRevAngle = 0;
	saturnRotAngle = 0;

	uranusRevAngle = 0;
	uranusRotAngle = 0;

	neptuneRevAngle = 0;
	neptuneRotAngle = 0;

	plutoRevAngle = 0;
	plutoRotAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(45, 35, 35), Vector3(0, 0, 0), Vector3(0, 1, 0));


	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 0), 36);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 0), 0.3f, 36);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("ring", Color(1, 0, 0), 18, 36);
	meshList[GEO_PLANET1] = MeshBuilder::GenerateSphere("planet1", Color(0, 1, 0), 18, 36);
	meshList[GEO_MOON1] = MeshBuilder::GenerateSphere("moon1", Color(1, 1, 0), 18, 36);
	meshList[GEO_RING1] = MeshBuilder::GenerateRing("ring1", Color(1, 0.6f, 0.4f), 1.5f, 36);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("sun", Color(1, 1, 0), 18, 36);
	meshList[GEO_MERCURY] = MeshBuilder::GenerateSphere("mercury", Color(0.6f, 0.3f, 0.2f), 18, 36);
	meshList[GEO_VENUS] = MeshBuilder::GenerateSphere("venus", Color(1, 0, 0), 18, 36);
	meshList[GEO_EARTH] = MeshBuilder::GenerateSphere("earth", Color(0, 1, 0), 18, 36);
	meshList[GEO_MARS] = MeshBuilder::GenerateSphere("mars", Color(0.5f, 0.3f, 0.1f), 18, 36);
	meshList[GEO_JUPITER] = MeshBuilder::GenerateSphere("jupitar", Color(1, 0.6f, 0.4f), 18, 36);
	meshList[GEO_SATURN] = MeshBuilder::GenerateSphere("saturn", Color(0.9f, 0.7f, 0.5f), 18, 36);
	meshList[GEO_URANUS] = MeshBuilder::GenerateSphere("uranus", Color(0.7f, 0.8f, 0.9f), 18, 36);
	meshList[GEO_NEPTUNE] = MeshBuilder::GenerateSphere("neptune", Color(0, 0, 1), 18, 36);
	meshList[GEO_PLUTO] = MeshBuilder::GenerateSphere("pluto", Color(0.5, 0.5, 0.5), 18, 36);
	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene4::Update(double dt)
{
	camera.Update(dt);
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	rotateAngle += (float)(10 * dt);
	planet1RevAngle += (float)(100 * dt);
	planet1RotAngle += (float)(10 * dt);
	moon1RotAngle += (float)(100 * dt);

	mercuryRevAngle += (float)(90 * dt);
	mercuryRotAngle += (float)(10 * dt);

	venusRevAngle += (float)(80 * dt);
	venusRotAngle += (float)(10 * dt);

	earthRevAngle += (float)(70 * dt);
	earthRotAngle += (float)(10 * dt);

	marsRevAngle += (float)(60 * dt);
	marsRotAngle += (float)(10 * dt);

	jupiterRevAngle += (float)(50 * dt);
	jupiterRotAngle += (float)(10 * dt);

	saturnRevAngle += (float)(40 * dt);
	saturnRotAngle += (float)(10 * dt);

	uranusRevAngle += (float)(30 * dt);
	uranusRotAngle += (float)(10 * dt);

	neptuneRevAngle += (float)(20 * dt);
	neptuneRotAngle += (float)(10 * dt);

	plutoRevAngle += (float)(10 * dt);
	plutoRotAngle += (float)(10 * dt);
}

void Scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 translate, rotate, scale;
	Mtx44 MVP;

	//These will be replaced by matrix stack soon
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;

	//Set all matrices to identity
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	//Set projection matrix to perspective mode
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near plane, Far plane

	modelStack.PushMatrix();
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();
	modelStack.PopMatrix();

	//rotate.SetToRotation(rotateAngle, 0, 0, 1);
	//model = rotate;
	/*MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE]->Render();*/
	//modelStack.PushMatrix();
	//modelStack.Rotate(rotateAngle,0,1,0);
	//modelStack.Translate(10, 0, 0);
	//modelStack.Rotate(20 * rotateAngle, 0, 1, 0);
	////modelStack.Scale(5, 5, 5);
	//MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	//meshList[GEO_PLANET1]->Render();
	//modelStack.PopMatrix();
	//modelStack.PushMatrix();

	//modelStack.Rotate(planet1RevAngle, 0, 1, 0);
	
	modelStack.PushMatrix();

	modelStack.Rotate(planet1RotAngle, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SUN]->Render();

	modelStack.PopMatrix();
	
	modelStack.PushMatrix();

	//modelStack.Rotate(planet1RevAngle, 1, 0, 0);
	modelStack.Rotate(mercuryRevAngle, 0, 1, 0);
	modelStack.Translate(4, 0, 0);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(mercuryRotAngle, 1, 0, 0);
	//modelStack.Translate(5, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MERCURY]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(venusRevAngle, 0, 1, 0);
	modelStack.Translate(8, 0, 0);
	modelStack.Scale(1.2f, 1.2f, 1.2f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(venusRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_VENUS]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	/*modelStack.PushMatrix();

	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(moon1RotAngle, 1, 0, 0);
	modelStack.Translate(3, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MOON1]->Render();

	modelStack.PopMatrix();*/

	modelStack.Rotate(earthRevAngle, 0, 1, 0);
	modelStack.Translate(12, 0, 0);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(earthRotAngle, 1, 0, 0);
	modelStack.Scale(1.3f, 1.3f, 1.3f);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_EARTH]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(marsRevAngle, 0, 1, 0);
	modelStack.Translate(16, 0, 0);
	modelStack.Scale(1.1f, 1.1f, 1.1f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(marsRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MARS]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(jupiterRevAngle, 0, 1, 0);
	modelStack.Translate(20, 0, 0);
	modelStack.Scale(1.9f, 1.9f, 1.9f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(jupiterRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_JUPITER]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(saturnRevAngle, 0, 1, 0);
	modelStack.Translate(24, 0, 0);
	modelStack.Scale(1.8f, 1.8f, 1.8f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(saturnRotAngle, 1, 0, 0);

	modelStack.PushMatrix();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SATURN]->Render();

	modelStack.Scale(1.5f, 1.5f, 1.5f);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_RING1]->Render();

	modelStack.Rotate(180, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_RING1]->Render();
	//modelStack.PushMatrix();

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	

	modelStack.PushMatrix();

	modelStack.Rotate(uranusRevAngle, 0, 1, 0);
	modelStack.Translate(28, 0, 0);
	modelStack.Scale(1.4f, 1.4f, 1.4f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(uranusRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_URANUS]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(neptuneRevAngle, 0, 1, 0);
	modelStack.Translate(32, 0, 0);
	modelStack.Scale(1.3f, 1.3f, 1.3f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(neptuneRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_NEPTUNE]->Render();

	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Rotate(plutoRevAngle, 0, 1, 0);
	modelStack.Translate(36, 0, 0);
	//modelStack.Scale(1.3f, 1.3f, 1.3f);
	modelStack.Rotate(30, 1, 0, 0);
	modelStack.Rotate(plutoRotAngle, 1, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_PLUTO]->Render();

	modelStack.PopMatrix();
	//modelStack.PopMatrix();
	//modelStack.PopMatrix();
}

void Scene4::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
