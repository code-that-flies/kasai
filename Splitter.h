//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_SPLITTER_H
#define KASAI_SPLITTER_H

#include "Feed.h"

template <class T>
class Splitter : public T {
public:
    typedef T (*_splitter)(T);
    _splitter splitter;

    void Split(Feed<T>& add_to) {
        add_to.Add(splitter(*this));
    }
};


#endif //KASAI_SPLITTER_H
