module;
#ifdef _DEBUG
#include <catch2/catch_template_test_macros.hpp>
#endif 

module chess.ChessRule;

namespace mbg {
  /*************************************************************************/
  SquareGroups& ChessRule::pickKingForCastling(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const {
    if ( isFreeSquare((*getTileMap())[row][col + 1])
      && isFreeSquare((*getTileMap())[row][col + 2]) )
    {
      ChessItem::Pointer nearRook = getPieceFromSquare(row, col + 3);
      if (nearRook != nullptr && nearRook->getMoveCount() == 0) {
        groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row][col + 2]);
      }
    }

    if ( isFreeSquare((*getTileMap())[row][col - 1])
      && isFreeSquare((*getTileMap())[row][col - 2])
      && isFreeSquare((*getTileMap())[row][col - 3]) )
    {
      ChessItem::Pointer farRook = getPieceFromSquare(row, col - 4);
      if (farRook != nullptr && farRook->getMoveCount() == 0) {
        groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row][col - 2]);
      }
    }

    return groupsOfPlaceableSquares;
  }

  /*****************************************************************************
   *
   * @return 2 vectors of squares. The vector at index 0 has the empty squares,
   *         The vector at index 1 has the squares that the enemy pieces is
   *         obtaining.
   *****************************************************************************/
  SquareGroups ChessRule::pickKing(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2); ///< [0]: is placeable empty squares, [1] is placeable squares of the opponent

    if (getPieceFromSquare(row, col)->getMoveCount() == 0) {
      pickKingForCastling(groupsOfPlaceableSquares, row, col);
    }

    if (row > 0) {
      addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 1, col);
      if (col > 0) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 1, col - 1);
      }

      if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 1, col + 1);
      }
    }

    if (row < NUMBER_OF_SQUARES_PER_LINE - 1) {
      addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 1, col);
      if (col > 0) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 1, col - 1);
      }

      if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 1, col + 1);
      }
    }

    if (col > 0) {
      addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row, col - 1);
    }

    if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
      addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row, col + 1);
    }

    return groupsOfPlaceableSquares;
  }

  /*************************************************************************
   * \param groupsOfPlaceableSquares [0] is placeable empty squares, [1] is 
   *                                 placeable squares of the opponent
   *************************************************************************/
  SquareGroups& ChessRule::pickRook(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const {
    //find placeable squares in column of number `col`
    for (Integer i = row - 1; i > -1; --i) {
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, i, col);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    for (Integer i = row + 1; i < NUMBER_OF_SQUARES_PER_LINE; ++i) {
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, i, col);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    //find placeable squares in the row of number `row`
    for (Integer j = col - 1; j > -1; --j) {
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row, j);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    for (Integer j = col + 1; j < NUMBER_OF_SQUARES_PER_LINE; ++j) {
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row, j);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    return groupsOfPlaceableSquares;
  }

  /*************************************************************************/
  SquareGroups ChessRule::pickKnight(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2); ///< [0] is placeable empty squares, [1] is placeable squares of the opponent

    if (row >= 2) {
      if (col >= 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 2, col - 1);
      }

      if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 2, col + 1);
      }
    }

    if (row < NUMBER_OF_SQUARES_PER_LINE - 2) {
      if (col >= 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 2, col - 1);
      }

      if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 2, col + 1);
      }
    }

    if (col >= 2) {
      if (row >= 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 1, col - 2);
      }

      if (row < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 1, col - 2);
      }
    }

    if (col < NUMBER_OF_SQUARES_PER_LINE - 2) {
      if (row >= 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - 1, col + 2);
      }

      if (row < NUMBER_OF_SQUARES_PER_LINE - 1) {
        addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + 1, col + 2);
      }
    }

    return groupsOfPlaceableSquares;
  }

  /*************************************************************************
   * \param [out] groupsOfPlaceableSquares [0] is placeable empty squares, [1] is placeable squares of the opponent
   *************************************************************************/
  SquareGroups& ChessRule::pickBishop(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const {
    Integer min, max;

    //find placeable squares in the diameter up
    if (row < col) {
      min = row;
      max = col;
    }
    else {
      min = col;
      max = row;
    }

    for (Integer k = 1; k <= min; ++k) { //<=> (k < row) && (k < col)
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - k, col - k);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    for (Integer k = 1; k < NUMBER_OF_SQUARES_PER_LINE - max; ++k) { //<=> (k < NUMBER_OF_SQUARES_PER_LINE - row) && (k < NUMBER_OF_SQUARES_PER_LINE - col)
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + k, col + k);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    //find placeable squares in the diameter down
    if (row < NUMBER_OF_SQUARES_PER_LINE - 1 - col) {
      min = row;
      max = NUMBER_OF_SQUARES_PER_LINE - 1 - col;
    }
    else {
      min = NUMBER_OF_SQUARES_PER_LINE - 1 - col;
      max = row;
    }

    for (Integer l = 1; l <= min; ++l) { //<=> (l < row) && (l < NUMBER_OF_SQUARES_PER_LINE - col)
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row - l, col + l);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    for (Integer l = 1; l < NUMBER_OF_SQUARES_PER_LINE - max; ++l) { //<=> (l < NUMBER_OF_SQUARES_PER_LINE - row) && (l < col)
      SquareState squareState = addPlaceableSquaresToGroups(groupsOfPlaceableSquares, row + l, col - l);
      if (squareState == ENEMY || squareState == ALLY) {
        break;
      }
    }

    return groupsOfPlaceableSquares;
  }

  /***************************************************************************
  SquareGroups& ChessRule::pickPawnFirstTime(SquareGroups& groupsOfPlaceableSquares, Integer row, Integer col) const {
    Integer step = (getThisPlayer()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;

    if (isFreeSquare( (*getTileMap())[row + step][col]) ) {
      groupsOfPlaceableSquares[EMPTY].emplace_back( (*getTileMap())[row + step][col] );
      if (isFreeSquare((*getTileMap())[row + 2 * step][col])) {
        groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row + 2 * step][col]);
      }
    }

    if (col > 0) {
      if (not isFreeSquare( (*getTileMap())[row + step][col - 1] )) {
        if (not isAllySquare( (*getTileMap())[row + step][col - 1]) ) {
          groupsOfPlaceableSquares[ENEMY].emplace_back( (*getTileMap())[row + step][col - 1] );
        }
      }
    }

    if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
      if (not isFreeSquare( (*getTileMap())[row + step][col + 1]) ) {
        if (not isAllySquare( (*getTileMap())[row + step][col + 1]) ) {
          groupsOfPlaceableSquares[ENEMY].emplace_back( (*getTileMap())[row + step][col + 1] );
        }
      }
    }

    return groupsOfPlaceableSquares;
  }
  */

  /*************************************************************************/
  SquareGroups ChessRule::pickPawn(Integer row, Integer col) const {
    SquareGroups groupsOfPlaceableSquares(2); ///< [0] is placeable empty squares, [1] is placeable squares of the opponent
    Integer const STEP = (getThisPlayer()->getTurn() == PlayerTurn::FIRST) ? 1 : -1;

    if (col > 0) {
      if (not isFreeSquare((*getTileMap())[row + STEP][col - 1])) {
        if (not isAllySquare((*getTileMap())[row + STEP][col - 1])) {
          groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col - 1]);
        }
      }
    }

    if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
      if (not isFreeSquare((*getTileMap())[row + STEP][col + 1])) {
        if (not isAllySquare((*getTileMap())[row + STEP][col + 1])) {
          groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col + 1]);
        }
      }
    }

    if (isFreeSquare((*getTileMap())[row + STEP][col])) {
      groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row + STEP][col]);
    }

    if (getPieceFromSquare(row, col)->getMoveCount() == 0) {  //handle with the situation in that the pawn moves first time
      if (isFreeSquare((*getTileMap())[row + 2 * STEP][col])) {
        groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row + 2 * STEP][col]);
      }
    }
    else if ( (NUMBER_OF_SQUARES_PER_LINE - 1 + STEP) >> 1 == row ) { /// ((NUMBER_OF_SQUARES_PER_LINE - 1 + STEP)/2) is the row that en passant capturing occurs (it is 4 if step == 1, and is 3 if step == -1
      if (col > 0 && encodeMoveInfo(PieceIndex::PAWN, row + 2 * STEP, col - 1, row, col - 1) == getLastMoveInfo()) {
        groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col - 1]);
      }
      else if (col < NUMBER_OF_SQUARES_PER_LINE - 1 && encodeMoveInfo(PieceIndex::PAWN, row + 2 * STEP, col + 1, row, col + 1) == getLastMoveInfo()) {
        groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col + 1]);
      }
    }

    

      /*
    if (getPieceFromSquare(row, col)->getMoveCount() == 0) {
      pickPawnFirstTime(groupsOfPlaceableSquares, row, col);
    }
    else {
      Integer const SPECIAL_ROW = (NUMBER_OF_SQUARES_PER_LINE - 1 + STEP) / 2; ///< The row that en passant capturing occurs (it is 4 if step == 1, and is 3 if step == -1)

      if (isFreeSquare((*getTileMap())[row + STEP][col])) {
        groupsOfPlaceableSquares[EMPTY].emplace_back((*getTileMap())[row + STEP][col]);
      }

      if (col > 0) {
        if (not isFreeSquare((*getTileMap())[row + STEP][col - 1])) { //capture an enemy piece normally
          if (not isAllySquare((*getTileMap())[row + STEP][col - 1])) {
            groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col - 1]);
          }
        }
        else if ( SPECIAL_ROW == row //capture an enemy piece en passant
               && encodeMoveInfo(PieceIndex::PAWN, row + 2*STEP, col - 1, row, col - 1) == getLastMoveInfo() )
        {
          groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col - 1]);
        }
      }

      if (col < NUMBER_OF_SQUARES_PER_LINE - 1) {
        if (not isFreeSquare((*getTileMap())[row + STEP][col + 1])) {
          if (not isAllySquare((*getTileMap())[row + STEP][col + 1])) {
            groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col + 1]);
          }
        }
        else if (not isFreeSquare((*getTileMap())[row][col + 1])) {
          if (not isAllySquare((*getTileMap())[row][col + 1])
            && std::static_pointer_cast<ChessItem>(getPieceFromSquare(row, col))->getMoveCount() == 1)
          {
            groupsOfPlaceableSquares[ENEMY].emplace_back((*getTileMap())[row + STEP][col + 1]);
          }
        }
      }
    }
    */

    return groupsOfPlaceableSquares;
  }

  /*************************************************************************/
  void ChessRule::captureEnPassant(ChessItem::Pointer pawn, Integer destRow, Integer destCol, Integer step) const {
    //capture the enemy Pawn
    removePieceFromPlayer(getPieceFromSquare(destRow - step, destCol));

    //getPieceFromSquare(destRow, destCol)->removeTile(0);
    (*getTileMap())[destRow - step][destCol]->removeItem(0);

    //move the ally pawn
    pawn->getTile()->removeItem(0);
    pawn->setSquare((*getTileMap())[destRow][destCol]);
    (*getTileMap())[destRow][destCol]->addItem(pawn);

    //pawn->increaseMoveCount();

  }

  /*************************************************************************/
  void ChessRule::castleKingSide(ChessItem::Pointer king, Integer destRow, Integer destCol) const {
    //move the near Rook to the square of (destRow, destCol - 1)
    ChessItem::Pointer nearRook = getPieceFromSquare(destRow, destCol + 1);

    nearRook->getTile(0)->removeItem(0);
    nearRook->setTile(0, (*getTileMap())[destRow][destCol - 1]);
    (*getTileMap())[destRow][destCol - 1]->addItem(nearRook);

    //nearRook->increaseMoveCount();

    //move the king to the square of (destRow, destCol)
    king->getTile(0)->removeItem(0);
    king->setTile(0, (*getTileMap())[destRow][destCol]);
    (*getTileMap())[destRow][destCol]->addItem(king);

    //king->increaseMoveCount();
  }

  /*************************************************************************/
  void ChessRule::castleQueenSide(ChessItem::Pointer king, Integer destRow, Integer destCol) const {
    //move the far Rook to the square of (destRow, destCol + 1)
    ChessItem::Pointer farRook = getPieceFromSquare(destRow, destCol - 2);

    farRook->getTile(0)->removeItem(0);
    farRook->setTile(0, (*getTileMap())[destRow][destCol + 1]);
    (*getTileMap())[destRow][destCol + 1]->addItem(farRook);

    //farRook->increaseMoveCount();

    //move the king to the square of (destRow, destCol)
    king->getTile(0)->removeItem(0);
    king->setTile(0, (*getTileMap())[destRow][destCol]);
    (*getTileMap())[destRow][destCol]->addItem(king);

    //king->increaseMoveCount();
  }

  /***************************************************************************/
  Boolean ChessRule::checkRookCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    if (endRow == startRow) {
      Integer max, min;
      if (endCol > startCol) {
        max = endCol;
        min = startCol;
      }
      else {
        min = endCol;
        max = startCol;
      }

      for (Integer i = min + 1; i < max; ++i) {
        if (not isFreeSquare((*getTileMap())[endRow][i])) {
          return false;
        }
      }

      return true;
    }

    if (endCol == startCol) {
      Integer max, min;
      if (endRow > startRow) {
        max = endRow;
        min = startRow;
      }
      else {
        min = endRow;
        max = startRow;
      }

      for (Integer i = min + 1; i < max; ++i) {
        if (not isFreeSquare((*getTileMap())[i][endCol])) {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  /*************************************************************************/
  Boolean ChessRule::checkBishopCapturable(Integer startRow, Integer startCol, Integer endRow, Integer endCol) const {
    Integer rowDistance = endRow - startRow;
    Integer colDistance = endCol - startCol;

    if ((rowDistance == colDistance) ^ (rowDistance == -colDistance)) {
      Integer dr, dc;
      if (rowDistance > 0) {
        dr = 1;
      }
      else {
        dr = -1;
      }

      if (colDistance > 0) {
        dc = 1;
      }
      else {
        dc = -1;
      }

      for (Integer i = startRow + dr, j = startCol + dc;
        i != endRow;
        i += dr, j += dc)
      {
        if (not isFreeSquare((*getTileMap())[i][j])) {
          return false;
        }
      }
      return true;
    }

    return false;
  }
  
  /*************************************************************************/
  Boolean ChessRule::inCheck(Player::Pointer checkingPlayer, Integer destRow, Integer destCol) const {
    for (Integer i = 0; i < checkingPlayer->getNumberOfItems(); ++i) {
      if ( (this->*check_capture_procedures_[checkingPlayer->item(i)->getType()])(
              checkingPlayer->item(i)->getTile(0)->getRow(),
              checkingPlayer->item(i)->getTile(0)->getCol(),
              destRow,
              destCol ))
      {
        return true;
      }
    }

    return false;
  }
}
