#include <catch.hpp>

#include "gawo/spriterenderer.hpp"

#include "glits/glits.hpp"

#include <iostream>
#include <memory>

#if (USE_SDL)
#include "sdlwrapper/sdlwindow.hpp"
#else
#include "x11wrapper/x11window.hpp"
#endif

TEST_CASE("gawo/spriterenderer", "general tests") {

#if (USE_SDL)
  sdl2::SDLWindow wnd;
#else
  x11::X11Window wnd;
#endif
  wnd.open();

  SECTION("check renderer") {
    auto renderer = std::make_shared<SpriteRenderer>();
    REQUIRE(renderer->getGraph().lock());
    glViewport(0, 0, 1532, 978);
    renderer->resize(1532, 978);
    SECTION("empty screen") {
      glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      RenderToTexture rtt;
      rtt.init();
      REQUIRE(glits::check_framebuffer("../gawo-testdata/test-spriterenderer-empty.png", 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
    }

    SECTION("load sprite texture") {
      auto t1 = renderer->loadTexture("../gawo-testdata/sprite1.png");
      REQUIRE(t1);
      SECTION("create Sprite") {
        auto s1 = std::make_shared<Sprite>(t1, renderer->getGraph());

        SECTION("uninitialized sprite") {
          REQUIRE(s1);
          REQUIRE(s1->getWidth() == 0);
          REQUIRE(s1->getHeight() == 0);
          REQUIRE(s1->getLayer() == 0);
          REQUIRE(s1->getX() == 0);
          REQUIRE(s1->getY() == 0);
        }

        SECTION("change sprite-width") {
          s1->setWidth(100);
          REQUIRE(s1->getWidth() == 100);
        }
        SECTION("change sprite-height") {
          s1->setHeight(50);
          REQUIRE(s1->getHeight() == 50);
        }
        SECTION("change sprite-layer") {
          s1->setLayer(1);
          REQUIRE(s1->getLayer() == 1);
        }
        SECTION("change sprite-x") {
          s1->setX(10);
          REQUIRE(s1->getX() == 10);
        }
        SECTION("change sprite-y") {
          s1->setY(20);
          REQUIRE(s1->getY() == 20);
        }

        s1->setWidth(200);
        s1->setHeight(100);
        s1->setLayer(0);
        s1->setX(50);
        s1->setY(70);

        SECTION("render single sprite") {
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          renderer->initialize();
          renderer->render(*s1);
          renderer->finish();
          REQUIRE(glits::check_framebuffer("../gawo-testdata/test-spriterenderer-single_sprite.png", 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
        }
      }
      // Sprite s2("../gawo-testdata/spriterenderer-sprite2.png", s1);
    }
  }
}
