//
// Created by Neirokan on 30.04.2020
//

#include "ReliableMsg.h"
#include "../utils/Time.h"
#include "../Consts.h"

ReliableMsg::ReliableMsg(sf::Packet &packet, sf::IpAddress address, sf::Uint16 port) : packet(packet), address(address),
                                                                                       port(port),
                                                                                       lastTry(-std::numeric_limits<double>::max()),
                                                                                       firstTry(Time::time()) {}

ReliableMsg::ReliableMsg(const ReliableMsg &msg) : packet(msg.packet), address(msg.address), port(msg.port),
                                                   lastTry(msg.lastTry), firstTry(msg.firstTry) {}

bool ReliableMsg::trySend(sf::UdpSocket &socket) {
    if (Time::time() - firstTry > Consts::NETWORK_TIMEOUT) {
        return false;
    }
    if (Time::time() - lastTry > Consts::NETWORK_RELIABLE_RETRY_TIME) {
        lastTry = Time::time();
        socket.send(packet, address, port);
    }
    return true;
}
