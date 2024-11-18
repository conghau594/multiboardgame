export module game_controller.GameService;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;
import core.EntityView;

namespace mbg {
  export class Game2DController;

  export class GameService : public Object {
  public:
    using Pointer = SharedPointer<GameService>;

    SharedPointer<Game2DController> getGameController() const { return game_ctrler_.lock(); }
    virtual ~GameService() = default;

    //graphic services
    virtual Boolean isCursorOnExpectedArea() = 0;

    virtual EntityView::Pointer getEntityUnderCursor() = 0;
    
    virtual Boolean isMouseMoved() = 0;

    virtual Boolean isMouseButtonPressed() = 0;

    virtual Boolean isMouseButtonReleased() = 0;

    //logic game services
    virtual Boolean isTimeout() = 0;

    virtual Boolean doesSomeoneResign() = 0;

    virtual Boolean isTurnTaken() = 0;

    virtual Boolean shouldSubmit() = 0;

  //protected://contructors
    GameService(SharedPointer<Game2DController> gameCtrler)
      : game_ctrler_(gameCtrler)
    { }

  private://data members
    WeakPointer<Game2DController> game_ctrler_;
  };
}