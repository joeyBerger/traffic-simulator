#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:
	T receive();
    void send(T &&msg);
    void clearQueue();
private:
    std::deque <T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    //enums
    enum TrafficLightPhase {
        red,
        green
    };

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();// {return _currentPhase;}

private:
    // typical behaviour methods
    void cycleThroughPhases();
	long returnRandomWaitTime();
    void toggleTrafficLightPhase(TrafficLightPhase currentState);

    std::shared_ptr<MessageQueue<TrafficLightPhase>> queue;

    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
};

#endif