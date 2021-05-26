//
// Created by Neirokan on 30.04.2020
//

#ifndef INC_3DZAVR_UDPCONNECTION_H
#define INC_3DZAVR_UDPCONNECTION_H

#include <SFML/Network.hpp>

class UDPConnection
{
private:
    sf::Uint16 _id;
    sf::IpAddress _ip;
    sf::Uint16 _port;
    double lastMsg;

public:
    explicit UDPConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port);
    [[nodiscard]] sf::Uint16 id() const;
    [[nodiscard]] const sf::IpAddress& ip() const;
    [[nodiscard]] sf::Uint16 port() const;
    [[nodiscard]] bool timeout() const;
    bool same(sf::IpAddress& ip, sf::Uint16 port) const;
    void update();
    void send(sf::UdpSocket& socket, sf::Packet& packet);
};


#endif //INC_3DZAVR_UDPCONNECTION_H
