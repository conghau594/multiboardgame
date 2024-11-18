module;
#ifdef _DEBUG
import <iostream>;
#endif // DEBUG


export module chess.WaitingBoardFocused;

import foundation.String;
import foundation.SharedPointer;

import game_controller.BoardGameState;
import chess.SDLChessGameService;
import chess.SDLChessGameController;

namespace mbg {
  export class WaitingBoardFocused : public BoardGameState {
  public://types
    using Pointer = SharedPointer<WaitingBoardFocused>;

  public://functions
    virtual ~WaitingBoardFocused() = default;

    String getID() override { return typeid(*this).name(); }

    SDLChessGameController::Pointer getChessGameController() const {
      return std::static_pointer_cast<SDLChessGameController>(getGameController());
    }

    void setNextState(BoardGameState::Pointer nextState) { currently_waiting_state_ = nextState; }
    BoardGameState::Pointer getNextState() const { return currently_waiting_state_; }

    void onEnemyResign() { }
    void onTimeout() { }

    //render, update
    void render() override { }
    void update() override {
      if (game_service_->doesSomeoneResign() || game_service_->isTimeout()) {
        if (game_service_->doesSomeoneResign()) {
          onEnemyResign();
        }

        if (game_service_->isTimeout()) {
          onTimeout();
        }
        return;
      }

      if (game_service_->isCursorOnExpectedArea()) {
        getChessGameController()->pushState(currently_waiting_state_);
      }
    }
    
    //onEnter, onExit

    Boolean onEntering() override {     
      return true;
    }

    Boolean onExiting() override {
      return true;
    }

  public://static funtions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new WaitingBoardFocused(std::forward<ARGS>(args)...));
    }

  //protected://contructors
    WaitingBoardFocused(SDLChessGameService::Pointer gameService) 
      : BoardGameState(gameService->getChessGameController()),
        game_service_(gameService)
    { }

  private://member functions
    using BoardGameState::getGameController;

  private://data members
    SDLChessGameService::Pointer game_service_;
    BoardGameState::Pointer currently_waiting_state_;
  };
}

