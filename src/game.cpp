#include "game.h"
#include <iostream>
#include "SDL.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      normalFood(std::make_shared<SDL_Point>()), 
	    poisonFood(std::make_shared<SDL_Point>())
	  //obstacle(grid_width, grid_height) 
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, normalFood, poisonFood, snake.obstacle); //adding the obstacle and the 2 different food types to the render.

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing food.
    // adding the second type of food and the obstacle to be checked
    if (!snake.SnakeCell(x, y) && !snake.SnakeCell(x + 2, y - 3) && !snake.obstacle->ObstacleCell(x,y)) {
      normalFood->x = x;
      normalFood->y = y;

      poisonFood->x = x + 2;
      poisonFood->y = y - 3;
      return;
    }
  }
}
  
void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
	
  //locking to protect the data (score)
  std::lock_guard<std::mutex> lock(mtx);
  
  // Check if there's food over here
  if (normalFood->x == new_x && normalFood->y == new_y) {
    score++;
    PlaceFood();
    //while the snake is growing the obstacle is going to be placed as soon as the score reachs 8.
    snake.obstacle->PlaceObstacle(snake, score, normalFood, poisonFood);
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
    
  } else if (poisonFood->x == new_x && poisonFood->y == new_y) {
    if(snake.size > 1) {
      score--;
      PlaceFood();
      //while the snake is decreasing the obstacle is going to be placed as soon as the score reachs 8.
      snake.obstacle->PlaceObstacle(snake, score, normalFood, poisonFood);
      // Shrink snake and decrease speed.
      snake.ShrinkBody();
      snake.speed -= 0.02;
    } else {
      snake.alive = false; //while decreasing the snake dies after its size become 0
    }
  // the snake dies after eating the obstacle  
  } else if (snake.obstacle->ObstacleCell(new_x, new_y)) {
    snake.alive = false;
  } 
}

int Game::GetScore() const { 
  //locking to protect the data (score)
  std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mtx)); //I had to use the const_cast here because I am trying to lock a const mutex
  return score; 
}

int Game::GetSize() const { return snake.size; }

//1- implementing the player name function
void Game::PlayerName(const std::string &name) {
  playerName = name;
}

void Game::HighScore() {
  // Check if there is a task running
  if (highScoreTask.valid() && highScoreTask.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      return;
  }

  // Perform the high score saving asynchronously
  highScoreTask = std::async(std::launch::async, [this]() {
    //trying to open the text file and read it
    try {
      const std::string mainFilePath = "/home/workspace/CppND-Capstone-Snake-Game/players_scores.txt";
      const std::string tmpFilePath = "/home/workspace/CppND-Capstone-Snake-Game/tmp.txt";
      std::ifstream readFile(mainFilePath);
      //if the file doesn't exist it created a new file and start writing into it
      if(!readFile.is_open()) {
        std::ofstream newFile(mainFilePath);
        newFile << std::setw(20) << std::left << "Player Name" << std::setw(10) << std::right << "Score\n";
        newFile << "------------------------------\n";
        newFile << std::setw(20) << std::left << playerName << std::setw(10) << std::right << score << "\n";
        newFile.close();
        return;
      }
      //if the file is_open I create a new temporary file and write into it.
      std::ofstream writeFile(tmpFilePath);
    
      std::vector<std::pair<std::string, int>> playersScores; //vector to save each pair inside. each pair consists of name and score.
      //now trying to read the main file
      std::string line;
      int lineCount{};
      while(std::getline(readFile, line)) {
        lineCount++;
        if(lineCount <= 2) {
          continue;
        }
        //after reading the title I start reading the 3rd line and saving every name and its score in the created vector.
        std::istringstream linestream(line);
        std::string playerName;
        int existScore;
      
        if(linestream >> playerName >> existScore) {
          playersScores.emplace_back(playerName, existScore);
        } else {
          std::cerr << "Error reading line.\n";
        }
      }
      //now I go through each name in the vector.
      bool playerFound = false;
      for(auto &vec : playersScores) {
        if(vec.first == playerName) { //if the name is already inside the file I do next
          if(score > vec.second) { // now I check if the new score is bigger than the one saved in the file
            vec.second = score; // I save the new score's value inside the score in the vector
          }
          playerFound = true;
        }
      }
    //if we don't find the entered player name in the file. I add the new one and its score to the vector of pair
      if(!playerFound) {
        playersScores.emplace_back(playerName, score);
      }
      //then I write the title and every player name and it's score in the new temporary file.
      writeFile << std::setw(20) << std::left << "Player Name" << std::setw(10) << std::right << "Score\n";
      writeFile << "------------------------------\n";
      for(const auto &vec : playersScores) {
        writeFile << std::setw(20) << std::left << vec.first << std::setw(10) << std::right << vec.second <<"\n";
      }
      //closing both files
      readFile.close();
      writeFile.close();
      //deleting the old main file and the temporary file becomes the main file and changing it's name
      std::remove(mainFilePath.c_str());
      std::rename(tmpFilePath.c_str(), mainFilePath.c_str());
    } catch (const std::exception &err) {
      std::cerr << "Error: " << err.what() << "\n";
    }
  });  
}