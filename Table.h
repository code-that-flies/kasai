//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_TABLE_H
#define KASAI_TABLE_H
#include <vector>
#include <string>

using std::vector;
using std::string;

#include "Utility.h"


class Table {
public:
    class Data_Structure {
    public:
        Prototype* parent;
        void AttemptToTriggerEvent(string name, string val);
        void AttemptToTriggerEvent(string name, int val);
        void AttemptToTriggerEvent(string name, float val);
        void AttemptToTriggerEvent(string name, bool val);
    };

    class Column : public vector<Prototype*>, public Data_Structure {
    public:
        Column();
        ~Column();


        void append(string val);
        void append(int val);
        void append(float val);
        void append(bool val);
        void append(MemberFunction mb_fn);
        //   void push_back(int_Prototype* val);
        // void push_back(float_Prototype* val);

    };
    class Row : public map<string, Prototype*>, public Data_Structure {
    public:

        void append(string name, string val);
        void append(string name, int val);
        void append(string name, float val);
        void append(string name, bool val);
        void append(string name, MemberFunction mb_fn);


        void commit(string name, Prototype* prototype);
        void commit(string name, string val, ETYPE etype);

    };
    // TODO
    void Add(string name, Prototype defaultValue);

    // TODO
    void Add(string name) {

    }
};


#endif //KASAI_TABLE_H
