#ifndef IMODEL
#define IMODEL

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class IModel
{
  public:
    virtual ~IModel() {}

    virtual void Update() = 0;

    virtual void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition) = 0;

    virtual void DrawGui() = 0;

    virtual void Clear() = 0;
};

#endif