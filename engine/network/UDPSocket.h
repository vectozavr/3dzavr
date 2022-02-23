//
// Created by Neirokan on 30.04.2020
//

#ifndef ENGINE_UDPSOCKET_H
#define ENGINE_UDPSOCKET_H

#include <memory>
#include <map>
#include <functional>

#include "ReliableMsg.h"
#include "UDPConnection.h"
#include "MsgType.h"

class UDPSocket final {
private:
    sf::UdpSocket _socket;
    sf::Uint16 _nextRelyMsgId;
    sf::Uint16 _ownId;
    const sf::Uint16 _serverId = 0;

    std::map<sf::Uint16, UDPConnection> _connections;
    std::map<sf::Uint16, ReliableMsg> _relyPackets;
    std::map<sf::Uint32, double> _confirmTimes;
    std::function<bool(sf::Uint16)> _timeoutCallback;

    bool confirmed(sf::Uint16 msgId, sf::Uint16 senderId);

public:
    explicit UDPSocket();

    bool bind(sf::Uint16 port);

    void unbind();

    void setTimeoutCallback(std::function<bool(sf::Uint16)> callback);

    void addConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port);

    void removeConnection(sf::Uint16 id);

    void setId(sf::Uint16 id);

    [[nodiscard]] sf::Uint16 ownId() const;

    [[nodiscard]] sf::Uint16 serverId() const;

    void send(const sf::Packet &packet, const sf::IpAddress &ip, sf::Uint16 port);

    void send(const sf::Packet &packet, sf::Uint16 id);

    void sendRely(const sf::Packet &packet, const sf::IpAddress &ip, sf::Uint16 port);

    void sendRely(const sf::Packet &packet, sf::Uint16 id);

    void update();

    MsgType receive(sf::Packet &packet, sf::Uint16 &senderId);

    ~UDPSocket();
};


#endif //INC_3DZAVR_UDPSOCKET_H
