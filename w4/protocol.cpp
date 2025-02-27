#include "protocol.h"
#include <cstring> // memcpy


void send_join(ENetPeer* peer)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t), ENET_PACKET_FLAG_RELIABLE);
    *packet->data = E_CLIENT_TO_SERVER_JOIN;

    enet_peer_send(peer, 0, packet);
}

void send_new_entity(ENetPeer* peer, const Entity& ent)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(Entity),
        ENET_PACKET_FLAG_RELIABLE);
    CustomBitstream bs(packet->data);
    bs.write(E_SERVER_TO_CLIENT_NEW_ENTITY);
    bs.write(ent);

    enet_peer_send(peer, 0, packet);
}

void send_set_controlled_entity(ENetPeer* peer, uint16_t eid)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(uint16_t),
        ENET_PACKET_FLAG_RELIABLE);

    CustomBitstream bs(packet->data);
    bs.write(E_SERVER_TO_CLIENT_SET_CONTROLLED_ENTITY);
    bs.write(eid);

    enet_peer_send(peer, 0, packet);
}

void send_entity_state(ENetPeer* peer, uint16_t eid, float x, float y)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(uint16_t) +
        2 * sizeof(float),
        ENET_PACKET_FLAG_UNSEQUENCED);

    CustomBitstream bs(packet->data);
    bs.write(E_CLIENT_TO_SERVER_STATE);
    bs.write(eid);
    bs.write(x);
    bs.write(y);

    enet_peer_send(peer, 1, packet);
}

void send_snapshot(ENetPeer* peer, uint16_t eid, float x, float y, float bodySize)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(uint16_t) +
        3 * sizeof(float),
        ENET_PACKET_FLAG_UNSEQUENCED);
    CustomBitstream bs(packet->data);
    bs.write(E_SERVER_TO_CLIENT_SNAPSHOT);
    bs.write(eid);
    bs.write(x);
    bs.write(y);
    bs.write(bodySize);

    enet_peer_send(peer, 1, packet);
}

void send_eaten_entity(ENetPeer* peer, uint16_t eid, float x, float y, float bodySize)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(uint16_t) +
        3 * sizeof(float),
        ENET_PACKET_FLAG_UNSEQUENCED);
    CustomBitstream bs(packet->data);
    bs.write(E_SERVER_TO_CLIENT_CONTROLLED_ENTITY_EATEN);
    bs.write(eid);
    bs.write(x);
    bs.write(y);
    bs.write(bodySize);

    enet_peer_send(peer, 1, packet);
}

void send_entity_eat(ENetPeer* peer, uint16_t eid, float bodySize)
{
    ENetPacket* packet = enet_packet_create(nullptr, sizeof(uint8_t) + sizeof(uint16_t) +
        sizeof(float),
        ENET_PACKET_FLAG_UNSEQUENCED);
    CustomBitstream bs(packet->data);
    bs.write(E_SERVER_TO_CLIENT_CONTROLLED_ENTITY_EAT);
    bs.write(eid);
    bs.write(bodySize);

    enet_peer_send(peer, 1, packet);
}

MessageType get_packet_type(ENetPacket* packet)
{
    return (MessageType)*packet->data;
}

void deserialize_new_entity(ENetPacket* packet, Entity& ent)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(ent);
}

void deserialize_set_controlled_entity(ENetPacket* packet, uint16_t& eid)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(eid);
}

void deserialize_entity_state(ENetPacket* packet, uint16_t& eid, float& x, float& y)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(eid);
    bs.read(x);
    bs.read(y);
}

void deserialize_snapshot(ENetPacket* packet, uint16_t& eid, float& x, float& y, float& bodySize)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(eid);
    bs.read(x);
    bs.read(y);
    bs.read(bodySize);
}

void deserialize_eaten_entity(ENetPacket* packet, uint16_t& eid, float& x, float& y, float& bodySize)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(eid);
    bs.read(x);
    bs.read(y);
    bs.read(bodySize);
}

void deserialize_entity_eat(ENetPacket* packet, uint16_t& eid, float& bodySize)
{
    MessageType skip;
    CustomBitstream bs(packet->data);
    bs.read(skip);
    bs.read(eid);
    bs.read(bodySize);
}

