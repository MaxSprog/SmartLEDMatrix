#include <Adafruit_NeoPixel.h>
#include <microDS3231.h>

#define LED_BRIGHTNESS 50
#define LED_PIN 8
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define LED_COUNT (LED_WIDTH * LED_HEIGHT)
#define spc 1
#define lenshf 0

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
    {4, 7}};

bool syms[12][7][4] = {
    {// 0
     {1, 1, 1, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1}},
    {// 1
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1}},
    {// 2
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {1, 1, 1, 1},
     {1, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 1, 1, 1}},
    {// 3
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {1, 1, 1, 1}},
    {// 4
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1}},
    {// 5
     {1, 1, 1, 1},
     {1, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {1, 1, 1, 1}},
    {// 6
     {1, 1, 1, 1},
     {1, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 1, 1, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1}},
    {// 7
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1}},
    {// 8
     {1, 1, 1, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1}},
    {// 9
     {1, 1, 1, 1},
     {1, 0, 0, 1},
     {1, 0, 0, 1},
     {1, 1, 1, 1},
     {0, 0, 0, 1},
     {0, 0, 0, 1},
     {1, 1, 1, 1}},
    {// :
     {0, 0, 0, 0},
     {1, 1, 0, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {1, 1, 0, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0}},
    {// unknown
     {1, 0, 0, 1},
     {0, 1, 1, 0},
     {0, 1, 1, 0},
     {1, 1, 1, 1},
     {0, 1, 1, 0},
     {0, 1, 1, 0},
     {1, 0, 0, 1}}};

void setup()
{
  Serial.begin(9600);

  strip.begin();
  strip.show();
  strip.setBrightness(LED_BRIGHTNESS);

  clearAllPixels();
}

String prev = "";
uint32_t col_T = strip.Color(75, 0, 255);
uint32_t col_else = strip.Color(0, 0, 0);
int count = 0;
int shft = 0;
int length = 0;
int shift_prev = 0;

void loop()
{
  for (int i = 0; i < LED_WIDTH; i++)
    light(i, 7, strip.gamma32(strip.ColorHSV(count + (256 * (i + (LED_WIDTH - 1))) % (5 * 65536))));
  drawTime(col_T, shft/10);
  delay(10);
  shft += lenshf;
  if(shft >= 310)
    shft = 0;
  count += 256;
  if (count >= 5 * 65536)
    count = 0;
}

// time visualizer
void drawTime(uint32_t col, int shift)
{
  String t = rtc.getTimeString().substring(0, 5);
  length = sizes[t[0] - '0'][0] + sizes[t[1] - '0'][0] + sizes[10][0] + sizes[t[3] - '0'][0] + sizes[t[4] - '0'][0] + 4 * spc;
  if (t != prev)
  {
    int lastX = (LED_WIDTH - length) / 2;
    for(int i = 0; i < 5; i++){
      if(i == 2) { lastX += sizes[10][0] + spc; continue;}
      if(t[i] != prev[i])
        clear(lastX, 0, sizes[prev[i] - '0'][0], sizes[prev[i] - '0'][1]);
      lastX += sizes[prev[i] - '0'][0] + spc;
    }
    prev = t;
  }
  if(shift != shift_prev){
    clear(0, 0, LED_WIDTH, LED_HEIGHT-1);
    shift_prev = shift;
  }
  drawSymbol(prev[0] - '0', (LED_WIDTH - length) / 2 - shift, col, 0);
  drawSymbol(prev[1] - '0', sizes[prev[0] - '0'][0] + spc + (LED_WIDTH - length) / 2 - shift, col, 0);
  drawSymbol(10, sizes[prev[0] - '0'][0] + sizes[prev[1] - '0'][0] + 2 * spc + (LED_WIDTH - length) / 2 - shift, col, 0);
  drawSymbol(prev[3] - '0', sizes[prev[0] - '0'][0] + sizes[prev[1] - '0'][0] + sizes[10][0] + 3 * spc + (LED_WIDTH - length) / 2 - shift, col, 0);
  drawSymbol(prev[4] - '0', length - sizes[prev[4]-'0'][0] + (LED_WIDTH - length) / 2 - shift, col, 0);
  strip.show();
}

int cycleX(int i){
  return (i < 0 ? LED_WIDTH - (-i % LED_WIDTH) : i % LED_WIDTH);
}

int cycleY(int j){
  return (j < 0 ? LED_HEIGHT - (-j % LED_HEIGHT) : j % (LED_HEIGHT-1));
}

// set i, j pixel to color
void light(int i, int j, uint32_t color)
{
  strip.setPixelColor(LED_HEIGHT * i + (i % 2 ? (LED_HEIGHT - 1) - j : j), color);
}

// set a rectangle of pixels black
void clear(int x, int y, int w, int h){
  for(int i = x; i < x + w; i++){
    for(int j = y; j < y + h; j++){
      light(i, j, strip.Color(0, 0, 0));
    }
  }
  strip.show();
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
        light(cycleX(i + start), cycleY(j), color);
        delay(wait);
      }
    }
  }
}
