//
// Created by Neirokan on 30.04.2020
//

#include <fstream>
#include <sstream>
#include "ServerUDP.h"
#include "utils/Time.h"
#include "MsgType.h"
#include "config.h"
#include "../utils/Log.h"

ServerUDP::ServerUDP() : _lastBroadcast(-INFINITY), _working(false)
{
    _socket.setTimeoutCallback(std::bind(&ServerUDP::timeout, this, std::placeholders::_1));
}

bool ServerUDP::isWorking() const
{
    return _working;
}

bool ServerUDP::start(sf::Uint16 port)
{
    _working = _socket.bind(port);

    if(_working)
        Log::log("ServerUDP: server successfully started.");
    else
        Log::log("ServerUDP: failed to start the server.");

    return _working;
}

void ServerUDP::update()
{
    if (!isWorking())
        return;

    while (process());

    // World state broadcast
    if (Time::time() - _lastBroadcast > 1.0 / WORLD_UPDATE_RATE) {
        broadcast();
        _lastBroadcast = Time::time();
    }

    // Socket update
    _socket.update();
}

void ServerUDP::stop()
{
    for (auto it = _clients.begin(); it != _clients.end();)
    {
        sf::Packet packet;
        packet << MsgType::Disconnect << *it;
        _socket.send(packet, *it);
        _clients.erase(it++);
    }

    _socket.unbind();
    _working = false;

    Log::log("ServerUDP: the server was killed.");
}

bool ServerUDP::timeout(sf::Uint16 playerId)
{
    sf::Packet packet;
    packet << MsgType::Disconnect << playerId;

    _clients.erase(playerId);

    for (auto client : _clients)
        _socket.sendRely(packet, client);

    Log::log("ServerUDP: client Id = " + std::to_string(playerId) + " disconnected due to timeout.");

    return true;
}

// Recive and process message.
// Returns true, if some message was received.
bool ServerUDP::process()
{
    sf::Packet packet;
    sf::Packet sendPacket;
    sf::Uint16 senderId;
    MsgType type;

    if ((type = _socket.receive(packet, senderId)) == MsgType::Empty)
        return false;

    switch (type)
    {
        // here we process any operations based on msg type
        case MsgType::Connect:

            Log::log("ServerUDP: client Id = " + std::to_string(senderId) + " connecting...");

            processConnect(senderId);
            break;
        case MsgType::ClientUpdate:

            processClientUpdate(senderId, packet);
            break;
        case MsgType::Disconnect:
            Log::log("ServerUDP: client Id = " + std::to_string(senderId) + " disconnected.");

            sendPacket << MsgType::Disconnect << senderId;
            _clients.erase(senderId);
            _socket.removeConnection(senderId);
            for (auto client : _clients)
                _socket.sendRely(sendPacket, client);

            processDisconnect(senderId);
            break;
        default:
            processCustomPacket(type, packet);
    }

    return true;
}
