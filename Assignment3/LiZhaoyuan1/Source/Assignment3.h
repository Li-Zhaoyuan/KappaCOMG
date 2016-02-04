#ifndef ASSIGNMENT3_H_
#define ASSIGNMENT3_H_

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "MatrixStack.h"

class Assignment3 : public Scene
{

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_TEXT,
		GEO_SNOWBALLPILE,
		GEO_ROBBY,
		GEO_SNOWWALL,
		GEO_SNOWBARRICADE,
		GEO_SNOWFORT,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

public:
	Assignment3();
	~Assignment3();

	void skybox();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Light light[2];
	bool enableLight = false;
	float LSPEED = 10.f;
	double fps = 0;

	void renderMesh(Mesh *mesh, bool enableLight);
	void lighting();
	void lighting2();
	void RenderSkybox();
	void interactionWithRobby();
	void interactionWithSnowBalls();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	/*float rotateAngle;
	float planet1RevAngle, planet1RotAngle, moon1RotAngle;*/

	Camera3 camera;

	float boxX = 0;
	float boxZ = 0;
	float camPosX = 0;
	float camPosY = 0;
	float camPosz = 0;
	float robbyRotate = 0;
	float robbySnowBallCount = 0;
	float jumpHeight = 0;
	float timer = 0;

	bool talktoRobby = false;
	bool showdialog1 = false;
	bool showdialog2 = false;
	bool interactWithSnowBall = false;
	bool showdialog3 = false;
	bool showdialog4 = false;
	bool showdialog5 = false;
	bool showdialog6 = false;
	bool showdialog7 = false;
	bool showdialog8 = false;
	bool showdialog13= false;
	bool showdialog14= false;
	bool showdialog15= false;
	bool showdialog16= false;
	bool robbyJump = false;
	bool robbyJumpUp = false;
	bool robbyJumpDown = false;
	bool roobyThrow = false;
	bool robbyAquiredSnowBall = false;
	bool throwSnowball = false;
	bool isHit = false;

	int snowBallCount = 0;

	Vector3 boxCoord;
	Vector3 snowBallCoord;
	Vector3 snowBallCoord1;
	Vector3 snowBallCoord2;
	Vector3 snowBallCoord3;
	Vector3 snowBallCoord4;
	Vector3 snowBallCoord5;
	Vector3 snowBallCoord6;
	Vector3 snowWallCoord;
	Vector3 robbyCoord;
	Vector3 snowBarricadeCoord;
	Vector3 tempView;
	Vector3 tempPosition;
	Vector3 ballCoord;

	MS modelStack, viewStack, projectionStack;
};

#endif