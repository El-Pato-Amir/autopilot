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

template<typename V>
class Postman {
public:
    Postman(std::shared_ptr<concurrency::BlockingMap<std::string, V>> const& a_map, std::shared_ptr<Observer> const& a_observer);

    void update(std::string const& a_key, V const& a_value);    

private:
    std::shared_ptr<concurrency::BlockingMap<std::string, V>> m_map;
    std::shared_ptr<Observer> m_observer;
};

} // namespace fgear

#include "inl/postman.hxx"

#endif // POSTMAN_HPP