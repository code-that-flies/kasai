//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_TABLE_H
#define KASAI_TABLE_H
#include <vector>
#include <string>

using std::vector;
using std::string;

class Prototype;



class Table {
public:
    class Column : public vector<Prototype*> {
    public:
        Column();
        ~Column();


        void append(string val);
        void append(int val);
        void append(float val);
        //   void push_back(int_Prototype* val);
        // void push_back(float_Prototype* val);

    };
    // TODO
    void Add(string name, Prototype defaultValue);

    // TODO
    void Add(string name) {

    }
};


#endif //KASAI_TABLE_H
