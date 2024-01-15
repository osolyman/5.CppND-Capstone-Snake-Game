#include "obstacle.h"
#include <iostream>

Obstacle::Obstacle(std::size_t grid_width, std::size_t grid_height)
  		: engine(dev()),
      	  random_w(0, static_cast<int>(grid_width - 1)),
      	  random_h(0, static_cast<int>(grid_height - 1)) {
}

bool Obstacle::GenerateObstacle(int score) {
  return score >= 8;
}

bool Obstacle::ObstacleCell(int x, int y) {
  return (x == obst.x && y == obst.y);
}

void Obstacle::PlaceObstacle(Snake snake, int score, std::shared_ptr<SDL_Point> normalFood,std::shared_ptr<SDL_Point> poisonFood) {
  //generating the obstacle only if the necessary score get reached
  if(GenerateObstacle(score)) {
    int x, y;
    
    while(true) {
      x = random_w(engine);
      y = random_h(engine);
      
      if (!snake.SnakeCell(x, y) && x != normalFood->x && y != normalFood->y && x != poisonFood->x && y != poisonFood->y) {
        obst.x = x;
      	obst.y = y;
        return;
      }
    }
  } else {
    obst.x = -1;
    obst.y = -1;
  }
}
  