//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_EVENTENGINE_H
#define KASAI_EVENTENGINE_H

#include "Utility.h"
#include <map>
#include <string>
#include <vector>
#include <mutex>

class File;

using std::string;
using std::vector;
using std::map;

class EventEngine {
public:
    struct Event {
    public:
        string name;
        Prototype* info;

        Event(string name, Prototype* info);
    };

    static File* eventLog;
    static vector<Event> events;
    static std::mutex triggerMutex;

    map<string, vector<event_watcher>> event_watchers;

    int latestProcessedEvent;

    EventEngine();

    void Add(string event_name, event_watcher eventWatcher) {
        if (event_watchers.find(event_name) == event_watchers.end()) {
            event_watchers[event_name] = vector<event_watcher>();
        }

        event_watchers[event_name].push_back(eventWatcher);
    }

    void TryTrigger() {
        if (latestProcessedEvent < events.size() - 1) {
            latestProcessedEvent++;
            auto& event = events[latestProcessedEvent];

            Trigger(event.name, event.info);
        }
    }

    static void Trigger(string name, Prototype* value) {
        std::lock_guard<std::mutex> guard(triggerMutex);

        events.push_back(Event(name, value));
    }

    void Trigger(const string event_name, const Prototype* info) {
        if (event_watchers.find(event_name) == event_watchers.end()) {
            return;
        }

        event_engine->Begin(event_name);
        for (auto event_watcher : event_watchers[event_name]) {
            event_watcher(info);
        }
        event_engine->End(event_name);
    }


    void Begin(string event_name);

    void End(string event_name);


};


#endif //KASAI_EVENTENGINE_H
