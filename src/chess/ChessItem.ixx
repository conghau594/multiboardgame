export module chess.ChessItem;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;

import core.EntityView;
import game_model.Item;
import game_model.Tile;
import game_model.Player;

namespace mbg {
  export class ChessItem : public Item {
  public://types
    using Pointer = SharedPointer<ChessItem>;

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new ChessItem(std::forward<ARGS>(args)...));
    }

    Integer getMoveCount() const { return move_count_; }
    Tile::Pointer getTile() { return Item::getTile(0); }
    Tile::Pointer getTile(Integer i) { return Item::getTile(i); }
    Tile::Pointer getSquare() { return Item::getTile(0); }
    Player::Pointer getOwner() { return Item::getOwner(0); }
    Integer getIndexFromPlayer() const { return index_from_player_; }
    Integer getIndexFromManager() const { return index_from_player_; }
    
    Integer getRow() { return getTile()->getRow(); }
    Integer getCol() { return getTile()->getCol(); }

    void setIndexFromPlayer(Integer i) { index_from_player_ = i; }
    void setIndexFromManager(Integer i) { index_from_manager_ = i; }
    void setSquare(Tile::Pointer square) { setTile(0, square); }
    void increaseMoveCount() { ++move_count_; }

  protected:
    ChessItem() = default;
    ChessItem(ChessItem const&) = default;
    ChessItem(String name, Integer type, Double value)
      : Item(name, type, value)
    { }
    ChessItem(String name, Integer type, Double value, Integer moveCount, EntityView::Pointer view)
      : Item(name, type, value, view), 
        move_count_(moveCount)
    { }    
    
  private:
    Integer move_count_;
    Integer index_from_player_;
    Integer index_from_manager_;
  };
}