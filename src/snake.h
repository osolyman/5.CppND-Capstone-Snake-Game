#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include "obstacle.h"

class Obstacle;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height);

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

  void ShrinkBody(); //2- function to shrink the body when eating poison
  
  Obstacle *obstacle; //3- obstacle object as a pointer
 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  
  bool shrinking{false}; //2- bool shrinking like growing
};

#endif