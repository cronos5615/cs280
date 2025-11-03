#include <iostream>
#include "MTime.h"

MTime::MTime(int hrs, int min, int sec){
    hours = hrs;
    minutes = min;
    seconds = sec;
}
void MTime::SetTime(int hrs, int min, int sec){
    hours = hrs;
    minutes = min;
    seconds = sec;
}
void MTime::DisplayTime(){
    std::string temp_time = "";
    if (hours < 10){
        temp_time += '0' + std::to_string(hours) + ':';
    } 
    else{
        temp_time += std::to_string(hours) + ':';
    }
    if (minutes<10){
        temp_time += '0' + std::to_string(minutes) + ':';
    }
    else{
        temp_time += std::to_string(minutes) + ':';
    }
        if (seconds<10){
        temp_time += '0' + std::to_string(seconds);
    }
    else{
        temp_time += std::to_string(seconds);
    }
    std::cout << temp_time;
}
bool MTime::operator==(MTime & OtherTime){
    if(hours == OtherTime.hours){
        if(minutes == OtherTime.minutes){
            if(seconds == OtherTime.seconds)return true;
        }
    }
    return false;
}
bool MTime::operator>(MTime & OtherTime){
    int otherhour = OtherTime.hours;
    int othermin = OtherTime.minutes;
    int othersec = OtherTime.seconds;
    if(hours>otherhour) return true;
    else if (hours==otherhour){
        if(minutes>othermin)return true;
        else if(minutes==othermin){
            if(seconds>othersec)return true;
        }
    }
    return false;
}
ostream & operator<<(ostream & out, const MTime &timeObj){
    int hours = timeObj.hours;
    int minutes = timeObj.minutes;
    int seconds = timeObj.seconds;
    std::string full,end;
    if(hours<12) end = " AM";
    else end = " PM";
    if(hours>12){
        full += std::to_string(hours-12) + ':';
    }
    else if(hours == 0){
        full += "12:";
    }
    else{
        full += std::to_string(hours) + ':';
    }
    if(minutes<10){
        full += '0' + std::to_string(minutes) + ':';
    }
    else{
        full += std::to_string(minutes) + ':';
    }
    if(seconds<10){
        full += '0' + std::to_string(seconds);
    }
    else{
        full += std::to_string(seconds);
    }
    out << full << end;
    return out;
}
