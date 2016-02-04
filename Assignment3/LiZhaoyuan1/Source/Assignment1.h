#ifndef ASSIGNMENT_1_H
#define ASSIGNMENT_1_H

#include "Scene.h"

class Assignment1 : public Scene
{
	enum GEOMETRY_TYPE
	{
		LANDSCAPE_1 = 0,
		CLOUDS_2,
		SUN_3,
		EXPLOSION_4,
		CACTUS_5,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
public:
	Assignment1();
	~Assignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//init
	void landScape();
	void clouds();
	void sun();
	void explosion();
	void cactus();

	float rotateAngle;
	float translateX;
	float scaleAll;
	float flyBack;
	float cloudMovement;
	float explosionRotation;
	float explosionSize;
	float sizeValue;

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;


};

#endif