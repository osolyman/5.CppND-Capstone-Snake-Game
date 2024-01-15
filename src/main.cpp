#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  //1- adding the necessary code for saving the players name and score
  std::string playerName;
  std::cout << "Enter your name: ";
  std::getline(std::cin, playerName);
  
  if(playerName.empty()) {
    std::cerr << "Invalid player name.\n";
    return 1;
  }

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  
  game.PlayerName(playerName); //added for feature 1
  
  game.Run(controller, renderer, kMsPerFrame);
  
  game.HighScore(); //added for feature 1
  game.WaitForBackgroundTask(); // to make sure that the main program waits for this task to complete. 
  
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
    
  return 0;
}