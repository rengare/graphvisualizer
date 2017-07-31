#include "InstancedModel.h"

#define PI 3.14159265

InstancedModel::InstancedModel()
{
}

InstancedModel::InstancedModel(int shaderProgram)
{
    this->shaderProgram = shaderProgram;
    Create("");
}

InstancedModel::InstancedModel(string fileName, string textureName, GLuint texture, int shaderProgram)
{
    this->shaderProgram = shaderProgram;
    this->textureName = textureName;
    SetTexture(textureName, texture);

    Create(fileName);
}

InstancedModel::~InstancedModel()
{
}

void InstancedModel::AddInstanced(Position position)
{
    VertexData data;
    data.vertexPosition = position;
    (*bufferVertices).push_back(data);
    AddDataToBuffer();
}

void InstancedModel::AddInstanced(VertexData data)
{
    (*bufferVertices).push_back(data);
    AddDataToBuffer();
}

void InstancedModel::AddInstanced(vector<VertexData> *data)
{
    bufferVertices = data;
    AddDataToBuffer();
}

void InstancedModel::AddInstanced(Position position, Color color)
{
    this->AddInstanced(position, color, 1);
}

void InstancedModel::AddInstanced(Position position, Color color, int size)
{
    VertexData data;
    data.vertexPosition = position;
    data.color = color;
    data.size = size;
    (*bufferVertices).push_back(data);
    AddDataToBuffer();
}

void InstancedModel::ShouldBindBuffers()
{
    if (vbos.size() == 0)
    {
        BindInstancedBuffers();
    }
}

glm::vec3 InstancedModel::GetPosition(int index)
{
    return (*bufferVertices)[index].vertexPosition.position;
}

void InstancedModel::AddDataToBuffer()
{
    ShouldBindBuffers();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (*bufferVertices).size() * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
}

void InstancedModel::BindInstancedBuffers()
{
    GLuint vao = 0;
    GLuint vbo = 0;
    // GLuint ibo = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (*bufferVertices).size() * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);

    int positionLocation = glGetAttribLocation(shaderProgram, "in_position");
    int colorLocation = glGetAttribLocation(shaderProgram, "in_color");
    int sizeLocation = glGetAttribLocation(shaderProgram, "in_size");

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(colorLocation);
    glEnableVertexAttribArray(sizeLocation);

    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));
    glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::size)));

    glBindVertexArray(positionLocation);
    glBindVertexArray(colorLocation);
    glBindVertexArray(sizeLocation);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    this->vao = vao;
    this->vbo = vbo;
    this->vbos.push_back(vbo);

    bounded = true;
}

void InstancedModel::UnbindInstancedBuffers()
{
    if (vbos.size() > 0)
    {
        glDeleteBuffers(vbos.size(), &vbos[0]);
        glDeleteVertexArrays(1, &vao);
        vbos.clear();
    }
}

int InstancedModel::GetInstancedSize()
{
    return this->bufferVertices->size();
}

void InstancedModel::SetDrawingMode(GLenum mode)
{
    drawingMode = mode;
}

void InstancedModel::Create(string fileName)
{
}

void InstancedModel::Update()
{
}

void InstancedModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
    if (bounded)
    {
        glUseProgram(shaderProgram);
        glm::vec3 lightPos = cameraPosition;
        lightPos[2] = -lightPos[2];

        glBindVertexArray(vao);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view_matrix"), 1, false, &view_matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection_matrix"), 1, false, &projection_matrix[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "light_pos"), 1, &lightPos[0]);

        glDrawArrays(drawingMode, 0, (*bufferVertices).size());
        //glDrawArrays(GL_TRIANGLES, 0, verticesSize);
    }
}

vector<VertexData>* InstancedModel::GetVertexData(){
    return this->bufferVertices;
}