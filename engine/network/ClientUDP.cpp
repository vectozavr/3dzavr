//
// Created by Neirokan on 30.04.2020
//

#include "ClientUDP.h"
#include "MsgType.h"
#include "../utils/Time.h"
#include "../utils/Log.h"
#include "../Consts.h"

ClientUDP::ClientUDP() {
    _socket.setTimeoutCallback([this](sf::Uint16 id) { return ClientUDP::timeout(id); });
}

bool ClientUDP::connected() const {
    return _socket.ownId();
}

bool ClientUDP::isWorking() const {
    return _working;
}

void ClientUDP::connect(sf::IpAddress ip, sf::Uint16 port) {
    _ip = ip;
    _port = port;
    sf::Packet packet;
    packet << MsgType::Connect << Consts::NETWORK_VERSION;
    _working = _socket.bind(0);
    _socket.addConnection(_socket.serverId(), ip, port);
    _socket.sendRely(packet, _socket.serverId());

    Log::log("ClientUDP::connect(): connecting to the server...");
}

void ClientUDP::update() {
    if (!isWorking()) {
        return;
    }

    while (isWorking() && process()) {}

    // Send new client information to server
    if (Time::time() - _lastBroadcast > 1.0 / Consts::NETWORK_WORLD_UPDATE_RATE && connected()) {
        updatePacket();
        _lastBroadcast = Time::time();
    }

    // Socket update
    _socket.update();
}

void ClientUDP::disconnect() {
    sf::Packet packet;
    packet << MsgType::Disconnect << _socket.ownId();
    _socket.send(packet, _socket.serverId());
    _socket.unbind();
    _working = false;

    Log::log("ClientUDP::disconnect(): disconnected from the server.");
    processDisconnected();
}

bool ClientUDP::timeout(sf::Uint16 id) {
    Log::log("ClientUDP::timeout(): timeout from the server.");

    if (id != _socket.serverId()) {
        return true;
    }
    disconnect();
    return false;
}

// Recive and process message.
// Returns true, if some message was received.
bool ClientUDP::process() {
    sf::Packet packet;
    sf::Uint16 senderId;
    sf::Uint16 targetId;

    MsgType type = _socket.receive(packet, senderId);

    if (type == MsgType::Empty) {
        return false;
    }
    if (!connected() && type != MsgType::Init) {
        return true;
    }

    switch (type) {
        // here we process any operations based on msg type
        case MsgType::Init:
            packet >> targetId;
            _socket.setId(targetId);

            Log::log("ClientUDP::process(): client Id = " + std::to_string(targetId) + " connected.");

            processInit(packet);
            break;
        case MsgType::ServerUpdate:

            processUpdate(packet);
            break;
        case MsgType::NewClient:

            Log::log("ClientUDP::process(): new client init...");

            processNewClient(packet);
            break;
        case MsgType::Disconnect:
            packet >> targetId;
            if (targetId == _socket.ownId()) {
                disconnect();
            }

            Log::log("ClientUDP::process(): client Id = " + std::to_string(targetId) + " disconnected from the server");

            processDisconnect(targetId);
            break;
        case MsgType::Custom:
            processCustomPacket(packet);
            break;
        case MsgType::Error:
            Log::log("ClientUDP::process(): Error message");
            break;
        default:
            Log::log("ClientUDP::process(): unknown message type " + std::to_string(static_cast<int>(type)));
    }

    return true;
}
