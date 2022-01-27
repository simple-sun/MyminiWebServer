#ifndef TIME_POINT_H
#define TIME_POINT_H

#include <string>
#include <chrono>


namespace Time
{
    const int usTos=1000'000;   //微妙到秒的进制转换

    using namespace std::chrono;

    class timePoint
    {
    public:

        using clockType=system_clock;  // 先用着 system_clock

        timePoint(time_point<system_clock> time)
            : time_(time)
        {
            
        }

        std::string toString();
    
        std::string toLogString();

        std::string toLogFileName();

    private:
        time_point<system_clock> time_;
    };
 
    inline time_point<system_clock> getNowTime()
    {
        return system_clock::now(); 
    }

} //namespace point

#endif