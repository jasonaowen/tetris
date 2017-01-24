#include "state.h"

GameState new_game(int width, int height) {
  GameState state;

  state.field.height = height;
  state.field.width = width;
  state.field.lines.resize(height);
  for (Line& line : state.field.lines) {
    line.resize(width);
    for (CellState& cell : line) {
      cell = CellState::EMPTY;
    }
  }

  state.active_block.position_x = width / 2;
  state.active_block.position_y = height - 1;
  state.active_block.tetrimino = Tetrimino::TETRIMINO_O;
  state.active_block.rotation = Rotation::UNROTATED;

  state.next_block = Tetrimino::TETRIMINO_T;

  state.milliseconds_per_turn = 1000;
  state.score = 0;
  state.lines = 0;

  return state;
}

GameState reduce(GameState state, Action action) {
  switch(action) {
    case Action::NEW_GAME:
      return new_game(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    default:
      return state;
  }
}

bool operator==(const Field& lhs, const Field& rhs) {
  return lhs.height == rhs.height
      && lhs.width == rhs.width
      && lhs.lines == rhs.lines;
}

bool operator==(const ActiveBlock& lhs, const ActiveBlock& rhs) {
  return lhs.position_x == rhs.position_x
      && lhs.position_y == rhs.position_y
      && lhs.tetrimino == rhs.tetrimino
      && lhs.rotation == rhs.rotation;
}

bool operator==(const GameState& lhs, const GameState& rhs) {
  return lhs.field == rhs.field
      && lhs.active_block == rhs.active_block
      && lhs.next_block == rhs.next_block
      && lhs.milliseconds_per_turn == rhs.milliseconds_per_turn
      && lhs.score == rhs.score
      && lhs.lines == rhs.lines;
}
