#include <algorithm>

#include "state.h"

Rotation rotate_clockwise(Rotation rotation) {
  switch(rotation) {
    case Rotation::UNROTATED: return Rotation::CLOCKWISE;
    case Rotation::CLOCKWISE: return Rotation::UPSIDE_DOWN;
    case Rotation::UPSIDE_DOWN: return Rotation::COUNTERCLOCKWISE;
    case Rotation::COUNTERCLOCKWISE: return Rotation::UNROTATED;
  }
}

Rotation rotate_counterclockwise(Rotation rotation) {
  switch(rotation) {
    case Rotation::UNROTATED: return Rotation::COUNTERCLOCKWISE;
    case Rotation::COUNTERCLOCKWISE: return Rotation::UPSIDE_DOWN;
    case Rotation::UPSIDE_DOWN: return Rotation::CLOCKWISE;
    case Rotation::CLOCKWISE: return Rotation::UNROTATED;
  }
}

const char* get_action_name(Action action) {
  switch (action) {
  case Action::NO_ACTION:               return "NO_ACTION";
  case Action::QUIT:                    return "QUIT";
  case Action::NEW_GAME:                return "NEW_GAME";
  case Action::TIME_FALL:               return "TIME_FALL";
  case Action::MOVE_LEFT:               return "MOVE_LEFT";
  case Action::MOVE_RIGHT:              return "MOVE_RIGHT";
  case Action::MOVE_DOWN:               return "MOVE_DOWN";
  case Action::ROTATE_CLOCKWISE:        return "ROTATE_CLOCKWISE";
  case Action::ROTATE_COUNTERCLOCKWISE: return "ROTATE_COUNTERCLOCKWISE";
  default:                              return "Unknown";
  }
}

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
      Tetromino::O,
      Rotation::UNROTATED
    },
    Tetromino::T,
    1000,
    0,
    0,
    GameProgress::IN_PROGRESS
  };

  return state;
}

bool is_legal_position(Field field, ActiveBlock active_block) {
  Shape shape = get_shape(active_block.tetromino, active_block.rotation);
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

GameState update_active_block(GameState state, ActiveBlock active_block) {
  return {
    state.field,
    active_block,
    state.next_block,
    state.milliseconds_per_turn,
    state.score,
    state.lines,
    state.progress
  };
}

GameState update_active_block_if_legal(GameState state, ActiveBlock active_block) {
  GameState new_state = update_active_block(state, active_block);

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
    old_state.active_block.tetromino,
    old_state.active_block.rotation
  });
}

GameState move_right(GameState old_state) {
  return update_active_block_if_legal(old_state, {
    old_state.active_block.position_x + 1,
    old_state.active_block.position_y,
    old_state.active_block.tetromino,
    old_state.active_block.rotation
  });
}

GameState rotate_clockwise(GameState old_state) {
  return update_active_block_if_legal(old_state, {
    old_state.active_block.position_x,
    old_state.active_block.position_y,
    old_state.active_block.tetromino,
    rotate_clockwise(old_state.active_block.rotation)
  });
}

GameState rotate_counterclockwise(GameState old_state) {
  return update_active_block_if_legal(old_state, {
    old_state.active_block.position_x,
    old_state.active_block.position_y,
    old_state.active_block.tetromino,
    rotate_counterclockwise(old_state.active_block.rotation)
  });
}

Field add_block_to_field(Field field, ActiveBlock active_block) {
  Field new_field = {
    field.height,
    field.width,
    field.lines
  };
  Shape shape = get_shape(active_block.tetromino, active_block.rotation);
  for (int shape_y = 0; shape_y < MAX_TETROMINO_HEIGHT; shape_y++) {
    int field_y = active_block.position_y - shape_y;
    for (int shape_x = 0; shape_x < MAX_TETROMINO_WIDTH; shape_x++) {
      int field_x = active_block.position_x + shape_x;
      if (shape[shape_y][shape_x] == CellState::FILLED) {
        new_field.lines[field_y][field_x] = CellState::FILLED;
      }
    }
  }
  return new_field;
}

bool line_is_filled(Line line) {
  return std::all_of(
    line.begin(),
    line.end(),
    [](CellState cell){ return cell == CellState::FILLED; });
}

typedef std::pair<Field, int> FieldWithFilledLineCount;

FieldWithFilledLineCount remove_filled_lines(Field field) {
  int removed_lines = 0;
  Field new_field = {
    field.height,
    field.width,
    std::vector<Line>()
  };
  new_field.lines.reserve(field.height);
  for (Line line : field.lines) {
    if (line_is_filled(line)) {
      removed_lines++;
    } else {
      new_field.lines.push_back(line);
    }
  }
  new_field.lines.resize(field.height, std::vector<CellState>(field.width, CellState::EMPTY));
  return FieldWithFilledLineCount(new_field, removed_lines);
}

ActiveBlock next_active_block(GameState state) {
  return {
    state.field.width / 2,
    state.field.height - 1,
    state.next_block,
    Rotation::UNROTATED
  };
}

int new_score(int old_score, int removed_lines) {
  int removed_line_value = 100 * removed_lines;
  if (removed_lines == MAX_TETROMINO_HEIGHT) {
    removed_line_value *= 2;
  }
  return old_score + removed_line_value;
}

GameState move_down(GameState old_state) {
  GameState moved = update_active_block(old_state, {
    old_state.active_block.position_x,
    old_state.active_block.position_y - 1,
    old_state.active_block.tetromino,
    old_state.active_block.rotation
  });

  if (is_legal_position(moved.field, moved.active_block)) {
    return moved;
  } else {
    Field field = add_block_to_field(old_state.field, old_state.active_block);
    FieldWithFilledLineCount fieldLines = remove_filled_lines(field);
    ActiveBlock active_block = next_active_block(old_state);
    return {
      fieldLines.first,
      active_block,
      Tetromino::O, // someday we'll have randomness
      old_state.milliseconds_per_turn,
      new_score(old_state.score, fieldLines.second),
      old_state.lines + fieldLines.second,
      is_legal_position(fieldLines.first, active_block)?
        GameProgress::IN_PROGRESS : GameProgress::GAME_OVER
    };
  }
}

GameState reduce(GameState state, Action action) {
  if (state.progress == GameProgress::GAME_OVER && action != Action::NEW_GAME) {
    return state;
  }
  switch(action) {
    case Action::NEW_GAME:
      return new_game(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    case Action::TIME_FALL:
      return move_down(state);

    case Action::MOVE_LEFT:
      return move_left(state);

    case Action::MOVE_RIGHT:
      return move_right(state);

    case Action::MOVE_DOWN:
      return move_down(state);

    case Action::ROTATE_CLOCKWISE:
      return rotate_clockwise(state);

    case Action::ROTATE_COUNTERCLOCKWISE:
      return rotate_counterclockwise(state);

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
      && lhs.tetromino == rhs.tetromino
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
