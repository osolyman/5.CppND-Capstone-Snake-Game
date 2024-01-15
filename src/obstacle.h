#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <random>
#include <memory>
#include "SDL.h"
#include "snake.h"

class Snake;

class Obstacle {
public:
  Obstacle(std::size_t grid_width, std::size_t grid_height); //constructor
  /*
  Obstacle(const Obstacle&) = delete; // Deleted copy constructor
  Obstacle& operator=(const Obstacle&) = delete; // Deleted copy assignment operator
  Obstacle(Obstacle&&) noexcept = default; // Move constructor
  Obstacle& operator=(Obstacle&&) noexcept = default; // Move assignment operator
  */
  
  bool GenerateObstacle(int score); //true if the score reachs 8
  bool ObstacleCell(int x, int y); //true if passed arguements equal to the obstacle position
  //function for placing the obstacle
  void PlaceObstacle(Snake snake, int score, std::shared_ptr<SDL_Point> normalFood,std::shared_ptr<SDL_Point> poisonFood);
  SDL_Point obst; //obstacle point
  
private:
  	std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
  
};  

#endif