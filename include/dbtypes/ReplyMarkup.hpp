#ifndef DB_REPLY_MARKUP_HPP
#define DB_REPLY_MARKUP_HPP

#include <odb/core.hxx>

class ReplyMarkup {
public:
    ReplyMarkup() {}
    ReplyMarkup(int id, long long reply_id) : id(id), reply_id(reply_id) {}

private:
    friend class odb::access;
    int id;
    long long reply_id;
};

#pragma db object(ReplyMarkup)
#pragma db member(ReplyMarkup::id) id

#endif  //! DB_REPLY_MARKUP_HPP