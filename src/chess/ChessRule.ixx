module;

export module chess.ChessRule;

import foundation.basetypes;
import foundation.Matrix;
import foundation.Function;
import foundation.SharedPointer;

import game_model.Game2DModel;
import game_model.Tile;
import game_model.TileMap;
import game_model.Player;
import game_model.Rule;
import foundation.PlayerTurn;

import chess.PieceIndex;
import chess.ChessItem;

namespace mbg {
  export using SquareGroups = Matrix<Tile::Pointer>;

  export class ChessRule : public Rule {
  public://types
    using Pointer = SharedPointer<ChessRule>;

    enum SquareState {
      EMPTY = 0,
      ENEMY,
      ALLY
    };

  public://member functions
    virtual ~ChessRule() = default;

    /***************************************************************************/
    TileMap::Pointer getTileMap() const { return getGameModel()->getTileMap(); }

    /***************************************************************************/
    Player::Pointer getThisPlayer() const {
      return getGameModel()->getPlayerManager()->getThisPlayer();
    }

    /***************************************************************************/
    Player::Pointer getOpponent() const {
      return getGameModel()->getPlayerManager()->operator[](1);
    }

    /***************************************************************************/
    ChessItem::Pointer getKing(Player::Pointer player) const {
      return std::static_pointer_cast<ChessItem>(player->item(0));
    }

    /***************************************************************************/
    void storeMoveInfo(Integer moveCode) {
      move_history_.emplace_back(moveCode);
    }

    /***************************************************************************/
    Integer getMoveInfo(SizeType i) const {
      return move_history_[i];
    }

    /***************************************************************************/
    Integer getLastMoveInfo() const {
      return move_history_.back();
    }

    /***************************************************************************/
    Integer getLastMoveInfo(PlayerTurn playerTurn) const {
      return (playerTurn == PlayerTurn::FIRST) ? 
               1 + move_history_[move_history_.size() >> 1 << 1] 
             :     move_history_[move_history_.size() >> 1 << 1];
    }

    /***************************************************************************/
    Integer getPieceTypeOfMove(SizeType i) const {
      return decodeMoveInfo(move_history_[i]).front();
    }

    /***************************************************************************/
    Integer getPieceTypeOfLastMove() const {
      return decodeMoveInfo(getLastMoveInfo()).front();
    }

    /***************************************************************************/
    Integer getPieceTypeOfLastMove(PlayerTurn playerTurn) const {
      return decodeMoveInfo(getLastMoveInfo(playerTurn)).front();
    }

    /***************************************************************************/
    Boolean isAllySquare(Tile::Pointer square) const {
      return square->item(0)->getOwner(0) == getThisPlayer();
    }

    /***************************************************************************/
    Boolean areHostilePieces(Item::Pointer piece1, Item::Pointer piece2) const {
      return piece1->getOwner(0) != piece2->getOwner(0);
    }
    /***************************************************************************/
    Boolean isFreeSquare(Tile::Pointer square) const { return not square->hasItem(); }
    
    /***************************************************************************/
    ChessItem::Pointer getPieceFromSquare(Integer row, Integer col) const {
      return getPieceFromSquare( (*getTileMap())[row][col] );
    }   

    /***************************************************************************/
    ChessItem::Pointer getPieceFromSquare(Tile::Pointer square) const {
      return square->hasItem() ? std::static_pointer_cast<ChessItem>( square->item(0) ) : nullptr;
    }

    /****at server side
    Boolean isCheckmated();
    Boolean isDraw();
    Boolean isResigning();
    Boolean isDeadSituation();
    */

    /***************************************************************************/
    //pick piece procedures
    SquareGroups& pickKingForCastling(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const;
    SquareGroups  pickKing(Integer row, Integer col) const;          ///< Return 2 vectors of squares that the king at the tile of (`row`, `col`) coordinates can move to.
    SquareGroups& pickRook(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const;
    SquareGroups& pickBishop(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const;
    SquareGroups  pickKnight(Integer row, Integer col) const;
    //SquareGroups& pickPawnFirstTime(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const;
    SquareGroups  pickPawn(Integer row, Integer col) const;

    inline SquareGroups pickQueen(Integer row, Integer col) const;
    inline SquareGroups pickRook(Integer row, Integer col) const;
    inline SquareGroups pickBishop(Integer row, Integer col) const;
    
    /***************************************************************************/
    //procedures for checking whether a specific square can be captured by a piece without informations about tilemap (a.k.a board)
    Boolean checkRookCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    Boolean checkBishopCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkKingCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkQueenCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkKnightCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkPawnCapturableInNormalWay(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    //inline Boolean checkPawnCapturableEnPassant(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;

    /***************************************************************************/
    //Procedures for checking whether a piece from a specific square can move to another specific square with informations about tilemap (a.k.a board).
    //These procedures use the `check...Capturable()` above.
    inline Boolean checkKingMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkKingMovableInNormalWay(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkQueenMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkRookMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkKnightMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;
    inline Boolean checkBishopMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const; 
    inline Boolean checkPawnMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const;

    /***************************************************************************/
    //move piece procedures
    inline Vector<ChessItem::Pointer> movePawn(ChessItem::Pointer pawn, Integer destRow, Integer destCol) const;
    inline Vector<ChessItem::Pointer> moveKing(ChessItem::Pointer king, Integer destRow, Integer destCol) const;
 
    inline Vector<ChessItem::Pointer> tryToMovePawn(ChessItem::Pointer pawn, Integer destRow, Integer destCol) const;
    inline Vector<ChessItem::Pointer> tryToMoveKing(ChessItem::Pointer king, Integer destRow, Integer destCol) const;
    inline Vector<ChessItem::Pointer> tryToMoveNormalPiece(ChessItem::Pointer piece, Integer destRow, Integer destCol) const;
      
    /***************************************************************************/
    inline SquareGroups pick(ChessItem::Pointer piece) const;
    inline Boolean checkMove(ChessItem::Pointer piece, Integer endRow, Integer endCol) const;
    inline Boolean inCheck(Player::Pointer checkingPlayer, Player::Pointer checkedPlayer) const;    ///< check if the `checkedPlayer` is in check by the `checkingPlayer`
    Boolean inCheck(Player::Pointer checkingPlayer, Integer xPos, Integer yPos) const;    

    inline Vector<ChessItem::Pointer> tryToMove(ChessItem::Pointer piece, Integer destRow, Integer destCol) const;
    inline Vector<ChessItem::Pointer> tryToMove(ChessItem::Pointer piece, Tile::Pointer square) const;
    inline Vector<ChessItem::Pointer> tryToMove(Integer startRow, Integer startCol, Integer destRow, Integer destCol) const;
    
    inline Vector<ChessItem::Pointer> move(ChessItem::Pointer piece, Integer destRow, Integer destCol) const;
    inline Vector<ChessItem::Pointer> move(ChessItem::Pointer piece, Tile::Pointer square) const;
    inline Vector<ChessItem::Pointer> move(Integer startRow, Integer startCol, Integer destRow, Integer destCol) const;
    
    /***************************************************************************/
    void captureEnPassant(ChessItem::Pointer pawn, Integer destRow, Integer destCol, Integer step = 1) const;
    void castleKingSide(ChessItem::Pointer king, Integer destRow, Integer destCol) const;
    void castleQueenSide(ChessItem::Pointer king, Integer destRow, Integer destCol) const;
    inline void promotePawn(ChessItem::Pointer pawn, PieceIndex pieceType) const;

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new ChessRule(std::forward<ARGS>(args)...));
    }
    inline static Integer encodeMoveInfo(PieceIndex piece, Integer startRow, Integer startCol, Integer endRow, Integer endCol) {
      constexpr Integer NUM_BITS = 3;     //3 is 3 bits need to encode each of 4 paramaters
      return  (piece    << 4 * NUM_BITS)
            | (startRow << 3 * NUM_BITS)
            | (startCol << 2 * NUM_BITS)
            | (endRow   <<     NUM_BITS)
            |  endCol;
    }

    inline static Vector<Integer> decodeMoveInfo(Integer data) {
      constexpr Integer NUM_BITS = 3;     //3 is 3 bits need to encode each of 4 paramaters
      constexpr Integer BIT_MASK = 0b111;
      constexpr Integer NUM_ELEMS = 5;
      Vector<Integer> info(NUM_ELEMS);
      info[4] =   BIT_MASK & data;                                    //index of `endCol`
      info[3] = ((BIT_MASK <<     NUM_BITS) & data) >>     NUM_BITS;  //index of `endRow`
      info[2] = ((BIT_MASK << 2 * NUM_BITS) & data) >> 2 * NUM_BITS;  //index of `startCol`
      info[1] = ((BIT_MASK << 3 * NUM_BITS) & data) >> 3 * NUM_BITS;  //index of `startRow`
      info[0] = ((BIT_MASK << 4 * NUM_BITS) & data) >> 4 * NUM_BITS;  //piece type

      return info;
    }
    inline static void removePieceFromPlayer(ChessItem::Pointer piece) {
      Player::Pointer player = piece->getOwner();
      player->item(piece->getIndexFromPlayer()) = player->getBack();
      static_cast<ChessItem*>(player->getBack().get())->setIndexFromPlayer(piece->getIndexFromPlayer());
      player->popBackItem();
    }

  //protected://contructors
    ChessRule(Game2DModel::Pointer gameModel)
    : Rule(gameModel),
      move_history_({0b111000000000000}),
      pick_procedures_({
        &ChessRule::pickKing,
        &ChessRule::pickQueen,
        &ChessRule::pickRook,
        &ChessRule::pickBishop,
        &ChessRule::pickKnight,
        &ChessRule::pickPawn
      }),
      check_capture_procedures_({
        &ChessRule::checkKingCapturable,
        &ChessRule::checkQueenCapturable,
        &ChessRule::checkRookCapturable,
        &ChessRule::checkBishopCapturable,
        &ChessRule::checkKnightCapturable,
        &ChessRule::checkPawnCapturableInNormalWay
      }),
      check_move_procedures_({
        &ChessRule::checkKingMovable,
        &ChessRule::checkQueenMovable,
        &ChessRule::checkRookMovable,
        &ChessRule::checkBishopMovable,
        &ChessRule::checkKnightMovable,
        &ChessRule::checkPawnMovable
      })
    { }

  private://member functions
    inline SquareState addPlaceableSquaresToGroups(SquareGroups& groupsOfPlaceableSquares, Integer r, Integer c) const;
    inline Vector<ChessItem::Pointer> placePieceOnSquare(ChessItem::Pointer piece, Integer destRow, Integer destCol) const;

  public://static data
    static Integer constexpr NUMBER_OF_SQUARES_PER_LINE = 8;
    static Integer constexpr NUMBER_OF_PIECES = 32;
    static Integer constexpr BOARD_SIZE = NUMBER_OF_SQUARES_PER_LINE * NUMBER_OF_SQUARES_PER_LINE;
    static Double  constexpr PIECE_VALUES[] = {20.0, 9.0, 5.0, 3.0, 3.0, 1.0};

  private://data members
    Vector< Integer > move_history_; ///< 
    Vector< SquareGroups(ChessRule::*)(Integer, Integer) const > pick_procedures_; ///< Procedures that return the pieces that this player can pick.
    Vector< Boolean(ChessRule::*)(Integer, Integer, Integer, Integer) const > check_capture_procedures_; ///< Procedures that is similar to check move procedures but without informations about the board 
    Vector< Boolean(ChessRule::*)(Integer, Integer, Integer, Integer) const > check_move_procedures_;    ///< Procedures for checking if the piece at a specific postion is able to move to other specific position.
                                                                                                         ///< Note that they operate with informations about the board.
  };


  /*************************************************************************/
  inline Boolean ChessRule::checkKingMovableInNormalWay(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    return (isFreeSquare((*getTileMap())[endRow][endCol]) || not isAllySquare((*getTileMap())[endRow][endCol]))
      && checkKingCapturable(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkKingMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    //TODO!045 Still need to supplement castling conditions
    if (getPieceFromSquare(startRow, startCol)->getMoveCount() == 0 && startRow == endRow) {
      if (endCol == startCol + 2) {
        if ( isFreeSquare((*getTileMap())[startRow][startCol + 1])
          && isFreeSquare((*getTileMap())[startRow][startCol + 2]))
        {
          ChessItem::Pointer nearRook = getPieceFromSquare(startRow, startCol + 3);
          if (nearRook != nullptr && nearRook->getMoveCount() == 0) {
            return true;
          }
        }
      }
      else if (endCol == startCol - 2) {
        if ( isFreeSquare((*getTileMap())[startRow][startCol - 1])
          && isFreeSquare((*getTileMap())[startRow][startCol - 2])
          && isFreeSquare((*getTileMap())[startRow][startCol - 3]))
        {
          ChessItem::Pointer farRook = getPieceFromSquare(startRow, startCol - 4);
          if (farRook != nullptr && farRook->getMoveCount() == 0) {
            return true;
          }
        }
      }
    }

    return checkKingMovableInNormalWay(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkQueenMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol)const {
    return   checkRookMovable(startRow, startCol, endRow, endCol)
          || checkBishopMovable(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkRookMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    return (isFreeSquare((*getTileMap())[endRow][endCol]) || not isAllySquare((*getTileMap())[endRow][endCol]))
         && checkRookCapturable(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkBishopMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    return (isFreeSquare((*getTileMap())[endRow][endCol]) || not isAllySquare((*getTileMap())[endRow][endCol]))
         && checkBishopCapturable(startRow, startCol, endRow, endCol);
  }

  /***************************************************************************/
  inline Boolean ChessRule::checkKnightMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    return (isFreeSquare((*getTileMap())[endRow][endCol]) || not isAllySquare((*getTileMap())[endRow][endCol]))
         && checkKnightCapturable(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkPawnMovable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    Integer const STEP = (getThisPlayer()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;

    if (endRow == startRow + STEP) {
      if (endCol == startCol) {
        return isFreeSquare((*getTileMap())[endRow][endCol]);
      }
      else if ((endCol == startCol + 1) || (endCol == startCol - 1)) {
        if (isFreeSquare((*getTileMap())[endRow][endCol])) {
          //[x]TODO!050 check if it can capture en passant
          return encodeMoveInfo(PieceIndex::PAWN, endRow + STEP, endCol, endRow - STEP, endCol) == getLastMoveInfo();
        }
        else {
          return not isAllySquare((*getTileMap())[endRow][endCol]);
        }
      }
    }
    else if (endRow == startRow + 2 * STEP) {
      if ( getPieceFromSquare(startRow, startCol)->getMoveCount() > 0 ) {
        return false;
      }

      if (endCol == startCol) {
        return isFreeSquare((*getTileMap())[endRow][endCol]) && isFreeSquare((*getTileMap())[endRow - STEP][endCol]);
      }
    }

    return false;
  }

  /*************************************************************************/
  inline SquareGroups ChessRule::pickQueen(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2);      ///< [0] is placeable empty squares, [1] is placeable squares of the opponent
    pickRook(groupsOfPlaceableSquares, row, col);   //find placeable squares in column of number `col` and row of number `row`
    pickBishop(groupsOfPlaceableSquares, row, col); //find placeable squares in the diameter up and down

    return groupsOfPlaceableSquares;
  }

  /*************************************************************************/
  inline SquareGroups ChessRule::pickRook(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2); ///< [0] is placeable empty squares, [1] is placeable squares of the opponent
    pickRook(groupsOfPlaceableSquares, row, col);
    return groupsOfPlaceableSquares;
  }

  /*************************************************************************/
  inline SquareGroups ChessRule::pickBishop(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2); ///< [0] is placeable empty squares, [1] is placeable squares of the opponent
    pickBishop(groupsOfPlaceableSquares, row, col);
    return groupsOfPlaceableSquares;
  }



  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::placePieceOnSquare(ChessItem::Pointer piece, Integer destRow, Integer destCol) const {
    Vector<ChessItem::Pointer> changedPieces{ piece };
    
    if ( not isFreeSquare((*getTileMap())[destRow][destCol]) )
    {
      removePieceFromPlayer(getPieceFromSquare(destRow, destCol));
      changedPieces.emplace_back(getPieceFromSquare(destRow, destCol));
      //getPieceFromSquare(destRow, destCol)->removeTile(0);
      (*getTileMap())[destRow][destCol]->setItem(0, piece);
    }
    else {
      (*getTileMap())[destRow][destCol]->addItem(piece);
    }

    piece->getSquare()->removeItem(0);
    piece->setSquare((*getTileMap())[destRow][destCol]);
    
    return changedPieces;
  }


  /*************************************************************************/
  inline Boolean ChessRule::checkKingCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    Boolean rowCond = (endRow == startRow + 1) || (endRow == startRow - 1);
    Boolean colCond = (endCol == startCol + 1) || (endCol == startCol - 1);
    return (rowCond && colCond) || (rowCond && (endCol == startCol)) || (colCond && (endRow == startRow));
  }


  /*************************************************************************/
  inline Boolean ChessRule::checkQueenCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    return  checkRookCapturable(startRow, startCol, endRow, endCol)
      || checkBishopCapturable(startRow, startCol, endRow, endCol);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkKnightCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    Integer rowDistance = std::abs(endRow - startRow);
    Integer colDistance = std::abs(endCol - startCol);
    return (rowDistance != 0) && (colDistance != 0) && (3 == rowDistance + colDistance);
  }

  /*************************************************************************/
  inline Boolean ChessRule::checkPawnCapturableInNormalWay(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    if ((endCol == startCol + 1) || (endCol == startCol - 1)) {
      if (getPieceFromSquare(startRow, startCol)->getOwner()->getTurn() == PlayerTurn::FIRST) {
        return endRow == startRow + 1;
      }
      else {
        return endRow == startRow - 1;
      }
    }

    return false;
  }

  /*************************************************************************/
  inline void ChessRule::promotePawn(ChessItem::Pointer pawn, PieceIndex pieceType) const {
    pawn->setType(pieceType);
    pawn->setValue(PIECE_VALUES[pieceType]);
  }

  /*************************************************************************/
  inline SquareGroups ChessRule::pick(ChessItem::Pointer piece) const {
    return  (this->*pick_procedures_[piece->getType()])(
              piece->getTile()->getRow(),
              piece->getTile()->getCol()
            );
  }

  /***************************************************************************/
  inline Boolean ChessRule::checkMove(ChessItem::Pointer piece, Integer endRow, Integer endCol) const {
    return  (this->*check_move_procedures_[piece->getType()])(
              piece->getTile()->getRow(),
              piece->getTile()->getCol(),
              endRow,
              endCol
            );
  }


  /*************************************************************************/
  inline Boolean ChessRule::inCheck(Player::Pointer checkingPlayer, Player::Pointer checkedPlayer) const {
    return inCheck(
             checkingPlayer, 
             getKing(checkedPlayer)->getTile()->getRow(),
             getKing(checkedPlayer)->getTile()->getCol()
           );
  }  
  
  /***************************************************************************
   * \return The index of `groupsOfPlaceableSquares` that the square (r, c) is
   *         pushed in. If not return 2 (ALLY).
   ***************************************************************************/
  inline ChessRule::SquareState ChessRule::addPlaceableSquaresToGroups(SquareGroups& groupsOfPlaceableSquares, Integer r, Integer c) const {
    if (isFreeSquare((*getTileMap())[r][c])) {
      groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[r][c]);
      return EMPTY;
    }

    if (not isAllySquare((*getTileMap())[r][c])) {
      groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[r][c]);
      return ENEMY;
    }

    return ALLY;
  }

  /***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMovePawn(ChessItem::Pointer pawn, Integer destRow, Integer destCol) const {
    Vector<ChessItem::Pointer> changedPieces;
    Integer step = (pawn->getOwner()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;

    if ( destCol != pawn->getCol()
      && not (*getTileMap())[destRow][destCol]->hasItem() )
    {
      Tile::Pointer startSquare = pawn->getTile();
      changedPieces.emplace_back(pawn);
      changedPieces.emplace_back(getPieceFromSquare(destRow - step, destCol));
      captureEnPassant(pawn, destRow, destCol, step);

      if (inCheck(getOpponent(), getThisPlayer())) {
        //reverse capturing en passant
        //step 1. replace the enemy pawn
        changedPieces[1]->setIndexFromPlayer(changedPieces[1]->getOwner()->getNumberOfItems());
        changedPieces[1]->getOwner()->addItem(changedPieces[1]);
        changedPieces[1]->getTile()->addItem(changedPieces[1]);

        //step 2. reverse the pawn moving
        changedPieces[0]->getTile()->removeItem(0);
        changedPieces[0]->setSquare(startSquare);
        startSquare->addItem(changedPieces[0]);
        
        //step 3. return empty changedPieces
        changedPieces.clear();
      }

      return changedPieces;
    }

    changedPieces = tryToMoveNormalPiece(pawn, destRow, destCol);
    
    return changedPieces;
  }

  /***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMoveKing(ChessItem::Pointer king, Integer destRow, Integer destCol) const {
    Vector<ChessItem::Pointer> changedPieces;
    if (king->getMoveCount() == 0) {
      if (destCol + 2 == king->getCol()) {
        if ( inCheck(getOpponent(), destRow, destCol)
          || inCheck(getOpponent(), destRow, destCol + 1)
          || inCheck(getOpponent(), destRow, destCol + 2))
        {
          return changedPieces;
        }
        else {
          changedPieces.emplace_back(king);
          changedPieces.emplace_back(getPieceFromSquare(destRow, destCol - 2));
          castleQueenSide(king, destRow, destCol);

          return changedPieces;
        }
      }
      else if (destCol - 2 == king->getCol()) {
        if ( inCheck(getOpponent(), destRow, destCol)
          || inCheck(getOpponent(), destRow, destCol - 1)
          || inCheck(getOpponent(), destRow, destCol - 2))
        {
          return changedPieces;
        }
        else {
          changedPieces.emplace_back(king);
          changedPieces.emplace_back(getPieceFromSquare(destRow, destCol + 1));
          castleKingSide(king, destRow, destCol);

          return changedPieces;
        }
      }
    }

    changedPieces = tryToMoveNormalPiece(king, destRow, destCol);
    return changedPieces;
  }

  /***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMoveNormalPiece(ChessItem::Pointer piece, Integer destRow, Integer destCol) const {
    Tile::Pointer startSquare = piece->getTile();
    Vector<ChessItem::Pointer> changedPieces = placePieceOnSquare(piece, destRow, destCol);

    if (inCheck(getOpponent(), getThisPlayer())) {
      //reverse moving
      //step 1. replace the enemy piece
      if (changedPieces.size() > 1) {
        changedPieces[1]->setIndexFromPlayer(changedPieces[1]->getOwner()->getNumberOfItems());
        changedPieces[1]->getOwner()->addItem(changedPieces[1]);
        changedPieces[1]->getSquare()->setItem(0, changedPieces[1]);
      }
      else {
        changedPieces[0]->getSquare()->removeItem(0);
      }

      //step 2. reverse the piece moving
      changedPieces[0]->setSquare(startSquare);
      startSquare->addItem(changedPieces[0]);

      //step 3. return empty changedPieces
      changedPieces.clear();
    }

    return changedPieces;
  }
  
  /***************************************************************************
 * \return List of squares that are changed.
 ***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMove(ChessItem::Pointer piece, Integer destRow, Integer destCol) const {
    if (piece->getType() == PieceIndex::PAWN) {
      return tryToMovePawn(piece, destRow, destCol);
    }
    else if (piece->getType() == PieceIndex::KING) {
      return tryToMoveKing(piece, destRow, destCol);
    }
    else {
      return tryToMoveNormalPiece(piece, destRow, destCol);
    }
  }

  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMove(ChessItem::Pointer piece, Tile::Pointer square) const {
    return tryToMove(piece, square->getRow(), square->getCol());
  }

  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::tryToMove(Integer startRow, Integer startCol, Integer destRow, Integer destCol) const {
    return tryToMove(getPieceFromSquare(startRow, startCol), destRow, destCol);
  }

  /***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::movePawn(ChessItem::Pointer pawn, Integer destRow, Integer destCol) const {
    Vector<ChessItem::Pointer> changedPieces;
    if ( destCol != pawn->getCol()
      && not (*getTileMap())[destRow][destCol]->hasItem() )
    {
      Integer step = (pawn->getOwner()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;
      changedPieces.emplace_back(pawn);
      changedPieces.emplace_back(getPieceFromSquare(destRow - step, destCol));
    
      captureEnPassant(pawn, destRow, destCol, step);
      pawn->increaseMoveCount();
      
      return changedPieces;
    }

    changedPieces = placePieceOnSquare(pawn, destRow, destCol);
    pawn->increaseMoveCount();
    return changedPieces;
  }
  
  /***************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::moveKing(ChessItem::Pointer king, Integer destRow, Integer destCol) const {
    Vector<ChessItem::Pointer> changedPieces;
    if (king->getMoveCount() == 0) {
      if (destCol + 2 == king->getCol()) {
        castleQueenSide(king, destRow, destCol);
        changedPieces.emplace_back(king);
        changedPieces.emplace_back(getPieceFromSquare(destRow, destCol + 1));

        king->increaseMoveCount();
        changedPieces[1]->increaseMoveCount(); //increases move count of the far rook

        return changedPieces;
      }
      else if (destCol - 2 == king->getCol()) {
        castleKingSide(king, destRow, destCol);
        changedPieces.emplace_back(king);
        changedPieces.emplace_back(getPieceFromSquare(destRow, destCol - 1));

        king->increaseMoveCount();
        changedPieces[1]->increaseMoveCount(); //increases move count of the near rook

        return changedPieces;
      }
    }

    changedPieces = placePieceOnSquare(king, destRow, destCol);
    king->increaseMoveCount();
    return changedPieces;
  }

  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::move(ChessItem::Pointer piece, Integer destRow, Integer destCol) const {
    if (piece->getType() == PieceIndex::PAWN) {
      return movePawn(piece, destRow, destCol);
    }
    else if (piece->getType() == PieceIndex::KING) {
      return moveKing(piece, destRow, destCol);
    }
    else {
      piece->increaseMoveCount();
      return placePieceOnSquare(piece, destRow, destCol);
    }
  }

  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::move(ChessItem::Pointer piece, Tile::Pointer square) const {
    return move(piece, square->getRow(), square->getCol());
  }

  /*************************************************************************/
  inline Vector<ChessItem::Pointer> ChessRule::move(Integer startRow, Integer startCol, Integer destRow, Integer destCol) const {
    return move(getPieceFromSquare(startRow, startCol), destRow, destCol);
  }
}