//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_TALLY_H
#define KASAI_TALLY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Tag.h"
#include "TallyTag.h"

using std::string;
using std::vector;
using std::map;

template <class T>
class Tally : public map<Tag, int> {
public:

    Tally() {
    }

    Tally(vector<TallyTag<T>*> tagsToTallyWith, const vector<T>&input) {
        for (auto* item: tagsToTallyWith) {
            (*this)[*item] = item->Tally(input);
        }
    }
};



#endif //KASAI_TALLY_H
