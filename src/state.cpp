#include "state.h"

GameState new_game(int width, int height) {
  GameState state = {
    {
      height,
      width,
      std::vector<std::vector<CellState>>(height, std::vector<CellState>(width, CellState::EMPTY))
    },
    {
      width / 2,
      height - 1,
      Tetrimino::TETRIMINO_O,
      Rotation::UNROTATED
    },
    Tetrimino::TETRIMINO_T,
    1000,
    0,
    0
  };

  return state;
}

bool is_legal_position(Field field, ActiveBlock active_block) {
  Shape shape = get_shape(active_block.tetrimino, active_block.rotation);
  for (int shape_y = 0; shape_y < 3; shape_y++) {
    for (int shape_x = 0; shape_x < 3; shape_x++) {
      if (shape[shape_y][shape_x] == CellState::FILLED) {
        int field_x = active_block.position_x + shape_x;
        int field_y = active_block.position_y - shape_y; // start at top of shape and work down
        if (field_x < 0 || field_x >= field.width ||
            field_y < 0 || field_y >= field.height) {
          return false; // out of bounds
        }
        if (field.lines[field_y][field_x] == CellState::FILLED) {
          return false; // overlapping
        }
      }
    }
  }
  return true;
}

GameState update_active_block_if_legal(GameState state, ActiveBlock active_block) {
  GameState new_state = {
    state.field,
    active_block,
    state.next_block,
    state.milliseconds_per_turn,
    state.score,
    state.lines
  };

  if (is_legal_position(new_state.field, new_state.active_block)) {
    return new_state;
  } else {
    return state;
  }
}

GameState move_left(GameState old_state) {
  return update_active_block_if_legal(old_state, {
    old_state.active_block.position_x - 1,
    old_state.active_block.position_y,
    old_state.active_block.tetrimino,
    old_state.active_block.rotation
  });
}

GameState move_right(GameState old_state) {
  return update_active_block_if_legal(old_state, {
    old_state.active_block.position_x + 1,
    old_state.active_block.position_y,
    old_state.active_block.tetrimino,
    old_state.active_block.rotation
  });
}

GameState reduce(GameState state, Action action) {
  switch(action) {
    case Action::NEW_GAME:
      return new_game(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    case Action::MOVE_LEFT:
      return move_left(state);
    case Action::MOVE_RIGHT:
      return move_right(state);
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
