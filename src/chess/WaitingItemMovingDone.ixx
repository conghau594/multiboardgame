module;
#ifdef _DEBUG
import <iostream>;
#endif

export module chess.WaitingItemMovingDone;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;
//import core.EntityView;
import game_controller.WaitingMouseButtonReleased;
import chess.SDLChessGameController;
import chess.SDLChessGameService;
import chess.ChessRule;
import chess.WaitingTurnTaken;

namespace mbg {
  export class WaitingItemMovingDone : public WaitingMouseButtonReleased {
  public://types
    using Pointer = SharedPointer<WaitingItemMovingDone>;

  public://functions
    virtual ~WaitingItemMovingDone() = default;

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
      getChessGameController()->getGameView()->addView(
        next_solid_square_,                 //arg 1: EntityView::Pointer
        Game2DView::DECORATION_3  //arg 2: Game2DView::EntityViewType
      );
      return true; 
    }

    /***************************************************************************/
    Boolean onExiting() override {
      getChessGameController()->getGameView()->removeView(picked_piece_);
      getChessGameController()->getGameView()->addView(picked_piece_, Game2DView::ITEM);

      //getChessGameController()->memorized_entities()[0] = nullptr;
      //getChessGameController()->getGameView()->removeView(solid_square_);
      //getChessGameController()->getCurrentState()->onEntering();
      return true;
    }

    //game service function
    /***************************************************************************/
    Boolean isEntityExpected(EntityView::Pointer entity) const {
      return entity == square_being_handled_;
      //return entity == getGameController()->memorized_entities()[0];
         //&& static_cast<Tile*>(entity->getModel().get())->hasItem();
    }

    /***************************************************************************/
    void returnPickedPieceView() {
      picked_piece_->parameters().screen_frame() = getCenteredRectangle(
        picked_piece_->parameters().screen_frame().w,
        picked_piece_->parameters().screen_frame().h,
        first_picked_square_->parameters().screen_frame()
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
      //[x]TODO!049 back to ItemPickable state
      getChessGameController()->getGameView()->removeView(next_solid_square_);
      getGameController()->popState();    //just back to the previous ChessPieceMoveable state
      getGameController()->getCurrentState()->onEntering();
    }

    /***************************************************************************/
    void onMouseMove(EntityView::Pointer entity) override { 
      SDLRectangle& pieceScreenFrame = picked_piece_->parameters().screen_frame();
      pieceScreenFrame.x = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorX() - pieceScreenFrame.w / 2;
      pieceScreenFrame.y = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorY() - pieceScreenFrame.h / 2;
    } ///< does nothing



    /***************************************************************************/
    void onMouseButtonRelease(EntityView::Pointer entity) override {
      Vector<ChessItem::Pointer> changedPieces;
      Boolean canMove = false;

      if (isEntityExpected(entity)) {
        changedPieces = getChessGameController()->getChessRule()->tryToMove(
          std::static_pointer_cast<ChessItem>(picked_piece_->getModel()),
          std::static_pointer_cast<Tile>(square_being_handled_->getModel())
        );

        if (not changedPieces.empty())
        {
          canMove = true;
        }
      }

      if(canMove) {
        changedPieces[0]->increaseMoveCount();
        getChessGameService()->submitMove(
          PieceIndex(changedPieces[0]->getType()),
          static_cast<Tile*>(first_picked_square_->getModel().get())->getRow(),
          static_cast<Tile*>(first_picked_square_->getModel().get())->getCol(),
          changedPieces[0]->getRow(),
          changedPieces[0]->getCol()
        );

        getInitialState()->substituteSolidSquares(next_solid_square_, first_solid_square_);
        getChessGameService()->movePieceView(changedPieces);
        
        transitionToSubmissionDisabledState();
      }
      else {//move failed
        returnPickedPieceView();
        getInitialState()->displaySolidSquares();
        getChessGameController()->getGameView()->removeViewSeries(
          next_solid_square_,
          first_solid_square_
        );

        getChessGameController()->popSeveralLastStates(3);     //return to `ItemPickable` state
        getChessGameController()->getCurrentState()->onEntering();
      }
    }

    /***************************************************************************/
    void onOtherEvent(EntityView::Pointer entity) override { } ///< Does nothing

    //state transition functions
    /***************************************************************************/
    void transitionToSubmissionDisabledState() {
      getChessGameController()->popSeveralLastStates(5);      //return to `WaitingTurnTaken` state
      getChessGameController()->getCurrentState()->onEntering();
    }


  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new WaitingItemMovingDone(std::forward<ARGS>(args)...));
    }

  protected://contructors
    WaitingItemMovingDone(
      GameService::Pointer boardService,
      SDLEntityView::Pointer squareBeingHandled,
      SDLEntityView::Pointer pickedSquare,
      SDLEntityView::Pointer pickedPiece,
      SDLEntityView::Pointer firstSolidSquare
    ) : WaitingMouseButtonReleased(boardService),
        next_solid_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::SOLID_SQUARE)),
        first_picked_square_(pickedSquare),
        picked_piece_(pickedPiece),
        square_being_handled_(squareBeingHandled),
        first_solid_square_(firstSolidSquare)
    {
      getChessGameService()->centerViewOnOtherView(next_solid_square_, square_being_handled_);
    }

  private://member functions
    using WaitingMouseButtonReleased::getGameController;

  private://data members
    SDLEntityView::Pointer first_picked_square_;///< The first square (aka the square before the `picked_piece_` moves)
    SDLEntityView::Pointer picked_piece_;
    SDLEntityView::Pointer first_solid_square_;
    SDLEntityView::Pointer next_solid_square_;
    SDLEntityView::Pointer square_being_handled_;
  };
}