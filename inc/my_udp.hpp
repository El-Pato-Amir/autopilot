#ifndef MY_UDP_HPP
#define MY_UDP_HPP

#include <cstdint>
#include <string>

namespace fgear {

class MyUdp {
public:
    MyUdp(std::string const& a_address, uint32_t const& a_port, uint32_t const& a_time_out);
    ~MyUdp();

    ssize_t send(std::string const& a_msg);
    std::string read();

    //in milliseconds
    void set_timeout(uint32_t const& a_time); 

private:    
    int32_t m_socket;
    uint32_t m_timeout;
};

} // namespace fgear

#endif // MY_UDP_HPP