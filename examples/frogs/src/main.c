#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <libgen.h>
#include <unistd.h>

#include "ncknge.h"

#include <math.h>
#ifndef M_PI
  #define M_PI 3.1415926535897
#endif

#include "resource/KBM_FROG.c"

float random_float()
{
  return (float)rand()/RAND_MAX;
}

int random_int(int x)
{
  return rand() % x;
}

float distance(float x1, float y1, float x2, float y2)
{
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

component* frog1;
component* frog2;

component* tongue1;
component* tongue2;

float fly_vx = 0;
float fly_vy = 0;

component* fly;
bitmap flyFrames[2];

int t;

void setup()
{
  component_add(quad_new(-50,-10,100,10,MAGENTA));
  component_add(quad_new(-50,-20,100,10,WHITE));

  char* frog_str = KBM_FROG;

  frog1 = component_add(bitmap_create(18, 6, frog_str));
  frog1->transform->translate->x = 14;
  frog1->transform->translate->y = -2;

  frog2 = component_add(bitmap_create(18, 6, frog_str));
  frog2->transform->translate->x = -22;
  frog2->transform->translate->y = -6;

  /* matrix hack, flips horizontally */
  frog2->transform->rot_scale->m00 *= -1;

  tongue1 = component_add(quad_new(0,0,1,2,RED));
  tongue1->transform->translate->x = 16;
  tongue1->transform->translate->y = 5;

  tongue2 = component_add(quad_new(0,0,1,2,RED));
  tongue2->transform->translate->x = -24;
  tongue2->transform->translate->y = 0;

  fly = component_add(bitmap_create(2, 3, "7 44 7"));
  flyFrames[1] = *(bitmap*)fly->fields;
  flyFrames[0] = *(bitmap*)bitmap_create(2, 3, " 7447 ")->fields;

  camera->translate->y = 10;

  t = 0;
}

void update()
{
  fly->fields = (void*)&flyFrames[ t % 2 ];
  fly->transform->translate->x += fly_vx;
  fly->transform->translate->y += fly_vy;

  int d = random_int(4);

  if (d == 0)
  {
    fly_vy = 1;
  }
  if (d == 1)
  {
    fly_vy = -1;
  }
  if (d == 2)
  {
    fly_vx = -1;
  }
  if (d == 3)
  {
    fly_vx = 1;
  }

  if (fly->transform->translate->y < -10)
  {
    fly_vy = 1;
  }
  if (fly->transform->translate->y > 30)
  {
    fly_vy = -1;
  }
  if (fly->transform->translate->x > 40)
  {
    fly_vx = -1;
  }
  if (fly->transform->translate->x < -40)
  {
    fly_vx = 1;
  }

  transform_set_angle(tongue1->transform, atan2(
        4 + fly->transform->translate->y - 5,
        1 + fly->transform->translate->x - 16
        ));
  ((quad*)tongue1->fields)->w = distance(16, 5,
    fly->transform->translate->x+1, fly->transform->translate->y+4);

  transform_set_angle(tongue2->transform, atan2(
        4 + fly->transform->translate->y - 0,
        1 + fly->transform->translate->x + 24
        ));
  ((quad*)tongue2->fields)->w = distance(-24, 0,
    fly->transform->translate->x+1, fly->transform->translate->y+4);
  t++;
}

void key(char k)
{
}

int main(int argc, char** argv) {
  chdir(dirname(argv[0]));
  execute(setup, update, key);
  return EXIT_SUCCESS;
}
