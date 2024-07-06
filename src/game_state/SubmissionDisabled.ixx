export module game_controller.SubmissionDisabled;

import foundation.basetypes;
import foundation.SharedPointer;
//import foundation.TypeInfo;
import foundation.String;

import game_controller.BoardGameState;
import game_controller.GameService;

namespace mbg {
  export class SubmissionDisabled : public BoardGameState {
  public://type
    using Pointer = SharedPointer<SubmissionDisabled>;

  public://member funtions
    virtual ~SubmissionDisabled() = default;

    Boolean onEntering() { return true; }
    Boolean onExiting() { return true; }

    String getID() override { return typeid(*this).name(); }
    GameService::Pointer getGameService() const { return board_service_; }

    inline void update() override {
      if (getGameService()->isTurnTaken()) {
        onTakingTurn();
      }
    }

    void render() override { }
    
    virtual void onTakingTurn() = 0;

  protected:
    SubmissionDisabled(
      GameService::Pointer boardService
    ) : BoardGameState(boardService->getGameController()),
        board_service_(boardService)
    { }

  private:
    GameService::Pointer board_service_;
  };  
}