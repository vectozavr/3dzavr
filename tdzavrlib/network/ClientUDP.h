//
// Created by Neirokan on 30.04.2020
//

#ifndef INC_3DZAVR_CLIENTUDP_H
#define INC_3DZAVR_CLIENTUDP_H

#include "ReliableMsg.h"
#include "UDPSocket.h"
#include "config.h"
#include <memory>

class ClientUDP
{
protected:
    UDPSocket _socket;
    double _lastBroadcast;
    bool _working;

    bool process();
    bool timeout(sf::Uint16 id);

public:
    // create new ClientUDP()
    explicit ClientUDP();

    [[nodiscard]] bool isWorking() const;
    [[nodiscard]] bool connected() const;
    void connect(sf::IpAddress ip, sf::Uint16 port);
    void disconnect();
    void update();

    // virtual functions
    virtual void updatePacket(){};

    virtual void processInit(sf::Packet& packet){};
    virtual void processUpdate(sf::Packet& packet){};
    virtual void processNewClient(sf::Packet& packet){};
    virtual void processDisconnect(sf::Uint16 targetId){};

    virtual void processDisconnected(){};

    virtual void processCustomPacket(MsgType type, sf::Packet& packet){};
};


#endif //INC_3DZAVR_CLIENTUDP_H