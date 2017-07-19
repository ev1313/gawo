#include <gawo/gl/renderer/camerarenderer.hpp>

static io::Logger& logger = io::logging().get_logger("gawo.CameraRenderer");

CameraRenderer::CameraRenderer()
  : SpriteRenderer() {
  translation = m_rootnode->addChild(glm::mat4x4());
  if (auto t = translation.lock())
    rotation = t->addChild(glm::mat4x4());
  
  if (auto r = rotation.lock())
    scalation = r->addChild(glm::mat4x4());
}

void CameraRenderer::move(int x, int y) {
  m_x += x;
  m_y += y;
  
  if (auto t = translation.lock())
    t->changeMatrix(glm::translate(glm::vec3(m_x, m_y, 0.0f)));
  else
    throw std::runtime_error("CameraRenderer::move error, translation lock failed");
}

void CameraRenderer::rotate(float r) {
  m_r += r;
  
  auto m = glm::translate(glm::vec3(m_width * 0.5f, m_height * 0.5f, 0.0f));
  m = m * glm::rotate(m_r, glm::vec3(0.0f, 0.0f, 1.0f));
  m = glm::translate(m, glm::vec3(m_width * -0.5f, m_height * -0.5f, 0.0f));
  
  if (auto rl = rotation.lock())
    rl->changeMatrix(m);
  else
    throw std::runtime_error("CameraRenderer::rotate error, rotation lock failed");
}

void CameraRenderer::zoom(float z, glm::vec3 pos) {
  m_z += z;
  
  if (m_z <= 0.1f)
    m_z = 0.1f;
  
  auto m = glm::translate(pos);
  m = m * glm::scale(glm::vec3(m_z, m_z, 1.0f));
  m = glm::translate(m, -pos);
  
  if (auto r = rotation.lock())
    r->changeMatrix(m);
  else
    throw std::runtime_error("CameraRenderer::zoom error, zoom lock failed");
}

std::weak_ptr <SceneGraph> CameraRenderer::getGraph() { return scalation; }
