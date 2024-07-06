export module core.ViewFrame;

import foundation.basetypes;
import foundation.Object;
import foundation.SharedPointer;

import core.Updatable;
import core.EventHandleable;
import core.Displayable;

import core.Window;
import core.AvatarManager;
import core.SceneStack;
import core.EventHandler;

namespace mbg {
  ///[abstract] A view frame represents an object that graphically displays app content on screen
  export class ViewFrame : public Object, public Updatable, public Displayable, public EventHandleable {
  public://types
    enum TimeUnit {
      SECOND = 1,
      MILLISECOND = 1000,
      MICROSECOND = 1000000,
      NANOSECOND = 1000000000
    };
    using Pointer = SharedPointer<ViewFrame>;
    
  public://member functions
    virtual ~ViewFrame() { }

    virtual bool initialize() = 0;   ///< Initializes the view frame of app with conrete window, conrete event handler and new avatar manager, new scene stack...
    
    //"EventHandleable"
    void handleEvent() override { event_handler_->handleEvent(); }; ///< Updates the EventHandlerManager of this.
    
    //"Updatable"
    void update()  override { scene_stack_->update(); }  ///< Updates app logic up to the app model.
    void render()  override { scene_stack_->render(); }  ///< Renders entities on the window.

    //"Displayable"
    void show()    override { getWindow()->show(); }
    void hide()    override { getWindow()->hide(); }
    void clear()   override { window_->clear(); }
    void display() override { window_->display(); }


    virtual SizeType getTime() = 0;         ///< Getting timer value in which unit of time is up to the user (must be synchronized with TIME_UNIT).
    virtual void delay(SizeType time) = 0;  ///< Sleeps an amount of `time` which is measured in TIME_UNIT.
    virtual void run();                     ///< Runs screen for app.

    bool shouldExit() const { return should_exit_; }
    void exit() { should_exit_ = true; }

    Window::Pointer getWindow() const { return window_; }
    AvatarManager::Pointer getAvatarManager() const { return avatar_mgr_; }
    SceneStack::Pointer getSceneStack() const { return scene_stack_; }
    EventHandler::Pointer getEventHandler() const { return event_handler_; }

    Integer getDelayTime() const { return DELAY_TIME; }

  protected://member functions
    void setWindow(Window::Pointer window) { window_ = window; }
    void setAvatarManager(AvatarManager::Pointer avatarMgr) { avatar_mgr_ = avatarMgr; }
    void setSceneStack(SceneStack::Pointer scenceStack) { scene_stack_ = scenceStack; }
    void setEventHandler(EventHandler::Pointer eventHandler) { event_handler_ = eventHandler; }

  protected://contructors
    ViewFrame(
      Window::Pointer window, 
      Integer fps, 
      TimeUnit timeUnit, 
      AvatarManager::Pointer avatarMgr,
      SceneStack::Pointer    sceneStack,
      EventHandler::Pointer  eventHandler
    ) : window_(window), FPS(fps), TIME_UNIT(timeUnit), avatar_mgr_(avatarMgr),
      scene_stack_(sceneStack), event_handler_(eventHandler)
    { }

  private://member data
    Window::Pointer        window_;
    AvatarManager::Pointer avatar_mgr_;
    SceneStack::Pointer    scene_stack_;
    EventHandler::Pointer  event_handler_;

    const TimeUnit TIME_UNIT;
    const Integer FPS;         ///< Number of frames per second
    const Integer DELAY_TIME = TIME_UNIT / FPS; ///< Equals to `TIME UNIT` divided by the `FPS`
    Boolean should_exit_ = false; ///< Be used for infinite loop of app
  };
}