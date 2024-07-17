#ifndef DB_PEERCHANNEL_HPP
#define DB_PEERCHANNEL_HPP

#include <odb/core.hxx>

struct PeerChannel {
public:
    PeerChannel() {}
    PeerChannel(long long id, long long channel_id) : id(id), channel_id(channel_id) {}

private:
    friend class odb::access;
    long long id;
    long long channel_id;
};

#pragma db object(PeerChannel)
#pragma db member(PeerChannel::id) id

#endif  //! DB_PEERCHANNEL_HPP
