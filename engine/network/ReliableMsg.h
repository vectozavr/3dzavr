//
// Created by Neirokan on 30.04.2020
//

#ifndef ENGINE_RELIABLEMSG_H
#define ENGINE_RELIABLEMSG_H

#include <SFML/Network.hpp>

class ReliableMsg final {
private:
    sf::Packet packet;
    const sf::IpAddress address;
    const sf::Uint16 port;
    const double firstTry;
    double lastTry;

public:
    ReliableMsg(sf::Packet &packet, sf::IpAddress address, sf::Uint16 port);

    ReliableMsg(const ReliableMsg &msg);

    bool trySend(sf::UdpSocket &socket);
};


#endif //INC_3DZAVR_RELIABLEMSG_H
