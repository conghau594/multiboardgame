export module game_controller.Game2DController;

import foundation.basetypes;
import foundation.SharedPointer;
import foundation.Vector;

import core.SceneController;
import core.SceneStack;
import core.EntityView;

import game_model.Game2DModel;
import game_view.Game2DView;
import game_controller.BoardGameState;

namespace mbg {
  export class Game2DController : public SceneController {
  public://types
    using Pointer = SharedPointer<Game2DController>;
    
  public://member funtions
    virtual ~Game2DController() = default;

    //Vector<EntityView::Pointer>& memorized_entities() { return memorized_entities_; }

    Game2DModel::Pointer getGameModel() const { return std::static_pointer_cast<Game2DModel>(getModel()); }    
    Game2DView::Pointer  getGameView() const { return std::static_pointer_cast<Game2DView>(getView()); }
    BoardGameState::Pointer getCurrentState() const { return state_stack_.back(); } ///< Gets the top state.
    BoardGameState::Pointer getGameState(Integer i) const { return state_stack_[i]; } ///< Gets the top state.
    
    SizeType getNumberOfStates() { return state_stack_.size(); } ///< Gets size of the stack.

    void setGameModel(Game2DModel::Pointer gameModel) { setModel(gameModel); }
    void setGameView(Game2DView::Pointer gameView) { setView(gameView); }
    
    Boolean hasState() { return state_stack_.empty(); }   ///< Checks if the stack is empty.    
    void clearStates() { state_stack_.clear(); }  ///< Clears object if needed.

    void pushState(BoardGameState::Pointer nextState) { ///< Adds an object for managing.
      if (nextState->onEntering()) {
        state_stack_.emplace_back(nextState);
      }
    }

    void popState() {///< Removes the top object.
      if (getCurrentState()->onExiting())
        state_stack_.pop_back();
    }

    void popSeveralLastStates(Integer number) {
      for (Integer i = 0; i < number; ++i) {
        popState();
      }
    }


    void changeState(BoardGameState::Pointer nextState) {
      if (nextState->getID() == getCurrentState()->getID())
        return;

      popState();
      pushState(nextState);
    }

  //protected://constructors
    Game2DController(
      Game2DModel::Pointer model,
      Game2DView::Pointer view,
      EventHandler::Pointer eventHandler,
      SceneStack::Pointer sceneStack,
      BoardGameState::Pointer firstGameState
    ) : SceneController(model, view, eventHandler, sceneStack)
    {
      if(firstGameState != nullptr)
        state_stack_.emplace_back(firstGameState);
    }

  private://member functions
    using SceneController::getModel;
    using SceneController::setModel;

    using SceneController::getView;
    using SceneController::setView;

  private:
    //Vector<EntityView::Pointer> memorized_entities_;
    Vector<BoardGameState::Pointer> state_stack_;
  };
}