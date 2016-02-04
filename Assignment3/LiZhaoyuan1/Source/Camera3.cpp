#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"



Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	translateChar = 0;
	rotateChar = 0;
	
	SetCursorPos( 800 / 2, 600 / 2);
}

void Camera3::Update(double dt, float bounds)
{
	POINT mousecursor;
	static const float CAMERA_SPEED = 25.f;
	
	GetCursorPos(&mousecursor);
	if ((Application::IsKeyPressed(VK_LEFT)))
	{
		float yaw = (float)(CAMERA_SPEED * 2 * dt);
		rotateChar += yaw;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;	
	}
	if (mousecursor.x<800 / 2)
	{
		float yaw = (float)(CAMERA_SPEED * 0.5 * dt*(800/2-mousecursor.x));
		rotateChar += yaw;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (position.x - (right.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x - (right.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x -= right.x * (float)(CAMERA_SPEED * dt);
			target.x -= right.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z - (right.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z - (right.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z -= right.z * (float)(CAMERA_SPEED * dt);
			target.z -= right.z * (float)(CAMERA_SPEED * dt);
		}

	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * 2 * dt);
		rotateChar += yaw;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
			
	}
	if (mousecursor.x>800 / 2)
	{
		float yaw = (float)(-CAMERA_SPEED* 0.5 * dt*(mousecursor.x - 800 / 2));
		rotateChar += yaw;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;

	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (position.x + (right.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x + (right.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x += right.x * (float)(CAMERA_SPEED * dt);
			target.x += right.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z + (right.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z + (right.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z += right.z * (float)(CAMERA_SPEED * dt);
			target.z += right.z * (float)(CAMERA_SPEED * dt);
		}
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		
			float pitch = (float)(CAMERA_SPEED * 2 * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			if (view.y < 0.984)
			{
				Mtx44 rotation;
				rotation.SetToRotation(pitch, right.x, right.y, right.z);
				view = rotation * view;
				target = view + position;
			}
			
	}
	if (mousecursor.y<600 / 2)
	{
		
			float pitch = (float)(CAMERA_SPEED * 0.5* dt*(600 / 2 - mousecursor.y));
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			if (view.y < 0.984)
			{
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = view + position;
		}

	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x + (view.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x + (view.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x += view.x * (float)(CAMERA_SPEED * dt);
			target.x += view.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z + (view.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z + (view.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z += view.z * (float)(CAMERA_SPEED * dt);
			target.z += view.z * (float)(CAMERA_SPEED * dt);
		}
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (target.y > -0.8)
		{
			float pitch = (float)(-CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = view + position;
		}
			
	}
	if (mousecursor.y>600 / 2)
	{
		if (target.y > -0.8)
		{
			float pitch = (float)(-CAMERA_SPEED * 0.5 * 2 * dt*(mousecursor.y - 600 / 2));
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = view + position;
		}

	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x - (view.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x - (view.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x -= view.x * (float)(CAMERA_SPEED * dt);
			target.x -= view.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z - (view.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z - (view.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z -= view.z * (float)(CAMERA_SPEED * dt);
			target.z -= view.z * (float)(CAMERA_SPEED * dt);
		}
	}
	
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
	SetCursorPos(800 / 2, 600 / 2);
	
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::UpdateCollision(float bounds, double dt)
{
	static const float CAMERA_SPEED = 25.f;
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (position.x - (right.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x - (right.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x += right.x * (float)(CAMERA_SPEED * dt);
			target.x += right.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z - (right.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z - (right.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z += right.z * (float)(CAMERA_SPEED * dt);
			target.z += right.z * (float)(CAMERA_SPEED * dt);
		}

	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (position.x + (right.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x + (right.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x -= right.x * (float)(CAMERA_SPEED * dt);
			target.x -= right.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z + (right.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z + (right.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z -= right.z * (float)(CAMERA_SPEED * dt);
			target.z -= right.z * (float)(CAMERA_SPEED * dt);
		}
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x + (view.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x + (view.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x -= view.x * (float)(CAMERA_SPEED * dt);
			target.x -= view.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z + (view.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z + (view.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z -= view.z * (float)(CAMERA_SPEED * dt);
			target.z -= view.z * (float)(CAMERA_SPEED * dt);
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x - (view.x * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.x - (view.x * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.x += view.x * (float)(CAMERA_SPEED * dt);
			target.x += view.x * (float)(CAMERA_SPEED * dt);
		}
		if (position.z - (view.z * (float)(CAMERA_SPEED * dt)) > -bounds
			&& position.z - (view.z * (float)(CAMERA_SPEED * dt)) < bounds)
		{
			position.z += view.z * (float)(CAMERA_SPEED * dt);
			target.z += view.z * (float)(CAMERA_SPEED * dt);
		}
	}

}