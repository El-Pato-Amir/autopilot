#ifndef OBSERVER_HPP
#define OBSERVER_HPP

namespace fgear {

class Observer {
public:
    virtual ~Observer() = default;

    virtual void notify() = 0;

protected:
    Observer() = default;
    Observer(Observer const&) = default;
    Observer& operator=(Observer const&) = default;
};

} // namespace fgear

#endif // OBSERVER_HPP