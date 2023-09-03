#include "variables.hpp"

#include <exception> // exceptions
#include <stdexcept> // std exceptions
#include <cmath> // abs


namespace fgear {

namespace {

bool compare_floats(float a_first, float a_second)
{
    constexpr float EPSILON = 0.00000001f;
    return std::abs(a_first - a_second) > EPSILON;
}

} // namespace

void Variables::insert(std::string const& a_key, float a_value)
{
    std::unique_lock lock{m_tx};
    if (m_map.contains(a_key)) {
        throw std::invalid_argument("key already exists");
    }
    m_map[a_key].store(a_value);
    ++m_items;
}

void Variables::set(std::string const& a_key, float a_value)
{
    m_map.at(a_key).store(a_value);
}

void Variables::exchange_if(std::string const& a_key, float a_value)
{
    std::unique_lock lock{m_tx};
    if(compare_floats(m_map.at(a_key).load(), a_value)) {
        m_map.at(a_key).store(a_value);
    }
}

float Variables::get(std::string const& a_key) const
{
    return m_map.at(a_key).load();
}

size_t Variables::size() const noexcept
{
    return m_items;
}


} // namespace fgear