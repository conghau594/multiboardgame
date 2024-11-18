module;
#ifdef _DEBUG
import <iostream>;
#endif // DEBUG

export module chess.ItemPickable;

import foundation.String;
import foundation.SharedPointer;
import core.EntityView;
import game_controller.WaitingMouseButtonPressed;
import game_model.Tile;
import chess.WaitingItemPickingDone;
import chess.SDLChessGameController;
import chess.SDLChessGameService;
import chess.WaitingTurnTaken;

namespace mbg {
  export class ItemPickable : public WaitingMouseButtonPressed {
  public://types
    using Pointer = SharedPointer<ItemPickable>;

  public://functions
    virtual ~ItemPickable() = default;

    /***************************************************************************/
    String getID() override { return typeid(*this).name(); }


    /***************************************************************************/
    SDLChessGameService::Pointer getChessGameService() const {
      return std::static_pointer_cast<SDLChessGameService>(getGameService());
    }

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
      getChessGameController()->getGameView()->addView(
        hollow_square_,                //arg 1: EntityView::Pointer
        Game2DView::DECORATION_4  //arg 2: Game2DView::EntityViewType
      );

      if ( getChessGameService()->isCursorOnExpectedArea() ) {
        EntityView::Pointer entity = getChessGameService()->getEntityUnderCursor();
        if (isExpectedSquare(entity)) {
          outlineSquare(entity);
          return true;
        }
      }

      hollow_square_->parameters().setColumn(SDLChessGameController::EMPTY);

      return true;
    }

    /***************************************************************************/
    Boolean onExiting() override {
      getChessGameController()->getGameView()->removeView(hollow_square_);
      return true;
    }

    //game service functions
    /***************************************************************************/
    void outlineSquare(EntityView::Pointer entity) const {
      //outline entity by the hollow square.
      SDLEntityParams& hollowSquareParams = hollow_square_->parameters();

#ifdef _DEBUG1
      std::cout << "\n\n...void onMouseMove(EntityView::Pointer entity)...";
      std::cout << "\n\thollow square screen before modifying:";
      std::cout << "\n\tx = " << hollowSquareParams.screen_frame().x;
      std::cout << "\n\ty = " << hollowSquareParams.screen_frame().y;
      std::cout << "\n\tw = " << hollowSquareParams.screen_frame().w;
      std::cout << "\n\th = " << hollowSquareParams.screen_frame().h;
#endif
      hollowSquareParams.setColumn(SDLChessGameController::HOLLOW_SQUARE);

      hollowSquareParams.screen_frame() = getCenteredRectangle(
        hollowSquareParams.screen_frame().w,
        hollowSquareParams.screen_frame().h,
        static_cast<SDLEntityView*>(entity.get())->parameters().screen_frame()
      );

#ifdef _DEBUG1
      std::cout << "\n\thollow square screen after modifying:";
      std::cout << "\n\tx = " << hollow_square_->parameters().screen_frame().x;
      std::cout << "\n\ty = " << hollow_square_->parameters().screen_frame().y;
      std::cout << "\n\tw = " << hollow_square_->parameters().screen_frame().w;
      std::cout << "\n\th = " << hollow_square_->parameters().screen_frame().h;
#endif
    }

    /***************************************************************************/
    Boolean isExpectedSquare(EntityView::Pointer entity) const { ///< Checks if the `entity` is pick-able.
      //[x]TODO!043 Only the tile view having ally piece could be picked
      Tile::Pointer tileModel = std::static_pointer_cast<Tile>(entity->getModel());
      if (not tileModel->hasItem()) {
#ifdef _DEBUG1
        int numItems = static_cast<Tile*>(entity->getModel().get())->getNumberOfItems();
        std::cout << "\nnumber of items of square (model) (" 
          << static_cast<Tile*>(entity->getModel().get())->getRow() << ", "
          << static_cast<Tile*>(entity->getModel().get())->getCol() << ") is "
          << numItems;
#endif // _DEBUG
        return false;
      }

#ifdef _DEBUG1
      auto numItems = static_cast<Tile*>(entity->getModel().get())->getNumberOfItems();
      std::cout << "\nnumber of items of square (model) ("
        << static_cast<Tile*>(entity->getModel().get())->getRow() << ", "
        << static_cast<Tile*>(entity->getModel().get())->getCol() << ") is "
        << numItems;
#endif // _DEBUG
      Boolean test = getChessGameController()->getChessRule()->isAllySquare(tileModel);
      return getChessGameController()->getChessRule()->isAllySquare(tileModel);
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
      //[x]TODO!045 Disappear hollow square from screen
      getGameController()->popState();
    }

    /***************************************************************************/
    void onMouseMove(EntityView::Pointer entity) override {
      if (nullptr == entity || square_being_handled_ == entity)
        return;

      square_being_handled_ = std::static_pointer_cast<SDLEntityView>(entity); //just non-nullptr entity is memorized
      if (not isExpectedSquare(entity)) {
        hollow_square_->parameters().setColumn(SDLChessGameController::EMPTY);
        return;
      }

      outlineSquare(entity);
    }
    
    /***************************************************************************/
    void onMouseButtonPress(EntityView::Pointer entity) override {
      if (not isExpectedSquare(entity)) {
        //handleFailedPick(entity);
        return;
      }

      square_being_handled_ = std::static_pointer_cast<SDLEntityView>(entity);
      transitionToWaitingMouseButtonReleasedState();
    }

    /***************************************************************************/
    void onOtherEvent(EntityView::Pointer entity) { }


    //state transition functions
    /***************************************************************************/
    void transitionToWaitingMouseButtonReleasedState() {
      onExiting();
      getChessGameController()->pushState(WaitingItemPickingDone::createPointer(
        getChessGameService(),
        square_being_handled_
      ));
    }

    /***************************************************************************/
    void transitionToSubmissionDisabledState() {

    }

  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new ItemPickable(std::forward<ARGS>(args)...));
    }

  //protected://contructors
    ItemPickable(SDLChessGameService::Pointer boardService) 
      : WaitingMouseButtonPressed(boardService),
        hollow_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::HOLLOW_SQUARE))  //SDLChessGameController::DecorationType, should be changed to SDLChessGameController::HOLLOW_SQUARE 
    { }

  private://member functions
    using WaitingMouseButtonPressed::getGameController;
    using WaitingMouseButtonPressed::getGameService;
    
  private://data members
    SDLEntityView::Pointer square_being_handled_;
    SDLEntityView::Pointer hollow_square_;
  };
}