#include <assert.h>
#include "litehtml.h"
#include "test_container.h"
using namespace litehtml;

static void WebColorParseTest() {
  test_container container;
  web_color c;
  c = web_color::from_string(_t(""), &container), assert(c.red == 0), assert(c.green == 0), assert(c.blue == 0);
  c = web_color::from_string(_t("#f0f"), &container), assert(c.red == 255), assert(c.green == 0), assert(c.blue == 255);
  c = web_color::from_string(_t("#ff00ff"), &container), assert(c.red == 255), assert(c.green == 0), assert(c.blue == 255);
  c = web_color::from_string(_t("rgb()"), &container), assert(c.red == 0), assert(c.green == 0), assert(c.blue == 0);
  c = web_color::from_string(_t("rgb(255,0,255)"), &container), assert(c.red == 255), assert(c.green == 0), assert(c.blue == 255);
  c = web_color::from_string(_t("red"), &container), assert(c.red == 255), assert(c.green == 0), assert(c.blue == 0);
  c = web_color::from_string(_t("unknown"), &container), assert(c.red == 0), assert(c.green == 0), assert(c.blue == 0);
}

void webColorTest() {
  WebColorParseTest();
}