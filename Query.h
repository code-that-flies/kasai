//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_QUERY_H
#define KASAI_QUERY_H

#include "Query.h"
#include "Utility.h"
#include "Util.h"
#include "MatchmakerUtility.h"
#include "IReversible.h"
#include "Tag.h"
#include "Tally.h"

// TODO: test this function in conjunction with the pattern matcher
template <class T>
class Query : public IReversible {
public:
    class Meta {
    public:
        vector<TallyTag<T>*> tagsToTallyWith; // To be altered and customised by the developer who writes the matchmakers in current use
        Tally<T> previousLineTally;
        Tally<T> currentLineTally;

        void Update(const vector<T>& raw) {
            this->previousLineTally = std::move(this->currentLineTally);
            this->currentLineTally = Tally(tagsToTallyWith, raw);
        }
    };

    // Each query has a given matchmaker.
    // This is the definition of a matchmaker!
    typedef bool (*Matchmaker)(const vector<T>& raw, int& rawIndex, vector<T>& query, int& queryIndex, bool inverted, typename Util<T>::Range* range, const Meta& metadata);

    // Query is an array of characters.
    // Being a match is a local 'success'
    vector<T> query;
    // Each query has a given matchmaker
    vector<Matchmaker> matchmakers;
    // Each given matchmaker may return
    // the opposite of what it is normally calculating if 'isInvetreds[currentQuery] == true'
    vector<bool> isInverteds;
    vector<int> repeatAmounts;
    map<int, typename Util<T>::Range> ranges;

    // Tags are used to:
    // generate Abstract Data Trees such as Abstract Syntax Trees
    // decide which parts of a raw[] to ignore as "already tagged"
    // and which parts of a raw[] to NOT ignore as "already tagged BUT with OVERLAPPING tags"
    vector<Tag> tags;
    Meta metadata;

    // Prefixes are before the match and suffixes are after the match
    // (Suffixes will interrupt a -1 (aka unlimited) length repeat if matched successfully)
    vector<vector<T>> prefixes;
    vector<Query> suffixes;

    vector<Query> OR_queries;
    vector<Query> AND_queries;
    vector<Query> NOT_queries;

    bool startFromEnd;

    vector<T> GenerateDefault();

    void AddPrefix(vector<T> prefix);

    void Add(T queryValue, Matchmaker matchmaker, bool isInverted, typename Util<T>::Range* range, int repeatAmount=1);

    int TrySubqueries(const vector<T>& raw, int currentRawCharacter, int currentQueryCharacter, int& currentQuery, int& direction, vector<Query>& queries);

    bool TryMatch(const vector<T>& raw, int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int direction, int offset);

    // Returns the reverse of match
    // (aka a re-'built' array)
    vector<T> Reverse(const vector<T>& input);

    // Returns -1 is the match is a fail, returns the max position if the match is a 'success' overall
    int Match(const vector<T>& raw, int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int& direction);
};

template<class T>
void Query<T>::AddPrefix(vector<T> prefix) {
    auto len = prefix.size();

    T addition[len];
    Copy(prefix, addition);
    prefixes.push_back(addition);
}

template<class T>
int
Query<T>::TrySubqueries(const vector<T>& raw, int currentRawCharacter, int currentQueryCharacter, int &currentQuery, int &direction,
                        vector<Query> &queries) {
    int result;

    for (int index = 0; index < OR_queries.size(); index++) {
        result = queries[index].Match(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction);

        if (result != -1)
            return result;
    }

    return -1;
}

template<class T>
bool Query<T>::TryMatch(const vector<T>& raw, int currentRawCharacter, int &currentQueryCharacter, int &currentQuery, int direction,
                        int offset) {
    typename Util<T>::Range* range = nullptr;

    if (ranges.find(currentQuery) != ranges.end()) {
        range = &ranges[currentQuery];
    }

    return matchmakers[currentQuery](raw, currentRawCharacter, query, currentQueryCharacter, isInverteds[currentQuery], range, metadata);
}

template<class T>
int Query<T>::Match(const vector<T>& raw, int currentRawCharacter, int &currentQueryCharacter, int &currentQuery, int &direction) {
    auto prefix_result = true;

    if (!prefixes.empty()) {
        prefix_result = false;
        for (auto& prefix : prefixes) {
            if (currentRawCharacter > prefix.size()) {
                vector<T> potentialPrefix;
                Util<T>::Slice(raw, potentialPrefix, currentRawCharacter - prefix.size(), prefix.size());
                if ( Util<T>::Same(potentialPrefix, prefix) ) {
                    prefix_result = true;
                    break;
                }
            }
        }
    }

    if (!prefix_result)
        return -1;

    // If an antiQuery succceeds, the query fails
    auto NOT_queriesResult = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, NOT_queries);
    if (NOT_queriesResult != -1)
        return -1;

    auto AND_result = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, AND_queries);
    if (AND_result == -1 && !AND_queries.empty())
        return -1;


    int offset;
    auto len = query.size();
    for (offset = 0; offset < raw.size() && currentQuery < len && offset >= 0; offset+=direction) {
        // Do continuous repeat if the repeatAmount is -1
        if (repeatAmounts[offset] == -1) {
            while (TryMatch(raw, currentRawCharacter + offset, currentQueryCharacter, currentQuery, direction, offset)
                   && offset < raw.size()
                   && offset > 0) {

                auto peeks_result = TrySubqueries(raw, currentRawCharacter + offset, currentQueryCharacter,
                                                  currentQuery, direction, suffixes);
                if (peeks_result != -1)
                    return peeks_result;
            }

            offset += direction;
        } else {
            for (int second_offset = 0;
                 second_offset < repeatAmounts[offset] && !(offset < 0 || offset > raw.size()); second_offset++) {
                bool isMatch = TryMatch(raw, currentRawCharacter + offset, currentQueryCharacter, currentQuery,
                                        direction, offset);

                if (!isMatch)
                    goto onFail;

                offset += direction;
            }
        }
    }
    // On success:
    return currentRawCharacter + offset;

    onFail:
    return TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, OR_queries);
}

template<class T>
void Query<T>::Add(T queryValue, Query::Matchmaker matchmaker, bool isInverted, typename Util<T>::Range *range, int repeatAmount) {
    query.push_back(queryValue);
    matchmakers.push_back(matchmaker);
    isInverteds.push_back(isInverted);
    repeatAmounts.push_back(repeatAmount);

    if (range != nullptr)
        ranges[matchmakers.size() - 1] = *range;
}

template<class T>
vector<T> Query<T>::Reverse(const vector<T>& input) {
    vector<T> result;

    if (prefixes.size() > 0)
        Merge(result, prefixes[0]);

    Merge(result, input);

    if (suffixes.size() > 0)
        Merge(result, suffixes[0].GenerateDefault());
}


template<class T>
vector<T> Query<T>::GenerateDefault() {
    vector<T> result;

    if (prefixes.size() > 0)
        Merge(result, prefixes);

    Merge(result, query);
    Merge(result, suffixes[0].GenerateDefault());

    return result;
}

#endif //KASAI_QUERY_H
