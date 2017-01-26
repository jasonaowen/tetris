#include <SDL2/SDL.h>
#include <iostream>

#include "state.h"

Action handle_event(SDL_Event event) {
  switch (event.type) {
  case SDL_QUIT:
    return Action::QUIT;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_a:
    case SDLK_LEFT:
      return Action::MOVE_LEFT;
    case SDLK_d:
    case SDLK_RIGHT:
      return Action::MOVE_RIGHT;
    case SDLK_s:
    case SDLK_DOWN:
      return Action::MOVE_DOWN;
    case SDLK_q:
    case SDLK_PAGEUP:
      return Action::ROTATE_COUNTERCLOCKWISE;
    case SDLK_e:
    case SDLK_PAGEDOWN:
      return Action::ROTATE_CLOCKWISE;
    case SDLK_ESCAPE:
      return Action::QUIT;
    case SDLK_n:
      return Action::NEW_GAME;
    default:
      SDL_LogInfo(
        SDL_LOG_CATEGORY_INPUT,
        "Unrecognized key: %s\n",
        SDL_GetKeyName(event.key.keysym.sym)
      );
      return Action::NO_ACTION;
    }
  case SDL_USEREVENT:
    return Action::TIME_FALL;
  }
  return Action::NO_ACTION;
}

int calculate_cell_size(GameState state, int view_width, int view_height) {
  int height_per_cell = view_height / state.field.height;
  int width_per_cell = view_width / state.field.width;
  if (height_per_cell > width_per_cell) {
    return width_per_cell;
  } else {
    return height_per_cell;
  }
}

void render(SDL_Renderer *renderer, GameState state) {
  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);

  SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF);
  SDL_RenderClear(renderer);

  int cell_size = calculate_cell_size(state, width, height);
  int field_left = (width - (cell_size * state.field.width)) / 2;
  int field_top = (height - (cell_size * state.field.height)) / 2;

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int field_y = 0; field_y < state.field.height; field_y++) {
    for (int field_x = 0; field_x < state.field.width; field_x++) {
      if (state.field.lines[field_y][field_x] == CellState::FILLED) {
        SDL_Rect rect = {
          field_left + cell_size * field_x,
          field_top + cell_size * ((state.field.height - 1) - field_y),
          cell_size,
          cell_size
        };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, 0xFF);
  Shape shape = get_shape(state.active_block.tetromino, state.active_block.rotation);
  for (int shape_y = 0; shape_y < MAX_TETROMINO_HEIGHT; shape_y++) {
    for (int shape_x = 0; shape_x < MAX_TETROMINO_WIDTH; shape_x++) {
      if (shape[shape_y][shape_x] == CellState::FILLED) {
        int field_x = state.active_block.position_x + shape_x;
        int field_y = state.active_block.position_y - shape_y;
        SDL_Rect rect = {
          field_left + cell_size * field_x,
          field_top + cell_size * (state.field.height - 1 - field_y),
          cell_size,
          cell_size
        };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void game_loop(SDL_Renderer *renderer) {
  GameState state;
  state = reduce(state, Action::NEW_GAME);

  bool should_quit = false;
  SDL_Event event;
  while (should_quit == false) {
    if (SDL_WaitEvent(&event) == 0) {
      SDL_Log("Error waiting for event: %s\n", SDL_GetError());
      should_quit = true;
    }
    Action action = handle_event(event);
    if (action == Action::QUIT) {
      should_quit = true;
    }
    SDL_LogInfo(
      SDL_LOG_CATEGORY_APPLICATION,
      "Applying action: %s\n",
      get_action_name(action)
    );

    state = reduce(state, action);

    render(renderer, state);
  }
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
    "Tetris",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    640,
    480,
    0
  );
  if (window == nullptr) {
    SDL_Log("Unable to create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == nullptr) {
    SDL_Log("Unable to create renderer: %s\n", SDL_GetError());
    return 1;
  }

  game_loop(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
