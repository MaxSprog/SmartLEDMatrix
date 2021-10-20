#include <Adafruit_NeoPixel.h>
#include <microDS3231.h>

#define LED_BRIGHTNESS 50
#define LED_PIN   8
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define LED_COUNT (LED_WIDTH * LED_HEIGHT)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
MicroDS3231 rtc;

int sizes[12][2] = {
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {4, 7},
  {2, 7},
  {4, 7}
};

bool syms[12][7][4] = {
  { // 0
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // 1
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1}
  },
  { // 2
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 1, 1}
  },
  { // 3
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // 4
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1}
  },
  { // 5
    {1, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // 6
    {1, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // 7
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1}
  },
  { // 8
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // 9
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {1, 1, 1, 1}
  },
  { // :
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
  },
  { // unknown
    {1, 0, 0, 1},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {1, 1, 1, 1},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {1, 0, 0, 1}
  }
};

void setup()
{
    Serial.begin(9600);
    
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNESS);
    
    clearAllPixels();
}

String prev = "";
uint32_t col_T = strip.Color(0, 0, 255);
uint32_t col_else = strip.Color(0, 0, 0);
int count = 0;
void loop()
{
    String t = rtc.getTimeString().substring(0, 5);
    if (t != prev)
    {
        clearAllPixels();
        prev = t;
    }
    
    drawSymbol(prev[0] - '0', 0 + 5, col_T, 1);
    drawSymbol(prev[1] - '0', 5 + 5, col_T, 1);
    drawSymbol(10, 10 + 5, col_T, 1);
    drawSymbol(prev[3] - '0', 13 + 5, col_T, 1);
    drawSymbol(prev[4] - '0', 18 + 5, col_T, 1);
    
    for (int i = 0; i < LED_WIDTH; i++)
        light(i, 7, strip.gamma32(strip.ColorHSV(count + (256 * (i + (LED_WIDTH - 1))) % (5 * 65536))));
    strip.show();
    delay(10);
    count += 256;
    if (count >= 5 * 65536)
        count = 0;
}

// set i, j pixel to color
void light(int i, int j, uint32_t color)
{
    strip.setPixelColor(LED_HEIGHT * i + (i % 2 ? (LED_HEIGHT - 1) - j : j), color);
}

// set all pixels to black
void clearAllPixels()
{
    for (int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
}

// *------>
// |     x
// | y
// V
// draw one symbol with code n at start with color
void drawSymbol(int n, int start, uint32_t color, int wait)
{
    int w = sizes[n][0];
    int h = sizes[n][1];
    for (int i = 0; i < w; i++)
    {
    for (int j = 0; j < h; j++)
    {
      if (syms[n][j][i])
      {
        light(i + start, j, color);
        delay(wait);
      }
    }
    }
    strip.show();
}
