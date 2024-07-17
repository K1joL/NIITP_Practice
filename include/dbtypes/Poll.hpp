#ifndef DB_POLL_HPP
#define DB_POLL_HPP

#include <odb/core.hxx>
#include <boost/date_time.hpp>
#include <string>

class Poll {
public:
    Poll() {}
    Poll(int poll_id, int message_id, long long id, const std::string& question, bool closed, bool public_voters,
         bool multiple_choice, bool quiz, int close_period, const boost::gregorian::date& close_date, bool min_,
         int total_voters, const std::string& solution, const std::vector<int>& option, long long voters, bool chosen, bool correct,
         const std::string& text, const std::vector<int>& option_)
        : poll_id(poll_id), message_id(message_id), id(id), question(question), 
        closed(closed), public_voters(public_voters), multiple_choice(multiple_choice), 
        quiz(quiz), close_period(close_period), close_date(close_date), min_(min_), 
        total_voters(total_voters), solution(solution), option(option), voters(voters),
        chosen(chosen), correct(correct), text(text), option_(option_) {}

private:
    friend class odb::access;
    int poll_id;
    int message_id;
    long long id;
    std::string question;
    bool closed;
    bool public_voters;
    bool multiple_choice;
    bool quiz;
    int close_period;
    boost::gregorian::date close_date;
    bool min_;
    int total_voters;
    std::string solution;
    std::vector<int> option;
    long long voters;
    bool chosen;
    bool correct;
    std::string text;
    std::vector<int> option_;
};

#pragma db object(Poll)
#pragma db member(Poll::id) id

#endif  //! DB_POLL_HPP