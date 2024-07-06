module;
import <SDL2/SDL.h>;

#ifdef _DEBUG
#endif
#define TEST

export module app.SDLAppViewFrame;

import foundation.basetypes;
import foundation.String;
import foundation.SharedPointer;
import foundation.PlayerTurn;

import core.ViewFrame;
import sdl_display;

import chess.SDLChessGameController;
import chess.WaitingBoardFocused;
import chess.ItemPickable;
import chess.WaitingTurnTaken;
import chess.SDLChessGameService;

import app.SDLAppEventHandler;
import chess.SDLChessGameTransceiver;


namespace mbg {
  export class SDLAppViewFrame final : public ViewFrame, public EnableSharedFromThis<SDLAppViewFrame> {
  public://types
    enum Initialization;
    using Pointer = SharedPointer<SDLAppViewFrame>;
  
  public://member functions
    inline bool initialize() override; ///< Initializes the view frame of app with conrete window, conrete event handler and new avatar manager, new scene stack...
    //void handleEvent() override { ;      ///< Updates the EventHandlerManager of this.

    SizeType getTime() override { return SDL_GetTicks(); } ///< Getting timer value in which unit of time is up to the user (must be synchronized with TIME_UNIT).
    void delay(SizeType millisec) override { SDL_Delay(static_cast<Integer>(millisec)); } ///< Sleeps an amount of `time` which is measured in TIME_UNIT.

  public://static template functions
    template<typename... ARGS> static inline Pointer createPointer(ARGS&&... args) {
      return Pointer(new SDLAppViewFrame(std::forward<ARGS>(args)...));
    }

  protected://contructors
    SDLAppViewFrame(SDLWindow::Pointer window, Integer fps)
      : ViewFrame(window, fps, TimeUnit::MILLISECOND, nullptr, nullptr, nullptr)
    { }
    SDLAppViewFrame(Integer fps)
      : ViewFrame(nullptr, fps, TimeUnit::MILLISECOND, nullptr, nullptr, nullptr)
    { }

#ifdef TEST
    virtual void run() override {
      initialize();
      //show();

      Integer startTime, remainingTime;
      while (!shouldExit()) {
        startTime = static_cast<Integer>(getTime());
        SDLEvent event;
        SDL_WaitEvent(&event);
        //std::cout << "\nevent on window = " << event.window.windowID;
        //std::cout << "\nthis window ID     = " << window_1->getWindowID();

        static_cast<SDLEventHandler*>(ViewFrame::getEventHandler().get())->update(&event);

        if (event.window.windowID == getWindow1()->getWindowID()) {
          ViewFrame::clear();
          ViewFrame::update();
          ViewFrame::render();
          ViewFrame::display();
        }
        else /*if (event.window.windowID == getWindow2()->getWindowID()) */ {
          window_2->clear();
          scene_stack_2->update();
          scene_stack_2->render();
          window_2->display();
        }

        remainingTime = getDelayTime() + startTime - static_cast<Integer>(getTime());
        if (remainingTime > 0) {
          delay(remainingTime);
        }
      }
    }

    SDLWindow::Pointer getWindow1() const { return std::static_pointer_cast<SDLWindow>(getWindow()); }
    SDLWindow::Pointer getWindow2() const { return std::static_pointer_cast<SDLWindow>(window_2);; }


    void setWindow2(Window::Pointer window) { window_2 = window; }
    void setAvatarManager2(AvatarManager::Pointer avatarMgr) { avatar_mgr_2 = avatarMgr; }
    void setSceneStack2(SceneStack::Pointer scenceStack) { scene_stack_2 = scenceStack; }

  private:
    Window::Pointer        window_2;
    AvatarManager::Pointer avatar_mgr_2;
    SceneStack::Pointer    scene_stack_2;
#endif

  };

  /***************************************************************************/
  enum SDLAppViewFrame::Initialization {
    INIT_TIMER = SDL_INIT_TIMER,                   ///< timer subsystem
    INIT_AUDIO = SDL_INIT_AUDIO,                   ///< audio subsystem
    INIT_VIDEO = SDL_INIT_VIDEO,                   ///< video subsystem; automatically initializes the events subsystem
    INIT_JOYSTICK = SDL_INIT_JOYSTICK,             ///< joystick subsystem; automatically initializes the events subsystem
    INIT_HAPTIC = SDL_INIT_HAPTIC,                 ///< haptic (force feedback) subsystem
    INIT_GAMECONTROLLER = SDL_INIT_GAMECONTROLLER, ///< controller subsystem; automatically initializes the joystick subsystem
    INIT_EVENTS = SDL_INIT_EVENTS,                 ///< events subsystem
    INIT_EVERYTHING = SDL_INIT_EVERYTHING,         ///<  all of the above subsystems
    //INIT_NOPARACHUTE = SDL_INIT_NOPARACHUTE      ///< compatibility; this flag is ignored
  };

  /***************************************************************************/
  inline bool SDLAppViewFrame::initialize() {
    if (SDL_Init(INIT_EVERYTHING) < 0)//TODO.008 Add log: init failed (SDL init everything failed)
      return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    constexpr Integer WINDOW_WIDTH = 820;
    constexpr Integer WINDOW_HEIGHT = 820;
    SDLWindow::Pointer window = SDLWindow::createPointer("Chess Game", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (not window->initialize()) //TODO.013 add log: init failed (init SDL window failed)
      return false;
    setWindow(window);

    AvatarManager::Pointer avatarMgr = AvatarManager::createPointer();
    setAvatarManager(avatarMgr);
    
    SDLAppEventHandler::Pointer eventHandler = SDLAppEventHandler::createPointer();
    setEventHandler(eventHandler);
    eventHandler->setViewFrame(shared_from_this());

    SceneStack::Pointer sceneStack = SceneStack::createPointer();
    setSceneStack(sceneStack);

    SDLChessGameTransceiver::Pointer transceiver = SDLChessGameTransceiver::createPointer(window);

    SDLChessGameController::Pointer chessGameCtrler = SDLChessGameController::createPointer(
      window,        // SDLWindow::Pointer 
      eventHandler,  // SDLAppEventHandler::Pointer
      avatarMgr,     // AvatarManager::Pointer
      transceiver    // enemy: SDLChessGameTransceiver::Pointer
    );
    chessGameCtrler->initialize(PlayerTurn::FIRST);

    SDLChessGameService::Pointer gameService = SDLChessGameService::createPointer(chessGameCtrler);
    WaitingBoardFocused::Pointer nextState = WaitingBoardFocused::createPointer(gameService);
    nextState->setNextState(ItemPickable::createPointer(gameService));

    WaitingTurnTaken::Pointer firstState = WaitingTurnTaken::createPointer(gameService, nextState);

    chessGameCtrler->pushState(firstState);
    chessGameCtrler->pushState(nextState);

    sceneStack->push(chessGameCtrler);
    window->show();
    


#ifdef TEST 
    
    SDLWindow::Pointer window2 = SDLWindow::createPointer("Chess Game 2", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (not window2->initialize()) //TODO.013 add log: init failed (init SDL window failed)
      return false;
    setWindow2(window2);

    AvatarManager::Pointer avatarMgr2 = AvatarManager::createPointer();
    setAvatarManager2(avatarMgr2);

    SceneStack::Pointer sceneStack2 = SceneStack::createPointer();
    setSceneStack2(sceneStack2);

    SDLChessGameTransceiver::Pointer transceiver2 = SDLChessGameTransceiver::createPointer(window2, transceiver);
    transceiver->setEnemyTransceiver(transceiver2);

#ifdef _DEBUG
    std::cout << "\n\n...bool SDLAppViewFrame::initialize()...";
    std::cout << "\n  windowID of transceiver 1 = " << transceiver->getWindow()->getWindowID();
    std::cout << "\n  windowID of transceiver 2 = " << transceiver2->getWindow()->getWindowID();
#endif // _DEBUG


    SDLChessGameController::Pointer chessGameCtrler2 = SDLChessGameController::createPointer(
      window2,        // SDLWindow::Pointer 
      eventHandler,  // SDLEventHandler::Pointer
      avatarMgr2,     // AvatarManager::Pointer
      transceiver2    //SDLChessGameTransceiver::Pointer
    );
    chessGameCtrler2->initialize(PlayerTurn::SECOND);

    SDLChessGameService::Pointer gameService2 = SDLChessGameService::createPointer(chessGameCtrler2);    

    WaitingBoardFocused::Pointer nextState2 = WaitingBoardFocused::createPointer(gameService2);
    nextState2->setNextState(ItemPickable::createPointer(gameService2));

    WaitingTurnTaken::Pointer firstState2 = WaitingTurnTaken::createPointer(gameService2, nextState2);
    chessGameCtrler2->pushState(firstState2);

    sceneStack2->push(chessGameCtrler2);
    window2->show();
    
#endif

    return true;
  }
}