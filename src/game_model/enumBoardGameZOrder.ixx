export module game_model.BoardGameZOrder;

namespace mbg {
  export enum BoardGameZOrder {
    GAME_VIEW = 0,
    TILE_MAP,
    TILE,
    ITEM,

    BUTTON = TILE_MAP,
    TEXT_EDIT = TILE_MAP,

    BOARD = TILE_MAP,
    SQUARE,
    DECORATION,
    PIECE
  };
}