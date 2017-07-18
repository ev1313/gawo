#include <gawo/gl/renderer/spriterenderer.hpp>
#include <gawo/io/log.hpp>

static io::Logger &logger = io::logging().get_logger("gawo.SpriteRenderer");

// static paths for the shaders
static const char *vss_normal = "../shaders/sprite.vs";

static const char *fss_normal = "../shaders/sprite.fs";

static const char *vss_colorpicking = "../shaders/colorpicking.vs";

static const char *fss_colorpicking = "../shaders/colorpicking.fs";

Sprite::Sprite() {}

Sprite::Sprite(std::shared_ptr <Texture> texture)
  : Sprite(texture, std::shared_ptr <SceneGraph>()) {}

Sprite::Sprite(std::shared_ptr <Texture> texture, std::weak_ptr <SceneGraph> parent)
  : tex(texture) {
  
  if (auto p = parent.lock())
    translation = p->addChild(glm::mat4x4());
  else
    translation = std::make_shared <SceneGraph>(glm::mat4x4());
  
  if (auto t = translation.lock())
    rotation = t->addChild(glm::mat4x4());
  else
    throw std::runtime_error("sprite::constructor failed translation lock");
  
  if (auto r = rotation.lock())
    scalation = r->addChild(glm::mat4x4());
  else
    throw std::runtime_error("sprite::constructor failed rotation lock");
  
  // this has to be done here, as a nasty hack, because the rotation won't work correctly
  //(m_width and m_height are set in setScale)
  setPosition(m_x, m_y);
  setScale(glm::vec3(m_width, m_height, 0.0f));
}

Sprite::Sprite(std::shared_ptr <Texture> texture, std::shared_ptr <Sprite> parent)
  : m_parent(parent) {
  if (parent)
    Sprite(texture, parent->getGraph());
}

void Sprite::setPosition(glm::ivec3 pos) {
  m_x = pos[0];
  m_y = pos[1];
  m_layer = (unsigned int) pos[2];
  if (auto t = translation.lock())
    t->changeMatrix(glm::translate(pos));
  else
    throw std::runtime_error("Sprite::setPosition translation lock failed");
}

void Sprite::setPosition(int x, int y) { setPosition(glm::vec3(x, y, m_layer)); }

void Sprite::setPosition(int x, int y, unsigned int layer) { setPosition(glm::vec3(x, y, layer)); }

void Sprite::setRotation(GLfloat rot) {
  auto m = glm::translate(glm::vec3(m_width * 0.5f, m_height * 0.5f, 0.0f));
  m = m * glm::rotate(rot, glm::vec3(0.0f, 0.0f, 1.0f));
  m = glm::translate(m, glm::vec3(m_width * -0.5f, m_height * -0.5f, 0.0f));
  
  if (auto r = rotation.lock())
    r->changeMatrix(m);
  else
    throw std::runtime_error("Sprite::setRotation rotation lock failed");
}

void Sprite::setScale(glm::ivec3 scale) {
  m_width = (unsigned int) scale[0];
  m_height = (unsigned int) scale[1];
  if (auto s = scalation.lock())
    s->changeMatrix(glm::scale(scale));
  else
    throw std::runtime_error("Sprite::setScale scalation lock failed");
}

void Sprite::setScale(unsigned int w, unsigned int h) { setScale(glm::ivec3(w, h, 1.0f)); }

void Sprite::setParent(std::shared_ptr <Sprite> parent) { m_parent = parent; }

void Sprite::setWidth(int w) { setScale((unsigned int) w, m_height); }

void Sprite::setHeight(int h) { setScale(m_width, (unsigned int) h); }

void Sprite::setX(int x) { setPosition(x, m_y); }

void Sprite::setY(int y) { setPosition(m_x, y); }

void Sprite::setLayer(int layer) { setPosition(m_x, m_y, (unsigned int) layer); }

glm::ivec2 Sprite::getRelativePosition(glm::ivec2 absolute) {
  if (m_parent)
    return m_parent->getRelativePosition(absolute - getPosition());
  
  return absolute - getPosition();
}

glm::ivec2 Sprite::getAbsolutePosition(glm::ivec2 relative) {
  if (m_parent)
    return m_parent->getRelativePosition(relative + getPosition());
  
  return relative + getPosition();
}

unsigned int Sprite::getWidth() { return m_width; }

unsigned int Sprite::getHeight() { return m_height; }

int Sprite::getX() { return m_x; }

int Sprite::getY() { return m_y; }

unsigned int Sprite::getLayer() { return m_layer; }

glm::ivec2 Sprite::getPosition() { return glm::ivec2(m_x, m_y); }

std::weak_ptr <SceneGraph> Sprite::getGraph() { return rotation; }

glm::mat4x4 Sprite::getMatrix() {
  if (auto s = scalation.lock())
    return s->getMatrix();
  
  return glm::mat4x4();
}

SpriteRenderer::SpriteRenderer(std::string normal_vs, std::string normal_fs, std::string color_vs,
                               std::string color_fs) {
  logger.log(io::LOG_INFO, "initializing renderer");
  
  m_rootnode = std::make_shared <SceneGraph>(glm::ortho(0.0f, (float) m_width, 0.0f, (float) m_height, -1000.0f, 1.0f));
  
  if (!m_rootnode) {
    logger.log(io::LOG_INFO, "couldn't initialize scenegraph rootnode");
  }
  
  // vertices (also directly used as tex coords)
  GLfloat vertices[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
  
  m_vbo.init();
  m_vbo.bind();
  m_vbo.fill(GL_STATIC_DRAW, sizeof(vertices), vertices);
  
  m_vao.init();
  m_vao.bind();
  m_vao.fill(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);
  
  m_vao.unbind();
  m_vbo.unbind();
  
  // todo check if shader location are there, else warn || error
  m_normal_shader.init();
  m_normal_shader.loadFile(normal_vs, GL_VERTEX_SHADER);
  m_normal_shader.loadFile(normal_fs, GL_FRAGMENT_SHADER);
  m_normal_shader_mvp = m_normal_shader.location("mvp");
  m_normal_shader_tex = m_normal_shader.location("tex");
  
  m_colorpicking_shader.init();
  m_colorpicking_shader.loadFile(color_vs, GL_VERTEX_SHADER);
  m_colorpicking_shader.loadFile(color_fs, GL_FRAGMENT_SHADER);
  m_colorpicking_shader_mvp = m_colorpicking_shader.location("mvp");
  m_colorpicking_shader_color = m_colorpicking_shader.location("color");
}

SpriteRenderer::SpriteRenderer()
  : SpriteRenderer(vss_normal, fss_normal, vss_colorpicking, fss_colorpicking) {}

SpriteRenderer::~SpriteRenderer() {
  logger.log(io::LOG_INFO, "closing renderer");
}

std::shared_ptr <Texture> SpriteRenderer::loadTexture(sdl2::SurfacePtr surface) {
  auto tex = std::make_shared <Texture>();
  tex->init();
  tex->bind();
  //todo: implement better loader checking for alpha channel, etc.
  tex->fill(0, 3, surface.get()->w, surface.get()->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface.get()->pixels);
  m_textures.push_back(tex);
  
  return tex;
}

std::shared_ptr <Texture> SpriteRenderer::loadFont(std::string text) {
  auto font = sdl2::FontPtr(TTF_OpenFont("../data/fonts/cmunti.ttf", 23));
  if (!font) {
    logger.log(io::LOG_ERROR, "failed loading font ../data/fonts/cmunti.ttf");
  }
  auto surface = sdl2::SurfacePtr(TTF_RenderText_Blended(font.get(), text.c_str(), SDL_Color{0, 0, 0, 255}));
  if (!surface) {
    logger.log(io::LOG_ERROR, "failed rendering font surface");
    throw std::runtime_error(SDL_GetError());
  }
  
  return loadTexture(std::move(surface));
}

std::shared_ptr <Texture> SpriteRenderer::loadTexture(std::string path) {
  auto surface = sdl2::SurfacePtr(IMG_Load(path.c_str()));
  if (!surface) {
    std::string error = "couldn't load image '" + path + "'.";
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "loading image failed", error.c_str(), NULL);
    
    logger.log(io::LOG_ERROR, error.c_str());
  }
  
  return loadTexture(std::move(surface));
}

void SpriteRenderer::resize(unsigned int w, unsigned int h) {
  m_width = w;
  m_height = h;
  glViewport(0, 0, m_width, m_height);
  m_rootnode->changeMatrix(glm::ortho(0.0f, (float) m_width, 0.0f, (float) m_height, -1000.0f, 1.0f));
}

void SpriteRenderer::initializeClick(unsigned int x, unsigned int y) {
  m_colorpicking_shader.bind();
  m_vao.bind();
  
  m_click_x = x;
  m_click_y = y;
  
  // just render a single pixel at the pickpoint
  //(hopefully antialiasing does not fuck up)
  glEnable(GL_SCISSOR_TEST);
  glScissor(m_click_x, m_click_y, 1, 1);
  
  // white = FFFFFF, therefore it shouldn't be reached
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int SpriteRenderer::finishClick() {
  glDisable(GL_SCISSOR_TEST);
  m_vao.unbind();
  m_colorpicking_shader.unbind();
  
  // get pixels now
  unsigned int data;
  glReadPixels(m_click_x, m_click_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);
  
  glClearColor(0.39f, 0.58f, 0.93f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  return data;
}

void SpriteRenderer::renderClick(Sprite &sprite, unsigned int i) {
  glUniformMatrix4fv(m_colorpicking_shader_mvp, 1, GL_FALSE, glm::value_ptr(sprite.getMatrix()));
  
  unsigned char r = (unsigned char) ((i & 0x000000FF) >> 0);
  unsigned char g = (unsigned char) ((i & 0x0000FF00) >> 8);
  unsigned char b = (unsigned char) ((i & 0x00FF0000) >> 16);
  
  glUniform3f(m_colorpicking_shader_color, r / 255.0f, g / 255.0f, b / 255.0f);
  
  sprite.tex->activate(GL_TEXTURE0);
  glUniform1f(m_normal_shader_tex, 0);
  
  glDrawArrays(GL_QUADS, 0, 4);
  
  sprite.tex->unbind();
}

void SpriteRenderer::initialize() {
  m_normal_shader.bind();
  m_vao.bind();
}

void SpriteRenderer::render(Sprite &sprite) {
  glUniformMatrix4fv(m_normal_shader_mvp, 1, GL_FALSE, glm::value_ptr(sprite.getMatrix()));
  
  sprite.tex->activate(GL_TEXTURE0);
  glUniform1f(m_normal_shader_tex, 0);
  
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  
  sprite.tex->unbind();
}

void SpriteRenderer::finish() {
  m_vao.unbind();
  m_normal_shader.unbind();
}

std::weak_ptr <SceneGraph> SpriteRenderer::getGraph() { return m_rootnode; }
