#include <gawo/scenegraph.hpp>

SceneGraph::SceneGraph(std::weak_ptr<SceneGraph> parent, glm::mat4x4 operator_matrix)
    : m_parent(parent)
    , m_operator_matrix(operator_matrix) {
  recalculate();
}

void SceneGraph::addChild(std::shared_ptr<SceneGraph> child) {
  m_children.push_back(child);

  if (child)
    child->recalculate();
}

void SceneGraph::removeChild(std::shared_ptr<SceneGraph> child) {
  // todo
}

void SceneGraph::changeMatrix(glm::mat4x4 matrix) {
  m_operator_matrix = matrix;
  recalculate();
}

glm::mat4x4 SceneGraph::getOperatorMatrix() { return m_operator_matrix; }

glm::mat4x4 SceneGraph::getMatrix() { return m_matrix; }

void SceneGraph::recalculate() {
  if (auto p = m_parent.lock())
    m_matrix = p->getMatrix() * m_operator_matrix;
  else
    m_matrix = m_operator_matrix;

  for (auto child : m_children) {
    if (child)
      child->recalculate();
  }
}
