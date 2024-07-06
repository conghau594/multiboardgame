module;
import <SDL2/SDL_events.h>;

export module chess.SDLChessGameEventManager;

import foundation.basetypes;
import foundation.SharedPointer;
import foundation.Vector;
import core.EventManager;


namespace mbg {
  export class SDLChessGameEventManager : public EventManager {
  public://types
    using Pointer = SharedPointer<SDLChessGameEventManager>;

    enum TypeIndex {
      ENEMY_MOVE = 0,
      WIN,
      TIE,
      LOSE,
      TIMEOUT,
      ENEMY_RESIGN,
      ENEMY_DISCONNECT
    };

    enum InfoIndex {
      TYPE = 0,   ///< value of ...[TYPE] = variable of enum TypeIndex
      PLAYER_TURN,
      TERMINATE, ///< info about game terminating due to timeout or enemy resign event: 
                 ///< 01 = timeout, 10 = enemy has resigned, 100 = enemy is disconnected
      PIECE_MOVE_INFO ///< info about chess piece move of enemy:
                 ///< xxx = start row, yyy = start col, zzz = end row, ttt = end col
    };

  public://member functions
    const UnsignedInt getFirstUserEventIndex() const { return FIRST_USER_EVENT_INDEX; }
    const UnsignedInt getEnemyMoveEventIndex() const { return FIRST_USER_EVENT_INDEX + ENEMY_MOVE; }
    const UnsignedInt getTimeoutEventIndex() const { return FIRST_USER_EVENT_INDEX + TIMEOUT; }
    const UnsignedInt getEnemyResignEventIndex() const { return FIRST_USER_EVENT_INDEX + ENEMY_RESIGN; }
    const UnsignedInt getEnemyDisconnectEventIndex() const { return FIRST_USER_EVENT_INDEX + ENEMY_DISCONNECT; }

  public://static functions
    template<typename... Args>
    static inline Pointer createPointer(Args&&... args) {
      return Pointer(new SDLChessGameEventManager(std::forward<Args>(args)...));
    }

  protected://contructors
    SDLChessGameEventManager(SizeType size = 8)
      : EventManager(size),
        FIRST_USER_EVENT_INDEX(SDL_RegisterEvents(NUMBER_OF_EVENTS))
    { 
      if (FIRST_USER_EVENT_INDEX == EVENT_INDEX_BOUNDARY) {
        //TODO!048 Log registering event failed
        return;
      }

    }

  private://static members
    static constexpr UnsignedInt EVENT_INDEX_BOUNDARY = UnsignedInt(-1);
    static constexpr Integer NUMBER_OF_EVENTS = ENEMY_DISCONNECT + 1;

  private://data members
    //Vector<UnsignedInt> type_indices_;
    const UnsignedInt FIRST_USER_EVENT_INDEX;
    
  };
}