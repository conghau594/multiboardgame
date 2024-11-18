module; 
#ifdef _DEBUG
import <iostream>;
#endif // DEBUG

export module chess.ItemMovable;

import foundation.Matrix;
import foundation.String;
import foundation.SharedPointer;
import core.EntityView;

import game_model.Tile;
import game_controller.WaitingMouseButtonPressed;
import chess.WaitingItemMovingDone;
import chess.SDLChessGameController;
import chess.SDLChessGameService;
import chess.ChessItem;
import chess.WaitingTurnTaken;


namespace mbg {
  export class ItemMovable : public WaitingMouseButtonPressed {
  public://types
    using Pointer = SharedPointer<ItemMovable>;

  public://functions
    virtual ~ItemMovable() = default;

    /***************************************************************************/
    String getID() override { return typeid(*this).name(); }

    /***************************************************************************/
    SDLChessGameController::Pointer getChessGameController() const {
      return std::static_pointer_cast<SDLChessGameController>(getGameController());
    }

    /***************************************************************************/
    SDLChessGameService::Pointer getChessGameService() const {
      return std::static_pointer_cast<SDLChessGameService>(getGameService());
    }

    /***************************************************************************/
    WaitingTurnTaken::Pointer getInitialState() const {
      return std::static_pointer_cast<WaitingTurnTaken>(getChessGameController()->getInitialState());
    }

    //onEnter, onExit
    /***************************************************************************/
    Boolean onEntering() override {
      SquareGroups groupsOfPlaceableSquares = getChessGameController()->getChessRule()->pick(
        std::static_pointer_cast<ChessItem>(picked_piece_->getModel())
      );

      if (groupsOfPlaceableSquares[0].empty() && groupsOfPlaceableSquares[1].empty()) {
        getChessGameController()->popSeveralLastStates(1);     
        getChessGameController()->getCurrentState()->onEntering();
        getChessGameController()->getGameView()->removeView(solid_square_);
        getInitialState()->displaySolidSquares();
        return false;
      }

      addDecorationViews(groupsOfPlaceableSquares);

      getChessGameController()->getGameView()->removeView(picked_piece_);
      getChessGameController()->getGameView()->addView(picked_piece_, Game2DView::DECORATION_6);

      SDLRectangle& pieceScreenFrame = picked_piece_->parameters().screen_frame();
      pieceScreenFrame.x = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorX() - pieceScreenFrame.w / 2;
      pieceScreenFrame.y = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorY() - pieceScreenFrame.h / 2;

      getChessGameController()->getGameView()->addView(
        hollow_square_,           //arg 1: EntityView::Pointer
        Game2DView::DECORATION_4  //arg 2: Game2DView::EntityViewType
      );

      if (getChessGameService()->isCursorOnExpectedArea()) {
        EntityView::Pointer entity = getChessGameService()->getEntityUnderCursor();
        if (isExpectedSquare(entity))
          outlineSquare(entity);
        else {
          hollow_square_->parameters().setColumn(SDLChessGameController::EMPTY);
        }
      }
      else {
        hollow_square_->parameters().setColumn(SDLChessGameController::EMPTY);
      }

      return true;
    }

    /***************************************************************************/
    Boolean onExiting() override {
      getChessGameController()->getGameView()->clearViewsFromAnchor(Game2DView::DECORATION_4);
      return true;
    }

    //game service functions
    /***************************************************************************/


    /***************************************************************************/
    void outlineSquare(EntityView::Pointer entity) const {
      //outline entity by the hollow square.
      SDLEntityParams& hollowSquareParams = hollow_square_->parameters();

      hollowSquareParams.setColumn(SDLChessGameController::HOLLOW_SQUARE);
      hollowSquareParams.screen_frame() = getCenteredRectangle(
        hollowSquareParams.screen_frame().w,
        hollowSquareParams.screen_frame().h,
        static_cast<SDLEntityView*>(entity.get())->parameters().screen_frame()
      );
    }


    /***************************************************************************/
    Boolean isExpectedSquare(EntityView::Pointer entity) { ///< Checks if the `entity` is pick-able.
      //[x]TODO!047 Only the tile view that the picked piece can be placed
#ifdef _DEBUG1
      std::cout << "\n\n...Boolean ItemMovable::isExpectedSquare(EntityView::Pointer entity)...";

      std::cout << "\nmouse tile.r = " << static_cast<Tile*>(entity->getModel().get())->getRow()
        << "\nmouse tile.c = " << static_cast<Tile*>(entity->getModel().get())->getCol();

      std::cout << "\nstart tile.x = " << (static_cast<Item*>(picked_piece_->getModel().get())->getTile(0).get())->getRow()
        << "\nstart tile.y = " << (static_cast<Item*>(picked_piece_->getModel().get())->getTile(0).get())->getCol();
#endif // _DEBUG

      return getChessGameController()->getChessRule()->checkMove(
        std::static_pointer_cast<ChessItem>(picked_piece_->getModel()),
        static_cast<Tile*>(entity->getModel().get())->getRow(),
        static_cast<Tile*>(entity->getModel().get())->getCol()
      );
    }
    
    /***************************************************************************/
    void returnPickedPieceView() {
      getChessGameController()->getGameView()->removeView(picked_piece_);
      getChessGameController()->getGameView()->addView(picked_piece_, Game2DView::ITEM);

      picked_piece_->parameters().screen_frame() = getCenteredRectangle(
        picked_piece_->parameters().screen_frame().w,
        picked_piece_->parameters().screen_frame().h,
        picked_square_->parameters().screen_frame()
      );
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
      //[x]TODO!048 back to the previous state
      returnPickedPieceView();
      getGameController()->popState();
    }

    /***************************************************************************/
    void onMouseMove(EntityView::Pointer entity) override {
      SDLRectangle& pieceScreenFrame = picked_piece_->parameters().screen_frame();
      pieceScreenFrame.x = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorX() - pieceScreenFrame.w / 2;
      pieceScreenFrame.y = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorY() - pieceScreenFrame.h / 2;

      if (nullptr == entity || square_being_handled_ == entity)
        return;

      Integer step = (std::static_pointer_cast<ChessItem>(picked_piece_->getModel())->getOwner()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;

      if (checkEnPassantCapturing(
            std::static_pointer_cast<Tile>(square_being_handled_->getModel()),
            step  ))
      {
        getChessGameController()->getGameView()->removeView(hollow_square_->getNext());
      }

      if (not isExpectedSquare(entity)) {
        hollow_square_->parameters().setColumn(SDLChessGameController::EMPTY);
      }
      else {
        outlineSquare(entity);

        if (checkEnPassantCapturing(
              std::static_pointer_cast<Tile>(entity->getModel()),
              step 
           ))
        {
          getChessGameController()->getGameView()->insertViewAfter(
            hollow_square_,
            getChessGameController()->createDecorationOfSquare(SDLChessGameController::SWORDS)
          );

          SDLEntityView::Pointer capturedSquareView = std::static_pointer_cast<SDLEntityView>(getChessGameController()->getChessRule()->getTileMap()->getTile(
            std::static_pointer_cast<Tile>(entity->getModel())->getRow() - step,
            std::static_pointer_cast<Tile>(entity->getModel())->getCol()
          )->getView());

          getChessGameService()->centerViewOnOtherView(
            std::static_pointer_cast<SDLEntityView>(hollow_square_->getNext()), 
            capturedSquareView
          );
        }

      }

      square_being_handled_ = std::static_pointer_cast<SDLEntityView>(entity);   //just non-nullptr entity is memorized
    }

    Boolean checkEnPassantCapturing(Tile::Pointer handledSquare, Integer step) const {
      if ( getChessGameController()->getChessRule()->getLastMoveInfo() == ChessRule::encodeMoveInfo(
              PieceIndex::PAWN, 
              handledSquare->getRow() + step, 
              handledSquare->getCol(), 
              handledSquare->getRow() - step, 
              handledSquare->getCol()) )
      {
        return true;
      }
      return false;
    }

    /***************************************************************************/
    void onMouseButtonPress(EntityView::Pointer entity) override { 
      if (not isExpectedSquare(entity)) {
        getInitialState()->displaySolidSquares();
        getChessGameController()->getGameView()->removeView(solid_square_);
        returnPickedPieceView();

        getGameController()->popSeveralLastStates(2);  //pop ItemMovable state and WaitingBoardFocused state
        getGameController()->getCurrentState()->onEntering(); //enter ItemPickable state
        return;
      }

      square_being_handled_ = std::static_pointer_cast<SDLEntityView>(entity);
      transitionToWaitingMouseButtonReleasedState();
    }

    /***************************************************************************/
    void onOtherEvent(EntityView::Pointer entity) override { }


    //state transition functions
    /***************************************************************************/
    void transitionToWaitingMouseButtonReleasedState() {
      onExiting();
      getChessGameController()->pushState(WaitingItemMovingDone::createPointer(
        getGameService(), 
        square_being_handled_,
        picked_square_,
        picked_piece_,
        solid_square_
      ));
    }

    /***************************************************************************/
    void transitionToSubmissionDisabledState() {

    }

  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new ItemMovable(std::forward<ARGS>(args)...));
    }

  protected:
    ItemMovable(
      GameService::Pointer boardService,
      SDLEntityView::Pointer pickedSquare,
      SDLEntityView::Pointer solidSquare
      //Vector<SDLEntityView::Pointer> opponentAimedSquares
    ) : WaitingMouseButtonPressed(boardService),
        picked_piece_(getChessGameService()->getPieceViewFromSquareView(pickedSquare)),
        hollow_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::HOLLOW_SQUARE)),  //SDLChessGameController::DecorationType, should be changed to SDLChessGameController::HOLLOW_SQUARE 
        square_being_handled_(pickedSquare),
        picked_square_(pickedSquare),
        solid_square_(solidSquare)
    {
      //addDecorationViews();
    }

  private:
    using WaitingMouseButtonPressed::getGameController;
    
    /***************************************************************************/
    void addDecorationViews(SquareGroups const& groupsOfPlaceableSquares) const {
      Vector<SDLEntityView::Pointer> decorations;

      for (const Tile::Pointer& emptySquare : groupsOfPlaceableSquares[ChessRule::EMPTY]) {
        decorations.emplace_back(getChessGameController()->createDecorationOfSquare(
          SDLChessGameController::CIRCLE  //SDLChessGameController::DecorationType
        ));

        getChessGameService()->centerViewOnOtherView(decorations.back(), std::static_pointer_cast<SDLEntityView>(emptySquare->getView()));
      }

      for (const Tile::Pointer& enemySquare : groupsOfPlaceableSquares[ChessRule::ENEMY]) {
        if (enemySquare->hasItem()) {
          //create CIRCLE decor
          decorations.emplace_back(getChessGameController()->createDecorationOfSquare(
            SDLChessGameController::CIRCLE  //SDLChessGameController::DecorationType
          ));
          getChessGameService()->centerViewOnOtherView(decorations.back(), std::static_pointer_cast<SDLEntityView>(enemySquare->getView()));

          //create SWORDS decor
          decorations.emplace_back(getChessGameController()->createDecorationOfSquare(
            SDLChessGameController::SWORDS  //SDLChessGameController::DecorationType
          ));
          getChessGameService()->centerViewOnOtherView(decorations.back(), std::static_pointer_cast<SDLEntityView>(enemySquare->getView()));
        }
        else {
          //create CIRCLE decor
          decorations.emplace_back(getChessGameController()->createDecorationOfSquare(
            SDLChessGameController::CIRCLE  //SDLChessGameController::DecorationType
          ));          
          getChessGameService()->centerViewOnOtherView(decorations.back(), std::static_pointer_cast<SDLEntityView>(enemySquare->getView()));
        }
      }

      for (Integer i = 0; i < decorations.size() - 1; ++i) {
        decorations[i]->setNext(decorations[i + 1]);
        decorations[decorations.size() - 1 - i]->setPrevious(decorations[decorations.size() - 2 - i]);
      }

      getChessGameController()->getGameView()->addViewSeries(
        decorations.front(),       //arg 1: EntityView::Pointer
        decorations.back(),        //arg 2: EntityView::Pointer
        Game2DView::DECORATION_4   //arg 3: Game2DView::EntityViewType
      );
    }
  private:
    SDLEntityView::Pointer picked_square_;
    SDLEntityView::Pointer picked_piece_;
    SDLEntityView::Pointer hollow_square_;
    SDLEntityView::Pointer solid_square_;
    SDLEntityView::Pointer square_being_handled_;
  };
}