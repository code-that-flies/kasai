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

    File* eventLog;
    static vector<Event>* events;
    static std::mutex triggerMutex;

    map<string, vector<event_watcher>> eventWatchers;

    int latestProcessedEvent;

    EventEngine();

    void Add(string event_name, event_watcher eventWatcher);

    bool TryTrigger();

    static void Trigger(string name, Prototype* value);

    void TryTrigger(const string event_name, const Prototype* info);

    void Begin(string event_name);

    void End(string event_name);


};


#endif //KASAI_EVENTENGINE_H
