#ifndef GAME_H
#define GAME_H

#include <random> 
#include <memory> //for shared pointers
#include <mutex>
#include <future>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h" //new obstacle class

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  
  //1- adding functions for saving the player's name and score
  void PlayerName(const std::string &name); //necessary function to set the player name
  void HighScore(); //function the text file is being read and/or written to save the new highest score
  
  void WaitForBackgroundTask() {
    // Check if there is already a task running
    if (highScoreTask.valid() && highScoreTask.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      // A task is still running, wait for it to complete
      highScoreTask.wait();
    }
  }

 private:
  Snake snake;
  //SDL_Point food; // created shared_ptr instead.

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  
  //1- creating a variable for the player name
  std::string playerName;
  
  //2- creating a variable for the different types of food
  std::shared_ptr<SDL_Point> normalFood;
  std::shared_ptr<SDL_Point> poisonFood;
  
  //3- obstacles
  //Obstacle obstacle;
  
  std::mutex mtx; //protecting the shared data (score)
  std::future<void> highScoreTask; // a synchronize task (reading/writing in the file)
};

#endif