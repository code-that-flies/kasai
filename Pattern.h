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
    typedef Subvector<T> TSubvector;
    typedef vector<TSubvector> TLineSubvectors;
    typedef vector<TLineSubvectors> TDocumentSubvectors;

    typedef vector<T> TLine;
    typedef vector<TLine> TDocument;

    typedef Query<T> TQuery;

    TDocumentSubvectors subvectors;
    vector<TQuery> queries;

    Pattern() {
        this->Add(&Process);
    }

    void AddQuery(TQuery& query) {
        queries.push_back(query);
    }

    void Process(TLine line) {
        TLineSubvectors subvectors;

        for (TQuery query: queries) {
            int endPosition;

            for (int startPosition = 0; startPosition < line.size(); startPosition++) {
                auto currentQueryCharacter = 0;
                endPosition = query.Match(line, startPosition, currentQueryCharacter, 0, 1);

                if (endPosition != -1) {
                    TSubvector subvector = TSubvector(startPosition, endPosition);
                    for (string tag: query.tags) {
                        subvector.AddTag(tag);
                    }

                    subvectors.push_back(subvector);

                    startPosition = endPosition;
                }
            }
        }

        this->subvectors.push_back(subvectors);
    }

};


#endif //KASAI_PATTERN_H
