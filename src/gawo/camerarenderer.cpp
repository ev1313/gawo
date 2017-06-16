#include <gawo/camerarenderer.hpp>

CameraRenderer::CameraRenderer()
    : SpriteRenderer() {
  translation = std::make_shared<SceneGraph>(m_rootnode, glm::mat4x4());
  m_rootnode->addChild(translation);
  rotation = std::make_shared<SceneGraph>(translation, glm::mat4x4());
  translation->addChild(rotation);
  scalation = std::make_shared<SceneGraph>(rotation, glm::mat4x4());
  rotation->addChild(scalation);
}

void CameraRenderer::move(int x, int y) {
  m_x += x;
  m_y += y;

  translation->changeMatrix(glm::translate(glm::vec3(m_x, m_y, 0.0f)));
}

void CameraRenderer::rotate(float r) {
  m_r += r;

  auto m = glm::translate(glm::vec3(m_width * 0.5f, m_height * 0.5f, 0.0f));
  m = m * glm::rotate(m_r, glm::vec3(0.0f, 0.0f, 1.0f));
  m = glm::translate(m, glm::vec3(m_width * -0.5f, m_height * -0.5f, 0.0f));

  rotation->changeMatrix(m);
}

void CameraRenderer::zoom(float z, glm::vec3 pos) {
  m_z += z;

  if (m_z <= 0.1f)
    m_z = 0.1f;

  auto m = glm::translate(pos);
  m = m * glm::scale(glm::vec3(m_z, m_z, 1.0f));
  m = glm::translate(m, -pos);

  rotation->changeMatrix(m);
}

std::weak_ptr<SceneGraph> CameraRenderer::getGraph() {
  return scalation;
}
