#include "state.h"

const char* blocks[] = {
  // I
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

  // J
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

  // L
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

  // O
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
  // S
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

  // T
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

  // Z
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

Shape parse_block(Tetromino tetromino, Rotation rotation) {
  int index = static_cast<int>(tetromino) * 4 + static_cast<int>(rotation);
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
  parse_block(Tetromino::I, Rotation::UNROTATED),
  parse_block(Tetromino::I, Rotation::CLOCKWISE),
  parse_block(Tetromino::I, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::I, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::J, Rotation::UNROTATED),
  parse_block(Tetromino::J, Rotation::CLOCKWISE),
  parse_block(Tetromino::J, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::J, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::L, Rotation::UNROTATED),
  parse_block(Tetromino::L, Rotation::CLOCKWISE),
  parse_block(Tetromino::L, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::L, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::O, Rotation::UNROTATED),
  parse_block(Tetromino::O, Rotation::CLOCKWISE),
  parse_block(Tetromino::O, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::O, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::S, Rotation::UNROTATED),
  parse_block(Tetromino::S, Rotation::CLOCKWISE),
  parse_block(Tetromino::S, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::S, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::T, Rotation::UNROTATED),
  parse_block(Tetromino::T, Rotation::CLOCKWISE),
  parse_block(Tetromino::T, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::T, Rotation::COUNTERCLOCKWISE),
  parse_block(Tetromino::Z, Rotation::UNROTATED),
  parse_block(Tetromino::Z, Rotation::CLOCKWISE),
  parse_block(Tetromino::Z, Rotation::UPSIDE_DOWN),
  parse_block(Tetromino::Z, Rotation::COUNTERCLOCKWISE),
};

Shape get_shape(Tetromino tetromino, Rotation rotation) {
  int index = static_cast<int>(tetromino) * 4 + static_cast<int>(rotation);
  return shapes[index];
}
