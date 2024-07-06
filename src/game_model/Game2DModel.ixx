export module game_model.Game2DModel;

import foundation.SharedPointer;
//import foundation.PlayerTurn;
import core.SceneModel;
import core.EntityModel;

import game_model.TileMap;
import game_model.ItemManager;
import game_model.PlayerManager;
import game_model.Rule;

namespace mbg {
  export class Game2DModel : public EntityModel, public SceneModel {
  public://types
    using Pointer = SharedPointer<Game2DModel>;

  public://member functions
    TileMap::Pointer getTileMap() const { return tile_map_; }
    ItemManager::Pointer getItemManager() const { return item_mgr_; }
    PlayerManager::Pointer getPlayerManager() const { return player_mgr_; }
    Rule::Pointer getRule() const { return rule_; }
    //PlayerTurn getPlayerTurn() const { return turn_; }

    //void setPlayerTurn(PlayerTurn turn) { turn_ = turn; }
    void setRule(Rule::Pointer rule) { rule_ = rule; }


  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new Game2DModel(std::forward<ARGS>(args)...));
    }

  protected://contructors
    Game2DModel(Rule::Pointer rule) : rule_(rule) 
    { }

    Game2DModel(
      EntityView::Pointer view,
      TileMap::Pointer tileMap,
      ItemManager::Pointer itemMgr,
      PlayerManager::Pointer playerMgr,
      Rule::Pointer rule
    ) : EntityModel(view), tile_map_(tileMap), item_mgr_(itemMgr), 
        player_mgr_(playerMgr), rule_(rule)//, turn_(PlayerTurn::FIRST)
    { }

  private://data members
    TileMap::Pointer tile_map_;
    ItemManager::Pointer item_mgr_;
    PlayerManager::Pointer player_mgr_;
    Rule::Pointer rule_;
    //PlayerTurn turn_;
  };
}