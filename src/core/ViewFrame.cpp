module core.ViewFrame;

namespace mbg {
  /***************************************************************************
  ****************************************************************************/
  void ViewFrame::run() {
    initialize();
    //show();

    Integer startTime, remainingTime;
    while (!should_exit_) {
      startTime = static_cast<Integer>(getTime());
      clear();

      handleEvent();
      update();
      render();
      display();

      remainingTime = DELAY_TIME + startTime - static_cast<Integer>(getTime());
      if (remainingTime > 0) {
        delay(remainingTime);
      }
    }
  }
}