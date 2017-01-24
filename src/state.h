#pragma once

#include <vector>

enum class CellState {
  EMPTY,
  FILLED
};

enum class Tetrimino {
  TETRIMINO_I, /* xxxx */
  TETRIMINO_J, /* xxx
                    x  */
  TETRIMINO_L, /* xxx
                  x    */
  TETRIMINO_O, /*  xx
                   xx  */
  TETRIMINO_S, /*  xx
                  xx   */
  TETRIMINO_T, /* xxx
                   x   */
  TETRIMINO_Z  /* xx
                   xx  */
};

enum class Rotation {
  UNROTATED,
  CLOCKWISE,
  UPSIDE_DOWN,
  COUNTERCLOCKWISE
};

typedef std::vector<std::vector<CellState>> Shape;
Shape get_shape(Tetrimino tetrimino, Rotation rotation);

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
  Tetrimino tetrimino;
  Rotation rotation;
};

struct GameState {
  Field field;
  ActiveBlock active_block;
  Tetrimino next_block;
  int milliseconds_per_turn;
  int score;
  int lines;
};

enum class Action {
  NEW_GAME,
  TIME_FALL,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_DOWN,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTERCLOCKWISE
};

GameState reduce(GameState state, Action action);
bool operator==(const Field& lhs, const Field& rhs);
bool operator==(const ActiveBlock& lhs, const ActiveBlock& rhs);
bool operator==(const GameState& lhs, const GameState& rhs);
