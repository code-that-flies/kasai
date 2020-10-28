//
// Created by Nathaniel Blair on 27/10/20.
//

#ifndef KASAI_TALLYTAG_H
#define KASAI_TALLYTAG_H


#include "Tag.h"

template <class T>
class TallyTag : public Tag {
public:
    TallyTag(string tag) : Tag(tag) {

    }

    virtual int Tally(const vector<T>& raw) {
        return -1; // Return a 'fail' (aka -1) by default
    }

};


#endif //KASAI_TALLYTAG_H
