#include <sstream>

#include "Assignment3.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Light.h"
#include "Material.h"
#include "Utility.h"

#include "LoadTGA.h"
//#include "LoadOBJ.h"
Assignment3::Assignment3()
{
}
Assignment3::~Assignment3()
{
}
void Assignment3::Init()
{

	//// Init VBO here

	//// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
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
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(-5, 2, -10);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.01f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(2, 2, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.01f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);


	//Initialize camera settings
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	boxCoord = camera.position;
	snowBallCoord.Set(-5,-1,-10) ;
	snowBallCoord1.Set(-5, -1, -10);
	snowBallCoord2.Set(10, -1, -3);
	snowBallCoord5.Set(-10, -1, -4);
	snowBallCoord6.Set(-12, -1, -14);
	snowWallCoord.Set(0,-1,-10);
	robbyCoord.Set(10,-0.45,-10);
	snowBarricadeCoord.Set(9, -1.2f, -10);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1));
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Pikachu2.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1));
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//snowFront.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1));
	meshList[GEO_BACK]->textureID = LoadTGA("Image//snowBack.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1));
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//blueSnow.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOTTOM]->material.kShininess = 5.f;

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1));
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//snowLeft.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1));
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//snowRight.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1));
	meshList[GEO_TOP]->textureID = LoadTGA("Image//snowTop.tga");

	meshList[GEO_SNOWWALL] = MeshBuilder::GenerateOBJ("snowwall", "OBJ//snowWall.obj");
	meshList[GEO_SNOWWALL]->textureID = LoadTGA("Image//marbleSnow.tga");
	meshList[GEO_SNOWWALL]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWWALL]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWWALL]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWWALL]->material.kShininess = 5.f;
	meshList[GEO_ROBBY] = MeshBuilder::GenerateOBJ("robby", "OBJ//robby.obj");
	meshList[GEO_ROBBY]->textureID = LoadTGA("Image//Robby_Texture.tga");
	meshList[GEO_ROBBY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROBBY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROBBY]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROBBY]->material.kShininess = 5.f;
	meshList[GEO_SNOWBARRICADE] = MeshBuilder::GenerateOBJ("snowBarricade", "OBJ//snowBarricade.obj");
	meshList[GEO_SNOWBARRICADE]->textureID = LoadTGA("Image//marbleSnow.tga");
	meshList[GEO_SNOWBARRICADE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBARRICADE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBARRICADE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBARRICADE]->material.kShininess = 5.f;
	meshList[GEO_SNOWBALLPILE] = MeshBuilder::GenerateOBJ("snowballpile", "OBJ//snowBallPile.obj");
	meshList[GEO_SNOWBALLPILE]->textureID = LoadTGA("Image//marbleSnow.tga");
	meshList[GEO_SNOWBALLPILE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBALLPILE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBALLPILE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWBALLPILE]->material.kShininess = 5.f;
	meshList[GEO_SNOWFORT] = MeshBuilder::GenerateOBJ("snowfort", "OBJ//snowFort.obj");
	meshList[GEO_SNOWFORT]->textureID = LoadTGA("Image//marbleSnow.tga");
	meshList[GEO_SNOWFORT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWFORT]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWFORT]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SNOWFORT]->material.kShininess = 5.f;
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 18, 36);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kShininess = 10.f;
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//TimesNewRoman.tga");
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);
}


void Assignment3::Update(double dt)
{
	camera.Update(dt, 15);
	fps = 1 / dt;
	
	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

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
	if (Application::IsKeyPressed('X'))
		enableLight = true;
	if (Application::IsKeyPressed('Z'))
		enableLight = false;
	if (Application::IsKeyPressed('Q') && snowBallCount>0)
	{
		ballCoord = camera.position;
		tempView = (camera.target - camera.position).Normalized();
		throwSnowball = true;
		tempPosition = camera.position;
		snowBallCount -= 1;
	}
	if (throwSnowball == true)
	{
		ballCoord += (tempView)*(float)(100 * dt);
		
	}
	if (robbyCoord.x + 1.f > ballCoord.x
		&& robbyCoord.x - 1.f < ballCoord.x
		&& robbyCoord.z + 1.f > ballCoord.z
		&& robbyCoord.z - 1.f < ballCoord.z
		&& robbyCoord.y + 1.f > ballCoord.y
		&& robbyCoord.y - 1.f < ballCoord.y)
	{
		isHit = true;
	}
	if (timer < 2.f && isHit == true)
	{
		timer += dt;
	}
	if (timer >= 2.f && isHit == true)
	{
		timer = 0.f;
		isHit = false;
	}

	if (boxCoord.x < camera.position.x)
	{
		boxX += (camera.position.x - boxCoord.x ) * (1.2f);
		boxCoord.x = camera.position.x;
	}
	if (boxCoord.x > camera.position.x)
	{
		boxX -= (boxCoord.x - camera.position.x) * (1.2f);
		boxCoord.x = camera.position.x;
	}
	if (boxCoord.z > camera.position.z)
	{
		boxZ -= (boxCoord.z - camera.position.z)* (1.2f);
		boxCoord.z = camera.position.z;
	}
	if (boxCoord.z < camera.position.z)
	{
		boxZ += (camera.position.z - boxCoord.z)* (1.2f);
		boxCoord.z = camera.position.z;
	}
	
	if (snowBallCoord.x + 1.f >camera.position.x
		&& snowBallCoord.x - 1.f < camera.position.x
		&& snowBallCoord.z + 1.f > camera.position.z
		&& snowBallCoord.z - 1.f < camera.position.z
		|| snowBallCoord1.x + 1.f >camera.position.x
		&& snowBallCoord1.x - 1.f < camera.position.x
		&& snowBallCoord1.z + 1.f > camera.position.z
		&& snowBallCoord1.z - 1.f < camera.position.z
		|| snowBallCoord2.x + 1.f >camera.position.x
		&& snowBallCoord2.x - 1.f < camera.position.x
		&& snowBallCoord2.z + 1.f > camera.position.z
		&& snowBallCoord2.z - 1.f < camera.position.z
		|| snowBallCoord5.x + 1.f >camera.position.x
		&& snowBallCoord5.x - 1.f < camera.position.x
		&& snowBallCoord5.z + 1.f > camera.position.z
		&& snowBallCoord5.z - 1.f < camera.position.z
		|| snowBallCoord6.x + 1.f >camera.position.x
		&& snowBallCoord6.x - 1.f < camera.position.x
		&& snowBallCoord6.z + 1.f > camera.position.z
		&& snowBallCoord6.z - 1.f < camera.position.z)
	{
		camera.UpdateCollision(497, dt);
	}
	if (snowWallCoord.x + 2.f >camera.position.x
		&& snowWallCoord.x - 1.f < camera.position.x
		&& snowWallCoord.z + 3.f > camera.position.z
		&& snowWallCoord.z - 3.f < camera.position.z)
	{
		camera.UpdateCollision(497, dt);
	}
	if (snowBarricadeCoord.x + 1.f >camera.position.x
		&& snowBarricadeCoord.x - 1.f < camera.position.x
		&& snowBarricadeCoord.z + 2.5f > camera.position.z
		&& snowBarricadeCoord.z - 2.5f < camera.position.z)
	{
		camera.UpdateCollision(497, dt);
	}
	if (robbyCoord.x + 1.f >camera.position.x
		&& robbyCoord.x - 1.f < camera.position.x
		&& robbyCoord.z + 1.f > camera.position.z
		&& robbyCoord.z - 1.f < camera.position.z)
	{
		camera.UpdateCollision(497, dt);
	}
	/*if (light[0].power < 100)
	{
		light[0].power += 1;
	}
	if (light[0].power >= 100)
	{
		light[0].power = 0;
	}*/
	camPosX = camera.position.x;
	camPosY = camera.position.y;
	camPosz = camera.position.z;

	interactionWithRobby();
	interactionWithSnowBalls();

	
	/*if (robbySnowBallCount > 0)
	{
		if (robbyJump == false)
		{
			robbyJumpUp = true;
		}
	}
	if (robbyJumpUp == true && robbyCoord.y < 2.45f)
	{
		jumpHeight = 1.f;
		robbyCoord.y += jumpHeight;
	}
	if (robbyJumpUp == true && jumpHeight >= 2.45f)
	{
		robbyJump = true;
		robbyJumpUp = false;
		robbyJumpDown = true;
	}
	if (robbyJumpDown == true && robbyCoord.y >= -0.45f)
	{
		robbyCoord.y -= jumpHeight;
	}*/
	//if (robbyJumpDown == true && jumpHeight <= 0.f)
	//{
	//	//robbyJump = false;
	//	robbyJumpDown = false;
	//}
	
}
void Assignment3::lighting()
{
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top()* light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_POINT)
	{

		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
}
void Assignment3::lighting2()
{
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top()* light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_POINT)
	{

		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

}
void Assignment3::renderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	/*if (enableLight)
	{
	this->enableLight = enableLight;
	}*/
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
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	}

}

void Assignment3::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Assignment3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}


void Assignment3::Render()
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
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	lighting();
	lighting2();
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	renderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();
	

	/*modelStack.PushMatrix();
	renderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(boxX, 496.9, boxZ);
	RenderSkybox();
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(snowWallCoord.x, 0, snowWallCoord.z);
	modelStack.Rotate(-90, 0, 1, 0);
	//modelStack.Scale(10, 10, 10);
	renderMesh(meshList[GEO_SNOWWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(snowBallCoord.x, snowBallCoord.y, snowBallCoord.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	renderMesh(meshList[GEO_SNOWBALLPILE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(snowBallCoord1.x, snowBallCoord1.y, snowBallCoord1.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	renderMesh(meshList[GEO_SNOWBALLPILE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(snowBallCoord2.x, snowBallCoord2.y, snowBallCoord2.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	renderMesh(meshList[GEO_SNOWBALLPILE], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(snowBallCoord5.x, snowBallCoord5.y, snowBallCoord5.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	renderMesh(meshList[GEO_SNOWBALLPILE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(snowBallCoord6.x, snowBallCoord6.y, snowBallCoord6.z);
	modelStack.Scale(0.2, 0.2, 0.2);
	renderMesh(meshList[GEO_SNOWBALLPILE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(robbyCoord.x, robbyCoord.y, robbyCoord.z);
	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(robbyRotate, 0, 1, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	renderMesh(meshList[GEO_ROBBY], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, -1, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1500, 1500, 1500);
	renderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(snowBarricadeCoord.x, snowBarricadeCoord.y, snowBarricadeCoord.z);
	renderMesh(meshList[GEO_SNOWBARRICADE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(snowBarricadeCoord.x, snowBarricadeCoord.y, snowBarricadeCoord.z);
	modelStack.Scale(7.5, 7.5, 7.5);
	renderMesh(meshList[GEO_SNOWFORT], true);
	modelStack.PopMatrix();

	if (throwSnowball == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(ballCoord.x, ballCoord.y, ballCoord.z);
		modelStack.Scale(0.2, 0.2, 0.2);
		renderMesh(meshList[GEO_SPHERE], true);
		modelStack.PopMatrix();
	}
	

	std::stringstream playerPos;
	playerPos << "X = " << camPosX << " Y = " << camPosY << " Z = " << camPosz;
	//RenderTextOnScreen(meshList[GEO_TEXT], playerPos.str(), Color(1, 0, 0), 2, 0, 18);
	std::stringstream ss;
	ss << "FPS:" << fps<<"         "<<playerPos.str();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 19);
	std::stringstream snowballcount;
	snowballcount << "Snow Ball Count: " << snowBallCount;
	RenderTextOnScreen(meshList[GEO_TEXT], snowballcount.str(), Color(0, 1, 0), 3, 0, 1);
	
	if (showdialog1 == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Hi Pingu, can you get me 1 snowball?", Color(1, 0, 1), 2, 0, 15);
	}
	if (showdialog2 == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Thank You!", Color(1, 0, 1), 2, 0, 15);
	}
	if (showdialog3 == true || showdialog5 == true || showdialog7 == true || showdialog13 == true || showdialog15 == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Gained 1 Snow Ball, Max capacity", Color(1, 0, 1), 2, 0, 15);
	}
	if (showdialog4 == true || showdialog6 == true || showdialog8 == true || showdialog14 == true || showdialog16 == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "You can only hold 1 Snow Ball!", Color(1, 0, 1), 2, 0, 15);
	}
	if (isHit == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "NICE SHOT", Color(1, 0, 1), 3, 0, 18);
	}
}

void Assignment3::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -498);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 498, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-498, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(498, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -498, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 498);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	renderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void Assignment3::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


void Assignment3::interactionWithRobby()
{
	if (robbyCoord.x + 3 >camera.position.x
		&& robbyCoord.x - 3 < camera.position.x
		&& robbyCoord.z + 3 > camera.position.z
		&& robbyCoord.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		robbyRotate = camera.rotateChar + 90;
		talktoRobby = true;
	}
	if (robbyCoord.x + 3 > camera.position.x
		&& robbyCoord.x - 3 < camera.position.x
		&& robbyCoord.z + 3 > camera.position.z
		&& robbyCoord.z - 3 < camera.position.z
		&& talktoRobby == true
		&& snowBallCount < 1)
	{
		if (showdialog2 != true)
		{
			showdialog1 = true;
		}
	}
	/*else
	{
	showdialog1 = false;
	robbyRotate = 0;
	talktoRobby = false;
	}*/
	if (robbyCoord.x + 3 > camera.position.x
		&& robbyCoord.x - 3 < camera.position.x
		&& robbyCoord.z + 3 > camera.position.z
		&& robbyCoord.z - 3 < camera.position.z
		&& talktoRobby == true
		&& snowBallCount == 1)
	{
		showdialog2 = true;
		showdialog1 = false;
		snowBallCount -= 1;
		robbySnowBallCount += 1;
	}
	if (robbyCoord.x + 3 < camera.position.x
		|| robbyCoord.x - 3 > camera.position.x
		|| robbyCoord.z + 3 < camera.position.z
		|| robbyCoord.z - 3 > camera.position.z)
	{
		showdialog1 = false;
		robbyRotate = 0;
		talktoRobby = false;
		showdialog2 = false;
	}
}

void Assignment3::interactionWithSnowBalls()
{
	if (snowBallCoord.x + 3 >camera.position.x
		&& snowBallCoord.x - 3 < camera.position.x
		&& snowBallCoord.z + 3 > camera.position.z
		&& snowBallCoord.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		if (snowBallCount<1)
		{
			showdialog3 = true;
		}
		snowBallCount = 1;
		if (showdialog3 == false)
		{
			showdialog4 = true;
		}
		interactWithSnowBall = true;
	}

	if (snowBallCoord1.x + 3 >camera.position.x
		&& snowBallCoord1.x - 3 < camera.position.x
		&& snowBallCoord1.z + 3 > camera.position.z
		&& snowBallCoord1.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		if (snowBallCount<1)
		{
			showdialog5 = true;
		}
		snowBallCount = 1;
		if (showdialog5 == false)
		{
			showdialog6 = true;
		}
		interactWithSnowBall = true;
	}

	if (snowBallCoord2.x + 3 >camera.position.x
		&& snowBallCoord2.x - 3 < camera.position.x
		&& snowBallCoord2.z + 3 > camera.position.z
		&& snowBallCoord2.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		if (snowBallCount<1)
		{
			showdialog7 = true;
		}
		snowBallCount = 1;
		if (showdialog7 == false)
		{
			showdialog8 = true;
		}
		interactWithSnowBall = true;
	}

	

	if (snowBallCoord5.x + 3 >camera.position.x
		&& snowBallCoord5.x - 3 < camera.position.x
		&& snowBallCoord5.z + 3 > camera.position.z
		&& snowBallCoord5.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		if (snowBallCount<1)
		{
			showdialog13 = true;
		}
		snowBallCount = 1;
		if (showdialog13 == false)
		{
			showdialog14 = true;
		}
		interactWithSnowBall = true;
	}

	if (snowBallCoord6.x + 3 >camera.position.x
		&& snowBallCoord6.x - 3 < camera.position.x
		&& snowBallCoord6.z + 3 > camera.position.z
		&& snowBallCoord6.z - 3 < camera.position.z
		&& Application::IsKeyPressed('E'))
	{
		if (snowBallCount<1)
		{
			showdialog15 = true;
		}
		snowBallCount = 1;
		if (showdialog15 == false)
		{
			showdialog16 = true;
		}
		interactWithSnowBall = true;
	}

	if (snowBallCoord.x + 3 < camera.position.x
		|| snowBallCoord.x - 3 > camera.position.x
		|| snowBallCoord.z + 3 < camera.position.z
		|| snowBallCoord.z - 3 > camera.position.z
		)
	{
		showdialog3 = false;
		showdialog4 = false;
		interactWithSnowBall = false;
	}

	if (snowBallCoord1.x + 3 < camera.position.x
		|| snowBallCoord1.x - 3 > camera.position.x
		|| snowBallCoord1.z + 3 < camera.position.z
		|| snowBallCoord1.z - 3 > camera.position.z
		)
	{
		showdialog5 = false;
		showdialog6 = false;
		interactWithSnowBall = false;
	}
	if (snowBallCoord2.x + 3 < camera.position.x
		|| snowBallCoord2.x - 3 > camera.position.x
		|| snowBallCoord2.z + 3 < camera.position.z
		|| snowBallCoord2.z - 3 > camera.position.z
		)
	{
		showdialog7 = false;
		showdialog8 = false;
		interactWithSnowBall = false;
	}

	

	if (snowBallCoord5.x + 3 < camera.position.x
		|| snowBallCoord5.x - 3 > camera.position.x
		|| snowBallCoord5.z + 3 < camera.position.z
		|| snowBallCoord5.z - 3 > camera.position.z
		)
	{
		showdialog13= false;
		showdialog14= false;
		interactWithSnowBall = false;
	}

	if (snowBallCoord6.x + 3 < camera.position.x
		|| snowBallCoord6.x - 3 > camera.position.x
		|| snowBallCoord6.z + 3 < camera.position.z
		|| snowBallCoord6.z - 3 > camera.position.z
		)
	{
		showdialog15= false;
		showdialog16= false;
		interactWithSnowBall = false;
	}
}