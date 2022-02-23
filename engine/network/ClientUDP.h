//
// Created by Neirokan on 30.04.2020
//

#ifndef ENGINE_CLIENTUDP_H
#define ENGINE_CLIENTUDP_H

#include "ReliableMsg.h"
#include "UDPSocket.h"
#include <memory>

class ClientUDP {
protected:
    UDPSocket _socket;
    double _lastBroadcast = -std::numeric_limits<double>::max();
    bool _working = false;
    sf::Uint16 _port{};
    sf::IpAddress _ip{};

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

    [[nodiscard]] sf::IpAddress serverIp() const { return _ip; }

    [[nodiscard]] sf::Uint16 serverPort() const { return _port; }

    // virtual functions
    virtual void updatePacket() {};

    virtual void processInit(sf::Packet &packet) {};

    virtual void processUpdate(sf::Packet &packet) {};

    virtual void processNewClient(sf::Packet &packet) {};

    virtual void processDisconnect(sf::Uint16 targetId) {};

    virtual void processCustomPacket(sf::Packet &packet) {};

    virtual void processDisconnected() {};

    virtual ~ClientUDP() = default;
};


#endif //INC_3DZAVR_CLIENTUDP_H