#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <filesystem>
#include <iostream>

namespace Debug
{
    enum Level
    {
        Error = 1,
        Warning = 2,
        Info = 3,
        Verbose = 4,
        Debug = 5,
        Marker = Debug,

        NoLevel = 6 // Do not filter messages in this case
    };

    extern Level CurrentDebugLevel;
}

class Log
{
public:
    explicit Log(Debug::Level level);
    ~Log();

    static void SetBuffer(std::shared_ptr<std::ostream> stream) {
        mStream = stream;
    }


    template <typename T>
    Log& operator<<(const T& rhs)
    {
        if (mShouldLog)
            (mStream.get() ? *mStream.get() : std::cout) << rhs;

        return *this;
    }

    Log& operator<<(const std::filesystem::path& rhs);

    Log& operator<<(const std::u8string& rhs);

    Log& operator<<(std::u8string_view rhs);

    Log& operator<<(const char8_t* rhs);

private:
    const bool mShouldLog;
    static std::shared_ptr<std::ostream> mStream;
};

#endif
