export module core.EventHandleable;

namespace mbg {
  export class EventHandleable {
  public:
    virtual void handleEvent() = 0;
  };
}