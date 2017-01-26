#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  std::cout << "Hello, world!" << std::endl;

  SDL_Quit();
  return 0;
}
