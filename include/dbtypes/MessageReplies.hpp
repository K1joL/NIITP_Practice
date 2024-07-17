#ifndef DB_MESSGAGE_REPLIES_HPP
#define DB_MESSGAGE_REPLIES_HPP

#include <odb/core.hxx>

struct MessageReplies {
public:
    MessageReplies() {}
    MessageReplies(long long id, int replies, int replies_pts, bool comments_, long long channel_id, long long max_id, long long read_max_id)
        : id(id), replies(replies), replies_pts(replies_pts), comments_(comments_), channel_id(channel_id), max_id(max_id), read_max_id(read_max_id) {}

private:
    friend class odb::access;
    long long id;
    int replies;
    int replies_pts;
    bool comments_;
    long long channel_id;
    long long max_id;
    long long read_max_id;
};

#pragma db object(MessageReplies)
#pragma db member(MessageReplies::id) id

#endif  //! DB_MESSGAGE_REPLIES_HPP