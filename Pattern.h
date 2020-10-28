//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_PATTERN_H
#define KASAI_PATTERN_H

#include <experimental/iterator>
#include <list>

#include "Utility.h"
#include "Feed.h"
#include "Query.h"
#include "Subvector.h"


using std::list;

template <class T>
void Process(Feed<vector<T>>* _self, vector<T> line);

template <typename T>
class Pattern : public Feed<vector<T>>, IReversible {
public:

    template <class TNodeValue>
    struct Node : public TNodeValue {
    public:
        vector<string> tags;
        map<string, Node<TNodeValue>*> * map_subnodes;
        vector<pair<string, Node<TNodeValue>*>> * vec_subnodes;
        int depth;

        Node() : depth(0) {

        }
        Node(vector<string> tags, TNodeValue value) : depth(0), TNodeValue(value) {

        }

        ~Node() {
            delete map_subnodes;
            delete vec_subnodes;
        }

        string RenderTags() {
            string result = "";

            for (auto tag: tags) {
                result += " " + tag;
            }

            return result;
        }

        void InitializeSubnodes() {
            map_subnodes = new map<string, Node<TNodeValue>*>();
            vec_subnodes = new vector<pair<string, Node<TNodeValue>*>>();
        }

        virtual void AddSubnode(Node<TNodeValue>* value) {
            if (vec_subnodes == nullptr)
                InitializeSubnodes();

            for (auto it = vec_subnodes->rbegin(); it != vec_subnodes->end(); it++) {
                auto fullname = (*it)->first;
                auto* subnode = (*it)->second;

                // TODO: optimize this block of code to pass around the
                //  intersection for the difference calculation
                if (Util<T>::Intersects(value->tags, subnode->tags)) {
                    value->tags = Util<T>::Difference(subnode->tags, value->tags);

                    value->depth++; // Increase node depth according to depth in the tree
                    subnode->AddSubnode(value);
                    return;
                }
            }

            auto key = value->RenderTags();
            map_subnodes[key] = value;
            vec_subnodes->emplace_back(key, value);
        }
    };

    template <class TNodeValue>
    struct AbstractTree : public Node<TNodeValue> {
    public:
        vector<Node<TNodeValue>*> nodes;

        ~AbstractTree() {
            for (auto* node: nodes) {
                delete node;
            }
        }

        void AddSubnode(Node<TNodeValue> value) {
            auto* toStore = new Node<TNodeValue>(value);

            nodes.push_back(toStore);

            Node<TNodeValue>::AddSubnode(toStore);
        }

        Node<TNodeValue>* GetFirstDeepestNodeWithTag(string tag) {
            Node<TNodeValue>* candidateForResult = nullptr;

            for (auto* node: nodes) {
                if (Util<T>::Has(tag)) {
                    if (candidateForResult->depth < node->depth) {
                        candidateForResult = node;
                    }
                }
            }

            return candidateForResult;
        }

        Node<TNodeValue>* GetFirstDeepestNodeWithTag(vector<string> tags) {
            Node<TNodeValue>* candidateForResult = nullptr;

            for (auto* node: nodes) {
                if (Util<T>::Contains(node->tags, tags)) {
                    if (candidateForResult->depth < node->depth) {
                        candidateForResult = node;
                    }
                }
            }

            return candidateForResult;
        }

        static AbstractTree<TNodeValue>* FromVector(vector<vector<pair<vector<string>, TNodeValue>>>& input) {
            auto* result = new AbstractTree<TNodeValue>();

            for (auto& lines: input) {
                for (auto& pair: lines) {
                    result->AddSubnode(Node<TNodeValue>(pair.first, pair.second));
                }
            }

            return result;
        }
    };

    typedef AbstractTree<string> AbstractSyntaxTree;

    typedef Subvector<T> TSubvector;
    typedef vector<TSubvector> TLineSubvectors;
    typedef vector<TLineSubvectors> TDocumentSubvectors;

    typedef vector<T> TLine;
    typedef vector<TLine> TDocument;

    typedef Query<T> TQuery;

    TDocumentSubvectors subvectors;
    list<Tag*> pairStarts;
    vector<TQuery> queries;
    unsigned int layer;

    Pattern();

    void NewLayer();

    vector<pair<vector<string>, vector<T>>> RenderWithTags(vector<string> tags, const TLine & line, unsigned int lineIndex);
    vector<pair<vector<string>, string>> RenderAsStringsWithTags(vector<string> tags, const string & line, unsigned int lineIndex);
    vector<pair<vector<string>, vector<T>>> RenderWithTags(const TLine& line, unsigned int lineIndex);
    vector<pair<vector<string>, string>> RenderAsStringsWithTags(const string& line, unsigned int lineIndex);

    vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> RenderWithTags(const TDocument& document);
    vector<vector<pair<vector<string>, typename Pattern<T>::TLine>>> RenderWithTags(vector<string> tags, const TDocument& document);

    vector<vector<pair<vector<string>, string>>> RenderAsStringsWithTags(const vector<string>& document);
    vector<vector<pair<vector<string>, string>>> RenderAsStringsWithTags(vector<string> tags, const vector<string>& document);

    vector<T> Render(const TLine& line, unsigned int lineIndex);
    vector<vector<T>> Render(const TDocument& document);

    vector<string> Render(const vector<string>& document);
    string Render(const string& line, unsigned int lineIndex);

    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    // but filtered to be only those with tags that intersect with the parameter 'tags'
    vector<vector<T>> Reverse(const TDocument & document, vector<string> tags, vector<T> toReplaceWith);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<vector<T>> Reverse(const TDocument & document, vector<T> toReplaceWith);

    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    // but filtered to be only those with tags that intersect with the parameter 'tags'
    vector<vector<T>> Reverse(const vector<string> & document, vector<string> tags, string toReplaceWith);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<vector<T>> Reverse(const vector<string> & document, string toReplaceWith);

    AbstractTree<vector<T>>* MakeTree(const TDocument& document) {
        return AbstractTree<vector<T>>::FromVector(RenderWithTags(document));
    }

    AbstractSyntaxTree * MakeSyntaxTree(const vector<string>& document) {
        return AbstractSyntaxTree::FromVector(RenderAsStringsWithTags(document));
    }

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
vector<pair<vector<string>, string>>
Pattern<T>::RenderAsStringsWithTags(vector<string> tags, const string& line, unsigned int lineIndex) {
    auto result = vector<pair<vector<string>, vector<T>>>();

    for (TSubvector subvector: subvectors[lineIndex]) {
        subvector.RenderAsStringWithTags(&result, line, tags);
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
vector<pair<vector<string>, string>> Pattern<T>::RenderAsStringsWithTags(const string& line, unsigned int lineIndex) {
    auto result = vector<pair<vector<string>, string>>();

    for (TSubvector subvector: subvectors[lineIndex]) {
        subvector.RenderAsStringsWithTags(&result, Util<T>::FromString(line));
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
vector<vector<pair<vector<string>, string>>> Pattern<T>::RenderAsStringsWithTags(const vector<string> &document) {
    auto result = vector<vector<pair<vector<string>, string>>>();

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
vector<vector<pair<vector<string>, string>>> Pattern<T>::RenderAsStringsWithTags(
        vector<string> tags, const vector<string>& document) {
    auto result = vector<vector<pair<vector<string>, string>>>();

    for (int index = 0; index < document.size(); index++) {
        result.push_back(
                std::move(
                        RenderAsStringsWithTags(
                                tags,
                                Util<T>::FromString(document[index]), // TODO: Optimize the pattern class to work faster with strings
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
string Pattern<T>::Render(const string& line, unsigned int lineIndex) {
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
vector<string> Pattern<T>::Render(const vector<string> &document) {
    auto result = vector<string>();

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
Pattern<T>::Reverse(const vector<string> &document, vector<string> tags, string toReplaceWith) {
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
vector<vector<T>> Pattern<T>::Reverse(const vector<string> &document, string toReplaceWith) {
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
    self->pairStarts = list<Tag*>();

    for (Query<T> query: self->queries) {
        auto endPosition = 0;
        auto startingPosition = 0;
        auto direction = 1;

        if (query.startFromEnd) {
            direction = -1;
            startingPosition = line.size() - 1;
        }
        for (int startPosition = startingPosition;
        startPosition < line.size() && startPosition >= 0;
        startPosition+=direction) {

            auto currentQueryCharacter = 0;
            auto currentQuery = 0;
            endPosition = query.Match(line, startPosition, currentQueryCharacter, currentQuery, direction);

            if (endPosition != -1) {
                auto& mostRecentTag = query.tags[query.tags.size() - 1];

                if (*mostRecentTag.meta == "start pair") {
                    self->pairStarts.push_back(&mostRecentTag);
                }
                else if (*mostRecentTag.meta == "end pair" && !self->pairStarts.empty()) {
                    self->pairStarts.pop_back();
                }

                Subvector<T> subvector = Subvector<T>(startPosition, endPosition);
                for (auto tag: query.tags) {
                    subvector.AddTag(tag);
                }
                if (!self->pairStarts.empty())
                    subvector.AddTag(*self->pairStarts.back());

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
