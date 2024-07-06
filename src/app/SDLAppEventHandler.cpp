module;
#ifdef _DEBUG
import <iostream>;
#endif 

module app.SDLAppEventHandler;

namespace mbg {
  /*****************************************************************************/
  void SDLAppEventHandler::handleUserEvent(SDLEvent* event) {
#ifdef _DEBUG
    std::cout << "\n\n...void SDLAppEventHandler::handleUserEvent(SDLEvent* event)...";
    std::cout << "\n  user event type = " << event->user.type - getChessGameEventManager()->getFirstUserEventIndex();

#endif 

    if (event->user.type == getChessGameEventManager()->getEnemyMoveEventIndex()) {
      (*getChessGameEventManager())[SDLChessGameEventManager::TYPE] = SDLChessGameEventManager::ENEMY_MOVE;
      (*getChessGameEventManager())[SDLChessGameEventManager::PLAYER_TURN] = Integer(event->user.data1);
      (*getChessGameEventManager())[SDLChessGameEventManager::PIECE_MOVE_INFO] = Integer(event->user.data2);


#ifdef _DEBUG
      auto& chessEventMgr = (*getChessGameEventManager());
      constexpr Integer NUM_BITS = 3;//3 is 3 bits need to encode each of 4 paramaters
      constexpr Integer BIT_MASK = 0b111;      
      auto data = (*getChessGameEventManager())[SDLChessGameEventManager::PIECE_MOVE_INFO];
      Vector<Integer> coords(4);
      std::cout << "\n event wnd  = " << event->user.windowID; //data
      std::cout << "\n event type = " << (*getChessGameEventManager())[SDLChessGameEventManager::TYPE]; //data
      std::cout << "\n data       = " << data; //data

      std::cout << "\n   startRow = " << (((BIT_MASK << 3 * NUM_BITS) & data) >> 3 * NUM_BITS); //index of `startRow`
      std::cout << "\n   startCol = " << (((BIT_MASK << 2 * NUM_BITS) & data) >> 2 * NUM_BITS); //index of `startCol`
      std::cout << "\n   endRow   = " << (((BIT_MASK << NUM_BITS) & data) >> NUM_BITS);         //index of `endRow`
      std::cout << "\n   endCol   = " << (BIT_MASK & data);                                     //index of `endCol`

#endif

    }
    else if (event->user.type == getChessGameEventManager()->getTimeoutEventIndex()) {
    }
    else if (event->user.type == getChessGameEventManager()->getEnemyResignEventIndex()) {
    }
    else if (event->user.type == getChessGameEventManager()->getEnemyDisconnectEventIndex()) {
    }


  }
}