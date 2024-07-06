export module core.Client;

import foundation.basetypes;
import foundation.Vector;
import foundation.Object;
import foundation.SharedPointer;
import core.ViewFrame;

namespace mbg {
  export class Client : public Object {
  public://types
    using Pointer = SharedPointer<Client>;

  public://member functions
    Client() = default;
    Client(ViewFrame::Pointer viewframe) : viewframes_(1) {
      viewframes_[0] = viewframe;
    }
    ViewFrame::Pointer& operator[] (SizeType i) { return viewframes_[i]; }
    
    void add(ViewFrame::Pointer viewframe) { viewframes_.emplace_back(viewframe); } ///< Adds new `ViewFrame` to this.
    //void remove(SizeType i);                ///< Removes the `ViewFrame` pointer at specified index from collection.
    void clear() { viewframes_.clear(); }   ///< Clears all viewframes from this.
    SizeType size() { return viewframes_.size(); } ///< Gets the number of the viewframes.

    virtual void run(SizeType i) { viewframes_[i]->run(); } ///< Runs viewframe `i` at this client.
  
  private:
    Vector<ViewFrame::Pointer> viewframes_;
  };
}
