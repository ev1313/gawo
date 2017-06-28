#include "x11wrapper/x11window.hpp"

#include <iostream>
#include <memory.h>
#include <sys/time.h>
#include <unistd.h>

namespace x11 {

X11Window::X11Window(unsigned int width, unsigned int height)
    : m_width(width)
    , m_height(height) {}

void X11Window::open() {
  m_display = XOpenDisplay(NULL);

  if (m_display == NULL) {
    throw std::runtime_error("XOpenDisplay NULL");
  }

  m_rootwnd = DefaultRootWindow(m_display);

  m_visualinfo = glXChooseVisual(m_display, 0, m_attributes);

  if (m_visualinfo == NULL) {
    throw std::runtime_error("glXChooseVisual NULL");
  }

  m_colormap = XCreateColormap(m_display, m_rootwnd, m_visualinfo->visual, AllocNone);

  m_swa.colormap = m_colormap;
  m_swa.event_mask = ExposureMask | KeyPressMask;

  m_window =
      XCreateWindow(m_display, m_rootwnd, 0, 0, m_width, m_height, 0, m_visualinfo->depth, InputOutput, m_visualinfo->visual, CWColormap | CWEventMask, &m_swa);

  m_atomWmDeleteWindow = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(m_display, m_window, &m_atomWmDeleteWindow, 1);

  XEvent xev;
  if (ISSET_FLAG(FULLSCREEN)) {
    Atom wm_state = XInternAtom(m_display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);

    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = m_window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
  }

  XMapWindow(m_display, m_window);
  XStoreName(m_display, m_window, "OpenGL Window");

  if (ISSET_FLAG(FULLSCREEN)) {
    XSendEvent(m_display, DefaultRootWindow(m_display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  }

  m_glcontext = glXCreateContext(m_display, m_visualinfo, NULL, GL_TRUE);

  XSync(m_display, False);

  glXMakeCurrent(m_display, m_window, m_glcontext);

  std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  XClearWindow(m_display, m_window);
  XMapRaised(m_display, m_window);
}

X11Window::~X11Window() {
  glXMakeCurrent(m_display, None, NULL);
  glXDestroyContext(m_display, m_glcontext);
  XFree(m_visualinfo);
  XDestroyWindow(m_display, m_window);
  XCloseDisplay(m_display);
}

void X11Window::handleevents() {
  if (XPending(m_display) > 0) {
    XEvent xev;
    XNextEvent(m_display, &xev);
    handle(xev);
  }
}

void X11Window::handle(XEvent event) {
  if (event.type == Expose) {
    XWindowAttributes attribs;
    XGetWindowAttributes(m_display, m_window, &attribs);
    m_width = attribs.width;
    m_height = attribs.height;
    resize();

  } else if (event.type == ClientMessage) {
    if (event.xclient.data.l[0] == (int)m_atomWmDeleteWindow) {
      stop();
    }
  } else if (event.type == DestroyNotify) {
    stop();
  }
}

void X11Window::swap() { glXSwapBuffers(m_display, m_window); }

void X11Window::poll() { handleevents(); }

void X11Window::init() {}
}
