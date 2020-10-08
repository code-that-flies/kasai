//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_PATTERN_H
#define KASAI_PATTERN_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Substring.h"

using std::string;
using std::vector;
using std::map;

typedef bool (*Filter)(int, string, string*, int, bool);
class Pattern : public vector<Substring*> {
public:
    int layer;

    struct Query {
    public:
        vector<Filter> matches;
        vector<string> queries;
        vector<bool> invertednesses;
        map<int, Query*> duplicates;
        map<int, Query*> peeks;
        map<int, int> amounts;
        bool startFromBeginning;
        // Allows you to choose whether or not to inherit the starting position of the parent query in the duplicates
        bool inheritStartCursor;
        string tag;

        Query(bool startFromBeginning = true, bool inheritStartCursor = false, string _tag="");

        bool Match(string* raw, int cursor, int limit);


        bool Match(string* raw, int cursor, int limit, int& resultEndCursor);

        void OR_query(int index, Query* query);

        void PEEK_query(int index, Query* query);

        void push_back(Filter match, string query, bool inverted, int amount);

        void push_back(Filter match, string query, bool inverted, bool unlimitedAmount=true);
    };

    string WHITESPACE;
    const static int ABBREVIATION_LENGTH = 4;
    string* raw;
    vector<int> tally;

    Pattern();

    ~Pattern();

    int tally_substrings() {
        return tally[tally.size() - 1];
    }

    Substring * add_substring(int start, int end, string tag);

    void new_layer();



    vector<string> render_substrings();

    vector<string> render_substrings(string tag);

    string replace_all(string toReplaceWith);

    static bool MatchUpperCase(int cursor, string query, string* raw, bool inverted);

    static bool MatchLowerCase(int cursor, string query, string* raw, int iteration, bool inverted);

    static bool MatchValue(int cursor, string query, string* raw, int iteration, bool inverted);


    static bool MatchAll(int cursor, string query, string* raw, int iteration, bool inverted);


    static bool MatchWhitespace(int cursor, string query, string* raw, int iteration, bool inverted);


    static bool MatchWord(int cursor, string query, string* raw, int iteration, bool inverted);

    static bool MatchNumber(int cursor, string query, string* raw, int iteration, bool inverted);

    static bool MatchLetter(int cursor, string query, string* raw, int iteration, bool inverted);

    static bool MatchLength(int cursor, string query, string* raw, int iteration, bool inverted);

    static bool MatchUpperCase(char value, bool inverted);

    static bool MatchLowerCase(char value, bool inverted);

    static bool MatchValue(char value, char query, bool inverted);


    static bool MatchWhitespace(char value, bool inverted);


    static bool MatchWord(char value, bool inverted);

    static bool MatchNumber(char value, bool inverted);

    static bool MatchLetter(char value, bool inverted);


    static size_type _FindAll(Filter filter, int cursor, string query, string* raw, int iteration, bool inverted = false);



    bool FindAll(string query, bool includeGapsBeforeResults = false, bool includeGapsAfterResults = false);

    bool FindAllWords();

    bool FindAllWords(Query query);


    bool FindAll(Query& query, int limit);

    static Pattern FromFindAll(string query, string * raw, bool includeGapsBeforeResults = false, bool includeGapsAfterResults = false);

    static Pattern FromFindAllWords(string * raw);

    static Pattern FromFindAll(string * raw, Query& query, int limit);
};




#endif //KASAI_PATTERN_H
