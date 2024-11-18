export module game_model.TileMap;

import foundation.basetypes;
import foundation.Vector;
import foundation.Matrix;
import foundation.SharedPointer;
import core.EntityModel;
import game_model.Tile;

namespace mbg {
  export class TileMap : public EntityModel {
  public://types
    using Pointer = SharedPointer<TileMap>;

  public://member functions
    virtual ~TileMap() = default;

    Vector<Tile::Pointer>& operator[](Integer row) { return tiles_[row]; }
    Tile::Pointer getTile(Integer row, Integer col) { return tiles_[row][col]; }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new TileMap(std::forward<ARGS>(args)...));
    }

  protected://contructors
    TileMap(Integer row, Integer column) 
      : tiles_(row)
    {
      for (Integer i = 0; i < row; ++i) {
        for (Integer j = 0; j < column; ++j) {
          tiles_[i].emplace_back(Tile::createPointer(i, j));  //row i, column j
        }
      }
    }

  private://data members
    Matrix<Tile::Pointer> tiles_;//row first, col following
  };
}