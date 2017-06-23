#include <gawo/scenegraph.hpp>

SceneGraph::SceneGraph(std::weak_ptr<SceneGraph> parent, glm::mat4x4 operator_matrix)
    : m_operator_matrix(operator_matrix) {
  m_parent = parent;

  recalculate();
}

SceneGraph::SceneGraph(glm::mat4x4 operator_matrix)
    : SceneGraph(std::shared_ptr<SceneGraph>(), operator_matrix) {}

std::weak_ptr<SceneGraph> SceneGraph::addChild(glm::mat4x4 operator_matrix) {
  auto child = std::make_shared<SceneGraph>(operator_matrix);
  child->setParent(shared_from_this());
  m_children.push_back(child);

  recalculate();

  return child;
}

/*
void SceneGraph::removeChild(std::weak_ptr<SceneGraph> child) {
  // TODO
  // m_children.remove(child);
  }*/

void SceneGraph::setParent(std::weak_ptr<SceneGraph> parent) {
  m_parent = parent;
  recalculate();
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
