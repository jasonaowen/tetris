#include "catch.hpp"

#include "../src/state.h"

TEST_CASE( "Can create new game", "[reducer]" ) {
  GameState state;
  state = reduce(state, Action::NEW_GAME);
  CHECK( state.field.height == DEFAULT_HEIGHT );
  CHECK( state.field.lines.size() == DEFAULT_HEIGHT );
  CHECK( state.field.width == DEFAULT_WIDTH );
  for (Line line : state.field.lines) {
    CHECK( line.size() == DEFAULT_WIDTH );
    for (CellState cell : line) {
      CHECK( cell == CellState::EMPTY );
    }
  }
}
