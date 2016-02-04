#ifndef ASSIGNMENT_2_H_
#define ASSIGNMENT_2_H_

#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Camera2.h"
#include "MatrixStack.h"

class Assignment2 : public Scene
{

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_HEMISHERE,
		GEO_BODY,
		GEO_BELLY,
		GEO_BEAK,
		GEO_BEAK_NOOT,
		GEO_RETINA,
		GEO_PUPIL,
		GEO_LEG,
		GEO_FEET,
		GEO_IGLOO,
		GEO_IGLOODOOR,
		GEO_SNOWPILE,
		GEO_SNOWHILL,
		GEO_ICICLES,
		GEO_ICEPOND,
		GEO_LIGHTBALL,
		GEO_SLED,
		GEO_SLEDFRONT,
		GEO_SNOWRAMP,
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
		U_TOTAL,
	};

public:
	Assignment2();
	~Assignment2();

	void pingu();
	void axes();
	void floor();
	void igloo();
	void snowhill();
	void sled();
	void snowpile();
	void snowramp();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool isWaving;
	bool waveCheck;
	bool isTilting;
	bool tiltCheck;
	bool isNoot;
	bool nootCheck;

	bool isWalking;
	bool isWalking2;
	bool isleg1Move;
	bool isleg2Move;
	bool ismoveLegs;

	bool pinguFocusview;

	bool sledAnimation;

	float nootTranslation;
	float nootValue;

	float tiltRotation;
	float tiltValue;

	float waveRotation;
	float waveValue;

	float turningPingu;
	float movingPinguX;
	float movingPinguZ;
	
	float moveleg1;
	float moveleg2;
	float leg1Value;
	float leg2Value;

	float sledTranslation;
	float sledTranslation2;
	float sledRotation;
	float sledElevation;

	int waveCount;
	int turnCount;

	

private:
	Light light[1];
	float LSPEED = 10.f;
	void renderMesh(Mesh *mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];


	Camera2 camera;

	MS modelStack, viewStack, projectionStack;
};

#endif