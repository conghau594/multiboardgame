module;
#ifdef _DEBUG
import <iostream>;
#endif

export module chess.SDLChessGameService;

import foundation.basetypes;
import foundation.SharedPointer;
import foundation.String;
import core.EntityView;
import core.MouseEventManager;

import game_view.Game2DView;
import game_model.Tile;
import game_controller.GameService;
import sdl_display.SDLEntityView;
import chess.SDLChessGameController;
import chess.SDLChessGameEventManager;
import chess.ChessRule;
import chess.ChessItem;
import chess.PieceIndex;

namespace mbg {
  export class SDLChessGameService : public GameService {
  public://types
    using Pointer = SharedPointer<SDLChessGameService>;

  public://member functions
    virtual ~SDLChessGameService() = default;
    SDLChessGameController::Pointer getChessGameController() const {
      return std::static_pointer_cast<SDLChessGameController>(getGameController());
    }

    SDLChessGameEventManager::Pointer getChessGameEventManager() const {
      return getChessGameController()->getAppEventHandler()->getChessGameEventManager();
    }

    //game view services
    inline EntityView::Pointer getEntityUnderCursor() override;

    inline Boolean isCursorOnExpectedArea() override;

    inline Boolean isMouseMoved() override;

    inline Boolean isMouseButtonPressed() override;

    inline Boolean isMouseButtonReleased() override;

    //game model services
    inline Boolean isTimeout() override;

    inline Boolean doesSomeoneResign() override;

    inline Boolean isTurnTaken() override;

    inline Boolean shouldSubmit() override;


    inline SDLEntityView::Pointer getPieceViewFromSquareView(EntityView::Pointer tileView) const {
      Tile* tileModel = static_cast<Tile*>(tileView->getModel().get());
      if (not tileModel->hasItem()) {
        return nullptr;
      }

      return std::static_pointer_cast<SDLEntityView>(tileModel->item(0)->getView());
    }

    inline void centerViewOnOtherView(SDLEntityView::Pointer centeredView, SDLEntityView::Pointer baseView) {
      centeredView->parameters().screen_frame() = getCenteredRectangle(
        centeredView->parameters().screen_frame().w,
        centeredView->parameters().screen_frame().h,
        baseView->parameters().screen_frame()
      );
    }

    Vector<Integer> getEnemyLastMove() const {
      return ChessRule::decodeMoveInfo( (*getChessGameEventManager())[SDLChessGameEventManager::PIECE_MOVE_INFO] );
    }

    Integer getEnemyLastMoveCode() const {
      return (*getChessGameEventManager())[SDLChessGameEventManager::PIECE_MOVE_INFO];
    }
 
    void submitMove(PieceIndex pieceType, Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
      Integer data = ChessRule::encodeMoveInfo(pieceType, startRow, startCol, endRow, endCol);
      getChessGameController()->getChessRule()->storeMoveInfo(data);

      (*getChessGameEventManager())[SDLChessGameEventManager::PLAYER_TURN] = getChessGameController()->getEnemyPlayer()->getTurn();

      getChessGameController()->getTransceiver()->sendEvent(
        getChessGameEventManager()->getFirstUserEventIndex() + SDLChessGameEventManager::ENEMY_MOVE,
        (*getChessGameEventManager())[SDLChessGameEventManager::PLAYER_TURN], //data1
        data      //data2
      );


#ifdef _DEBUG
      std::cout << "\n...SDLChessGameService::submitMove(PieceIndex pieceType, Integer startRow, Integer startCol, Integer endRow, Integer endCol)..." << data;
      std::cout << "\n data       = " << data;
      std::cout << "\n   pieceType = " << pieceType; //`ieceType`
      std::cout << "\n   startRow = " << startRow; //index of `startRow`
      std::cout << "\n   startCol = " << startCol; //index of `startCol`
      std::cout << "\n   endRow   = " << endRow;         //index of `endRow`
      std::cout << "\n   endCol   = " << endCol;                                     //index of `endCol`

#endif 
    }

    /***************************************************************************/
    void movePieceView(Vector<ChessItem::Pointer>& changedPieces) const {     
#ifdef _DEBUG
      std::cout << "\n\n...void movePiece(SDLEntityView::Pointer piece, SDLEntityView::Pointer square)...";
      std::cout << "\n   windowID = " << getChessGameController()->getWindow()->getWindowID();
#endif
      if (changedPieces[0]->getType() == PieceIndex::PAWN) {
        if ( changedPieces[0]->getTile()->getRow() == 0
          || changedPieces[0]->getTile()->getRow() == ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1)
        {
          //TODO!046 promotion only to a queen is just a workaround -> need to modify
          getChessGameController()->getChessRule()->promotePawn(changedPieces[0], PieceIndex::QUEEN);
          static_cast<SDLEntityView*>(changedPieces[0]->getView().get())->parameters().setColumn(PieceIndex::QUEEN);
        }
      }

      SDLRectangle& pieceScreenFrame = static_cast<SDLEntityView*>(changedPieces[0]->getView().get())->parameters().screen_frame();
      pieceScreenFrame = getCenteredRectangle(
        pieceScreenFrame.w,
        pieceScreenFrame.h,
        static_cast<SDLEntityView*>(changedPieces[0]->getTile()->getView().get())->parameters().screen_frame()
      );

      if (changedPieces.size() > 1) {
        if (getChessGameController()->getChessRule()->areHostilePieces(changedPieces[0], changedPieces[1])) {
          getChessGameController()->getGameView()->removeView(changedPieces[1]->getView());
        }
        else {
          SDLRectangle& pieceScreenFrame = static_cast<SDLEntityView*>(changedPieces[1]->getView().get())->parameters().screen_frame();
          pieceScreenFrame = getCenteredRectangle(
            pieceScreenFrame.w,
            pieceScreenFrame.h,
            static_cast<SDLEntityView*>(changedPieces[1]->getTile()->getView().get())->parameters().screen_frame()
          );
        }
      }
    }

    
    public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new SDLChessGameService(std::forward<ARGS>(args)...));
    }


  protected:
    SDLChessGameService(SharedPointer<SDLChessGameController> gameCtrler) 
      : GameService(gameCtrler)
    { }

  private:
    using GameService::getGameController;

  };

  /*****************************************************************************/
  inline Boolean SDLChessGameService::isCursorOnExpectedArea() {
    //[x]TODO!044 must check if cursor gets out of window
    SDLRectangle& tileMapScreenFrame = static_cast<SDLEntityView*>(getChessGameController()->getGameView()->getFirstTileView().get())->parameters().screen_frame();
    Integer xCursor = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorX();
    Integer yCursor = getChessGameController()->getEventHandler()->getMouseEventManager()->getCursorY();


    if ( (tileMapScreenFrame.x < xCursor) && (xCursor < tileMapScreenFrame.x + ChessRule::NUMBER_OF_SQUARES_PER_LINE * tileMapScreenFrame.w) ) {
      if ( (tileMapScreenFrame.y < yCursor) && (yCursor < tileMapScreenFrame.y + ChessRule::NUMBER_OF_SQUARES_PER_LINE * tileMapScreenFrame.h) ) {
#ifdef _DEBUG1
        std::cout << "\n Boolean SDLChessGameService::" << __func__;
        std::cout << "\n\tx cursor = " << xCursor;
        std::cout << "\n\ty cursor = " << yCursor;
#endif
        return true;
      }
    }


    return false;
  }

  /*****************************************************************************/
  inline EntityView::Pointer SDLChessGameService::getEntityUnderCursor() {
    SDLRectangle& firstTileScreenFrame = getChessGameController()->tile_view(0)->parameters().screen_frame();
    MouseEventManager& mouseEventMgr = *(getChessGameController()->getEventHandler()->getMouseEventManager());

#ifdef _DEBUG1
    std::cout << "\n\nEntityView::Pointer SDLChessGameService::getEntityUnderCursor(...";

    std::cout << "\n\t1st tile screen frame:"; 
    std::cout << "\n\tx = " << firstTileScreenFrame.x;
    std::cout << "\n\ty = " << firstTileScreenFrame.y;
    std::cout << "\n\tw = " << firstTileScreenFrame.w;
    std::cout << "\n\th = " << firstTileScreenFrame.h;
    std::cout << "\n\tcursor position:";
    std::cout << "\n\tx = " << mouseEventMgr.getCursorX();
    std::cout << "\n\ty = " << mouseEventMgr.getCursorY();

    Integer col = (mouseEventMgr[MouseEventManager::CURSOR_X] - firstTileScreenFrame.x) / firstTileScreenFrame.w;
    Integer row = (mouseEventMgr[MouseEventManager::CURSOR_Y] - firstTileScreenFrame.y) / firstTileScreenFrame.h;
    Integer tileViewIndex = SDLChessGameController::ChessRule::NUMBER_OF_SQUARES_PER_LINE * row + col;

    std::cout << "\n\trow = " << row;
    std::cout << "\n\tcol = " << col;
    std::cout << "\n\ttileViewIndex = " << tileViewIndex;

    SDLRectangle& entityUnderCursorScreenFrame = game_controller_->tile_view(tileViewIndex)->parameters().screen_frame();
    std::cout << "\n\tScreen frame of the entity under cursor:";
    std::cout << "\n\tx = " << entityUnderCursorScreenFrame.x;
    std::cout << "\n\ty = " << entityUnderCursorScreenFrame.y;
    std::cout << "\n\tw = " << entityUnderCursorScreenFrame.w;
    std::cout << "\n\th = " << entityUnderCursorScreenFrame.h;
#endif

    return getChessGameController()->tile_view(
      (mouseEventMgr.getCursorY() - firstTileScreenFrame.y) / firstTileScreenFrame.h,
      (mouseEventMgr.getCursorX() - firstTileScreenFrame.x) / firstTileScreenFrame.w
      );
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::isMouseMoved() {
    return getChessGameController()->getAppEventHandler()->getMouseEventManager()
              ->operator[](MouseEventManager::EVENT_TYPE) == SDLEventHandler::MOUSE_MOVED;
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::isMouseButtonPressed() {
    return getChessGameController()->getAppEventHandler()->getMouseEventManager()
              ->operator[](MouseEventManager::EVENT_TYPE) == SDLEventHandler::MOUSE_BUTTON_DOWN;
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::isMouseButtonReleased() {
    return getChessGameController()->getAppEventHandler()->getMouseEventManager()
              ->operator[](MouseEventManager::EVENT_TYPE) == SDLEventHandler::MOUSE_BUTTON_UP;
  }
  
  /*****************************************************************************/
  inline Boolean SDLChessGameService::isTimeout() {
    return false;
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::doesSomeoneResign() {
    return false;
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::isTurnTaken() {
    if ((*getChessGameEventManager())[SDLChessGameEventManager::PLAYER_TURN] == getChessGameController()->getGameModel()->getPlayerManager()->getThisPlayer()->getTurn())
      return true;

    return false;
  }

  /*****************************************************************************/
  inline Boolean SDLChessGameService::shouldSubmit() {
    return false;
  }
}