#pragma once

#include <memory>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

/*
  we store in each node matrixoperations like translating, rotating or scaling.

  these nodes don't store they actual object being rendered, but rather can be stored by (multiple) objects to be rendered.

  the intended usage is covered below:
  - the root node should contain the viewmatrix.
  - e.g. sprites add three matrices to the graph, one each for translation, rotation and scalation
    - if the position/etc. of the sprite changes, it updates the matrix with changeMatrix.
      - this induces "recalculate", which recalculates all matrices of the children

 */
class SceneGraph {
private:
  std::weak_ptr<SceneGraph> m_parent;
  std::vector<std::shared_ptr<SceneGraph>> m_children;

  glm::mat4x4 m_operator_matrix;
  glm::mat4x4 m_matrix;

public:
  SceneGraph(std::weak_ptr<SceneGraph> parent, glm::mat4x4 operator_matrix);

  void addChild(std::shared_ptr<SceneGraph> child);
  void removeChild(std::shared_ptr<SceneGraph> child);
  /*
    updates the operator matrix and recalculates the graph.
   */
  void changeMatrix(glm::mat4x4 matrix);
  /*
    the operator matrix will be applied to all children of the node.
   */
  glm::mat4x4 getOperatorMatrix();
  /*
    getMatrix returns ancestor.getMatrix() * Operatormatrix.
   */
  glm::mat4x4 getMatrix();
  /*
    recalculates m_matrix and calls recalculate in all children
   */
  void recalculate();
};
