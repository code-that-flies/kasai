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

void EventEngine::Add(string event_name, event_watcher eventWatcher) {
    if (eventWatchers.find(event_name) == eventWatchers.end()) {
        eventWatchers[event_name] = vector<event_watcher>();
    }

    eventWatchers[event_name].push_back(eventWatcher);
}

bool EventEngine::TryTrigger() {
    if (latestProcessedEvent < events->size() - 1) {
        latestProcessedEvent++;
        auto& event = (*events)[latestProcessedEvent];

        TryTrigger(event.name, event.info);
        return true;
    }

    return false;
}

void EventEngine::Trigger(string name, Prototype *value) {
    std::lock_guard<std::mutex> guard(triggerMutex);
    events->emplace_back(name, value);
}

void EventEngine::TryTrigger(const string event_name, const Prototype *info) {
    if (eventWatchers.find(event_name) == eventWatchers.end()) {
        return;
    }

    this->Begin(event_name);
    for (auto event_watcher : eventWatchers[event_name]) {
        event_watcher(info);
    }
    this->End(event_name);
}

EventEngine::Event::Event(string name, Prototype *info) : name(name), info(info) {

}
