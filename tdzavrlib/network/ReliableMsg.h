//
// Created by Neirokan on 30.04.2020
//

#ifndef INC_3DZAVR_RELIABLEMSG_H
#define INC_3DZAVR_RELIABLEMSG_H

#include <SFML/Network.hpp>

class ReliableMsg
{
private:
    sf::Packet packet;
    sf::IpAddress address;
    sf::Uint16 port;
    double lastTry;
    double firstTry;

public:
    ReliableMsg(sf::Packet& packet, sf::IpAddress address, sf::Uint16 port);
    ReliableMsg(const ReliableMsg& msg);

    bool trySend(sf::UdpSocket& socket);
};


#endif //INC_3DZAVR_RELIABLEMSG_H
