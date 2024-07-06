export module game_controller.BoardGameState;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;
import foundation.Vector;
import foundation.String;

import core.Stateful;
import core.Updatable;
import core.SceneController;
import core.EntityView;

import game_view.Game2DView;
import game_model.Game2DModel;
import game_controller.GameService;

namespace mbg {
  export class Game2DController;

  export class BoardGameState : public Object, public Updatable, public Stateful {
  public://types
    using Pointer = SharedPointer<BoardGameState>;

  public://member functions
    virtual ~BoardGameState() = default;

    virtual String getID() { return typeid(*this).name(); }
    
    SharedPointer<Game2DController> getGameController() const { 
      return game_ctrler_.lock(); }
    //GameService::Pointer getGameService() const { return board_service_; }

  protected:
    BoardGameState(
      SharedPointer<Game2DController> gameCtrler
    ) : game_ctrler_(gameCtrler)
    { }

  private:
    //GameService::Pointer board_service_;
    WeakPointer<Game2DController> game_ctrler_;
  };
}