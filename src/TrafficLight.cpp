#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}


template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 10 - 1);
    int thing = distr(eng);
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
  	
  	// launch cycleThroughPhases function in a thread
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

	float waitTime = returnRandomWaitTime();
    float accumulatedTime = 0.0;
  	
  	// while (true) {
    // 	std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //   	--waitTime;
    //   	if (waitTime<= 0) {
    //       	toggleTrafficLightPhase(_currentPhase);
    //     	waitTime = returnRandomWaitTime();
    //       //TODO: still need to send update method to message queue
    //       //TODO: update so this matches vehicle's implementation in drive()
    //     }
    // }

    // init stop watch
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now();
    while (true) {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
         // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        accumulatedTime += timeSinceLastUpdate;
        if (accumulatedTime >= waitTime) {
            toggleTrafficLightPhase(_currentPhase);
            waitTime = returnRandomWaitTime();
            //TODO: still need to send update method to message queue
        }
    }
}

float TrafficLight::returnRandomWaitTime()
{
    std::random_device rd;
    std::mt19937 eng(rd());
  	int range = 1000;
    std::uniform_int_distribution<> distr(0, range - 1);
    return 1000 * (4 + float(distr(eng))/(float(range/2)));
}

void TrafficLight::toggleTrafficLightPhase(TrafficLight::TrafficLightPhase currentPhase)
{
    _currentPhase = currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
}