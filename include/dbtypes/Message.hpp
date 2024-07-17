#ifndef DB_MESSAGE_HPP
#define DB_MESSAGE_HPP

#include <odb/core.hxx>
#include <boost/date_time.hpp>

class Message {
public:
    Message() {}
    Message(long long id, const boost::gregorian::date& date, const std::string& message, bool out, bool mentioned,
            bool media_unread, bool silent, bool post, bool from_scheduled, bool legacy, bool edit_hide, bool pinned,
            bool noforwards, const boost::gregorian::date& edit_date, int entities_id, long long views, long long forwards,
            bool nopremium, bool spoiler, int photo_id, int document_id, int poll_id, bool has_stickers, int ttl_seconds,
            int replies_id, long long post_author, long long grouped_id, long long peer_id_id, long long reply_to_id, long long from_id,
            long long fwd_from_id, long long via_bot_id, long long reply_markup_id)
        : id(id), date(date), message(message), out(out), mentioned(mentioned),
         media_unread(media_unread), silent(silent), post(post), from_scheduled(from_scheduled), 
         legacy(legacy), edit_hide(edit_hide), pinned(pinned), noforwards(noforwards), 
         edit_date(edit_date), entities_id(entities_id), views(views), forwards(forwards), 
         nopremium(nopremium), spoiler(spoiler), photo_id(photo_id), document_id(document_id), 
         poll_id(poll_id), has_stickers(has_stickers), ttl_seconds(ttl_seconds), replies_id(replies_id), 
         post_author(post_author), grouped_id(grouped_id), peer_id_id(peer_id_id), reply_to_id(reply_to_id),
         from_id(from_id), fwd_from_id(fwd_from_id), via_bot_id(via_bot_id), reply_markup_id(reply_markup_id) {}

private:
    friend class odb::access;
    long long id;
    boost::gregorian::date date;
    std::string message;
    bool out;
    bool mentioned;
    bool media_unread;
    bool silent;
    bool post;
    bool from_scheduled;
    bool legacy;
    bool edit_hide;
    bool pinned;
    bool noforwards;
    boost::gregorian::date edit_date;
    int entities_id;
    long long views;
    long long forwards;
    bool nopremium;
    bool spoiler;
    int photo_id;
    int document_id;
    int poll_id;
    bool has_stickers;
    int ttl_seconds;
    int replies_id;
    long long post_author;
    long long grouped_id;
    long long peer_id_id;
    long long reply_to_id;
    long long from_id;
    long long fwd_from_id;
    long long via_bot_id;
    long long reply_markup_id;
};

#pragma db object(Message)
#pragma db member(Message::id) id

#endif  //! DB_MESSAGE_HPP