//
// Created by Nathaniel Blair on 14/10/20.
//
#include "Utility.h"
#include "Event.h"
#include "File.h"

Event* Event::event_engine = new Event();
File* Event::event_log = new File();


map<string, vector<event_watcher>> Event::event_watchers = map<string, vector<event_watcher>>();

void Event::Begin(string event_name) {
    event_log->WriteLine("Begin:\t\t" + event_name);
}

void Event::End(string event_name) {
    event_log->WriteLine("End:\t\t" + event_name);
}
