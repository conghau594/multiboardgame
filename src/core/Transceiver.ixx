export module core.Transceiver;

import foundation.basetypes;
import foundation.Object;
import foundation.List;
import foundation.String;
import foundation.SharedPointer;


namespace mbg {
  export class Transceiver : public Object {
  public://types
    using Pointer = SharedPointer<Transceiver>;

  public://member functions
    
    void pushData(String data) { buffer_.emplace_back(data); }

    String popData() {
      String data = std::move(buffer_.front());
      buffer_.pop_front();
      return data;
    }

    Boolean isBufferEmpty() { return buffer_.empty(); }
    void sendData(Transceiver::Pointer other, String data) {
      other->pushData(data);
    }
    //virtual void sendData(Transceiver::Pointer other, Character* data, Integer size) = 0;
    //virtual Integer getData(Transceiver::Pointer other, Character* buffer) = 0; ///< Returns size of the data.
   

    template<typename T> T parseData(String data) {
      return *reinterpret_cast<const T*>(data.c_str());
    }

  public:
    template<typename... Args> static inline Pointer createPointer(Args... args) {
      return Pointer(new Transceiver(std::forward<Args>(args)...));
    }

  protected://contructors
    Transceiver() = default;

  private://member data
    List<String> buffer_;
  };
}