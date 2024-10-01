export module game_controller.WaitingMouseButtonReleased;

import foundation.SharedPointer;
//import foundation.TypeInfo;
import foundation.String;
import core.MouseEventManager;
import core.EntityView;

import game_controller.GameService;
import game_controller.BoardGameState;

namespace mbg {
  /*****************************************************************************
   * This state needs to handle 4 events:   
   *****************************************************************************/
  export class WaitingMouseButtonReleased : public BoardGameState {
  public://type
    using Pointer = SharedPointer<WaitingMouseButtonReleased>;

  public://member functions
    virtual ~WaitingMouseButtonReleased() = default;

    String getID() override { return typeid(*this).name(); }
    GameService::Pointer getGameService() const { return board_service_; }

    inline void update() override;
    void render() override { }

    virtual void onTimeout() = 0;
    virtual void onEnemyResign() = 0;
    virtual void onCursorLeaveBoard() = 0;
    virtual void onMouseMove(EntityView::Pointer entity) = 0;
    virtual void onMouseButtonRelease(EntityView::Pointer entity) = 0;
    virtual void onOtherEvent(EntityView::Pointer entity) = 0;

  protected:
    WaitingMouseButtonReleased(
      GameService::Pointer boardService
    ) : BoardGameState(boardService->getGameController()),
        board_service_(boardService)
    { }

  private:
    GameService::Pointer board_service_;
  };


  inline void WaitingMouseButtonReleased::update() {
    if ( getGameService()->doesSomeoneResign() || getGameService()->isTimeout() ) {
      if (getGameService()->doesSomeoneResign()) {
        onEnemyResign();
      }

      if (getGameService()->isTimeout()) {
        onTimeout();
      }
      return;
    }
    
    if (not getGameService()->isCursorOnExpectedArea()) {
      onCursorLeaveBoard();
      return;
    }

    EntityView::Pointer entity = getGameService()->getEntityUnderCursor();
    if( getGameService()->isMouseMoved() ){
      onMouseMove(entity);
    }
    else if ( getGameService()->isMouseButtonReleased() ) {
      onMouseButtonRelease(entity);
    }
    else {
      onOtherEvent(entity);
    }
  } 
}