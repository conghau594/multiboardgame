module;
#ifdef _DEBUG
import <iostream>;
#endif 

export module chess.SDLChessGameController;

import foundation.basetypes;
import foundation.SharedPointer;
import foundation.Vector;
import foundation.String;
import foundation.PlayerTurn;
import foundation.Function;

import game_controller.BoardGameState;
import core.AvatarManager;
import core.EntityView;
import core.EntityModel;

import sdl_display;
import game_model;

import chess.SDLChessGameTransceiver;
import chess.PieceIndex;
import chess.ChessRule;
import chess.ChessItem;
import app.SDLAppEventHandler;

namespace mbg {
  export class SDLChessGameController : public SDLGame2DController {
  public://type
    using Pointer = SharedPointer<SDLChessGameController>;

    enum AvatarIndex {
      SQUARES = 0,
      WHITE_PIECES,
      BLACK_PIECES,
      SQUARE_DECORATORS
    };

    enum DecorationType {
      HOLLOW_SQUARE,
      SOLID_SQUARE,
      CIRCLE,
      SWORDS,
      DANGER_SQUARE,
      SKULL,
      EMPTY
    };

  public://member functions
    Boolean onExiting()  { return true; }
    Boolean onEntering() { return true; }

    SDLEntityView::Pointer& tile_view(Integer i) { return tile_views_[i]; }
    SDLEntityView::Pointer& tile_view(Integer row, Integer col) { return tile_views_[ChessRule::NUMBER_OF_SQUARES_PER_LINE*row + col]; }

    void update() override { getCurrentState()->update(); }
    void render() override { getGameView()->render(getWindow()); }

    Vector<SDLEntityView::Pointer> createSquareDecorationViews(Vector<SDLEntityView::Pointer> const& squareViews, DecorationType decorType);
    inline SDLEntityView::Pointer createDecorationOfSquare(DecorationType decorType);

    //get views
    inline SDLAvatar::Pointer getSquaresAvatar() const {
      return std::static_pointer_cast<SDLAvatar>(getAvatarManager()->operator[](avatar_indices_[SQUARES]));
    }
    inline SDLAvatar::Pointer getWhitePiecesAvatar() const {
      return std::static_pointer_cast<SDLAvatar>(getAvatarManager()->operator[](avatar_indices_[WHITE_PIECES]));
    }
    inline SDLAvatar::Pointer getBlackPiecesAvatar() const {
      return std::static_pointer_cast<SDLAvatar>(getAvatarManager()->operator[](avatar_indices_[BLACK_PIECES]));
    }
    inline SDLAvatar::Pointer getSquareDecoratorAvatar() const {
      return std::static_pointer_cast<SDLAvatar>(getAvatarManager()->operator[](avatar_indices_[SQUARE_DECORATORS]));
    }
    inline SDLEntityView::Pointer getDecorationViewSample(DecorationType decorType) const { 
      return decor_view_samples_[decorType];
    }

    //get models
    Player::Pointer getThisPlayer() const {
      return getGameModel()->getPlayerManager()->operator[](0);
    }

    Player::Pointer getEnemyPlayer() const {
      return getGameModel()->getPlayerManager()->operator[](1);
    }

    inline ChessRule::Pointer getChessRule() const { 
      return std::static_pointer_cast<ChessRule>(getGameModel()->getRule());
    }


    //get others
    inline SDLAppEventHandler::Pointer getAppEventHandler() const {
      return std::static_pointer_cast<SDLAppEventHandler>(getEventHandler());
    }

    //SDLChessGameTransceiver::Pointer getThisTransceiver() const {
    //  return this_tranceiver_;
    //}
    SDLChessGameTransceiver::Pointer getTransceiver() const {
      return tranceiver_;
    }

    BoardGameState::Pointer getInitialState() const {
      return getGameState(0);
    }

    void setTransceiver(SDLChessGameTransceiver::Pointer tranceiver) {
      tranceiver_ = tranceiver;
    }

    virtual void loadAvatars(const Vector<String>& pngPaths); ///< Loads PNG images as avatars for entity
    
    virtual SDLGame2DView::Pointer createSceneView(Game2DModel::Pointer gameModel, PlayerTurn thisPlayerTurn);
    virtual Game2DModel::Pointer createSceneModel(PlayerTurn thisPlayerTurn);
    virtual void initialize(PlayerTurn thisPlayerTurn);  ///< Creates whole board entities, including squares, pieces...

  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new SDLChessGameController(std::forward<ARGS>(args)...));
    }

  //protected://contructors
    SDLChessGameController(
      SDLWindow::Pointer window, 
      SDLAppEventHandler::Pointer eventHandler, 
      AvatarManager::Pointer avatarMgr,
      //SDLChessGameTransceiver::Pointer thisTranceiver//,
      SDLChessGameTransceiver::Pointer tranceiver
    ) : SDLGame2DController(nullptr, nullptr, nullptr, window, eventHandler, avatarMgr),
      //this_tranceiver_(thisTranceiver),
      tranceiver_(tranceiver)
    { }

  private://member functions
    //model creators
    TileMap::Pointer createTileMapModel(Integer col, Integer row);
    PlayerManager::Pointer createPlayerManager(String thisPlayerId, PlayerTurn thisPlayerTurn, String opponentId, PlayerTurn opponentTurn); 
    
    ItemManager::Pointer createItemManager(TileMap::Pointer tileMap, PlayerManager::Pointer playerMgr);

    //view creators
    SDLEntityView::Pointer createTileMapView(TileMap::Pointer tileMap, Integer x, Integer y, Integer w, Integer h);    
    SDLEntityView::Pointer createTileMapView(TileMap::Pointer tileMap, SDLRectangle tileMapFrame);

    Vector<SDLEntityView::Pointer> createTileViews(SDLEntityView::Pointer tileMapView, PlayerTurn thisPlayerTurn, Integer offset = 0) const;  ///< Returns vector of all tile views.
    Vector<SDLEntityView::Pointer> createItemViews(ItemManager::Pointer itemMgr, Vector<SDLEntityView::Pointer> tileViews, PlayerTurn thisPlayerTurn, Double scale) const;  ///< Returns vector of all element views.

    Vector<SDLEntityView::Pointer> createSquareDecorationViewSamples(SDLRectangle tileScreenFrame, Double const decorScales[]);

  private://static data members
    static Integer constexpr NUMBER_OF_DECORATIONS = DecorationType::EMPTY;

  private://data members
    Vector<SDLEntityView::Pointer> decor_view_samples_;
    Vector<SDLEntityView::Pointer> tile_views_;
    Vector<Integer> avatar_indices_; //Stores indices of each avatar in avatar manager for this chess game
    SDLChessGameTransceiver::Pointer tranceiver_;
  };


  /****************************************************************************/
  inline SDLEntityView::Pointer SDLChessGameController::createDecorationOfSquare(DecorationType decorType)
  {
    return SDLEntityView::createPointer(
      getSquareDecoratorAvatar(),  //SDLAvatar::Pointer avatar,
      decor_view_samples_[decorType]->parameters().getAvatarUnitFrame(),  //SDLRectangle avatarUnitFrame,
      decor_view_samples_[decorType]->parameters().screen_frame(),//getCenteredRectangle(sampleDecorParam.screen_frame().w, sampleDecorParam.screen_frame().h, squareView->parameters().screen_frame()),  //SDLRectangle screenFrame,
      decor_view_samples_[decorType]->parameters().getRow(),              //Integer avatarRow,
      decorType,                              //Integer avatarCol,
      decor_view_samples_[decorType]->parameters().z_order()              //Integer zOrder,
    );
  }


  /***************************************************************************
  inline SDLEntityView::Pointer SDLChessGameController::getSquareDecoratorsView() const {
    return std::static_pointer_cast<SDLEntityView>(getGameView()->operator[](Game2DView::DECORATION_VIEW).lock());
  }
  */


}