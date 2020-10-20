//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_PATTERN_H
#define KASAI_PATTERN_H

#include "Utility.h"
#include "Feed.h"
#include "Query.h"
#include "Subvector.h"

//#include "Subvector.h"

template <typename T>
class Pattern : public Feed<vector<T>> {
public:
    vector<vector<Subvector<T>>>

    Pattern() {
        this->Add(&Process);
    }

    void Process(vector<T> line) {

    }

};


#endif //KASAI_PATTERN_H
