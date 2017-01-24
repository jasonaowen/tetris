#include "state.h"

const char* blocks[] = {
  // TETRIMINO_I
  "****"
  "    "
  "    "
  "    ",
  "*   "
  "*   "
  "*   "
  "*   ",
  "****"
  "    "
  "    "
  "    ",
  "*   "
  "*   "
  "*   "
  "*   ",

  // TETRIMINO_J
  "*** "
  "  * "
  "    "
  "    ",
  " *  "
  " *  "
  "**  "
  "    ",
  "*   "
  "*** "
  "    "
  "    ",
  "**  "
  "*   "
  "*   "
  "    ",

  // TETRIMINO_L
  "*** "
  "*   "
  "    "
  "    ",
  "**  "
  " *  "
  " *  "
  "    ",
  "  * "
  "*** "
  "    "
  "    ",
  "*   "
  "*   "
  "**  "
  "    ",

  // TETRIMINO_O
  "**  "
  "**  "
  "    "
  "    ",
  "**  "
  "**  "
  "    "
  "    ",
  "**  "
  "**  "
  "    "
  "    ",
  "**  "
  "**  "
  "    "
  "    ",
  // TETRIMINO_S
  " ** "
  "**  "
  "    "
  "    ",
  "*   "
  "**  "
  " *  "
  "    ",
  " ** "
  "**  "
  "    "
  "    ",
  "*   "
  "**  "
  " *  "
  "    ",

  // TETRIMINO_T
  "*** "
  " *  "
  "    "
  "    ",
  " *  "
  "**  "
  " *  "
  "    ",
  " *  "
  "*** "
  "    "
  "    ",
  "*   "
  "**  "
  "*   "
  "    ",

  // TETRIMINO_Z
  "**  "
  " ** "
  "    "
  "    ",
  " *  "
  "**  "
  "*   "
  "    ",
  "**  "
  " ** "
  "    "
  "    ",
  " *  "
  "**  "
  "*   "
  "    ",
};

Shape parse_block(Tetrimino tetrimino, Rotation rotation) {
  int index = static_cast<int>(tetrimino) * 4 + static_cast<int>(rotation);
  const char* block = blocks[index];
  std::vector<std::vector<CellState>> shape = {
    {CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY},
    {CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY},
    {CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY},
    {CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY}
  };
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      if (block[4 * y + x] == '*') {
        shape[y][x] = CellState::FILLED;
      }
    }
  }

  return shape;
}

std::vector<Shape> shapes = {
  parse_block(Tetrimino::TETRIMINO_I, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_I, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_I, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_I, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_J, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_J, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_J, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_J, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_L, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_L, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_L, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_L, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_O, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_O, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_O, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_O, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_S, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_S, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_S, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_S, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_T, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_T, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_T, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_T, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_Z, Rotation::UNROTATED),
  parse_block(Tetrimino::TETRIMINO_Z, Rotation::CLOCKWISE),
  parse_block(Tetrimino::TETRIMINO_Z, Rotation::UPSIDE_DOWN),
  parse_block(Tetrimino::TETRIMINO_Z, Rotation::COUNTERCLOCKWISE),
};

Shape get_shape(Tetrimino tetrimino, Rotation rotation) {
  int index = static_cast<int>(tetrimino) * 4 + static_cast<int>(rotation);
  return shapes[index];
}
