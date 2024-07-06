module;
#ifdef _DEBUG
import <iostream>;
#endif

export module game_controller.WaitingMouseButtonPressed;

import foundation.basetypes;
import foundation.SharedPointer;
//import foundation.TypeInfo;
import foundation.String;
import core.MouseEventManager;
import core.EntityView;

import game_view.Game2DView;
import game_model.Game2DModel;

import game_controller.BoardGameState;
import game_controller.GameService;
import game_controller.SubmissionDisabled;
import game_controller.WaitingMouseButtonReleased;

#ifdef _DEBUG
import sdl_display.SDLEntityView;
import sdl_display.SDLRectangle;
#endif

namespace mbg {
  export class WaitingMouseButtonPressed : public BoardGameState {
  public://type
    using Pointer = SharedPointer<WaitingMouseButtonPressed>;

  public://member functions
    virtual ~WaitingMouseButtonPressed() = default;

    GameService::Pointer getGameService() const { return board_service_; }
    String getID() override { return typeid(*this).name(); }

    inline void update() override;
    void render() override { }
    //Boolean onEntering() override { return true; }
    //Boolean onExiting() override { return true; }


    virtual void onTimeout() = 0;
    virtual void onEnemyResign() = 0;
    virtual void onCursorLeaveBoard() = 0;
    virtual void onMouseMove(EntityView::Pointer entity) = 0;
    virtual void onMouseButtonPress(EntityView::Pointer entity) = 0;
    virtual void onOtherEvent(EntityView::Pointer entity) = 0;

  protected:
    WaitingMouseButtonPressed(
      GameService::Pointer gameService
    ) : BoardGameState(gameService->getGameController()),
        board_service_(gameService)
    { }

  private:
    GameService::Pointer board_service_;
  };

  /***************************************************************************
   * Updates with 4 events: 1st is mouse move event, 2nd is mouse left button
   * press event, 3rd is user resign event, 4th is timeout event.
   * 
   * - 1st: if mouse cursor moves to pickable tile/item then this tile/item 
   *        is outlined.
   * - 2nd: if left button is pressed at the pickable tile then the enabled
   *        board state moves on to next substate: WaitingItemPicking, else 
   *        do nothing.
   * - 3rd, 4th: if a user resigns or if it is timeout, the game controller  
   *        moves on to next state: SubmissionDisabled.
   * 
   ***************************************************************************/
  inline void WaitingMouseButtonPressed::update() {
    if (getGameService()->doesSomeoneResign()) {
      onEnemyResign();

      if (getGameService()->isTimeout()) {
        onTimeout();
      }
      return;
    }
    
    if (getGameService()->isTimeout()) {
      onTimeout();
      return;
    }

    if ( not getGameService()->isCursorOnExpectedArea() ) {
      onCursorLeaveBoard();
      return;
    }
    
    EntityView::Pointer entity = getGameService()->getEntityUnderCursor();
    
#ifdef _DEBUG1
    std::cout << "\n\nvoid WaitingMouseButtonPressed::update()...";
    std::cout << "\nentity under cursor = " << entity;
    SDLRectangle& entityParams = static_cast<SDLEntityView*>(entity.get())->parameters().screen_frame();
    std::cout << "\nParameters:";
    std::cout << "\n\tx = " << entityParams.x;
    std::cout << "\n\ty = " << entityParams.y;
    std::cout << "\n\tw = " << entityParams.w;
    std::cout << "\n\th = " << entityParams.h;
#endif // _DEBUG

    if ( getGameService()->isMouseMoved() ) {
      onMouseMove(entity);
    } 
    else if ( getGameService()->isMouseButtonPressed() ) {
      onMouseButtonPress(entity);
    }
    else {
      onOtherEvent(entity);
    }
  }
}