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
    unsigned int layer;

    Pattern();

    void NewLayer();

    vector<pair<vector<string>, vector<T>>> RenderWithTags(vector<string> tags, const TLine & line, unsigned int lineIndex);
    vector<pair<vector<string>, vector<T>>> RenderWithTags(const TLine& line, unsigned int lineIndex);

    vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> RenderWithTags(const TDocument& document);
    vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> RenderWithTags(vector<string> tags, const TDocument& document);

    vector<T> Render(const TLine& line, unsigned int lineIndex);
    vector<vector<T>> Render(const TDocument& document);

    vector<string> RenderString(const vector<string>& document);
    string RenderString(const string line, unsigned int lineIndex);

    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    // but filtered to be only those with tags that intersect with the parameter 'tags'
    vector<vector<T>> Reverse(const TDocument & document, vector<string> tags, vector<T> toReplaceWith);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<vector<T>> Reverse(const TDocument & document, vector<T> toReplaceWith);

    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    // but filtered to be only those with tags that intersect with the parameter 'tags'
    vector<vector<T>> ReverseString(const vector<string> & document, vector<string> tags, string toReplaceWith);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<vector<T>> ReverseString(const vector<string> & document, string toReplaceWith);

    void AddQuery(TQuery& query);

};

template<typename T>
Pattern<T>::Pattern() : layer(0) {
    this->Add(Process);
}

template<typename T>
void Pattern<T>::NewLayer() {
    for (TLineSubvectors _subvectors : subvectors) {
        for (TSubvector subvector : _subvectors) {
            subvector.NewLayer();
        }
    }

    this->layer++;
}

template<typename T>
vector<pair<vector<string>, vector<T>>>
Pattern<T>::RenderWithTags(vector<string> tags, const Pattern::TLine &line, unsigned int lineIndex) {
    auto result = vector<pair<vector<string>, vector<T>>>();

    for (TSubvector subvector: subvectors[lineIndex]) {
        subvector.RenderWithTags(&result, line, tags);
    }

    return result;
}

template<typename T>
vector<pair<vector<string>, vector<T>>> Pattern<T>::RenderWithTags(const Pattern::TLine &line, unsigned int lineIndex) {
    auto result = vector<pair<vector<string>, TLine>>();

    for (TSubvector subvector: subvectors[lineIndex]) {
        subvector.RenderWithTags(&result, line);
    }

    return result;
}

template<typename T>
vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> Pattern<T>::RenderWithTags(const Pattern::TDocument &document) {
    auto result = vector<vector<pair<vector<string>, TLine>>>();

    for (int index = 0; index < document.size(); index++) {
        result.push_back(
                std::move(
                        RenderWithTags(
                                document[index],
                                index)
                ));
    }
}

template<typename T>
vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> Pattern<T>::RenderWithTags(vector<string> tags, const Pattern::TDocument &document) {
    auto result = vector<vector<pair<vector<string>, TLine>>>();

    for (int index = 0; index < document.size(); index++) {
        result.push_back(
                std::move(
                        RenderWithTags(
                                tags,
                                document[index],
                                index)
                ));
    }

    return result;
}

template<typename T>
vector<T> Pattern<T>::Render(const Pattern::TLine &line, unsigned int lineIndex) {
    auto result = TLine();

    for (TSubvector subvector: subvectors[lineIndex]) {
        subvector->Render(&result, line);
    }

    return result;
}

template<typename T>
string Pattern<T>::RenderString(const string line, unsigned int lineIndex) {
    return Util<T>::ToString(Render(Util<T>::FromString(line), lineIndex));
}

template<typename T>
vector<vector<T>> Pattern<T>::Render(const Pattern::TDocument &document) {
    auto result = TDocument();

    for (int index = 0; index < document.size(); index++) {
        result.push_back(
                std::move(
                        Render(
                                document[index],
                                index)
                ));
    }

    return result;
}

template<typename T>
vector<string> Pattern<T>::RenderString(const vector<string> &document) {
    auto result = vector<string>();

    for (int index = 0; index < document.size(); index++) {
        result.push_back(
                    std::move(
                            RenderString(
                                    document[index],
                                    index)
                ));
    }

    return result;
}

template<typename T>
vector<vector<T>> Pattern<T>::Reverse(const Pattern::TDocument &document, vector<string> tags, vector<T> toReplaceWith) {
    auto result = TDocument();
    auto prevSubstringEnd = 0;

    for (int index = 0; index < document.size(); index++) {
        result.push_back(TLine());

        for (auto subvector : this->subvectors[index]) {
            Util<T>::Merge(result[result.size() - 1], std::move(subvector->Reverse(tags, toReplaceWith, document[index], prevSubstringEnd)));
        }

        if (prevSubstringEnd < document[index].size())
            Util<T>::Merge(result,
                           Util<T>::Slice(
                                   document[index],
                                   prevSubstringEnd,
                                   document[index].size() - prevSubstringEnd
                           ));
    }

    return result;
}

template<typename T>
vector<vector<T>> Pattern<T>::Reverse(const Pattern::TDocument &document, vector<T> toReplaceWith) {
    auto result = TDocument();
    auto prevSubstringEnd = 0;

    for (int index = 0; index < document.size(); index++) {
        result.push_back(TLine());

        for (auto subvector : *this) {
            Util<T>::Merge(result[result.size() - 1], std::move(subvector->Reverse(toReplaceWith, document[index], prevSubstringEnd)));
        }

        if (prevSubstringEnd < document[index].size())
            Util<T>::Merge(result,
                           Util<T>::Slice(
                                   document[index],
                                   prevSubstringEnd,
                                   document[index].size() - prevSubstringEnd
                           ));
    }

    return result;
}

template<typename T>
void Pattern<T>::AddQuery(Pattern::TQuery &query) {
    queries.push_back(query);
}

template<typename T>
vector<vector<T>>
Pattern<T>::ReverseString(const vector<string> &document, vector<string> tags, string toReplaceWith) {
    auto rawResult = TDocument();
    auto prevSubstringEnd = 0;

    for (int index = 0; index < document.size(); index++) {
        rawResult.push_back(TLine());

        for (auto subvector : this->subvectors[index]) {
            Util<T>::Merge(rawResult[rawResult.size() - 1], std::move(
                    subvector->Reverse(tags, toReplaceWith, Util<T>::FromString(document[index]), prevSubstringEnd)));
        }

        if (prevSubstringEnd < document[index].size())
            Util<T>::Merge(rawResult,
                           Util<T>::Slice(
                                   Util<T>::FromString(document[index]),
                                   prevSubstringEnd,
                                   document[index].size() - prevSubstringEnd
                           ));
    }

    auto result = vector<string>();

    for (auto item : rawResult) {
        result.push_back(Util<T>::ToString(item));
    }

    return result;
}

template<typename T>
vector<vector<T>> Pattern<T>::ReverseString(const vector<string> &document, string toReplaceWith) {
    auto rawResult = TDocument();
    auto prevSubstringEnd = 0;

    for (int index = 0; index < document.size(); index++) {
        rawResult.push_back(TLine());

        for (auto subvector : this->subvectors[index]) {
            Util<T>::Merge(rawResult[rawResult.size() - 1], std::move(
                    subvector->Reverse(toReplaceWith, Util<T>::FromString(document[index]), prevSubstringEnd)));
        }

        if (prevSubstringEnd < document[index].size())
            Util<T>::Merge(rawResult,
                           Util<T>::Slice(
                                   Util<T>::FromString(document[index]),
                                   prevSubstringEnd,
                                   document[index].size() - prevSubstringEnd
                           ));
    }

    auto result = vector<string>();

    for (auto item : rawResult) {
        result.push_back(Util<T>::ToString(item));
    }

    return result;}


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

        if (query.tags.empty()) {
            self->NewLayer();
        }
    }

    self->subvectors.push_back(subvectors);
}

#endif //KASAI_PATTERN_H
