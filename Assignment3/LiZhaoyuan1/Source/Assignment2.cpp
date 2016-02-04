#include "Assignment2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Light.h"
#include "Material.h"
#include "Utility.h"
Assignment2::Assignment2()
{
}
Assignment2::~Assignment2()
{
}
void Assignment2::Init()
{

	//// Init VBO here

	//// Set background color to dark blue
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

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
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//Shading.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	glUseProgram(m_programID);

	light[0].position.Set(0, 10, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.01f;

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

	isWaving = false;
	waveCheck = false;
	isTilting = false;
	isNoot = false;
	isWalking = false;
	isWalking2 = false;
	isleg1Move = false;
	isleg2Move = false;
	pinguFocusview = false;
	sledAnimation = false;
	tiltRotation = 0.f;
	waveRotation = 75.f;
	nootTranslation = 0.8f;
	tiltValue = 0.f;
	waveValue = 0.f;
	nootValue = 0.f;
	waveCount = 0;
	turnCount = 0;

	turningPingu = 0.f;
	movingPinguX = 0.f;
	movingPinguZ = 0.f;
	moveleg1 = 0.f;
	moveleg2 = 0.f;
	leg1Value = 0.f;
	leg2Value = 0.f;


	sledTranslation = 0.f;
	sledTranslation2 = 0.f;
	sledRotation = 0.f;
	sledElevation = 0.f;

	//Initialize camera settings
	camera.Init(Vector3(20, 16, 16), Vector3(3, 9, 3), Vector3(0, 1, 0));


	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1));
	meshList[GEO_QUAD]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.5f, 0.1f, 0.5f);
	meshList[GEO_QUAD]->material.kShininess = 10.f;
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 0), 36);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 0), 0.3f, 36);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kShininess = 10.f;
	meshList[GEO_BODY] = MeshBuilder::GenerateSphere("body", Color(0, 0, 0), 18, 36);
	meshList[GEO_BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BODY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BODY]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BODY]->material.kShininess = 10.f;
	meshList[GEO_BEAK] = MeshBuilder::GenerateSphere("beak", Color(1, 0, 0), 18, 36);
	meshList[GEO_BEAK]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BEAK]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BEAK]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BEAK]->material.kShininess = 10.f;
	meshList[GEO_BEAK_NOOT] = MeshBuilder::GenerateCylinder("beak_noot", Color(1, 0, 0), 36);
	meshList[GEO_BEAK_NOOT]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BEAK_NOOT]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BEAK_NOOT]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BEAK_NOOT]->material.kShininess = 10.f;
	meshList[GEO_RETINA] = MeshBuilder::GenerateSphere("retina", Color(1, 1, 1), 18, 36);
	meshList[GEO_RETINA]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_RETINA]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RETINA]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RETINA]->material.kShininess = 10.f;
	meshList[GEO_PUPIL] = MeshBuilder::GenerateSphere("pupil", Color(0, 0, 0), 18, 36);
	meshList[GEO_PUPIL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_PUPIL]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PUPIL]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PUPIL]->material.kShininess = 10.f;
	meshList[GEO_BELLY] = MeshBuilder::GenerateSphere("belly", Color(1, 1, 1), 18, 36);
	meshList[GEO_BELLY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BELLY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BELLY]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BELLY]->material.kShininess = 10.f;
	meshList[GEO_LEG] = MeshBuilder::GenerateSphere("leg", Color(1, 0.5, 0), 18, 36);
	meshList[GEO_LEG]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_LEG]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_LEG]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_LEG]->material.kShininess = 10.f;
	meshList[GEO_FEET] = MeshBuilder::GenerateCylinder("feet", Color(1, 0.5, 0), 36);
	meshList[GEO_FEET]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_FEET]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FEET]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FEET]->material.kShininess = 10.f;
	meshList[GEO_IGLOO] = MeshBuilder::GenerateHemiSphere("igloo", Color(1, 1, 1), 18, 36);
	meshList[GEO_IGLOO]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_IGLOO]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_IGLOO]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_IGLOO]->material.kShininess = 10.f;
	meshList[GEO_IGLOODOOR] = MeshBuilder::GenerateCylinder("igloodoor", Color(1, 1, 1), 36);
	meshList[GEO_IGLOODOOR]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_IGLOODOOR]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_IGLOODOOR]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_IGLOODOOR]->material.kShininess = 10.f;
	meshList[GEO_SNOWPILE] = MeshBuilder::GenerateSphere("snowpile", Color(1, 1, 1), 18, 36);
	meshList[GEO_SNOWPILE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SNOWPILE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWPILE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWPILE]->material.kShininess = 10.f;
	meshList[GEO_SNOWHILL] = MeshBuilder::GenerateCube("snow hill", Color(1, 1, 1));
	meshList[GEO_SNOWHILL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SNOWHILL]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWHILL]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWHILL]->material.kShininess = 10.f;
	meshList[GEO_SNOWRAMP] = MeshBuilder::GenerateCube("snowramp", Color(1, 1, 1));
	meshList[GEO_SNOWRAMP]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SNOWRAMP]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWRAMP]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWRAMP]->material.kShininess = 10.f;
	meshList[GEO_ICEPOND] = MeshBuilder::GenerateCircle("pond", Color(0, 0.7, 1), 36);
	meshList[GEO_ICEPOND]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_ICEPOND]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_ICEPOND]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ICEPOND]->material.kShininess = 40.f;
	meshList[GEO_ICICLES] = MeshBuilder::GenerateCone("icicle", Color(0.1, 0.7, 1), 36);
	meshList[GEO_ICICLES]->material.kAmbient.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_ICICLES]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_ICICLES]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ICICLES]->material.kShininess = 100.f;
	meshList[GEO_SLED] = MeshBuilder::GenerateCube("sled", Color(0.5, 0.3, 0.1));
	meshList[GEO_SLED]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SLED]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SLED]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SLED]->material.kShininess = 10.f;
	meshList[GEO_SLEDFRONT] = MeshBuilder::GenerateCylinder("sledfront", Color(0.5, 0.3, 0.1), 36);
	meshList[GEO_SLEDFRONT]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_SLEDFRONT]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SLEDFRONT]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SLEDFRONT]->material.kShininess = 10.f;
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36);
	

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

void Assignment2::Update(double dt)
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

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('Q') && isWaving == false && isTilting == false && isNoot == false)
	{
		isWaving = true;
		isTilting = true;
		isNoot = true;
		tiltCheck = false;
	}
	
	if (isWaving == true)
	{
		if (waveValue > -160 && waveCheck == false)
		{
			waveRotation -= (float)(200 * dt);
			waveValue -= (float)(200 * dt);
		}
		else if (waveValue <= -160 && waveCheck == false)
		{
			waveCheck = true;
		}
		else if (waveCheck = true && waveValue <= -120)
		{
			waveRotation += (float)(200 * dt);
			waveValue += (float)(200 * dt);
		}
		else if (waveCheck = true && waveValue >= -120)
		{
			waveCheck = false;
			waveCount += 1;
		}
	}
	if (isTilting == true)
	{
		if (tiltValue > -15 && tiltCheck == false)
		{
			tiltRotation -= (float)(150 * dt);
			tiltValue -= (float)(150 * dt);
		}
		if (tiltValue <= -15 && tiltCheck == false)
		{
			tiltCheck = true;
		}
	}
	if (isNoot == true)
	{
		if (nootValue < 1 && nootCheck == false)
		{
			nootTranslation += (float)(10 * dt);
			nootValue += (float)(10 * dt);
		}
		else if (nootValue >= 1 && nootCheck == false)
		{
			nootCheck = true;
		}
		else if (nootCheck = true && nootValue >= -0.5)
		{
			nootTranslation -= (float)(10 * dt);
			nootValue -= (float)(10 * dt);
		}
		else if (nootCheck = true && nootValue <= -0.5)
		{
			nootCheck = false;
		}
	}
	if (waveCount == 2)
	{
		waveRotation = 75;
		waveValue = 0;
		waveCount = 0;
		tiltValue = 0;
		tiltRotation = 0;
		nootValue = 0;
		nootTranslation = 0.8f;
		isWaving = false;
		isTilting = false;
		isNoot = false;
	}
	if (Application::IsKeyPressed('W') && sledAnimation == false)
	{
		movingPinguX += (float)(sin(Math::DegreeToRadian(turningPingu)))*(15 * dt);
		movingPinguZ += (float)(cos(Math::DegreeToRadian(turningPingu)))*(15 * dt);
		isWalking = true;
	}
	
	else if (Application::IsKeyPressed('S')&& sledAnimation == false)
	{
		movingPinguX -= (float)(sin(Math::DegreeToRadian(turningPingu)))*(15 * dt);
		movingPinguZ -= (float)(cos(Math::DegreeToRadian(turningPingu)))*(15 * dt);
		isWalking = true;
	}
	else
	{
		isWalking = false;
	}
	if (Application::IsKeyPressed('D') && sledAnimation == false)
	{
		turningPingu -= (float)(500 * dt);
		isWalking2 = true;
	}
	else  if (Application::IsKeyPressed('A') && sledAnimation == false)
	{
		turningPingu += (float)(500 * dt);
		isWalking2 = true;
	}
	 else
	 {
		 isWalking2 = false;
	 }
	
	if (isWalking == true || isWalking2 == true)
	{
		if (moveleg1 < 45 && isleg2Move == false)
		{
			moveleg1 += (float)(200 * dt);
			leg1Value += (float)(200 * dt);
		}
		else if (leg1Value >= 45 && isleg2Move == false)
		{
			isleg2Move = true;
			moveleg1 = 0;
			leg1Value = 0;
		}
		else if (isleg2Move == true && leg2Value < 45)
		{
			moveleg2 += (float)(200 * dt);
			leg2Value += (float)(200 * dt);
		}
		else if (isleg2Move == true && leg2Value >= 45)
		{
			isleg2Move = false;
			moveleg2 = 0;
			leg2Value = 0;

		}
	}
	

	if (isWalking == false)
	{
		isleg1Move = false;
		isleg2Move = false;
		moveleg1 = 0;
		moveleg2 = 0;
	}
	
	if (Application::IsKeyPressed('R'))
	{
		movingPinguX = 0;
		movingPinguZ = 0;
		turningPingu = 0;
		light[0].position.x = 0;
		light[0].position.y = 10;
		light[0].position.z = 0;
	}

	if (Application::IsKeyPressed('F'))
	{
		pinguFocusview = false;
		camera.Init(Vector3(20, 16, 16), Vector3(3, 9, 3), Vector3(0, 1, 0));
	}
	else if (Application::IsKeyPressed('G'))
	{
		pinguFocusview = true;
	}

	if (pinguFocusview == true)
	{
		camera.Init(Vector3(movingPinguX, 25, movingPinguZ + 1), Vector3(movingPinguX, 1, movingPinguZ), Vector3(0, 1, 0));
	}
	if (Application::IsKeyPressed('Z'))
	{
		sledAnimation = true;
	}
	if (sledAnimation == true)
	{
		camera.Init(Vector3(sledTranslation, sledTranslation2+15, 16), Vector3(sledTranslation, 1, 3), Vector3(0, 1, 0));
		if (sledTranslation <= 10 && sledRotation == 0)
		{
			sledTranslation += (float)(10 * dt);
		}
		else if (sledTranslation > 10 && sledRotation <=900)
		{
			sledRotation += (float)(1000 * dt);
		}
		else if (sledRotation > 900 && sledTranslation > -100)
		{
			sledTranslation -= (float)(100 * dt);
			if (sledTranslation <= -40)
			{
				sledElevation = -45;
				sledTranslation2 += (float)(300 * dt);
			}
		}
		else
		{
			sledAnimation = false;
			camera.Init(Vector3(20, 16, 16), Vector3(3, 9, 3), Vector3(0, 1, 0));
			sledTranslation = 0;
			sledRotation = 0;
			sledElevation = 0;
			sledTranslation2 = 0;
		}
	}
}

void Assignment2::renderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	mesh->Render();
}

void Assignment2::Render()
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



	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	renderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	axes();
	floor();
	modelStack.PushMatrix();
	if (sledAnimation == true)
	{
		modelStack.Translate(sledTranslation, sledTranslation2, 0);
		modelStack.Translate(0, 1, 0);
		modelStack.Rotate(sledElevation, 0, 0, 1);
		modelStack.Rotate(sledRotation, 0, 1, 0);

	}
	if (sledAnimation == false)
	{
		modelStack.Translate(movingPinguX, 0, movingPinguZ);
	}
	pingu();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-15, 0, -15);
	modelStack.Rotate(-90, 1, 0, 0);
	igloo();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 20);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	snowpile();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(20, 0, -10);
	modelStack.Rotate(-45, 0, 1, 0);
	snowhill();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, 0);
	modelStack.Rotate(45, 0, 0, 1);
	snowramp();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	if (sledAnimation == true)
	{
		modelStack.Translate(sledTranslation, sledTranslation2, 0);
		modelStack.Rotate(sledElevation, 0, 0, 1);
		modelStack.Rotate(sledRotation, 0, 1, 0);
	}
	if (sledAnimation == false)
	{
		modelStack.Translate(0, 0, -5);
	}
	sled();
	modelStack.PopMatrix();
}

void Assignment2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Assignment2::pingu()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.1, 0);//move whole pingu
	if (sledAnimation == false)
	{
		modelStack.Rotate(turningPingu, 0, 1, 0);
	}
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Translate(0, 5, 0);
	modelStack.Rotate(tiltRotation, 1, 0, 0);
	modelStack.Translate(0, -5, 0);
	modelStack.PushMatrix();
	modelStack.Translate(0.65, 5, 0.45);
	modelStack.Scale(0.25, 0.25, 0.25);
	renderMesh(meshList[GEO_RETINA], true);//eye
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.65, 5, -0.45);
	modelStack.Scale(0.25, 0.25, 0.25);
	renderMesh(meshList[GEO_RETINA], true);//eye
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.8, 5.025, -0.55);
	modelStack.Scale(0.13, 0.13, 0.13);
	renderMesh(meshList[GEO_PUPIL], true);//eye
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.8, 5.025, 0.55);
	modelStack.Scale(0.13, 0.13, 0.13);
	renderMesh(meshList[GEO_PUPIL], true);//eye
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, 4.85, 0);
	modelStack.Scale(1, 0.85, 1);
	modelStack.Translate(1, 0, 0);
	renderMesh(meshList[GEO_BODY], true);//head
	modelStack.PopMatrix();
	if (isNoot == false)
	{
		modelStack.PushMatrix();
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.PushMatrix();
		modelStack.Translate(0.8, 9.45, 0);
		modelStack.Scale(2, 0.45, 0.85);
		renderMesh(meshList[GEO_BEAK], true);//beak
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else if (isNoot == true)
	{
		modelStack.PushMatrix();
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.PushMatrix();
		modelStack.Translate(nootTranslation, 9.45, 0);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Scale(0.55, 4, 0.55);
		renderMesh(meshList[GEO_BEAK_NOOT], true);//beak("noot" mode)
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2, 0);
	modelStack.Scale(1.2, 2.35, 1.75);
	renderMesh(meshList[GEO_BODY], true);//body
	modelStack.PopMatrix();
	

	modelStack.PushMatrix();
	modelStack.Translate(0.18, 1.9, 0);
	modelStack.Scale(1.25, 2.25, 1.5);
	renderMesh(meshList[GEO_BELLY], true);//belly
	modelStack.PopMatrix();
	
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 3.5, 1.3);
	modelStack.Rotate(waveRotation, 1, 0, 0);
	modelStack.Translate(0, 0, 2);
	modelStack.Scale(0.75, 0.15, 2);
	renderMesh(meshList[GEO_BODY], true);//right wing
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 3.5, -1.3);
	modelStack.Rotate(-75, 1, 0, 0);
	modelStack.Translate(0, 0, -2);
	modelStack.Scale(0.75, 0.15, 2);
	renderMesh(meshList[GEO_BODY], true);//left wing
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);

	modelStack.PushMatrix();
	modelStack.Rotate(moveleg1, 0, 0, 1);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 1);
	modelStack.Scale(0.15, 0.55, 0.15);
	modelStack.Translate(1, -1.5, 0);
	renderMesh(meshList[GEO_LEG], true);//right leg

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(moveleg2, 0, 0, 1);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1);
	modelStack.Scale(0.15, 0.55, 0.15);
	modelStack.Translate(1, -1.5, 0);
	renderMesh(meshList[GEO_LEG], true);//left leg

	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(moveleg2, 0, 0, 1);

	modelStack.PushMatrix();
	modelStack.Rotate(moveleg1, 0, 0, 1);
	modelStack.Translate(1.5, -1, 1);
	modelStack.Scale(1.5, 0.15, 0.75);
	renderMesh(meshList[GEO_FEET], true);//right feet

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(moveleg2, 0, 0, 1);
	modelStack.Translate(1.5, -1, -1);
	modelStack.Scale(1.5, 0.15, 0.75);
	renderMesh(meshList[GEO_FEET], true);//left feet
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Assignment2::floor()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -0.1, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();
}

void Assignment2::igloo()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(10, 10, 10);
	renderMesh(meshList[GEO_IGLOO], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(6, 5, 5);
	modelStack.Translate(2,0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	renderMesh(meshList[GEO_IGLOODOOR], true);
	modelStack.PopMatrix();
}

void Assignment2::snowpile()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(10, 10, 10);
	renderMesh(meshList[GEO_SNOWPILE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(9, 0, 0);
	modelStack.Scale(5, 5, 5);
	renderMesh(meshList[GEO_SNOWPILE], true);
	modelStack.PopMatrix();
}

void Assignment2::axes()
{
	modelStack.PushMatrix();
	renderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();
}

void Assignment2::snowhill()
{
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(25, 1, 0, 0);
	modelStack.Scale(12, 23, 6);
	renderMesh(meshList[GEO_SNOWHILL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 10.4, 6);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(12, 10, 3);
	renderMesh(meshList[GEO_SNOWHILL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0.1, 0.1);
	modelStack.Scale(15, 10, 15);
	renderMesh(meshList[GEO_ICEPOND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.5, 7, 10);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 5.5, 0.5);
	renderMesh(meshList[GEO_ICICLES], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2, 7, 10);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 5.5, 0.5);
	renderMesh(meshList[GEO_ICICLES], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, 7, 9);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 5.5, 0.5);
	renderMesh(meshList[GEO_ICICLES], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1.5, 6.5, 8.5);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 6, 0.5);
	renderMesh(meshList[GEO_ICICLES], true);
	modelStack.PopMatrix();
}

void Assignment2::sled()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1.5);
	modelStack.Scale(4, 0.5, 0.5);
	renderMesh(meshList[GEO_SLED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 1.5);
	modelStack.Scale(4, 0.5, 0.5);
	renderMesh(meshList[GEO_SLED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1, 0.3, 0);
	modelStack.Scale(0.5, 0.5, 3.5);
	renderMesh(meshList[GEO_SLED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, 0.3, 0);
	modelStack.Scale(0.5, 0.5, 3.5);
	renderMesh(meshList[GEO_SLED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, 0.8, 0);
	modelStack.Scale(5, 0.5, 3.5);
	renderMesh(meshList[GEO_SLED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1, 0.5, 0);
	modelStack.Scale(1.8, 0.5, 1.8);
	renderMesh(meshList[GEO_SLEDFRONT], true);
	modelStack.PopMatrix();
}

void Assignment2::snowramp()
{
	modelStack.PushMatrix();
	modelStack.Scale(6, 23, 12);
	renderMesh(meshList[GEO_SNOWRAMP], true);
	modelStack.PopMatrix();
}