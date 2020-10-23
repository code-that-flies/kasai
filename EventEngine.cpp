//
// Created by Nathaniel Blair on 14/10/20.
//
#include "Utility.h"
#include "EventEngine.h"
#include "File.h"

vector<EventEngine::Event>* EventEngine::events = new vector<EventEngine::Event>();

std::mutex EventEngine::triggerMutex = std::mutex();

void EventEngine::Begin(string event_name) {
    eventLog->WriteLine("Begin:\t\t" + event_name);
}

void EventEngine::End(string event_name) {
    eventLog->WriteLine("End:\t\t" + event_name);
}

EventEngine::EventEngine() : latestProcessedEvent(-1) {

}

EventEngine::Event::Event(string name, Prototype *info) : name(name), info(info) {

}
