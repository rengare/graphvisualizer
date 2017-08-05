//#include "InstancedModel.h"
//
//#define PI 3.14159265
//
//InstancedModel::InstancedModel()
//{
//}
//
//InstancedModel::InstancedModel(int shaderProgram)
//{
//    this->shaderProgram = shaderProgram;
//    Create("");
//}
//
//InstancedModel::~InstancedModel()
//{
//}
//
//void InstancedModel::AddInstanced(VertexData data)
//{
//    (*bufferVertices).push_back(data);
//    AddDataToBuffer();
//}
//
//void InstancedModel::AddInstanced(vector<VertexData> *data)
//{
//    bufferVertices = data;
//    AddDataToBuffer();
//}
//
//void InstancedModel::ShouldBindBuffers()
//{
//    if (vbos.size() == 0)
//    {
//        BindInstancedBuffers();
//    }
//}
//
//glm::vec3 InstancedModel::GetPosition(int index)
//{
//    return (*bufferVertices)[index].vertexPosition;
//}
//
//void InstancedModel::AddDataToBuffer()
//{
//    BindInstancedBuffers();
//}
//
//void InstancedModel::BindInstancedBuffers()
//{
//    GLuint vao = 0;
//    GLuint vbo = 0;
//    // GLuint ibo = 0;
//
//    glGenBuffers(1, &structDataSSBO);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, structDataSSBO);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(VertexData), &(*bufferVertices)[0], GL_DYNAMIC_COPY);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, structDataSSBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, structDataSSBO);
//    glBufferData(GL_ARRAY_BUFFER, (*bufferVertices).size() * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
//
//    int positionLocation = glGetAttribLocation(shaderProgram, "in_position");
//    int colorLocation = glGetAttribLocation(shaderProgram, "in_color");
//    int sizeLocation = glGetAttribLocation(shaderProgram, "in_size");
//
//    glEnableVertexAttribArray(positionLocation);
//    glEnableVertexAttribArray(colorLocation);
//    glEnableVertexAttribArray(sizeLocation);
//
//    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
//    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));
//    glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::size)));
//
//    glBindVertexArray(positionLocation);
//    glBindVertexArray(colorLocation);
//    glBindVertexArray(sizeLocation);
//
//    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//    //glBindVertexArray(0);
//
//    this->vao = vao;
//    this->vbo = vbo;
//
//    bounded = true;
//}
//
//void InstancedModel::UnbindInstancedBuffers()
//{
//}
//
//int InstancedModel::GetInstancedSize()
//{
//    return this->bufferVertices->size();
//}
//
//void InstancedModel::SetDrawingMode(GLenum mode)
//{
//    drawingMode = mode;
//}
//
//void InstancedModel::AddComputeShader(GLuint shader)
//{
//    computeShader = shader;
//}
//
//void InstancedModel::Create(string fileName)
//{
//}
//
//void InstancedModel::Update()
//{
//}
//
//void InstancedModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
//{
//    if (bounded && isVisible)
//    {
//        glBindVertexArray(vao);
//
//        if (computeShader)
//        {
//            glUseProgram(computeShader);
//        }
//
//        glDispatchCompute(128, 1, 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
//
//        glUseProgram(shaderProgram);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view_matrix"), 1, false, &view_matrix[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection_matrix"), 1, false, &projection_matrix[0][0]);
//
//        glDrawArrays(drawingMode, 0, (*bufferVertices).size());
//        //glDrawArrays(GL_TRIANGLES, 0, verticesSize);
//
//        glBindVertexArray(0);
//    }
//}
//
//vector<VertexData> *InstancedModel::GetVertexData()
//{
//    return this->bufferVertices;
//}