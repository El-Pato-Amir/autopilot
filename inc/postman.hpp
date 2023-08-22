#ifndef POSTMAN_HPP
#define POSTMAN_HPP
//for server

#include "blocking_map.hpp"
#include "observer.hpp"
//TODO: manager class

#include <string> // std::string
#include <memory> // shared_ptr
#include <mutex> // mutex

//TODO: enum class
//TODO: read map from file

namespace fgear {

template<typename K, typename V>
class Postman {
public:
    Postman(concurrency::BlockingMap<K, V> const& a_map, std::shared_ptr<Observer<K>> const& a_observer);

    void update(K const& a_key, V const& a_value);    

private:
    concurrency::BlockingMap<K, V> m_map;
    std::shared_ptr<Observer<K>> m_observer;
};

} // namespace fgear

#include "inl/postman.hxx"

#endif // POSTMAN_HPP