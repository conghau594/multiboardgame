export module game_model.Rule;

import foundation.Object;
import foundation.SharedPointer;

namespace mbg {
  export class Game2DModel;

  export class Rule : public Object {
  public: 
    using Pointer = SharedPointer<Rule>;
    virtual ~Rule() = 0;
    SharedPointer<Game2DModel> getGameModel() const { return game_model_.lock(); }

  protected:
    Rule(SharedPointer<Game2DModel> gameModel) 
    : game_model_(gameModel)
    { }

  private:
    WeakPointer<Game2DModel> game_model_;
  };
  Rule::~Rule() {}
}