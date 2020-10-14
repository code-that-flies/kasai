//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_EVENT_H
#define KASAI_EVENT_H

#include "Utility.h"
#include <map>
#include <string>
#include <vector>

class File;

using std::string;
using std::vector;
using std::map;

class Event {
public:

    static File* event_log;
    static Event* event_engine;

    static map<string, vector<event_watcher>> event_watchers;

    static void Add(string event_name, event_watcher eventWatcher) {
        if (event_watchers.find(event_name) == event_watchers.end()) {
            event_watchers[event_name] = vector<event_watcher>();
        }

        event_watchers[event_name].push_back(eventWatcher);
    }

    static void Trigger(string event_name, const Prototype* info) {
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


#endif //KASAI_EVENT_H
