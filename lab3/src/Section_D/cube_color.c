// CSE 474
// Lab 3 Section D (4)
// Luke Jiang, Hantao Liu
// 19/07/2018

// This program draws a blue rotating cube on the LCD screen as described in
// Section D (4). Touch the screen to change the color of the cube.

#include <string.h>
#include <math.h>
#include "driverlib/SSD2119.h"

#define M_PI      (3.14159265358979323846)

// define colors for drawing the cube
#define BLACK     (0)
#define WHITE     ((0xFF>>3)<<11) | ((0xFF>>2)<<5) | (0xFF>>3)
#define BLUE      ((0x00>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3)

// one inch = 68.57 pixels
#define SCALE     (68.57)

// define origin as the center of the screen
#define ORIGINX   (160)
#define ORIGINY   (120)

// colors for shading the cube
unsigned short const colorwheel[15] = {
  ((0xFF>>3)<<11) | ((0xFF>>2)<<5) | (0xFF>>3),  //15 ? bright white           (#FFFFFF) 	111111 	63
  ((0x00>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3),  //1 ? blue                    (#0000AA) 	000001 	1
  ((0x00>>3)<<11) | ((0xAA>>2)<<5) | (0x00>>3),  //2 ? green                   (#00AA00) 	000010 	2
  ((0x00>>3)<<11) | ((0xAA>>2)<<5) | (0xAA>>3),  //3 ? cyan                    (#00AAAA) 	000011 	3
  ((0xAA>>3)<<11) | ((0x00>>2)<<5) | (0x00>>3),  //4 ? red                     (#AA0000) 	000100 	4
  ((0xAA>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3),  //5 ? magenta                 (#AA00AA) 	000101 	5
  ((0xAA>>3)<<11) | ((0x55>>2)<<5) | (0x00>>3),  //6 ? brown                   (#AA5500) 	010100 	20
  ((0xAA>>3)<<11) | ((0xAA>>2)<<5) | (0xAA>>3),  //7 ? white / light gray      (#AAAAAA) 	000111 	7
  ((0x55>>3)<<11) | ((0x55>>2)<<5) | (0x55>>3),  //8 ? dark gray /bright black (#555555) 	111000 	56
  ((0x55>>3)<<11) | ((0x55>>2)<<5) | (0xFF>>3),  //9 ? bright blue             (#5555FF) 	111001 	57
  ((0x55>>3)<<11) | ((0xFF>>2)<<5) | (0x55>>3),  //10 ? bright green           (#55FF55) 	111010 	58
  ((0x55>>3)<<11) | ((0xFF>>2)<<5) | (0xFF>>3),  //11 ? bright cyan            (#55FFFF) 	111011 	59
  ((0xFF>>3)<<11) | ((0x55>>2)<<5) | (0x55>>3),  //12 ? bright red             (#FF5555) 	111100 	60
  ((0xFF>>3)<<11) | ((0x55>>2)<<5) | (0xFF>>3),  //13 ? bright magenta         (#FF55FF) 	111101 	61
  ((0xFF>>3)<<11) | ((0xFF>>2)<<5) | (0x55>>3)   //14 ? bright yellow          (#FFFF55) 	111110 	62
};

// RGB names of each color
char colorwheel_name[15][8] = {
  "#FFFFFF",
  "#0000AA",
  "#00AA00",
  "#00AAAA",
  "#AA0000",
  "#AA00AA",
  "#AA5500",
  "#AAAAAA",
  "#555555",
  "#5555FF",
  "#55FFFF",
  "#FF5555",
  "#FF55FF",
  "#FFFF55"
};

typedef struct Point {
  double x;
  double y;
} Point;

// Initialize the cube, rect, and circle.
void init_cube(Point *cube, Point *circle, Point *rect);
// Rotate the colored cube 90 degrees each iteration.
void rotate(Point *cube, Point *circle, Point *rect);
// Draw the cube on LCD.
void draw_cube(Point *cube, unsigned short color);
// Draw circle on LCD.
void draw_circle(Point *circle, unsigned short color);
// Draw rectangle on LCD.
void draw_rect(Point *rect, unsigned short color, int i);
// Helper draw_line converts double to unsigned short.
void LCD_DrawLine1(double startX, double startY, double endX, double endY, unsigned short color);
// Read current touch coordinate and write to fields of parameter point.
void coord(Point *point);

int main() {
  LCD_Init();
  Touch_Init();
  Point cube[8];
  Point rect[4];
  Point circle;
  init_cube(cube, &circle, rect);
  rotate(cube, &circle, rect);
}

void init_cube(Point *cube, Point *circle, Point *rect) {
  cube[0].x = 0;
  cube[0].y = 0;
  cube[1].x = 0.3 * sqrt(3) * SCALE;
  cube[1].y = -0.3 * SCALE;
  cube[2].x = 0.6 * SCALE;
  cube[2].y = -0;
  cube[3].x = (0.3* sqrt(3) + 0.6) * SCALE;
  cube[3].y = -0.3 * SCALE;
  cube[4].x = 0;
  cube[4].y = -0.6 * SCALE;
  cube[5].x = 0.3* sqrt(3) * SCALE;
  cube[5].y = -0.9 * SCALE;
  cube[6].x = 0.6 * SCALE;
  cube[6].y = -0.6 * SCALE;
  cube[7].x = (0.3 * sqrt(3) + 0.6 ) * SCALE;
  cube[7].y = -0.9 * SCALE;

  circle->x = (-0.3 + 0.3 * sqrt(3)) / 2 * SCALE;
  circle->y = -(0.3 * sqrt(3) + 1.5) / 2 * SCALE;

  rect[0].x = 0;
  rect[0].y = -0.9 * SCALE;

  rect[1].x = 0;
  rect[1].y = 0;

  rect[2].x = -(0.3 * sqrt(3) + 0.6) * SCALE;
  rect[2].y = 0;

  rect[3].x = -0.9 * SCALE;
  rect[3].y = - (0.6 + 0.3 * sqrt(3)) * SCALE;
}

void rotate(short theta, Point *cube, Point *circle, Point *rect) {
  Point new_p;
  Point next_circle;
  Point pos;
  Point newPos;
  coord(&pos);
  int count = 0;
  int touch_count = 0;

  while (1) {
    draw_rect(rect, colorwheel[touch_count % 15], count % 4);
    draw_circle(circle, BLACK);
    next_circle.x = -circle->y;
    next_circle.y = circle->x;
    draw_circle(&next_circle, BLACK);
    draw_cube(cube, WHITE);

    for (int i = 0; i < 2000000; i++);

    coord(&newPos);
    if (abs(newPos.x - pos.x) > 10 || abs((newPos.y - pos.y) > 10)) {
      LCD_SetCursor(0, 0);
      touch_count++;
      LCD_PrintString(colorwheel_name[touch_count % 15]);
    }

    draw_rect(rect, BLACK, count % 4);
    draw_cube(cube, BLACK);

    for (int i = 0; i < 8; i++) {
      new_p.x =  ( - (cube[i].y));
      new_p.y =  ((cube[i].x));
      cube[i].x = new_p.x;
      cube[i].y = new_p.y;   
    }

    circle->x = next_circle.x;
    circle->y = next_circle.y;

    count++;
  }
}

void draw_cube(Point *cube,  unsigned short color) {
  LCD_DrawLine1(cube[0].x + ORIGINX, cube[0].y + ORIGINY, cube[1].x+ ORIGINX, cube[1].y+ ORIGINY, color);
  LCD_DrawLine1(cube[2].x + ORIGINX, cube[2].y + ORIGINY, cube[3].x+ ORIGINX, cube[3].y+ ORIGINY, color);
  LCD_DrawLine1(cube[4].x + ORIGINX, cube[4].y + ORIGINY, cube[5].x+ ORIGINX, cube[5].y+ ORIGINY, color);
  LCD_DrawLine1(cube[6].x + ORIGINX, cube[6].y + ORIGINY, cube[7].x+ ORIGINX, cube[7].y+ ORIGINY, color);
  LCD_DrawLine1(cube[0].x + ORIGINX, cube[0].y + ORIGINY, cube[4].x+ ORIGINX, cube[4].y+ ORIGINY, color);
  LCD_DrawLine1(cube[1].x + ORIGINX, cube[1].y + ORIGINY, cube[5].x+ ORIGINX, cube[5].y+ ORIGINY, color);
  LCD_DrawLine1(cube[2].x + ORIGINX, cube[2].y + ORIGINY, cube[6].x+ ORIGINX, cube[6].y+ ORIGINY, color);
  LCD_DrawLine1(cube[3].x + ORIGINX, cube[3].y + ORIGINY, cube[7].x+ ORIGINX, cube[7].y+ ORIGINY, color);
  LCD_DrawLine1(cube[0].x + ORIGINX, cube[0].y + ORIGINY, cube[2].x+ ORIGINX, cube[2].y+ ORIGINY, color);
  LCD_DrawLine1(cube[1].x + ORIGINX, cube[1].y + ORIGINY, cube[3].x+ ORIGINX, cube[3].y+ ORIGINY, color);
  LCD_DrawLine1(cube[4].x + ORIGINX, cube[4].y + ORIGINY, cube[6].x+ ORIGINX, cube[6].y+ ORIGINY, color);
  LCD_DrawLine1(cube[5].x + ORIGINX, cube[5].y + ORIGINY, cube[7].x+ ORIGINX, cube[7].y+ ORIGINY, color);    
}

void LCD_DrawLine1(double startX, double startY, double endX, double endY, unsigned short color){
  LCD_DrawLine((unsigned short) startX, (unsigned short) startY, (unsigned short)endX, (unsigned short) endY, color);
}

void draw_circle(Point *circle, unsigned short color) {
  unsigned short radius = (unsigned short) (0.3 * SCALE);
  LCD_DrawFilledCircle((unsigned short) (circle->x + 160), (unsigned short) (circle->y + 120), radius, color);
}

void draw_rect(Point *rect, unsigned short color, int i) {
  double width = (0.3 * sqrt(3) + 0.6 ) * SCALE;
  double height = 0.9 * SCALE;
  if (i % 2) {
    double temp = height;
    height = width;
    width = temp;
  }
  LCD_DrawFilledRect((unsigned short) (rect[i].x + ORIGINX), (unsigned short) (rect[i].y + ORIGINY), (short) width, (short) height, color);
}

void coord(Point *point) {
  Touch_ReadX();
  Touch_ReadY();
  long res = Touch_GetCoords();
  point->y = (double) (res & 0xFF);
  point->x = (double) (res >> 16);
}


