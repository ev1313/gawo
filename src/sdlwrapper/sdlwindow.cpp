#include "sdlwrapper/sdlwindow.hpp"

namespace sdl2 {
SDLWindow::SDLWindow()
    : m_width(640)
    , m_height(400) {
  SDL_Log("initializing sdl");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    throw std::runtime_error(SDL_GetError());
  }
}

void SDLWindow::open() {
  SDL_Log("opening window");
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  m_window = WindowPtr(SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED));
  if (m_window == NULL) {
    SDL_Log("failed initializing the window.");
    throw std::runtime_error(SDL_GetError());
  }

  m_context = SDL_GL_CreateContext(m_window.get());
  if (m_context == NULL) {
    SDL_Log("failed initializing the context.");
    throw std::runtime_error(SDL_GetError());
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_Log("failed initializing sdl image.");
    throw std::runtime_error(SDL_GetError());
  }

  if (TTF_Init() != 0) {
    SDL_Log("failed initializing sdl ttf");
    throw std::runtime_error(SDL_GetError());
  }

  m_initializedgl = true;
}

SDLWindow::~SDLWindow() {
  SDL_Log("closing window");

  if (m_context != NULL)
    SDL_GL_DeleteContext(m_context);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void SDLWindow::poll() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
    handleEvent(event);
}

void SDLWindow::swap() { SDL_GL_SwapWindow(m_window.get()); }

void SDLWindow::handleEvent(SDL_Event event) {
  // TODO
  // handle more events
  if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_Log("resized window");
      if (m_initializedgl) {
        m_width = (uint32_t) event.window.data1;
        m_height = (uint32_t) event.window.data2;
        onResize(m_width, m_height);
      }
    }
  }
}

unsigned int SDLWindow::getWidth() { return m_width; }

unsigned int SDLWindow::getHeight() { return m_height; }

void SDLWindow::setWidth(uint32_t w) {
  m_width = w;
  SDL_SetWindowSize(m_window.get(), w, m_height);
}
void SDLWindow::setHeight(uint32_t h) {
  m_height = h;
  SDL_SetWindowSize(m_window.get(), m_width, h);
}

bool SDLWindow::initializedGL() { return m_initializedgl; }

void SDLWindow::setTitle(std::string title) { SDL_SetWindowTitle(m_window.get(), title.c_str()); }

std::string SDLWindow::getTitle() {
  const char* c = SDL_GetWindowTitle(m_window.get());
  std::string b;
  if (c != NULL)
    b = c;

  return b;
}

uint8_t SDLWindow::getMouseButton() {
  uint8_t b;

  return b;
}

uint8_t SDLWindow::getMouseState() {
  uint8_t b;

  return b;
}
}
