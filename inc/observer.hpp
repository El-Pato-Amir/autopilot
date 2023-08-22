#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string> // std::string

namespace fgear {

class Observer {
public:
    virtual ~Observer() = default;

    virtual void notify(std::string const& a_message) = 0;

protected:
    Observer() = default;
    Observer(Observer const&) = default;
    Observer& operator=(Observer const&) = default;
};

} // namespace fgear

#endif // OBSERVER_HPP