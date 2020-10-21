//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_FEED_H
#define KASAI_FEED_H

#include <vector>

using std::vector;

template <class TLine>
class Feed : public vector<TLine> {
public:
    typedef void (*Processor)(TLine);
    vector<Processor> feed_scanning_processors;

    Feed();

    void Add(Processor processor);

    void Add(TLine line);

    void OnNewInfo(TLine line) {
        Add(line);
    }
};

template<class TLine>
void Feed<TLine>::Add(Feed::Processor processor) {
    feed_scanning_processors.push_back(processor);
}

template<class TLine>
void Feed<TLine>::Add(TLine firstline) {
    this->push_back(firstline);
    for (int i = 0; i < this->size(); i++) {
        feed_scanning_processors[i]((*this)[this->size() - 1]);
    }
}

template<class TLine>
Feed<TLine>::Feed() {

}


#endif //KASAI_FEED_H
