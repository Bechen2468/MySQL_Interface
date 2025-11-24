#ifndef MY_DATETIME_H
#define MY_DATETIME_H

#include <ctime>
#include <chrono>
#include <stdexcept>


namespace sqlxeigen::datatype {
struct Datetime {
    long long raw;

    Datetime(): raw(std::time(nullptr)) {}

    explicit Datetime(long long raw): raw(raw) {}


    bool operator==(const Datetime& dt) const { return raw == dt.raw; }
    bool operator==(Datetime& dt) const { return raw == dt.raw; }
    bool operator==(const Datetime& dt) { return raw == dt.raw; }
    bool operator==(Datetime& dt) { return raw == dt.raw; }
    
    bool operator==(const long long& dt) const { return raw == dt; }
    bool operator==(long long& dt) const { return raw == dt; }
    bool operator==(const long long& dt) { return raw == dt; }
    bool operator==(long long& dt) { return raw == dt; }


    int year() const       { return _get_tm().tm_year + 1900; }
    int month() const      { return _get_tm().tm_mon + 1; }
    int day() const        { return _get_tm().tm_mday; }
    int hour() const       { return _get_tm().tm_hour; }
    int minute() const     { return _get_tm().tm_min; }
    int second() const     { return _get_tm().tm_sec; }

    void year(int y)   { _edit_tm([&](std::tm& t){ t.tm_year = y - 1900; }); }
    void month(int m)  { _edit_tm([&](std::tm& t){ t.tm_mon = m - 1; }); }
    void day(int d)    { _edit_tm([&](std::tm& t){ t.tm_mday = d; }); }
    void hour(int h)   { _edit_tm([&](std::tm& t){ t.tm_hour = h; }); }
    void minute(int m) { _edit_tm([&](std::tm& t){ t.tm_min = m; }); }
    void second(int s) { _edit_tm([&](std::tm& t){ t.tm_sec = s; }); }

    

    std::string to_string() const {
        char buf[32];
        std::tm t = _get_tm();
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
        return buf;
    }

private:
    // Copy of std::tm in UTC
    std::tm _get_tm() const {
        std::tm out{};
    #if defined(_WIN32)
        gmtime_s(&out, &raw);
    #else
        gmtime_r(&timestamp, &out);
    #endif
        return out;
    }


    // Modify tm and update timestamp
    template<typename F>
    void _edit_tm(F&& func) {        
        std::tm t = _get_tm();
        func(t);
    #if defined(_WIN32)
        raw = _mkgmtime(&t); // Windows
    #else
        raw = timegm(&t);    // POSIX
    #endif
    }
};


};
#endif