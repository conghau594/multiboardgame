module;
#ifdef _DEBUG
import <iostream>;
#include <catch2/catch_test_macros.hpp>
#endif 

module chess.SDLChessGameController;

namespace mbg {
  /****************************************************************************/
  void SDLChessGameController::initialize(PlayerTurn thisPlayerTurn) {  ///< Creates whole board entities, including squares, pieces...
    Vector<String> pngPaths;
    pngPaths.emplace_back("assets/squares.png");
    pngPaths.emplace_back("assets/white chess pieces.png");
    pngPaths.emplace_back("assets/black chess pieces.png");
    pngPaths.emplace_back("assets/decorations.png");

    loadAvatars(pngPaths);
    Game2DModel::Pointer  gameModel = createSceneModel(thisPlayerTurn);
    SDLGame2DView::Pointer gameView = createSceneView(gameModel, thisPlayerTurn);

    setGameModel(gameModel);
    setGameView(gameView);
  }

  /****************************************************************************/
  TileMap::Pointer SDLChessGameController::createTileMapModel(Integer col, Integer row) {
    return TileMap::createPointer(row, col);
  }

  /****************************************************************************/
  Game2DModel::Pointer SDLChessGameController::createSceneModel(PlayerTurn thisPlayerTurn) {
    TileMap::Pointer tileMap = createTileMapModel(ChessRule::NUMBER_OF_SQUARES_PER_LINE, ChessRule::NUMBER_OF_SQUARES_PER_LINE);
    PlayerManager::Pointer playerMgr = createPlayerManager(
      "Me",
      thisPlayerTurn,
      "Enemy",
      (thisPlayerTurn == PlayerTurn::FIRST) ? PlayerTurn::SECOND : PlayerTurn::FIRST
    );
    ItemManager::Pointer itemMgr = createItemManager(tileMap, playerMgr);

    Game2DModel::Pointer gameModel = Game2DModel::createPointer(
      nullptr,   //EntityView::Pointer gameView,
      tileMap,   //TileMap::Pointer tileMap,
      itemMgr,   //ItemManager::Pointer itemMgr,
      playerMgr, //PlayerManager::Pointer playerMgr,
      nullptr    //Rule::Pointer rule
    );

    gameModel->setRule(ChessRule::createPointer(gameModel));

    return gameModel;
  }

  /****************************************************************************/
  PlayerManager::Pointer SDLChessGameController::createPlayerManager(
    String thisPlayerId, PlayerTurn thisPlayerTurn, String opponentId, PlayerTurn opponentTurn)
  {
    PlayerManager::Pointer playerMgr = PlayerManager::createPointer(thisPlayerId, thisPlayerTurn);
    playerMgr->add(Player::createPointer(opponentId, opponentTurn));
    return playerMgr;
  }

  /****************************************************************************/
  SDLGame2DView::Pointer SDLChessGameController::createSceneView
  (Game2DModel::Pointer gameModel, PlayerTurn thisPlayerTurn)
  {
    constexpr Double DECORATION_SCALES[NUMBER_OF_DECORATIONS + 1] = {
      1.15,    ///< scale of the hollow purple square
      1.0,    ///< scale of the solid square
      1.0,    ///< scale of the small solid circle
      1.0,    ///< scale of the circle and crossed swords
      1.0,    ///< scale of the skull
      1.0,    ///< scale of the crossed swords
      1.0
    };
    constexpr Double CHESS_PIECE_SCALE = 1.0;
    constexpr Integer BOARD_OFFSET = 10;

    SDLRectangle gameViewFrame{
      0,
      0,
      getWindow()->getWidth(),
      getWindow()->getHeight()
    };

    Integer minSideBoardView = (gameViewFrame.h < gameViewFrame.w) ? gameViewFrame.h : gameViewFrame.w;
    const Integer REMAINDER = (minSideBoardView - 2 * BOARD_OFFSET) % ChessRule::NUMBER_OF_SQUARES_PER_LINE; ///< The remainder of game screen after placing tiles

    SDLEntityView::Pointer tileMapView = createTileMapView(
      gameModel->getTileMap(),
      REMAINDER / 2,      //for aligning the tile map to center of the game view
      REMAINDER / 2 + REMAINDER % 2,
      minSideBoardView - REMAINDER / 2 - REMAINDER % 2,   //need to subtract REMAINDER%2 for the width and height of tile map to be divisible by NUMBER_OF_TILE_VIEW
      minSideBoardView - REMAINDER / 2
    );

    tile_views_ = createTileViews(tileMapView, thisPlayerTurn, BOARD_OFFSET);
    Vector<SDLEntityView::Pointer> itemViews = createItemViews(gameModel->getItemManager(), tile_views_, thisPlayerTurn, CHESS_PIECE_SCALE);

    decor_view_samples_ = createSquareDecorationViewSamples(tile_views_[0]->parameters().screen_frame(), DECORATION_SCALES);
    SDLEntityParams gameViewParams{
      gameViewFrame,              //avatarFrame
      gameViewFrame,              //screenFrame
      SDLPoint(0, 0),             //center
      0.0,                        //angle
      0,                          //row
      0,                          //column
      0,                          //zOrder
      SDLEntityParams::FLIP_NONE  //avatarFlip
    };

    SDLGame2DView::Pointer gameView = SDLGame2DView::createPointer(
      nullptr,          //SDLAvatar::Pointer  avatar,
      gameViewParams,   //SDLEntityParams parameters,
      nullptr,          //SDLEntityView::Pointer firstBackgroundView,
      tileMapView,      //SDLEntityView::Pointer firstTileMapView,
      tile_views_[0],   //SDLEntityView::Pointer firstTileView,
      itemViews[0]     //SDLEntityView::Pointer firstItemView,
      /*nullptr,          //SDLEntityView::Pointer firstUIButtonView,
      nullptr,          //SDLEntityView::Pointer firstTextInputView,
      nullptr,          //SDLEntityView::Pointer firstDecorationView1,
      nullptr,     //SDLEntityView::Pointer firstDecorationView2,
      nullptr,          //SDLEntityView::Pointer firstDecorationView3,
      nullptr           //SDLEntityView::Pointer firstOtherView
      */
    );

    //gameView->swapAnchors(Game2DView::ITEM, Game2DView::UI_BUTTON);
    //gameView->swapAnchors(Game2DView::DECORATION_1, Game2DView::UI_BUTTON);
    //gameView->swapAnchors(Game2DView::DECORATION_2, Game2DView::TEXT_INPUT);

    return gameView;
  }


  /***************************************************************************
   * DONE
   ***************************************************************************/
  ItemManager::Pointer SDLChessGameController::createItemManager(TileMap::Pointer tileMap, PlayerManager::Pointer playerMgr) {
    ItemManager::Pointer itemMgr = ItemManager::createPointer();

    //creates KING items and links to the corresponding tiles
    itemMgr->add(ChessItem::createPointer("white king", PieceIndex::KING, ChessRule::PIECE_VALUES[PieceIndex::KING]));
    itemMgr->back()->addTile((*tileMap)[0][4]);
    (*tileMap)[0][4]->addItem(itemMgr->back());
    
    itemMgr->add(ChessItem::createPointer("black king", PieceIndex::KING, ChessRule::PIECE_VALUES[PieceIndex::KING]));
    itemMgr->back()->addTile((*tileMap)[7][4]);
    (*tileMap)[7][4]->addItem(itemMgr->back());

    //creates QUEEN items and links to the corresponding tiles
    itemMgr->add(ChessItem::createPointer("white queen", PieceIndex::QUEEN, ChessRule::PIECE_VALUES[PieceIndex::QUEEN]));
    itemMgr->back()->addTile((*tileMap)[0][3]);
    (*tileMap)[0][3]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black queen", PieceIndex::QUEEN, ChessRule::PIECE_VALUES[PieceIndex::QUEEN]));
    itemMgr->back()->addTile((*tileMap)[7][3]);
    (*tileMap)[7][3]->addItem(itemMgr->back());

    //creates ROOK items and links to the corresponding tiles
    itemMgr->add(ChessItem::createPointer("white rook", PieceIndex::ROOK, ChessRule::PIECE_VALUES[PieceIndex::ROOK]));
    itemMgr->back()->addTile((*tileMap)[0][0]);
    (*tileMap)[0][0]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black rook", PieceIndex::ROOK, ChessRule::PIECE_VALUES[PieceIndex::ROOK]));
    itemMgr->back()->addTile((*tileMap)[7][0]);
    (*tileMap)[7][0]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("white rook", PieceIndex::ROOK, ChessRule::PIECE_VALUES[PieceIndex::ROOK]));
    itemMgr->back()->addTile((*tileMap)[0][7]);
    (*tileMap)[0][7]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black rook", PieceIndex::ROOK, ChessRule::PIECE_VALUES[PieceIndex::ROOK]));
    itemMgr->back()->addTile((*tileMap)[7][7]);
    (*tileMap)[7][7]->addItem(itemMgr->back());


    //creates BISHOP items and links to the corresponding tiles
    itemMgr->add(ChessItem::createPointer("white bishop", PieceIndex::BISHOP, ChessRule::PIECE_VALUES[PieceIndex::BISHOP]));
    itemMgr->back()->addTile((*tileMap)[0][2]);
    (*tileMap)[0][2]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black bishop", PieceIndex::BISHOP, ChessRule::PIECE_VALUES[PieceIndex::BISHOP]));
    itemMgr->back()->addTile((*tileMap)[7][2]);
    (*tileMap)[7][2]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("white bishop", PieceIndex::BISHOP, ChessRule::PIECE_VALUES[PieceIndex::BISHOP]));
    itemMgr->back()->addTile((*tileMap)[0][5]);
    (*tileMap)[0][5]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black bishop", PieceIndex::BISHOP, ChessRule::PIECE_VALUES[PieceIndex::BISHOP]));
    itemMgr->back()->addTile((*tileMap)[7][5]);
    (*tileMap)[7][5]->addItem(itemMgr->back());

    //creates KNIGHT items and links to the corresponding tiles
    itemMgr->add(ChessItem::createPointer("white knight", PieceIndex::KNIGHT, ChessRule::PIECE_VALUES[PieceIndex::KNIGHT]));
    itemMgr->back()->addTile((*tileMap)[0][1]);
    (*tileMap)[0][1]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black knight", PieceIndex::KNIGHT, ChessRule::PIECE_VALUES[PieceIndex::KNIGHT]));
    itemMgr->back()->addTile((*tileMap)[7][1]);
    (*tileMap)[7][1]->addItem(itemMgr->back());
    
    itemMgr->add(ChessItem::createPointer("white knight", PieceIndex::KNIGHT, ChessRule::PIECE_VALUES[PieceIndex::KNIGHT]));
    itemMgr->back()->addTile((*tileMap)[0][6]);
    (*tileMap)[0][6]->addItem(itemMgr->back());

    itemMgr->add(ChessItem::createPointer("black knight", PieceIndex::KNIGHT, ChessRule::PIECE_VALUES[PieceIndex::KNIGHT]));
    itemMgr->back()->addTile((*tileMap)[7][6]);
    (*tileMap)[7][6]->addItem(itemMgr->back());


    //creates PAWN items and links to the corresponding tiles
    for (Integer i = 0; i < ChessRule::NUMBER_OF_SQUARES_PER_LINE; ++i) {
      itemMgr->add(ChessItem::createPointer("white pawn", PieceIndex::PAWN, ChessRule::PIECE_VALUES[PieceIndex::PAWN]));
      itemMgr->back()->addTile((*tileMap)[1][i]);
      (*tileMap)[1][i]->addItem(itemMgr->back());

      itemMgr->add(ChessItem::createPointer("black pawn", PieceIndex::PAWN, ChessRule::PIECE_VALUES[PieceIndex::PAWN]));
      itemMgr->back()->addTile((*tileMap)[6][i]);
      (*tileMap)[6][i]->addItem(itemMgr->back());
    }

    //assigns items to 2 players
    Player::Pointer whitePlayer, blackPlayer;
    if ((*playerMgr)[0]->getTurn() == PlayerTurn::FIRST) {
      whitePlayer = (*playerMgr)[0];
      blackPlayer = (*playerMgr)[1];
    }
    else {
      whitePlayer = (*playerMgr)[1];
      blackPlayer = (*playerMgr)[0];
    }

    whitePlayer->resizeOfItemList(ChessRule::NUMBER_OF_PIECES / 2);
    blackPlayer->resizeOfItemList(ChessRule::NUMBER_OF_PIECES / 2);

    for (Integer i = 0; i < ChessRule::NUMBER_OF_PIECES / 2; ++i) {
      whitePlayer->item(i) = (*itemMgr)[2*i];
      (*itemMgr)[2*i]->addOwner(whitePlayer);
      static_cast<ChessItem*>((*itemMgr)[2 * i].get())->setIndexFromPlayer(i);
      
      blackPlayer->item(i) = (*itemMgr)[2*i + 1];
      (*itemMgr)[2*i + 1]->addOwner(blackPlayer);
      static_cast<ChessItem*>((*itemMgr)[2*i+1].get())->setIndexFromPlayer(i);
    }

#ifdef _DEBUG1
    for (Integer i = 3; i < 6; ++i) {
      for (Integer j = 0; j < ChessRule::NUMBER_OF_SQUARES_PER_LINE; ++j) {
        CAPTURE(i, j, (*tileMap)[i][j]->getNumberOfItems());
        CHECK( 0 == (*tileMap)[i][j]->getNumberOfItems());
      }
    }    
    
    for ( Integer i : {0, 1, 6, 7} ) {
      for (Integer j = 0; j < ChessRule::NUMBER_OF_SQUARES_PER_LINE; ++j) {
        CAPTURE(i, j);
        CHECK(1 == (*tileMap)[i][j]->getNumberOfItems());
      }
    }

    {
      CAPTURE((*playerMgr)[0]->getNumberOfItems());
      CHECK(16 == (*playerMgr)[0]->getNumberOfItems());

      CAPTURE((*playerMgr)[1]->getNumberOfItems());
      CHECK(16 == (*playerMgr)[1]->getNumberOfItems());
    }

    for (auto i = 0; i < 32; ++i) {
      CAPTURE(i, (*itemMgr)[i]->getNumberOfTiles());
      CHECK(1 == (*itemMgr)[i]->getNumberOfTiles());
      
      CAPTURE(i, (*itemMgr)[i]->getNumberOfOwners());
      CHECK(1 == (*itemMgr)[i]->getNumberOfOwners());
    }
#endif // _DEBUG

    return itemMgr;
  }

  /****************************************************************************
   * DONE
   * \param pngPaths A vector of PNG images. The 1st is squares image, the 2nd 
   *                 is white pieces, the 3rd is black pieces, and the rests
   *                 are some decorations.
   ****************************************************************************/
  void SDLChessGameController::loadAvatars(const Vector<String>& pngPaths) {
    for (Integer i = 0; i < pngPaths.size(); ++i) {
      getAvatarManager()->add(SDLAvatar::createPointer(i, pngPaths[i], getWindow()));
      avatar_indices_.emplace_back(i);
    }
  }


  /****************************************************************************/
  SDLEntityView::Pointer SDLChessGameController::createTileMapView
  (TileMap::Pointer tileMap, SDLRectangle tileMapFrame)
  {
    return createTileMapView(tileMap, tileMapFrame.x, tileMapFrame.y, tileMapFrame.w, tileMapFrame.h);
  }

  /****************************************************************************
   * DONE
   * Creates the view of tile map, also links it to the model and vice versa.
   * 
   * \param tileMap The model of this tile map view.
   * \param x       The x position of this tile map view.
   * \param y       The y position of this tile map view.
   * \param w       The width of this tile map view.
   * \param h       The height of this tile map view.
   ****************************************************************************/
  SDLEntityView::Pointer SDLChessGameController::createTileMapView(TileMap::Pointer tileMapModel, Integer x, Integer y, Integer w, Integer h) {
    SDLRectangle boardFrame{ x, y, w, h };
    SDLEntityView::Pointer tileMapView = SDLEntityView::createPointer(SDLEntityParams(
      boardFrame,                 //avatarFrame
      boardFrame,                 //screenFrame
      SDLPoint{ 0, 0 },           //center
      0.0,                        //angle
      0,                          //row
      0,                          //column
      BoardGameZOrder::BOARD,  //zOrder
      SDLEntityParams::FLIP_NONE  //avatarFlip
    ));
    
    tileMapModel->setView(tileMapView);
    tileMapView->setModel(tileMapModel);
    return tileMapView;
  }

  /*******************************************************************************
   * DONE
   * \param scale Rate for zooming in or zooming out avatar frame while displaying
   *              on screen.
   *******************************************************************************/
  Vector<SDLEntityView::Pointer> SDLChessGameController::createTileViews(SDLEntityView::Pointer tileMapView, PlayerTurn thisPlayerTurn, Integer offset) const {
    //queries squares texture info
    Integer squaresAvatarWidth, squaresAvatarHeight;
    SDL_QueryTexture(
      getSquaresAvatar()->getTexture(),
      nullptr,
      nullptr,
      &squaresAvatarWidth,
      &squaresAvatarHeight
    );
    
    SDLRectangle squareAvatarUnitFrame{
      0,
      0,
      squaresAvatarWidth / 2,  //"2" is the avatar of squares has 4 consecutive squares and we need the width of only one.
      squaresAvatarHeight
    };

    //creates frame of squares on screen
    SDLRectangle& tileMapScreenFrame = tileMapView->parameters().screen_frame();
    Double scale;
    if (tileMapScreenFrame.h < tileMapScreenFrame.w)
      scale = (static_cast<Double>(tileMapScreenFrame.h - 2*offset) / ChessRule::NUMBER_OF_SQUARES_PER_LINE) / squaresAvatarHeight;
    else
      scale = (static_cast<Double>(tileMapScreenFrame.w - 2*offset) / ChessRule::NUMBER_OF_SQUARES_PER_LINE) / squaresAvatarHeight;

    SDLRectangle squareScreenFrame{ //base screen frame for each square/tile view
      tileMapScreenFrame.x + offset,
      tileMapScreenFrame.y + offset,
      static_cast<Integer>(squareAvatarUnitFrame.w * scale),
      static_cast<Integer>(squareAvatarUnitFrame.h * scale)
    };

    //creates tile_views
    TileMap& tileMapModel = *(static_cast<TileMap*>(tileMapView->getModel().get()));
    Vector<SDLEntityView::Pointer> tileViews;
        
    Function< Tile::Pointer(Integer, Integer) > getTileModel;
    if (thisPlayerTurn == PlayerTurn::FIRST) {
      getTileModel = [&tileMapModel](Integer rowOnScreen, Integer columnOnScreen) -> Tile::Pointer {
        return tileMapModel[ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - rowOnScreen][columnOnScreen];
      };
    }
    else {
      getTileModel = [&tileMapModel](Integer rowOnScreen, Integer columnOnScreen) -> Tile::Pointer {
        return tileMapModel[rowOnScreen][ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - columnOnScreen];

        //return tileMapModel[rowInScreen][ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - colInScreen];
      };
    }

    //!!!NOTE: the tiles is ordered row by row
    for (Integer i = 0; i < ChessRule::BOARD_SIZE; ++i) {
      Integer rowOnScreen = i / ChessRule::NUMBER_OF_SQUARES_PER_LINE;
      Integer columnOnScreen = i % ChessRule::NUMBER_OF_SQUARES_PER_LINE;
      tileViews.emplace_back(SDLEntityView::createPointer(
        getSquaresAvatar(),                 //avatar
        squareAvatarUnitFrame,              //avatarUnitFrame
        squareScreenFrame + SDLVector{ columnOnScreen * squareScreenFrame.w, rowOnScreen * squareScreenFrame.h },  //screenFrame
        0,                                  //avatar row
        (rowOnScreen + columnOnScreen) % 2, //avatar column
        BoardGameZOrder::SQUARE             //zOrder
      ));

      Tile::Pointer tileModel = getTileModel(rowOnScreen, columnOnScreen);
      tileViews[i]->setModel(tileModel);
      tileModel->setView(tileViews[i]);

    }

    tileViews.shrink_to_fit();

#ifdef _DEBUG1
    if (thisPlayerTurn == PlayerTurn::FIRST) {
      for (auto i : { 0, 1, 6, 7 }) {
        for (Integer j = 0; j < ChessRule::NUMBER_OF_SQUARES_PER_LINE; ++j) {
          CAPTURE(tileMapModel[i][j]->getView(), tileViews[(ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - i) * ChessRule::NUMBER_OF_SQUARES_PER_LINE + j]);
          CHECK(tileMapModel[i][j]->getView() == tileViews[(ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - i) * ChessRule::NUMBER_OF_SQUARES_PER_LINE + j]);
        }
      }
    }
    else {
      for (auto i : { 0, 1, 6, 7 }) {
        for (Integer j = 0; j < ChessRule::NUMBER_OF_SQUARES_PER_LINE; ++j) {
          CAPTURE(tileMapModel[i][j]->getView(), tileViews[(ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - j) + ChessRule::NUMBER_OF_SQUARES_PER_LINE * i]);
          CHECK(tileMapModel[i][j]->getView() == tileViews[(ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - j) + ChessRule::NUMBER_OF_SQUARES_PER_LINE * i]);
        }
      }

    }
#endif

    //links tile views forward and backward
    for (Integer i = 0; i < ChessRule::BOARD_SIZE - 1; ++i) {
      tileViews[i]->setNext(tileViews[i + 1]);
      tileViews[ChessRule::BOARD_SIZE - 1 - i]->setPrevious(tileViews[ChessRule::BOARD_SIZE - 2 - i]);
    }

    return tileViews;
  }

  /***************************************************************************
   * Creates chess piece views for each chess piece model from itemMgr, then
   * links them.
   * \param itemMgr  Manager of chess piece models.
   * \param tileView We need this for specifying screen frame for each chess
   *                 piece view.
   * \param scale    Screen frame of chess pieces = scale x screen frame of square
   ***************************************************************************/
  Vector<SDLEntityView::Pointer> SDLChessGameController::createItemViews
  (ItemManager::Pointer itemMgr, Vector<SDLEntityView::Pointer> tileViews, PlayerTurn thisPlayerTurn, Double scale) const 
  {
    //creates pieces avatar frame
    constexpr Integer NUM_PIECE_TYPES = 6;

    SDLRectangle whitePieceAvatarUnitFrame{  ///< the avatar of white or black pieces each has 6 pieces and we need the width of only one.
      0, 
      0, 
      getWhitePiecesAvatar()->getWidth() / NUM_PIECE_TYPES,
      getWhitePiecesAvatar()->getHeight()
    };  
    SDLRectangle blackPieceAvatarUnitFrame{  ///< the avatar of white or black pieces each has 6 pieces and we need the width of only one.
      0, 
      0, 
      getBlackPiecesAvatar()->getWidth() / NUM_PIECE_TYPES,
      getBlackPiecesAvatar()->getHeight() };
    
    Function< Integer(Integer, Integer) > getTileViewIndex;
    if (thisPlayerTurn == PlayerTurn::FIRST) {
      getTileViewIndex = [](Integer row, Integer col) -> Integer {
          return col + ChessRule::NUMBER_OF_SQUARES_PER_LINE * (ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - row);
      };
    }
    else {
      getTileViewIndex = [](Integer row, Integer col) -> Integer {
        return ChessRule::NUMBER_OF_SQUARES_PER_LINE - 1 - col + ChessRule::NUMBER_OF_SQUARES_PER_LINE * row;
      };
    }

    Vector<SDLEntityView::Pointer> itemViews(ChessRule::NUMBER_OF_PIECES);

    for (Integer i = 0; i < ChessRule::NUMBER_OF_PIECES; ++i) { //loop through `itemMgr`
      Integer tileViewIndex = getTileViewIndex(
        (*itemMgr)[i]->getTile(0)->getRow(), 
        (*itemMgr)[i]->getTile(0)->getCol() 
      );
      
      SDLRectangle& tileScreenFrame = tileViews[tileViewIndex]->parameters().screen_frame();
      //SDLRectangle pieceScreenFrame = 
      //add view of pieces
      itemViews[i] = SDLEntityView::createPointer(
        i%2==0 ? getWhitePiecesAvatar() : getBlackPiecesAvatar(),  //SDLAvatar::Pointer avatar,
        i%2==0 ? whitePieceAvatarUnitFrame : blackPieceAvatarUnitFrame,                     //SDLRectangle avatarUnitFrame,
        getCenteredRectangle(Integer(scale * tileScreenFrame.w), Integer(scale * tileScreenFrame.h), tileScreenFrame),  //SDLRectangle screenFrame,
        0,                                //Integer avatarRow,
        (*itemMgr)[i]->getType(),         //Integer avatarCol,
        BoardGameZOrder::ITEM             //Integer zOrder,
      );

      itemViews[i]->setModel((*itemMgr)[i]);
      (*itemMgr)[i]->setView(itemViews[i]);
    }

    //links item views forward and backward
    for (Integer i = 0; i < ChessRule::NUMBER_OF_PIECES - 1; ++i) {
      itemViews[i]->setNext(itemViews[i + 1]);
      itemViews[ChessRule::NUMBER_OF_PIECES - 1 - i]->setPrevious(itemViews[ChessRule::NUMBER_OF_PIECES - 2 - i]);
    }
    
    return itemViews;
  }
  
  Vector<SDLEntityView::Pointer> SDLChessGameController::createSquareDecorationViewSamples(SDLRectangle tileScreenFrame, Double const decorScales[]) {
    SDLRectangle decoratorAvatarUnitFrame {  ///< the avatar of white or black pieces each has 6 pieces and we need the width of only one.
      0,
      0,
      getSquareDecoratorAvatar()->getWidth() / NUMBER_OF_DECORATIONS,
      getSquareDecoratorAvatar()->getHeight()
    };

    Vector<SDLEntityView::Pointer> decoratorViews(NUMBER_OF_DECORATIONS + 1);

    for (Integer i = 0; i < NUMBER_OF_DECORATIONS + 1; ++i) {
      SDLRectangle decoratorScreenFrame{  ///< the avatar of white or black pieces each has 6 pieces and we need the width of only one.
        -getSquareDecoratorAvatar()->getWidth(),  //for the decorator residing outside the window
        -getSquareDecoratorAvatar()->getHeight(), //for the decorator residing outside the window
        static_cast<Integer>(decorScales[i] * tileScreenFrame.w),
        static_cast<Integer>(decorScales[i] * tileScreenFrame.h)
      };

      decoratorViews[i] = SDLEntityView::createPointer(
        nullptr,                    //SDLAvatar::Pointer avatar,
        decoratorAvatarUnitFrame,   //SDLRectangle avatarUnitFrame,
        decoratorScreenFrame,       // getCenteredRectangle(DECORATION_SCALES[i] * tileScreenFrame.w, DECORATION_SCALES[i] * tileScreenFrame.h, tileScreenFrame)//SDLRectangle screenFrame,
        0,                          //Integer avatarRow,
        i,                          //Integer avatarCol,
        BoardGameZOrder::DECORATION //Integer zOrder
      );
    }

    return decoratorViews;
  }  

  /***************************************************************************/
  Vector<SDLEntityView::Pointer> SDLChessGameController::createSquareDecorationViews
  (Vector<SDLEntityView::Pointer> const& squareViews, DecorationType decorType) 
  {
    const Integer NUMBER_OF_VIEWS = Integer(squareViews.size());
    Vector<SDLEntityView::Pointer> decorViews(NUMBER_OF_VIEWS);

    for (Integer i = 0; i < NUMBER_OF_VIEWS; ++i) {
      decorViews[i] = createDecorationOfSquare(decorType);
    }

    //links decorator views forward then backward
    for (Integer i = 0; i < NUMBER_OF_VIEWS - 1; ++i) {
      decorViews[i]->setNext(decorViews[i + 1]);
      decorViews[NUMBER_OF_VIEWS - 1 - i]->setPrevious(decorViews[NUMBER_OF_VIEWS - 2 - i]);
    }

    return decorViews;
  }
}