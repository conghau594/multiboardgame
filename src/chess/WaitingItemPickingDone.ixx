export module chess.WaitingItemPickingDone;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;

import core.EntityView;
import game_model.Tile;
import game_controller.WaitingMouseButtonReleased;
import chess.SDLChessGameController;
import chess.ItemMovable;
import chess.WaitingBoardFocused;
import chess.WaitingTurnTaken;


namespace mbg {
  export class WaitingItemPickingDone : public WaitingMouseButtonReleased {
  public://types
    using Pointer = SharedPointer<WaitingItemPickingDone>;

  public://functions
    virtual ~WaitingItemPickingDone() = default;

    /***************************************************************************/
    String getID() override { return typeid(*this).name(); }

    /***************************************************************************/
    SDLChessGameController::Pointer getChessGameController() const {
      return std::static_pointer_cast<SDLChessGameController>(getGameController());
    }

    /***************************************************************************/
    WaitingTurnTaken::Pointer getInitialState() const {
      return std::static_pointer_cast<WaitingTurnTaken>(getChessGameController()->getInitialState());
    }

    //onEnter, onExit
    /***************************************************************************/
    Boolean onEntering() override {
      getInitialState()->disappearSolidSquares();
      getChessGameController()->getGameView()->addView(
        solid_square_,                 //arg 1: EntityView::Pointer
        Game2DView::DECORATION_3  //arg 2: Game2DView::EntityViewType
      );

      return true;
    }

    /***************************************************************************/
    Boolean onExiting() override {
      return true;
    }

    //game service function
    /***************************************************************************/
    Boolean isExpectedSquare(EntityView::Pointer entity) {
      return entity == square_being_handled_;
        //&& static_cast<Tile*>(entity->getModel().get())->hasItem();
    }

    //handle event functions
    /***************************************************************************/
    void onTimeout() override {
      transitionToSubmissionDisabledState();
    }

    /***************************************************************************/
    void onEnemyResign() override {
      transitionToSubmissionDisabledState();
    }

    /***************************************************************************/
    void onCursorLeaveBoard() override {
      //[x]TODO!046 Disappear purple square from screen
      getChessGameController()->getGameView()->removeView(solid_square_);
      getGameController()->popState();
      getGameController()->getCurrentState()->onEntering();
    }

    /***************************************************************************/
    void onMouseMove(EntityView::Pointer entity) override { } ///< does nothing

    /***************************************************************************/
    void onMouseButtonRelease(EntityView::Pointer entity) override {
      if (not isExpectedSquare(entity)) {
        getInitialState()->displaySolidSquares();
        getChessGameController()->getGameView()->removeView(solid_square_);

        getChessGameController()->popState();  //back to the previous ItemPickable state
        getChessGameController()->getCurrentState()->onEntering();
      }
      else if (getGameService()->shouldSubmit()) {
        transitionToSubmissionDisabledState();
      }
      else {
        //handle successfull pick and transition to next state

        transitionToNextEntityPickableState();
      }
    }

    /***************************************************************************/
    void onOtherEvent(EntityView::Pointer entity) override { } ///< Does nothing


    //state transition functions
    /***************************************************************************/
    void transitionToNextEntityPickableState() {
      WaitingBoardFocused::Pointer nextState = WaitingBoardFocused::createPointer(
        std::static_pointer_cast<SDLChessGameService>(getGameService())
      );

      nextState->setNextState(ItemMovable::createPointer(
        getGameService(),
        square_being_handled_,
        solid_square_
      ));

      getChessGameController()->changeState(nextState);
      nextState->update();
    }

    /***************************************************************************/
    void transitionToSubmissionDisabledState() {

    }


  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new WaitingItemPickingDone(std::forward<ARGS>(args)...));
    }

  protected://contructors
    WaitingItemPickingDone(
      GameService::Pointer boardService, 
      SDLEntityView::Pointer squareBeingHandled
    ) : WaitingMouseButtonReleased(boardService),
        square_being_handled_(squareBeingHandled),
        solid_square_(getChessGameController()->createDecorationOfSquare(
          SDLChessGameController::SOLID_SQUARE   //SDLChessGameController::DecorationType
        ))
    { 
      
      solid_square_->parameters().screen_frame() = getCenteredRectangle(
        solid_square_->parameters().screen_frame().w,
        solid_square_->parameters().screen_frame().h,
        square_being_handled_->parameters().screen_frame()
      );
    }

  private://member functions
    using WaitingMouseButtonReleased::getGameController;

  private://data members
    SDLEntityView::Pointer solid_square_;
    SDLEntityView::Pointer square_being_handled_;
  };
}