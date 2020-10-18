//
// Created by Nathaniel Blair on 18/10/20.
//

#ifndef KASAI_STRQUERY_H
#define KASAI_STRQUERY_H


#include "Query.h"
#include "Utility.h"


typedef bool (*StrMatchmaker)(string * raw, int& rawIndex, string * query, int& queryIndex, bool inverted);

// TODO: test this function in conjunction with the pattern matcher
class StrQuery : public Query {
public:
    // Each query is an array of characters. Any of them being a match is a local 'success'
    vector<string> queries;
    // Each antiQuery is an array of character. Any of them being a match is a total and overall 'fail'
    vector<StrQuery> antiQueries;
    vector<StrMatchmaker> matchmakers;
    vector<bool> isInverteds;
    vector<string> prefixes;
    vector<StrQuery> peeks;
    vector<StrQuery> subqueries;
    vector<int> repeatAmounts;


    int TrySubqueries(string * raw, int currentRawCharacter, int currentQueryCharacter, int& currentQuery, int& direction, vector<StrQuery>& queries) {
        int result;

        for (int index = 0; index < subqueries.size(); index++) {
            result = queries[index].Match(raw, currentRawCharacter, currentQueryCharacter, currentQuery, direction);

            if (result != -1)
                return result;
        }

        return -1;
    }

    bool TryMatch(string * raw, int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int& direction, int offset) {
        return matchmakers[currentQuery](raw, currentRawCharacter, &queries[offset], currentQueryCharacter, isInverteds[offset]);
    }

    // Returns -1 is the match is a fail, returns the end position if the match is a 'success' overall
    int Match(string * raw, int currentRawCharacter, int& currentQueryCharacter, int& currentQuery, int& direction) {
        auto prefix_result = true;

        if (!prefixes.empty()) {
            prefix_result = false;
            for (auto& prefix : prefixes) {
                if (currentRawCharacter > prefix.size()) {
                    if (raw->substr(currentRawCharacter - prefix.size(), prefix.size())== prefix ) {
                        prefix_result = true;
                        break;
                    }
                }
            }
        }

        if (!prefix_result)
            return -1;

        auto antiqueriesResult = TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, int& direction, antiQueries);
        if (antiqueriesResult != -1)
            return -1;

        for (int offset = 0; offset < raw->size() && currentQuery < queries.size(); offset+=direction) {
            for (int queryCharacterIndex = 0; queryCharacterIndex < queries[currentQuery].length(); queryCharacterIndex++) {
                // Do continuous repeat if the repeatAmount is -1
                if (repeatAmounts[offset] == -1) {
                    while (TryMatch(raw, currentRawCharacter + offset, queryCharacterIndex, currentQuery, direction, offset)
                           && offset < raw->size())  {
                        for (auto peek : peeks) {
                            auto result = peek.Match(raw, currentRawCharacter + offset, currentQueryCharacter, direction, currentQuery);
                            if (result != -1)
                                return result;
                        }
                    }

                    offset+=direction;
                }
                else {
                    for (int second_offset = 0; second_offset < repeatAmounts[offset]; second_offset++) {
                        bool isMatch = TryMatch(raw, currentRawCharacter + offset, queryCharacterIndex, currentQuery, direction, offset);

                        if (!isMatch)
                            goto onFail;

                        offset+=direction;
                    }
                }
            }

        }

        onFail:
            return TrySubqueries(raw, currentRawCharacter, currentQueryCharacter, currentQuery, subqueries);
    }

};


#endif //KASAI_STRQUERY_H
