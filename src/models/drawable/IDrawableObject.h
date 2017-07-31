#ifndef IDRAWABLE_OBJECT_H
#define IDRAWABLE_OBJECT_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class IDrawableObject
{
  public:
    virtual ~IDrawableObject() = 0;

    virtual void Draw() = 0;
    virtual void Draw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, const glm::vec3 &cameraPosition) = 0;
    virtual void SetProgram(GLuint shaderProgram) = 0;

    virtual void Update() = 0;
    virtual void Destroy() = 0;

    virtual GLuint GetVao() const = 0;
    virtual const vector<GLuint> &GetVbos() const = 0;

    virtual void SetTexture(string textureName, GLuint texture) = 0;
    virtual glm::vec3 GetPosition(int index) = 0;

    virtual const GLuint GetTexture(string textureName) const = 0;
};

inline IDrawableObject::~IDrawableObject() {}

#endif // !IDRAWABLE_OBJECT_H
