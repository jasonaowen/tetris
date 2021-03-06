#pragma once

#include <random>
#include <vector>

enum class CellState {
  EMPTY,
  FILLED
};

const int MAX_TETROMINO_WIDTH = 4;
const int MAX_TETROMINO_HEIGHT = 4;

enum class Tetromino {
  I, /* xxxx */
  J, /* xxx
          x  */
  L, /* xxx
        x    */
  O, /*  xx
         xx  */
  S, /*  xx
        xx   */
  T, /* xxx
         x   */
  Z  /* xx
         xx  */
};
const int TETROMINO_COUNT = 7;

enum class Rotation {
  UNROTATED,
  CLOCKWISE,
  UPSIDE_DOWN,
  COUNTERCLOCKWISE
};

Rotation rotate_clockwise(Rotation);
Rotation rotate_counterclockwise(Rotation);

typedef std::vector<std::vector<CellState>> Shape;
Shape get_shape(Tetromino tetromino, Rotation rotation);

typedef std::vector<CellState> Line;

struct Field {
  int height;
  int width;
  std::vector<Line> lines;
};

const int DEFAULT_WIDTH = 10;
const int DEFAULT_HEIGHT = 20;

struct ActiveBlock {
  int position_x; // position of left edge of block
  int position_y; // position of top edge of block
  Tetromino tetromino;
  Rotation rotation;
};

enum class GameProgress {
  IN_PROGRESS,
  GAME_OVER
};

typedef std::mt19937 RNG;

struct GameState {
  Field field;
  ActiveBlock active_block;
  Tetromino next_block;
  int milliseconds_per_turn;
  int score;
  int lines;
  GameProgress progress;
  RNG rng;
};

enum class Action {
  NO_ACTION,
  QUIT,
  NEW_GAME,
  TIME_FALL,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_DOWN,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTERCLOCKWISE
};

const char* get_action_name(Action);

GameState reduce(GameState state, Action action);
bool operator==(const Field& lhs, const Field& rhs);
bool operator==(const ActiveBlock& lhs, const ActiveBlock& rhs);
bool operator==(const GameState& lhs, const GameState& rhs);
