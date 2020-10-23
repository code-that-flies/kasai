//
// Created by Nathaniel Blair on 14/10/20.
//
#include "Utility.h"
#include "EventEngine.h"
#include "File.h"

EventEngine* EventEngine::event_engine = new EventEngine();
File* EventEngine::eventLog = new File();


map<string, vector<event_watcher>> EventEngine::event_watchers = map<string, vector<event_watcher>>();

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
