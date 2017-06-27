#include "sdlwrapper/sdlwindow.hpp"

sdl2::SDLWindow::SDLWindow()
    : m_width(640)
    , m_height(400) {
  SDL_Log("initializing window");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    throw std::runtime_error(SDL_GetError());
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  m_window = sdl2::WindowPtr(SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height,
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

sdl2::SDLWindow::~SDLWindow() {
  SDL_Log("closing window");

  if (m_context != NULL)
    SDL_GL_DeleteContext(m_context);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void sdl2::SDLWindow::poll() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
    handleEvent(event);
}

void sdl2::SDLWindow::handleEvent(SDL_Event event) {
  // TODO
  // handle more events
  if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_Log("resized window");
      if (m_initializedgl) {
        m_width = event.window.data1;
        m_height = event.window.data2;
        onResize(m_width, m_height);
      }
    }
  }
}

unsigned int sdl2::SDLWindow::getWidth() { return m_width; }

unsigned int sdl2::SDLWindow::getHeight() { return m_height; }

bool sdl2::SDLWindow::initializedGL() { return m_initializedgl; }

void sdl2::SDLWindow::setTitle(std::string title) { SDL_SetWindowTitle(m_window.get(), title.c_str()); }

std::string sdl2::SDLWindow::getTitle() {
  const char* c = SDL_GetWindowTitle(m_window.get());
  std::string b;
  if (c != NULL)
    b = c;

  return b;
}
