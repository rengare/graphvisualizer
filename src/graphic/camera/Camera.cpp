#include "Camera.h"

Camera::Camera(AppConfig &config, float farView, float nearView, float angle, glm::vec3 position, glm::vec3 rotation) : farView(farView),
                                                                                                                        nearView(nearView),
                                                                                                                        angle(angle)
{
    this->width = config.width;
    this->height = config.height;
    aspectRatio = width / height;

    projectionMatrix = glm::perspective(glm::radians(angle), aspectRatio, nearView, farView);
    this->cameraPosition = position;
    this->cameraRotation = rotation;

    Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    MakeRotation();
}

glm::mat4 Camera::GetViewMatrix()
{
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return projectionMatrix;
}

void Camera::Up()
{
    Translate(
        0,
        -cameraVelocity.y,
        0);
}

void Camera::Down()
{
    Translate(
        0,
        cameraVelocity.y,
        0);
}

void Camera::Forward()
{
    Translate(
        cameraVelocity.x * glm::sin(glm::radians(-cameraRotation.y)),
        cameraVelocity.y * sin(glm::radians(cameraRotation.x)),
        cameraVelocity.z * glm::cos(glm::radians(-cameraRotation.y)));
}

void Camera::Back()
{
    Translate(
        -cameraVelocity.x * glm::sin(glm::radians(-cameraRotation.y)),
        -cameraVelocity.y * sin(glm::radians(cameraRotation.x)),
        -cameraVelocity.z * glm::cos(glm::radians(-cameraRotation.y)));
}

void Camera::SetPosition(glm::vec3 pos)
{
    cameraPosition = pos;
    Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

void Camera::MakePosition()
{
    ResetPosition();
    translationMatrix = glm::translate(translationMatrix, glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));
}

void Camera::SetCameraVelocity(glm::vec3 velocity)
{
    this->cameraVelocity = velocity;
}

void Camera::SetRotationVelocity(glm::vec3 velocity)
{
    rotationVelocity = velocity;
}

void Camera::SetKeyboardRotationRatio(float ratio)
{
    rotationRatio = ratio;
}

glm::vec3 Camera::GetPosition()
{
    return cameraPosition;
}

void Camera::Left()
{
    Translate(
        cameraVelocity.x * glm::cos(glm::radians(cameraRotation.y)),
        0,
        cameraVelocity.z * glm::sin(glm::radians(cameraRotation.y)));
}

void Camera::Right()
{
    Translate(
        -cameraVelocity.x * glm::cos(glm::radians(cameraRotation.y)),
        0,
        -cameraVelocity.z * glm::sin(glm::radians(cameraRotation.y)));
}

void Camera::RotateLeft()
{
    cameraRotation.y -= rotationRatio;
    MakeRotation();
}

void Camera::RotateRight()
{
    cameraRotation.y += rotationRatio;
    MakeRotation();
}

void Camera::RotateUp()
{
    cameraRotation.x -= rotationRatio;
    MakeRotation();
}

void Camera::RotateDown()
{
    cameraRotation.x += rotationRatio;
    MakeRotation();
}

void Camera::MouseMovement(MousePosition mousePosition)
{
    //float minRangeValue = 1;
    cameraRotation.y += (mousePosition.x - mousePosition.previousX) * rotationVelocity.x;
    cameraRotation.x += (mousePosition.y - mousePosition.previousY) * rotationVelocity.y;

    MakeRotation();
}

void Camera::Translate(float x, float y, float z)
{
    translationMatrix = glm::translate(translationMatrix, glm::vec3(x, y, z));
    cameraPosition = (glm::vec3)translationMatrix[3];
}

void Camera::MakeRotation()
{
    ResetRotation();
    Rotate(cameraRotation.x, 1, 0, 0);
    Rotate(cameraRotation.y, 0, 1, 0);
}

void Camera::Rotate(float rotation, float x, float y, float z)
{
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation), glm::vec3(x, y, z));
}

void Camera::Update()
{
    viewMatrix = rotationMatrix * translationMatrix;
}

void Camera::Reset()
{
    translationMatrix = glm::translate(glm::mat4(1.f), glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));
    cameraRotation = glm::vec3(0.f);
    ResetRotation();
}

void Camera::JumpTo(glm::vec3 pos)
{
    cameraPosition = pos;
    MakePosition();
}

void Camera::ResetPosition()
{
    translationMatrix = glm::translate(glm::mat4(1.f), glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));
}

void Camera::ResetRotation()
{
    rotationMatrix = glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(1.f, 1.f, 1.f));
}
