export module chess.WaitingTurnTaken;

import foundation.String;
import foundation.Vector;
import game_controller.SubmissionDisabled;
import chess.SDLChessGameController;
import chess.SDLChessGameService;
import chess.WaitingBoardFocused;

namespace mbg {
  export class WaitingTurnTaken : public SubmissionDisabled {
  public://type
    using Pointer = SharedPointer<WaitingTurnTaken>;

  public://member functions
    inline String getID() override { return typeid(*this).name(); }

    /***************************************************************************/
    inline SDLChessGameController::Pointer getChessGameController() const {
      return std::static_pointer_cast<SDLChessGameController>(getGameController());
    }


    /***************************************************************************/
    inline SDLChessGameService::Pointer getChessGameService() const {
      return std::static_pointer_cast<SDLChessGameService>(getGameService());
    }

    /***************************************************************************/
    inline void substituteSolidSquares(SDLEntityView::Pointer firstSquare, SDLEntityView::Pointer lastSquare) {
      getChessGameController()->getGameView()->removeViewSeries(first_solid_square_, last_solid_square_);
      first_solid_square_ = firstSquare;
      last_solid_square_ = lastSquare;
    }

    /***************************************************************************/
    inline void displaySolidSquares() {
      first_solid_square_->parameters().setColumn(SDLChessGameController::SOLID_SQUARE);
      last_solid_square_->parameters().setColumn(SDLChessGameController::SOLID_SQUARE);
    }

    /***************************************************************************/
    inline void disappearSolidSquares() {
      first_solid_square_->parameters().setColumn(SDLChessGameController::EMPTY);
      last_solid_square_->parameters().setColumn(SDLChessGameController::EMPTY);
    }

    /***************************************************************************/
    inline void displayDangerSquare(ChessItem::Pointer piece) {
      danger_square_->parameters().setColumn(SDLChessGameController::DANGER_SQUARE);
      getChessGameService()->centerViewOnOtherView(
        danger_square_,
        std::static_pointer_cast<SDLEntityView>(piece->getTile()->getView())
      );

      swords_->parameters().setColumn(SDLChessGameController::SWORDS);
      getChessGameService()->centerViewOnOtherView(
        swords_, 
        std::static_pointer_cast<SDLEntityView>(piece->getTile()->getView())
      );
    }

    /***************************************************************************/
    inline void disappearDangerSquare() {
      danger_square_->parameters().setColumn(SDLChessGameController::EMPTY);
      static_cast<SDLEntityView*>(danger_square_->getNext().get())->parameters().setColumn(SDLChessGameController::EMPTY);
    }

    /***************************************************************************/
    inline Boolean onEntering() override {
      if (getChessGameController()->getChessRule()->inCheck(
            getChessGameController()->getThisPlayer(),          //checking player
            getChessGameController()->getEnemyPlayer()     ))   //checked player
      {
        displayDangerSquare(getChessGameController()->getChessRule()->getKing(getChessGameController()->getEnemyPlayer()));
      }
      else {
        disappearDangerSquare();
      }
      return true;
    }

    /***************************************************************************/
    inline Boolean onExiting() override {
      return true;
    }

    /***************************************************************************/
    void onTakingTurn() override {
      getChessGameController()->getChessRule()->storeMoveInfo(getChessGameService()->getEnemyLastMoveCode());
      Vector<Integer> enemyMoveCoords = getChessGameService()->getEnemyLastMove();

      Vector<ChessItem::Pointer> changedPieces = getChessGameController()->getChessRule()->move(
        enemyMoveCoords[1],  //startRow
        enemyMoveCoords[2],  //startCol
        enemyMoveCoords[3],  //endRow
        enemyMoveCoords[4]   //endCol
      );

      getChessGameService()->movePieceView(changedPieces);
      disappearDangerSquare();

      getChessGameService()->centerViewOnOtherView(
        first_solid_square_,
        std::static_pointer_cast<SDLEntityView>(getChessGameController()->getGameModel()->getTileMap()->getTile(enemyMoveCoords[1], enemyMoveCoords[2])->getView())
      );

      getChessGameService()->centerViewOnOtherView(
        last_solid_square_,
        std::static_pointer_cast<SDLEntityView>(getChessGameController()->getGameModel()->getTileMap()->getTile(enemyMoveCoords[3], enemyMoveCoords[4])->getView())
      );
        
      displaySolidSquares();


      if (getChessGameController()->getChessRule()->inCheck(
        getChessGameController()->getEnemyPlayer(),          //checking player
        getChessGameController()->getThisPlayer()))   //checked player
      {
        displayDangerSquare(getChessGameController()->getChessRule()->getKing(getChessGameController()->getThisPlayer()));
      }

      transitionToEntityPickableState();
    }

    /***************************************************************************/
    void transitionToEntityPickableState() {
      onExiting();
      getChessGameController()->pushState(next_state_);
    }

  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new WaitingTurnTaken(std::forward<ARGS>(args)...));
    }

  protected://member functions
    WaitingTurnTaken(
      SDLChessGameService::Pointer boardService,
      WaitingBoardFocused::Pointer nextState
    ) : SubmissionDisabled(boardService), 
        next_state_(nextState),
        danger_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::DANGER_SQUARE)),
        swords_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::SWORDS)),
        first_solid_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::SOLID_SQUARE)),
        last_solid_square_(getChessGameController()->createDecorationOfSquare(SDLChessGameController::SOLID_SQUARE))
    {
      first_solid_square_->setNext(last_solid_square_);
      last_solid_square_->setPrevious(first_solid_square_);
      getChessGameController()->getGameView()->addViewSeries(
        first_solid_square_,
        last_solid_square_,
        Game2DView::DECORATION_3
      );

      danger_square_->setNext(swords_);
      swords_->setPrevious(danger_square_);
      getChessGameController()->getGameView()->addViewSeries(
        danger_square_, 
        swords_,
        Game2DView::DECORATION_5
      );

      //disappearDangerSquare();
      //disappearSolidSquares();
    }

  private:
    WaitingBoardFocused::Pointer next_state_;
    SDLEntityView::Pointer danger_square_;
    SDLEntityView::Pointer swords_;
    SDLEntityView::Pointer first_solid_square_;
    SDLEntityView::Pointer last_solid_square_;
  };
}
