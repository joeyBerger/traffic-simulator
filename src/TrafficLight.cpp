#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
  	std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });
  
  	T msg = std::move(_queue.back());
  	_queue.pop_back();
  
  	return msg;  
}


template <typename T>
void MessageQueue<T>::send(T &&msg)
{ 
    // perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutex);
  
  	// std::cout << "   Traffic Light Message " << msg << " has been sent to the queue" << std::endl;
  	_queue.push_back(std::move(msg));
  	_cond.notify_one(); 	
}

template <typename T>
void MessageQueue<T>::clearQueue()
{
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.clear();
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red; 
}

void TrafficLight::waitForGreen()
{
    queue->clearQueue();
    while(true) {
      TrafficLightPhase message = queue->receive();
      if (message == TrafficLightPhase::green) return;
    }
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{ 	
  	// launch cycleThroughPhases function in a thread
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
	long waitTime = returnRandomWaitTime();
  	queue = std::make_shared<MessageQueue<TrafficLightPhase>>();

    // init stop watch
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now();
    while (true) {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
         // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= waitTime) {
            toggleTrafficLightPhase(_currentPhase);
            queue->send(std::move(_currentPhase));

            // reset wait time / last update
            waitTime = returnRandomWaitTime();
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}

// returns a random time between 4000 and 6000 milliseconds
long TrafficLight::returnRandomWaitTime()
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 1000 - 1);
    return 1000 * (4 + float(distr(eng))/(500.0));
}

// toggles between green and red
void TrafficLight::toggleTrafficLightPhase(TrafficLight::TrafficLightPhase currentPhase)
{
    _currentPhase = currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
}