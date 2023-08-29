#ifndef CLIENT_HPP
#define CLIENT_HPP

namespace fgear {

class Client {
public:
    virtual ~Client() = default;
    
protected:
    Client() = default;
    Client(Client const&) = default;
    Client& operator=(Client const&) = default;
};

} // namespace fgear

#endif // CLIENT_HPP