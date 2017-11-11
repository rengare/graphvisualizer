#ifndef FR_IMPROVED_MODEL
#define FR_IMPROVED_MODEL

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_sdl_gl3.h"

#include "../../config/AppConfig.h"
#include "../../graphic/GraphicsStructure.h"
#include "../../graphic/Shader.h"
#include "../../graphic/ComputeShader.h"
#include "../../helper/RandomNumberGenerator.h"
#include "../../helper/nanoflann.h"

#include "../IModel.h"

using namespace std;
using namespace nanoflann;

struct PointCloud
{
  typedef float coord_t; //!< The type of each coordinate
  
  int size;
  VertexData *pts;
}; // end of PointCloud

// And this is the "dataset to kd-tree" adaptor class:
template <typename Derived>
struct PointCloudAdaptor
{
  typedef typename Derived::coord_t coord_t;

  const Derived &obj; //!< A const ref to the data set origin

  /// The constructor that sets the data set source
  PointCloudAdaptor(const Derived &obj_) : obj(obj_) {}

  /// CRTP helper method
  inline const Derived &derived() const { return obj; }

  // Must return the number of data points
  inline size_t kdtree_get_point_count() const { return derived().size; }

  // Returns the dim'th component of the idx'th point in the class:
  // Since this is inlined and the "dim" argument is typically an immediate value, the
  //  "if/else's" are actually solved at compile time.
  inline coord_t kdtree_get_pt(const size_t idx, int dim) const
  {
    if (dim == 0)
      return derived().pts[idx].vertexPosition.x;
    else if (dim == 1)
      return derived().pts[idx].vertexPosition.y;
    else
      return derived().pts[idx].vertexPosition.z;
  }

  // Optional bounding-box computation: return false to default to a standard bbox computation loop.
  //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
  //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
  template <class BBOX>
  bool kdtree_get_bbox(BBOX & /*bb*/) const { return false; }

}; // end of PointCloudAdaptor

class FRModelImproved : public IModel
{
public:
  FRModelImproved();
  ~FRModelImproved();
  FRModelImproved(AppConfig *config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections);

  void Update() override;

  void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition) override;

  void DrawGui() override;

  void Clear() override;

private:
  void PrepareBuffers();

  void PrepareNodes();
  void PrepareEdges();

  void UpdateNodes();
  void UpdateEdges();

  void DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);
  void DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

  void PassUniforms(GLuint shader);

  vector<VertexData> *bufferVertices;
  vector<VertexData> *edgeVertices;
  vector<ConnectionIndices> *fromToConnections;

  GLuint nodeVao;
  GLuint nodeSsbo;
  GLuint fromToSsbo, repulsiveSsbo;

  GLuint edgeVao;
  GLuint edgeSsbo;

  Shader *nodeShader, *edgeShader;

  ComputeShader *repulsiveCompute;
  ComputeShader *attractiveCompute;
  ComputeShader *updateCompute;
  ComputeShader *linesCompute;

  PointCloud pc;

  int nodeSize;
  int edgeSize;
  int fromToConnectionSize;
  AppConfig *config;

  float speed = 100.0;
  float area = 100.0;
  float gravity = 10.0;
  int graphType = 0;

  typedef PointCloudAdaptor<PointCloud> PC2KD;
  PC2KD *pc2kd;
  // construct a kd-tree index:
  typedef KDTreeSingleIndexAdaptor<
      L2_Simple_Adaptor<float, PC2KD>,
      PC2KD,
      3 /* dim */
      >
      my_kd_tree_t;

      my_kd_tree_t *index;
};

#endif
