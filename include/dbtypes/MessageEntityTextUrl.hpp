#ifndef DB_MESSAGE_ENTITY_TEXT_URL
#define DB_MESSAGE_ENTITY_TEXT_URL

#include <odb/core.hxx>
#include <string>

struct MessageEntityTextUrl {
public:
    MessageEntityTextUrl() {}
    MessageEntityTextUrl(int id, int offset_, int length_, const std::string& url) : id(id), offset_(offset_), length_(length_), url(url) {}

private:
    friend class odb::access;
    int id;
    int offset_;
    int length_;
    std::string url;
};

#pragma db object(MessageEntityTextUrl)
#pragma db member(MessageEntityTextUrl::id) id

#endif  //! DB_MESSAGE_ENTITY_TEXT_URL