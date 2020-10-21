//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_PATTERN_H
#define KASAI_PATTERN_H

#include "Utility.h"
#include "Feed.h"
#include "Query.h"
#include "Subvector.h"



template <class T>
void Process(Feed<vector<T>>* _self, vector<T> line);

template <typename T>
class Pattern : public Feed<vector<T>> {
public:
    typedef Subvector<T> TSubvector;
    typedef vector<TSubvector> TLineSubvectors;
    typedef vector<TLineSubvectors> TDocumentSubvectors;

    typedef vector<T> TLine;
    typedef vector<TLine> TDocument;

    typedef Query<T> TQuery;

    TDocumentSubvectors subvectors;
    vector<TQuery> queries;

    Pattern();

    void AddQuery(TQuery& query) {
        queries.push_back(query);
    }

};

template<typename T>
Pattern<T>::Pattern() {
    this->Add(Process);
}


template<class T>
void Process(Feed<vector<T>> *_self, vector<T> line) {
    auto self = (Pattern<T>*)_self;


    vector<Subvector<T>> subvectors;

    for (Query<T> query: self->queries) {
        auto endPosition = 0;

        for (int startPosition = 0; startPosition < line.size(); startPosition++) {
            auto currentQueryCharacter = 0;
            auto currentQuery = 0;
            auto direction = 1;
            endPosition = query.Match(line, startPosition, currentQueryCharacter, currentQuery, direction);

            if (endPosition != -1) {
                Subvector<T> subvector = Subvector<T>(startPosition, endPosition);
                for (string tag: query.tags) {
                    subvector.AddTag(tag);
                }

                subvectors.push_back(subvector);

                startPosition = endPosition;
            }
        }
    }

    self->subvectors.push_back(subvectors);
}

#endif //KASAI_PATTERN_H
