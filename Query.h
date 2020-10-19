//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_QUERY_H
#define KASAI_QUERY_H

#include "Query.h"
#include "Utility.h"
#include "MatchmakerUtility.h"
#include "IReversible.h"


// TODO: test this function in conjunction with the pattern matcher
template <class T>
class Query : public IReversible {
public:

    class Range : public IReversible {
    public:
        T min;
        T max;
        bool relative;
        T defaultValue;

        Range(T min, T max, bool relative, T defaultValue);

        bool In(T queryValue, T rawValue);

        T Reverse();
    };

    typedef bool (*Matchmaker)(T raw[], int& rawIndex, T query[], int& queryIndex, bool inverted, Range* range);

    // Each query is an array of characters. Any of them being a match is a local 'success'
    vector<T[]> queries;
    // Each antiQuery is an array of character. Any of them being a match is a total and overall 'fail'
    vector<Query> antiQueries;
    vector<Matchmaker> matchmakers;
    vector<bool> isInverteds;
    vector<T[]> prefixes;
    vector<Query> peeks;
    vector<Query> OR_queries;
    vector<Query> AND_queries;
    vector<int> repeatAmounts;
    map<int, Range> ranges;

    static void Slice(T raw[], T result[], unsigned int start, unsigned int length);

    static void Copy(T raw[], T result[]);

    static void Merge(vector<T>& result, T input[]);

    static void Merge(vector<T>& result, vector<T>& input);

    vector<T> GenerateDefault() {
        vector<T> result;


        if (prefixes.size() > 0)
            Merge(result, prefixes);

        Merge(result, queries[0]);
        Merge(result, peeks[0].GenerateDefault());

        return result;
    }

    void AddPrefix(T prefix[]);

    void AddQuery(T query[]);

    void Add(T query[], Matchmaker matchmaker, bool isInverted, Range* range);

    int TrySubqueries(T * raw, int currentRawCharacter, int currentQueryCharacter, int& currentQuery, int& direction, vector<Query>& queries);

    bool TryMatch(T raw[], int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int direction, int offset);

    vector<T> Reverse(T input[]);

    // Returns -1 is the match is a fail, returns the end position if the match is a 'success' overall
    int Match(T raw[], int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int& direction);
};

template<class T>
void Query<T>::Slice(T *raw, T *result, unsigned int start, unsigned int length) {
    std::copy(std::begin(raw) + start, std::begin(raw) + start + length, std::begin(result));

}

template<class T>
void Query<T>::Copy(T *raw, T *result) {
    std::copy(std::begin(raw), std::begin(raw), std::begin(result));

}

template<class T>
void Query<T>::AddPrefix(T *prefix) {
    auto len = sizeof prefix / sizeof prefix[0];

    T addition[len];
    Copy(prefix, addition);
    prefixes.push_back(addition);
}

template<class T>
void Query<T>::AddQuery(T *query) {
    auto len = sizeof query / sizeof query[0];

    T addition[len];
    Copy(query, addition);
    prefixes.push_back(addition);
}

template<class T>
int
Query<T>::TrySubqueries(T *raw, int currentRawCharacter, int currentQueryCharacter, int &currentQuery, int &direction,
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
bool Query<T>::TryMatch(T *raw, int currentRawCharacter, int &currentQueryCharacter, int &currentQuery, int direction,
                        int offset) {
    auto range = nullptr;

    if (ranges.find(currentQuery) != ranges.end()) {
        range = ranges[currentQuery];
    }

    return matchmakers[currentQuery](raw, currentRawCharacter, &queries[offset], currentQueryCharacter, isInverteds[offset], range);
}

template<class T>
int Query<T>::Match(T *raw, int currentRawCharacter, int &currentQueryCharacter, int &currentQuery, int &direction) {
    auto prefix_result = true;

    if (!prefixes.empty()) {
        prefix_result = false;
        for (auto& prefix : prefixes) {
            if (currentRawCharacter > prefix.size()) {
                T potentialPrefix[prefix.size()];
                if (Slice(raw, potentialPrefix, currentRawCharacter - prefix.size(), prefix.size()) == prefix ) {
                    prefix_result = true;
                    break;
                }
            }
        }
    }

    if (!prefix_result)
        return -1;

    // If an antiQuery succceeds, the query fails
    auto antiqueriesResult = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, antiQueries);
    if (antiqueriesResult != -1)
        return -1;

    auto AND_result = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, AND_queries);
    if (AND_result == -1)
        return -1;


    int offset;
    for (offset = 0; offset < raw->size() && currentQuery < queries.size() && offset > 0; offset+=direction) {
        for (int queryCharacterIndex = 0; queryCharacterIndex < queries[currentQuery].length(); queryCharacterIndex++) {
            // Do continuous repeat if the repeatAmount is -1
            if (repeatAmounts[offset] == -1) {
                while (TryMatch(raw, currentRawCharacter + offset, queryCharacterIndex, currentQuery, direction, offset)
                       && offset < raw->size()
                       && offset > 0)  {

                    auto peeks_result = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, peeks);
                    if (peeks_result != -1)
                        return peeks_result;
                }

                offset+=direction;
            }
            else {
                for (int second_offset = 0; second_offset < repeatAmounts[offset] && !(offset < 0 || offset > raw->size()); second_offset++) {
                    bool isMatch = TryMatch(raw, currentRawCharacter + offset, queryCharacterIndex, currentQuery, direction, offset);

                    if (!isMatch)
                        goto onFail;

                    offset+=direction;
                }
            }
        }

    }
    // On success:
    return currentRawCharacter + offset;

    onFail:
    return TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction, OR_queries);
}

template<class T>
void Query<T>::Add(T *query, Query::Matchmaker matchmaker, bool isInverted, Query::Range *range) {
    Add(query);
    matchmakers.push_back(matchmaker);
    isInverteds.push_back(isInverted);

    if (range != nullptr)
        ranges[queries.size() - 1] = *range;
}

template<class T>
vector<T> Query<T>::Reverse(T *input) {
    vector<T> result;

    if (prefixes.size() > 0)
        Merge(result, prefixes[0]);

    Merge(result, input);

    if (peeks.size() > 0)
        Merge(result, peeks[0].GenerateDefault());
}

template<class T>
void Query<T>::Merge(vector<T> &result, T *input) {
    std::vector<T> _input(std::begin(input), std::end(input));
    result.insert(result.end(), _input.begin(), _input.end());
}

template<class T>
void Query<T>::Merge(vector<T> &result, vector<T> &input) {
    result.insert(result.end(), input.begin(), input.end());
}


#endif //KASAI_QUERY_H
