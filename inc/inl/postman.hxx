#include "../postman.hpp"

namespace fgear {

template<typename V>
Postman<V>::Postman(std::shared_ptr<concurrency::BlockingMap<std::string, V>> const& a_map, std::shared_ptr<Observer> const& a_observer)
: m_map{a_map}
, m_observer{a_observer}
{
}

template<typename V>
void Postman<V>::update(std::string const& a_key, V const& a_value)
{
    m_map[a_key] = a_value;
    m_observer.get().notify(a_key);
}

} //namespace fgear
