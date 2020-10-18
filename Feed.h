//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_FEED_H
#define KASAI_FEED_H

#include <vector>

using std::vector;

template <class T>
class Feed : public vector<T> {
public:
    typedef void (*Processor)(T);
    vector<Processor> feed_scanning_processors;

    Feed();

    void Add(Processor processor);

    void Add(T line);

    void OnNewInfo(T line) {
        Add(line);
    }
};

template<class T>
void Feed<T>::Add(Feed::Processor processor) {
    feed_scanning_processors.push_back(processor);
}

template<class T>
void Feed<T>::Add(T firstline) {
    this->push_back(firstline);
    for (int i = 0; i < this->size(); i++) {
        feed_scanning_processors[i]((*this)[this->size() - 1]);
    }
}

template<class T>
Feed<T>::Feed() {

}


#endif //KASAI_FEED_H
