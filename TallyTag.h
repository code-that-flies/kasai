//
// Created by Nathaniel Blair on 27/10/20.
//

#ifndef KASAI_TALLYTAG_H
#define KASAI_TALLYTAG_H


#include "Tag.h"

template <class T>
class TallyTag : public Tag {
public:
    virtual unsigned int Tally(const vector<T>& raw) {
        return 0; // Do nothing by default
    }

};


#endif //KASAI_TALLYTAG_H
