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

void render_field(SDL_Renderer *renderer, Field field, int cell_size) {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int field_y = 0; field_y < field.height; field_y++) {
    for (int field_x = 0; field_x < field.width; field_x++) {
      if (field.lines[field_y][field_x] == CellState::FILLED) {
        SDL_Rect rect = {
          cell_size * field_x,
          cell_size * ((field.height - 1) - field_y),
          cell_size,
          cell_size
        };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
}

void render_active_block(SDL_Renderer *renderer,
                         ActiveBlock active_block,
                         int field_height,
                         int cell_size) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, 0xFF);
  Shape shape = get_shape(active_block.tetromino, active_block.rotation);
  for (int shape_y = 0; shape_y < MAX_TETROMINO_HEIGHT; shape_y++) {
    for (int shape_x = 0; shape_x < MAX_TETROMINO_WIDTH; shape_x++) {
      if (shape[shape_y][shape_x] == CellState::FILLED) {
        int field_x = active_block.position_x + shape_x;
        int field_y = active_block.position_y - shape_y;
        SDL_Rect rect = {
          cell_size * field_x,
          cell_size * (field_height - 1 - field_y),
          cell_size,
          cell_size
        };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
}

void render_next_block(SDL_Renderer *renderer,
                       SDL_Rect next_block_view,
                       int cell_size,
                       Tetromino next_block) {
  SDL_RenderSetViewport(renderer, &next_block_view);
  SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF);
  SDL_RenderFillRect(renderer, NULL);

  SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
  Shape shape = get_shape(next_block, Rotation::UNROTATED);
  for (int shape_y = 0; shape_y < MAX_TETROMINO_HEIGHT; shape_y++) {
    for (int shape_x = 0; shape_x < MAX_TETROMINO_WIDTH; shape_x++) {
      if (shape[shape_y][shape_x] == CellState::FILLED) {
        SDL_Rect rect = {
          cell_size * shape_x,
          cell_size * shape_y,
          cell_size,
          cell_size
        };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
}

void render(SDL_Renderer *renderer, GameState state) {
  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);

  SDL_RenderSetViewport(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
  SDL_RenderClear(renderer);

  int cell_size = calculate_cell_size(state, width, height);
  SDL_Rect field = {
    (width - (cell_size * state.field.width)) / 2,
    (height - (cell_size * state.field.height)) / 2,
    cell_size * state.field.width,
    cell_size * state.field.height
  };
  SDL_RenderSetViewport(renderer, &field);
  SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF);
  SDL_RenderFillRect(renderer, NULL);

  render_field(renderer, state.field, cell_size);
  render_active_block(renderer, state.active_block, state.field.height, cell_size);

  SDL_Rect next_block = {
    field.x + field.w + 10,
    10,
    cell_size * MAX_TETROMINO_WIDTH,
    cell_size * MAX_TETROMINO_HEIGHT
  };
  render_next_block(renderer, next_block, cell_size, state.next_block);

  SDL_RenderPresent(renderer);
}

Uint32 timer_callback(Uint32 interval, void *param) {
  SDL_Event event;
  SDL_zero(event);
  event.type = SDL_USEREVENT;
  SDL_PushEvent(&event);

  return interval;
}

struct UIGameState {
  GameState game_state;
  SDL_TimerID timer_id;
};

UIGameState ui_reduce(UIGameState state, Action action) {
  GameState new_game_state = reduce(state.game_state, action);
  switch (action) {
  case Action::NEW_GAME:
  case Action::MOVE_DOWN:
    SDL_RemoveTimer(state.timer_id);
    return {
      new_game_state,
      SDL_AddTimer(
        new_game_state.milliseconds_per_turn,
        timer_callback,
        nullptr
      )
    };
  case Action::TIME_FALL:
    if (state.game_state.milliseconds_per_turn !=
        new_game_state.milliseconds_per_turn) {
      SDL_RemoveTimer(state.timer_id);
      return {
        new_game_state,
        SDL_AddTimer(
          new_game_state.milliseconds_per_turn,
          timer_callback,
          nullptr
        )
      };
    } else {
      return {
        new_game_state,
        state.timer_id
      };
    }
  default:
    return {
      new_game_state,
      state.timer_id
    };
  }
}

void game_loop(SDL_Renderer *renderer) {
  UIGameState state = ui_reduce({
      {},
      0
    },
    Action::NEW_GAME
  );

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

    state = ui_reduce(state, action);

    render(renderer, state.game_state);
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
