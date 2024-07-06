export module game_model.PlayerManager;

import foundation.Object;
import foundation.Vector;
import foundation.String;
import foundation.SharedPointer;
import foundation.PlayerTurn;
import game_model.Player;

namespace mbg {
  export class PlayerManager : public Object {
  public://types
    using Pointer = SharedPointer<PlayerManager>;

  public://member functions
    virtual ~PlayerManager() = default;

    Player::Pointer operator[](Integer i) { return players_[i]; }
    Player::Pointer getThisPlayer() { return this_player_; }

    void add(Player::Pointer tile) { players_.emplace_back(tile); }
    SizeType size() { return players_.size(); }

    virtual void removePlayer(Integer i) {
      if (i >= players_.size() || i < 0)
        return;

      if (i != players_.size() - 1) {
        players_[i] = players_.back();
      }
      players_.pop_back();
    }

  public://static functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new PlayerManager(std::forward<ARGS>(args)...));
    }

  protected:
    PlayerManager() : players_(1), this_player_(players_[0])
    { }

    PlayerManager(String thisPlayerId, PlayerTurn thisPlayerTurn)
      : this_player_(Player::createPointer(thisPlayerId, thisPlayerTurn))
    {
      players_.emplace_back(this_player_);
    }

    PlayerManager(
      String thisPlayerId,
      PlayerTurn thisPlayerTurn,
      EntityView::Pointer thisPlayerView)
      : this_player_(Player::createPointer(thisPlayerId, thisPlayerTurn, thisPlayerView))
    {
      players_.emplace_back(this_player_);
    }


  private:
    Vector<Player::Pointer> players_;   ///< list of players in this game. The 0-th player also is this_player_
    Player::Pointer this_player_;
  };
}