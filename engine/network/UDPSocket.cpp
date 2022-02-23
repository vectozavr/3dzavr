//
// Created by Neirokan on 30.04.2020
//

#include <algorithm>

#include "UDPSocket.h"
#include "../utils/Time.h"
#include "../Consts.h"

UDPSocket::UDPSocket() : _ownId(0), _nextRelyMsgId(0) {
    _socket.setBlocking(false);
}

void UDPSocket::addConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port) {
    _connections.insert({id, UDPConnection(id, ip, port)});
}

void UDPSocket::removeConnection(sf::Uint16 id) {
    _connections.erase(id);
}

bool UDPSocket::bind(sf::Uint16 port) {
    return _socket.bind(port) == sf::Socket::Status::Done;
}

void UDPSocket::unbind() {
    sf::Packet packet;
    packet << MsgType::Disconnect << _ownId;

    for (auto it = _connections.begin(); it != _connections.end();) {
        send(packet, it->first);
        _connections.erase(it++);
    }

    _relyPackets.clear();
    _confirmTimes.clear();
    _socket.unbind();
    setId(0);
}

void UDPSocket::setTimeoutCallback(std::function<bool(sf::Uint16)> callback) {
    _timeoutCallback = std::move(callback);
}

void UDPSocket::setId(sf::Uint16 id) {
    _ownId = id;
}

sf::Uint16 UDPSocket::ownId() const {
    return _ownId;
}

sf::Uint16 UDPSocket::serverId() const {
    return _serverId;
}

void UDPSocket::sendRely(const sf::Packet &packet, const sf::IpAddress &ip, sf::Uint16 port) {
    sf::Packet finalPacket;
    finalPacket << _ownId << true << _nextRelyMsgId;
    finalPacket.append(packet.getData(), packet.getDataSize());
    _relyPackets.insert({_nextRelyMsgId++, ReliableMsg(finalPacket, ip, port)});
}

void UDPSocket::sendRely(const sf::Packet &packet, sf::Uint16 id) {
    if (!_connections.count(id)) {
        return;
    }
    this->sendRely(packet, _connections.at(id).ip(), _connections.at(id).port());
}

void UDPSocket::send(const sf::Packet &packet, const sf::IpAddress &ip, sf::Uint16 port) {
    sf::Packet finalPacket;
    finalPacket << _ownId << false << _serverId;
    finalPacket.append(packet.getData(), packet.getDataSize());
    _socket.send(finalPacket, ip, port);
}

void UDPSocket::send(const sf::Packet &packet, sf::Uint16 id) {
    if (!_connections.count(id)) {
        return;
    }
    this->send(packet, _connections.at(id).ip(), _connections.at(id).port());
}

void UDPSocket::update() {
    for (auto it = _connections.begin(); it != _connections.end();) {
        if (!it->second.timeout()) {
            ++it;
        } else {
            if (_timeoutCallback && !_timeoutCallback(it->first)) {
                return;
            }
            _connections.erase(it++);
        }
    }

    for (auto it = _relyPackets.begin(); it != _relyPackets.end();) {
        if (!it->second.trySend(_socket)) {
            _relyPackets.erase(it++);
        } else {
            ++it;
        }
    }

    for (auto it = _confirmTimes.begin(); it != _confirmTimes.end();) {
        if (Time::time() - it->second > Consts::NETWORK_TIMEOUT) {
            _confirmTimes.erase(it++);
        } else {
            ++it;
        }
    }
}

MsgType UDPSocket::receive(sf::Packet &packet, sf::Uint16 &senderId) {
    // Receive message
    sf::IpAddress ip;
    sf::Uint16 port;

    packet.clear();
    if (_socket.receive(packet, ip, port) != sf::Socket::Status::Done) {
        return MsgType::Empty;
    }

    // Read header
    bool reply = false;
    sf::Uint16 msgId = 0;
    MsgType type;
    senderId = 0;
    if (!(packet >> senderId >> reply >> msgId >> type)) {
        return MsgType::Error;
    }

    if (_connections.count(senderId)) {
        _connections.at(senderId).update();
    }

    if (type == MsgType::Confirm) {
        _relyPackets.erase(msgId);
        // you don't need this information on the highest levels
        return MsgType::Empty;
    }

    if (type == MsgType::Connect) {
        sf::Uint32 version = 0;
        if (!(packet >> version) || version != Consts::NETWORK_VERSION) {
            return MsgType::Error;
        }
        sf::Uint16 tmp;
        for (tmp = Consts::NETWORK_MAX_CLIENTS; tmp >= 1; tmp--) {
            if (!_connections.count(tmp)) {
                senderId = tmp;
            } else {
                if (_connections.at(tmp).same(ip, port)) {
                    return MsgType::Error;
                }
            }
        }
        _connections.insert({senderId, UDPConnection(senderId, ip, port)});
    }

    if (!_connections.count(senderId) || !_connections.at(senderId).same(ip, port) ||
        reply && confirmed(msgId, senderId)) {
        return MsgType::Error;
    }
    return type;
}

bool UDPSocket::confirmed(sf::Uint16 msgId, sf::Uint16 senderId) {
    sf::Packet confirmPacket;
    confirmPacket << _ownId << false << msgId << MsgType::Confirm;
    _connections.at(senderId).send(_socket, confirmPacket);

    sf::Uint32 confirmId;
    confirmId = (senderId << 16) | msgId;

    bool repeat = _confirmTimes.count(confirmId);
    _confirmTimes[confirmId] = Time::time();

    return repeat;
}

UDPSocket::~UDPSocket() {
    unbind();
}
