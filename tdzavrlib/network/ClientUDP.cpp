//
// Created by Neirokan on 30.04.2020
//

#include "ClientUDP.h"
#include "MsgType.h"
#include <thread>
#include "../utils/Time.h"
#include <cmath>
#include "../utils/Log.h"

ClientUDP::ClientUDP() : _lastBroadcast(-INFINITY), _working(false)
{
    _socket.setTimeoutCallback(std::bind(&ClientUDP::timeout, this, std::placeholders::_1));
}

bool ClientUDP::connected() const
{
    return _socket.ownId();
}

bool ClientUDP::isWorking() const
{
    return _working;
}

void ClientUDP::connect(sf::IpAddress ip, sf::Uint16 port)
{
    sf::Packet packet;
    packet << MsgType::Connect << NETWORK_VERSION;
    _working = _socket.bind(0);
    _socket.addConnection(_socket.serverId(), ip, port);
    _socket.sendRely(packet, _socket.serverId());

    Log::log("ClientUDP: connecting to the server...");
}

void ClientUDP::update()
{
    if (!isWorking())
        return;

    while (isWorking() && process());

    // Send new client information to server
    if (Time::time() - _lastBroadcast > 1.0 / WORLD_UPDATE_RATE && connected()) {
        updatePacket();
        _lastBroadcast = Time::time();
    }

    // Socket update
    _socket.update();
}

void ClientUDP::disconnect()
{
    sf::Packet packet;
    packet << MsgType::Disconnect << _socket.ownId();
    _socket.send(packet, _socket.serverId());
    _socket.unbind();
    _working = false;

    Log::log("ClientUDP: disconnected from the server.");
    processDisconnected();
}

bool ClientUDP::timeout(sf::Uint16 id)
{
    Log::log("ClientUDP: timeout from the server.");

    if (id != _socket.serverId())
        return true;
    disconnect();
    return false;
}

// Recive and process message.
// Returns true, if some message was received.
bool ClientUDP::process()
{
    sf::Packet packet;
    sf::Uint16 senderId;
    sf::Uint16 targetId;
    MsgType type;

    if ((type = _socket.receive(packet, senderId)) == MsgType::Empty)
        return false;
    if (!connected() && type != MsgType::Init)
        return true;

    switch (type)
    {
        // here we process any operations based on msg type
        case MsgType::Init:
            packet >> targetId;
            _socket.setId(targetId);

            Log::log("ClientUDP: client Id = " + std::to_string(targetId) + " connected.");

            processInit(packet);
            break;
        case MsgType::Update:

            processUpdate(packet);
            break;
        case MsgType::NewClient:

            Log::log("ClientUDP: new client init...");

            processNewClient(packet);
            break;
        case MsgType::Disconnect:
            packet >> targetId;
            if (targetId == _socket.ownId())
                disconnect();

            Log::log("ClientUDP: client Id = " + std::to_string(targetId) + " disconnected from the server");

            processDisconnect(targetId);
            break;
        default:
            processCustomPacket(type, packet);
    }

    return true;
}
