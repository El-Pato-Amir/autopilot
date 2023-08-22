#include "../postman.hpp"

namespace fgear {

template<typename K, typename V>
Postman<K, V>::Postman(concurrency::BlockingMap<K, V> const& a_map, std::shared_ptr<Observer<K>> const& a_observer)
: m_map{a_map}
, m_observer{a_observer}
{
}

template<typename K, typename V>
void Postman<K, V>::update(K const& a_key, V const& a_value)
{
    m_map[a_key] = a_value;
    m_observer.get().notify(a_key);
}

} //namespace fgear
