#include "catch.hpp"

#include "../src/state.h"

void check_identical_except_active_block(GameState lhs, GameState rhs) {
  CHECK(lhs.field == rhs.field);
  CHECK(lhs.next_block == rhs.next_block);
  CHECK(lhs.milliseconds_per_turn == rhs.milliseconds_per_turn);
  CHECK(lhs.score == rhs.score);
  CHECK(lhs.lines == rhs.lines);
  CHECK(lhs.progress == rhs.progress);
}

GameState default_game_with_active_block(ActiveBlock active_block) {
  return {
    {
      DEFAULT_HEIGHT,
      DEFAULT_WIDTH,
      std::vector<std::vector<CellState>>(DEFAULT_HEIGHT,
          std::vector<CellState>(DEFAULT_WIDTH, CellState::EMPTY))
    },
    active_block,
    Tetromino::T,
    1000,
    0,
    0,
    GameProgress::IN_PROGRESS,
    RNG(0)
  };
}

TEST_CASE("Can create new game", "[reducer]") {
  GameState state = reduce({}, Action::NEW_GAME);
  CHECK(state.field.height == DEFAULT_HEIGHT);
  CHECK(state.field.lines.size() == DEFAULT_HEIGHT);
  CHECK(state.field.width == DEFAULT_WIDTH);
  for (Line line : state.field.lines) {
    CHECK(line.size() == DEFAULT_WIDTH);
    for (CellState cell : line) {
      CHECK(cell == CellState::EMPTY);
    }
  }
}

TEST_CASE("Can move left", "[reducer]") {
  GameState new_game, moved_left;
  new_game = reduce(new_game, Action::NEW_GAME);
  moved_left = reduce(new_game, Action::MOVE_LEFT);

  check_identical_except_active_block(moved_left, new_game);
  CHECK(moved_left.active_block.position_x ==
        new_game.active_block.position_x - 1);
}

TEST_CASE("Cannot move left when against left wall", "[reducer]") {
  GameState state = default_game_with_active_block({
    0,
    DEFAULT_HEIGHT - 1,
    Tetromino::O,
    Rotation::UNROTATED
  });

  GameState moved_left = reduce(state, Action::MOVE_LEFT);

  REQUIRE(moved_left == state);
}

TEST_CASE("Cannot move left when against previously dropped block", "[reducer]") {
  GameState state = default_game_with_active_block({
    1,
    DEFAULT_HEIGHT - 1,
    Tetromino::O,
    Rotation::UNROTATED
  });
  state.field.lines[DEFAULT_HEIGHT - 1][0] = CellState::FILLED;

  GameState moved_left = reduce(state, Action::MOVE_LEFT);

  REQUIRE(moved_left == state);
}

TEST_CASE("Can move left fitting around previously dropped block", "[reducer]") {
  GameState state = default_game_with_active_block({
    1,
    DEFAULT_HEIGHT - 1,
    Tetromino::S,
    Rotation::UNROTATED
  });
  state.field.lines[DEFAULT_HEIGHT - 1][0] = CellState::FILLED;

  GameState moved_left = reduce(state, Action::MOVE_LEFT);

  check_identical_except_active_block(moved_left, state);
  CHECK(moved_left.active_block.position_x ==
        state.active_block.position_x - 1);
}

TEST_CASE("Can move right", "[reducer]") {
  GameState new_game, moved_right;
  new_game = reduce(new_game, Action::NEW_GAME);

  moved_right = reduce(new_game, Action::MOVE_RIGHT);

  check_identical_except_active_block(moved_right, new_game);
  CHECK(moved_right.active_block.position_x ==
        new_game.active_block.position_x + 1);
}

TEST_CASE("Cannot move right when against right wall", "[reducer]") {
  GameState state = default_game_with_active_block({
    DEFAULT_WIDTH - 2,
    DEFAULT_HEIGHT - 1,
    Tetromino::O,
    Rotation::UNROTATED
  });

  GameState moved_right = reduce(state, Action::MOVE_RIGHT);

  REQUIRE(moved_right == state);
}

TEST_CASE("Cannot move right when against previously dropped block", "[reducer]") {
  GameState state = default_game_with_active_block({
    DEFAULT_WIDTH - 3,
    DEFAULT_HEIGHT - 1,
    Tetromino::O,
    Rotation::UNROTATED
  });
  state.field.lines[DEFAULT_HEIGHT - 1][DEFAULT_WIDTH - 1] = CellState::FILLED;

  GameState moved_right = reduce(state, Action::MOVE_RIGHT);

  REQUIRE(moved_right == state);
}

TEST_CASE("Can move right fitting around previously dropped block", "[reducer]") {
  GameState state = default_game_with_active_block({
    DEFAULT_WIDTH - 4,
    DEFAULT_HEIGHT - 1,
    Tetromino::Z,
    Rotation::UNROTATED
  });
  state.field.lines[DEFAULT_HEIGHT - 1][DEFAULT_WIDTH - 1] = CellState::FILLED;

  GameState moved_right = reduce(state, Action::MOVE_RIGHT);

  check_identical_except_active_block(moved_right, state);
  CHECK(moved_right.active_block.position_x ==
        state.active_block.position_x + 1);
}

TEST_CASE("Can rotate clockwise", "[reducer]") {
  GameState new_game, rotated;
  new_game = reduce(new_game, Action::NEW_GAME);

  rotated = reduce(new_game, Action::ROTATE_CLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::CLOCKWISE);

  rotated = reduce(rotated, Action::ROTATE_CLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::UPSIDE_DOWN);

  rotated = reduce(rotated, Action::ROTATE_CLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::COUNTERCLOCKWISE);

  rotated = reduce(rotated, Action::ROTATE_CLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated == new_game);
}

TEST_CASE("Cannot rotate clockwise if surrounded", "[reducer]") {
  const int initial_x = 5;
  GameState state = default_game_with_active_block({
    initial_x,
    DEFAULT_HEIGHT - 1,
    Tetromino::I,
    Rotation::CLOCKWISE
  });
  for (int y = 0; y < DEFAULT_HEIGHT; y++) {
    for (int x = 0; x < DEFAULT_WIDTH; x++) {
      if (x != initial_x) {
        state.field.lines[y][x] = CellState::FILLED;
      }
    }
  }

  GameState rotated = reduce(state, Action::ROTATE_CLOCKWISE);

  REQUIRE(rotated == state);
}

TEST_CASE("Cannot rotate clockwise if up against wall", "[reducer]") {
  GameState state = default_game_with_active_block({
    DEFAULT_WIDTH - 1,
    DEFAULT_HEIGHT - 1,
    Tetromino::I,
    Rotation::CLOCKWISE
  });

  GameState rotated = reduce(state, Action::ROTATE_CLOCKWISE);

  REQUIRE(rotated == state);
}

TEST_CASE("Can rotate counterclockwise", "[reducer]") {
  GameState new_game, rotated;
  new_game = reduce(new_game, Action::NEW_GAME);

  rotated = reduce(new_game, Action::ROTATE_COUNTERCLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::COUNTERCLOCKWISE);

  rotated = reduce(rotated, Action::ROTATE_COUNTERCLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::UPSIDE_DOWN);

  rotated = reduce(rotated, Action::ROTATE_COUNTERCLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated.active_block.rotation == Rotation::CLOCKWISE);

  rotated = reduce(rotated, Action::ROTATE_COUNTERCLOCKWISE);

  check_identical_except_active_block(rotated, new_game);
  CHECK(rotated == new_game);
}

TEST_CASE("Cannot rotate counterclockwise if surrounded", "[reducer]") {
  const int initial_x = 5;
  GameState state = default_game_with_active_block({
    initial_x,
    DEFAULT_HEIGHT - 1,
    Tetromino::I,
    Rotation::CLOCKWISE
  });
  for (int y = 0; y < DEFAULT_HEIGHT; y++) {
    for (int x = 0; x < DEFAULT_WIDTH; x++) {
      if (x != initial_x) {
        state.field.lines[y][x] = CellState::FILLED;
      }
    }
  }

  GameState rotated = reduce(state, Action::ROTATE_COUNTERCLOCKWISE);

  REQUIRE(rotated == state);
}

TEST_CASE("Cannot rotate counterclockwise if up against wall", "[reducer]") {
  GameState state = default_game_with_active_block({
    DEFAULT_WIDTH - 1,
    DEFAULT_HEIGHT - 1,
    Tetromino::I,
    Rotation::CLOCKWISE
  });

  GameState rotated = reduce(state, Action::ROTATE_COUNTERCLOCKWISE);

  REQUIRE(rotated == state);
}

TEST_CASE("Can move down", "[reducer]") {
  GameState new_game, moved;
  new_game = reduce(new_game, Action::NEW_GAME);

  moved = reduce(new_game, Action::MOVE_DOWN);

  check_identical_except_active_block(moved, new_game);
  CHECK(moved.active_block.position_x == new_game.active_block.position_x);
  CHECK(moved.active_block.position_y == new_game.active_block.position_y - 1);
  CHECK(moved.active_block.tetromino == new_game.active_block.tetromino);
  CHECK(moved.active_block.rotation == new_game.active_block.rotation);
}


TEST_CASE("Can fall down", "[reducer]") {
  GameState new_game, moved;
  new_game = reduce(new_game, Action::NEW_GAME);

  moved = reduce(new_game, Action::TIME_FALL);

  check_identical_except_active_block(moved, new_game);
  CHECK(moved.active_block.position_x == new_game.active_block.position_x);
  CHECK(moved.active_block.position_y == new_game.active_block.position_y - 1);
  CHECK(moved.active_block.tetromino == new_game.active_block.tetromino);
  CHECK(moved.active_block.rotation == new_game.active_block.rotation);
}

TEST_CASE("Can move down to deposit block", "[reducer]") {
  GameState state = default_game_with_active_block({
    0,
    0,
    Tetromino::I,
    Rotation::UNROTATED
  });

  GameState moved = reduce(state, Action::MOVE_DOWN);

  CHECK(moved.field.lines[0][0] == CellState::FILLED);
  CHECK(moved.field.lines[0][1] == CellState::FILLED);
  CHECK(moved.field.lines[0][2] == CellState::FILLED);
  CHECK(moved.field.lines[0][3] == CellState::FILLED);
  CHECK(moved.field.lines[0][4] == CellState::EMPTY);
}

TEST_CASE("Can fall down to complete line", "[reducer]") {
  GameState state = default_game_with_active_block({
    0,
    0,
    Tetromino::I,
    Rotation::UNROTATED
  });
  for (int x = 4; x < DEFAULT_WIDTH; x++) {
    state.field.lines[0][x] = CellState::FILLED;
  }

  GameState moved = reduce(state, Action::TIME_FALL);

  GameState expected = default_game_with_active_block({
    DEFAULT_WIDTH / 2,
    DEFAULT_HEIGHT - 1,
    state.next_block,
    Rotation::UNROTATED
  });

  CHECK(moved.field == expected.field); // empty field
  for (int x = 0; x < DEFAULT_WIDTH; x++) {
    CHECK(moved.field.lines[0][x] == CellState::EMPTY);
  }
  CHECK(moved.active_block == expected.active_block);
  CHECK(moved.lines == 1);
}

TEST_CASE("New game starts out in progress", "[reducer]") {
  GameState state = reduce({}, Action::NEW_GAME);

  REQUIRE(state.progress == GameProgress::IN_PROGRESS);
}

TEST_CASE("Game ends when new piece is blocked", "[reducer]") {
  // All but the last line and last column is full
  GameState state = reduce({}, Action::NEW_GAME);
  state.active_block.tetromino = Tetromino::I;
  for (int y = 0; y < state.field.height - 1; y++) {
    for (int x = 0; x < state.field.width - 1; x++) {
      state.field.lines[y][x] = CellState::FILLED;
    }
  }

  state = reduce(state, Action::TIME_FALL);

  REQUIRE(state.progress == GameProgress::GAME_OVER);
}

TEST_CASE("Random number sequence of seed 0", "[exploratory]") {
  RNG rng = RNG(0);
  std::uniform_int_distribution<int> distribution(0, TETROMINO_COUNT - 1);

  CHECK(distribution(rng) == 3);
  CHECK(distribution(rng) == 4);
  CHECK(distribution(rng) == 5);
  CHECK(distribution(rng) == 5);
  CHECK(distribution(rng) == 4);
  CHECK(distribution(rng) == 6);
  CHECK(distribution(rng) == 3);
  CHECK(distribution(rng) == 5);
}

GameState fall_until_new_block(GameState original) {
  GameState next = original;
  while (original.field == next.field) {
    next = reduce(next, Action::TIME_FALL);
  }
  return next;
}

TEST_CASE("Next block is chosen randomly (and stably)", "[reducer]") {
  GameState prev = default_game_with_active_block({
    0,
    0,
    Tetromino::I,
    Rotation::UNROTATED
  });

  GameState next = fall_until_new_block(prev);
  CHECK(next.next_block == Tetromino::O);

  next = fall_until_new_block(next);
  CHECK(next.next_block == Tetromino::S);

  next = fall_until_new_block(next);
  CHECK(next.next_block == Tetromino::T);

  next = fall_until_new_block(next);
  CHECK(next.next_block == Tetromino::T);

  next = fall_until_new_block(next);
  CHECK(next.next_block == Tetromino::S);

  next = fall_until_new_block(next);
  CHECK(next.next_block == Tetromino::Z);
}
