//
// Created by Neirokan on 30.04.2020
//

#ifndef INC_3DZAVR_SERVERUDP_H
#define INC_3DZAVR_SERVERUDP_H

#include "World.h"
#include "Camera.h"
#include "ReliableMsg.h"
#include "UDPSocket.h"
#include <memory>
#include <set>

class ServerUDP
{
protected:
    UDPSocket _socket;
    double _lastBroadcast;
    bool _working;

    bool process();
    bool timeout(sf::Uint16 id);

    std::set<sf::Uint16> _clients{};

public:
    explicit ServerUDP();
    [[nodiscard]] bool isWorking() const;
    bool start(sf::Uint16 port);
    void stop();
    void update();

    // virtual functions
    virtual void broadcast(){};

    // here you have to send Init message back to 'targetId' and send NewClient message to all '_clients'
    virtual void processConnect(sf::Uint16 senderId){};
    virtual void processClientUpdate(sf::Uint16 senderId, sf::Packet& packet){};
    virtual void processDisconnect(sf::Uint16 senderId){};

    virtual void processCustomPacket(MsgType type, sf::Packet& packet){};
};


#endif //INC_3DZAVR_SERVERUDP_H