#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>

#include "../../config/AppConfig.h"
#include "../../system/input/InputManager.h"


class Camera
{
public:
	Camera(AppConfig& config, glm::vec3 position = glm::vec3{ 0 }, glm::vec3 rotation = glm::vec3{ 0 }, float farView = 2000.0f, float nearView = 0.1f, float angle = 45.0f);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void Up();

	void Down();
	
	void SetPosition(glm::vec3 pos);
	void MakePosition();
	void SetCameraVelocity(glm::vec3 velocity);
	void SetRotationVelocity(glm::vec3 velocity);
	void MakeRotation();
	
	void SetKeyboardRotationRatio(float ratio);
	glm::vec3 GetPosition();

	void Left();
	void Right();
	void Forward();
	void Back();
	void RotateLeft();
	void RotateRight();
	void RotateUp();
	void RotateDown();

	void MouseMovement(MousePosition mousePosition);

	void Update();
	void Rotate( float rotation, float x, float y, float z );
	void Reset();

	void JumpTo(glm::vec3 pos);

	glm::vec3 cameraPosition{ 0.f, 0.f, -20.f };
	glm::vec3 cameraRotation{ 1.f };

private:
	void ResetRotation();
	void ResetPosition();
	void Translate( float x, float y, float z );


	int width;
	int height;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	glm::vec3 cameraVelocity{ 0.5f };
	glm::vec3 rotationVelocity{ 0.5f };

	float aspectRatio;
	float rotationRatio = 1.f;
	float angle = 45.0f, nearView = 0.1f, farView = 2000.0f;
};
#endif // !CAMERA_H
