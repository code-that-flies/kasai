//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Pattern.h"

Pattern::Query::Query(string _tag, bool startFromBeginning, bool inheritStartCursor, bool prioritizePeeks)
: tag(_tag),
startFromBeginning(startFromBeginning),
inheritStartCursor(inheritStartCursor),
prioritizePeeks(prioritizePeeks),
INNER_OR("") {
}

bool Pattern::Query::Match(string *raw, int cursor, int limit) {
    int offset = 0;

    bool result = false;

    bool prefix_result = false;
    if (!valid_prefixes.empty()) {
        for (auto& prefix : valid_prefixes) {
            if (cursor > prefix.size()) {
                if (raw->substr(cursor - prefix.size(), prefix.size())== prefix ) {
                    prefix_result = true;
                    break;
                }
            }
        }
    }

    if (!valid_prefix_characters.empty() && cursor > 0) {
        for (auto& prefix : valid_prefix_characters) {
            if (prefix == (*raw)[cursor - 1]) {
                prefix_result = true;
                break;
            }
        }
    }

    if (!prefix_result && (!valid_prefix_characters.empty() || !valid_prefixes.empty()))
        return false;


    if (startFromBeginning) {
        if (!inheritStartCursor) {
            for( auto const& [index, query] : duplicates ) {
                if (query->Match(raw, index + cursor, limit - index)) {
                    if (Substring::isClip)
                        return true;
                    else if (query->matches.size() < (limit - index))
                        return true;
                }
            }
        }
        for (int index = 0; (index < matches.size() && (index + offset) < limit); index++) {
            if (duplicates.find(index) != duplicates.end()) {
                if (duplicates[index]->Match(raw, index + cursor, limit - index)) {
                    if (Substring::isClip)
                        return true;
                    else if (duplicates[index]->matches.size() < (limit - index))
                        return true;
                }
            }

            if (amounts.find(index) != amounts.end()) {
                if (amounts[index] > 1)
                    offset++;

                if (amounts[index] == -1) {
                    while( (offset + index) < limit) {
                        result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);


                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(index + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }

                            if (!result)
                                break;
                        }
                        if (prioritizePeeks) {
                            if (peeks.find(-1) != peeks.end()) {
                                if (peeks[-1]->Match(raw, index + cursor + offset, peeks[-1]->matches.size())) {
                                    return true;
                                }
                            }
                        }

                        offset++;
                    }
                }
                else {
                    for (int i = 0; ((index + offset) < limit) && offset < amounts[index]; offset++) {
                        result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(index + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }
                        }

                        if (!result)
                            return false;
                    }
                }

            }
            else {
                result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);
            }

            if (!result) {
                return false;
            }
        }

    }
    else { // Start from end
        if (!inheritStartCursor) {
            for( auto const& [index, query] : duplicates )
            {
                if (query->Match(raw, index + cursor, limit - index)) {
                    if (Substring::isClip)
                        return true;
                    else if (query->matches.size() < (limit - index))
                        return true;
                }
            }
        }

        for (int index = 0; (index < matches.size() && (index + offset) < limit); index++) {

            if (duplicates.find(index) != duplicates.end()) {

                if (duplicates[index]->Match(raw, limit - matches.size() + index - 1 + cursor + offset, limit - index)) {
                    if (Substring::isClip)
                        return true;
                    else if (duplicates[index]->matches.size() < (limit - index))
                        return true;


                }
            }

            if (amounts.find(index) != amounts.end()) {
                if (amounts[index] > 1)
                    offset++;

                if (amounts[index] == -1) {
                    while( (offset + index) < limit) {
                        result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }

                            if (!result)
                                break;
                        }


                        if (prioritizePeeks) {
                            if (peeks.find(-1) != peeks.end()) {
                                if (peeks[-1]->Match(raw, limit - matches.size() + index - 1 + cursor + offset, peeks[-1]->matches.size())) {
                                    return true;
                                }
                            }
                        }

                        offset++;
                    }
                }
                else {
                    for (int i = 0; ((index + offset) < limit) && offset < amounts[index]; offset++) {
                        result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1, invertednesses[index]);


                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }
                        }

                        if (!result)
                            return false;
                    }
                }

            }
            else {
                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1,
                                           invertednesses[index]);
            }

            for( auto const& [index, query] : peeks )  {
                if (query->Match(raw, matches.size() + index + cursor + offset - 1, query->matches.size() + offset)) {
                    if (!Substring::isClip && !((query->matches.size() + offset) < (raw->size() - index)))
                        return false;
                } else {
                    return false;
                }
            }

            if (!result) {
                return false;
            }
        }
    }


    return result;
}

bool Pattern::Query::Match(string *raw, int cursor, int limit, int &resultEndCursor) {
    int offset = 0;

    bool result = false;


    if (startFromBeginning) {
        if (!inheritStartCursor) {
            for( auto const& [index, query] : duplicates ) {
                if (query->Match(raw, index + cursor, limit - index, resultEndCursor)) {
                    if (Substring::isClip)
                        return true;
                    else if (query->matches.size() < (limit - index))
                        return true;
                }
            }
        }
        for (int index = 0; (index < matches.size() && (index + offset) < limit); index++) {
            if (duplicates.find(index) != duplicates.end()) {
                if (duplicates[index]->Match(raw, index + cursor, limit - index, resultEndCursor)) {
                    if (Substring::isClip)
                        return true;
                    else if (duplicates[index]->matches.size() < (limit - index))
                        return true;
                }
            }

            if (amounts.find(index) != amounts.end()) {
                offset++;

                if (amounts[index] == -1) {
                    while( (offset + index) < limit) {
                        result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(index + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }

                            if (!result)
                                break;
                        }
                        if (prioritizePeeks) {
                            if (peeks.find(-1) != peeks.end()) {
                                if (peeks[-1]->Match(raw, index + cursor + offset, peeks[-1]->matches.size())) {
                                    return true;
                                }
                            }
                        }

                        offset++;
                    }
                }
                else {
                    for (int i = 0; ((index + offset) < limit) && offset < amounts[index]; offset++) {
                        result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(index + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }
                        }

                        if (!result)
                            return false;
                    }
                }

            }
            else {
                result = (*matches[index])(index + cursor + offset, queries[index], raw, 1, invertednesses[index]);
            }

            if (!result) {
                return false;
            }
        }

    }
    else { // Start from end
        if (!inheritStartCursor) {
            for( auto const& [index, query] : duplicates )
            {
                if (query->Match(raw, index + cursor, limit - index, resultEndCursor)) {
                    if (Substring::isClip)
                        return true;
                    else if (query->matches.size() < (limit - index))
                        return true;
                }
            }
        }

        for (int index = 0; (index < matches.size() && (index + offset) < limit); index++) {

            if (duplicates.find(index) != duplicates.end()) {

                if (duplicates[index]->Match(raw, limit - matches.size() + index - 1 + cursor + offset, limit - index, resultEndCursor)) {
                    if (Substring::isClip)
                        return true;
                    else if (duplicates[index]->matches.size() < (limit - index))
                        return true;


                }
            }

            if (amounts.find(index) != amounts.end()) {
                offset++;

                if (amounts[index] == -1) {
                    while( (offset + index) < limit) {
                        result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }

                            if (!result)
                                break;
                        }
                        if (prioritizePeeks) {
                            if (peeks.find(-1) != peeks.end()) {
                                if (peeks[-1]->Match(raw, limit - matches.size() + index - 1 + cursor + offset, peeks[-1]->matches.size())) {
                                    return true;
                                }
                            }
                        }

                        offset++;
                    }
                }
                else {
                    for (int i = 0; ((index + offset) < limit) && offset < amounts[index]; offset++) {
                        result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1, invertednesses[index]);

                        if (!result) {
                            for (int i = 0; i < INNER_OR.size(); i++) {
                                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, INNER_OR, raw, i, invertednesses[index]);

                                if (result)
                                    break;
                            }
                        }


                        if (!result)
                            return false;
                    }
                }

            }
            else {
                result = (*matches[index])(limit - matches.size() + index - 1 + cursor + offset, queries[index], raw, 1,
                                           invertednesses[index]);
            }
            if (!result) {
                return false;
            }
        }
    }

    for( auto const& [index, query] : peeks )  {
        // NOTE: peeks cannot internally have offset or they will not function correctly
        if (query->Match(raw, index + cursor + matches.size() - 1 + offset, query->matches.size())) {
            if (!Substring::isClip && ((query->matches.size() + offset + index + cursor + matches.size() - 1) > (raw->size())))
                return false;
        } else {
            return false;
        }
    }


    resultEndCursor = cursor + matches.size() + offset;
    return result;
}

void Pattern::Query::OR_query(int index, Pattern::Query *query) {
    duplicates[index] = query;
}

void Pattern::Query::PEEK_query(int index, Pattern::Query *query) {
    peeks[index] = query;
}

void Pattern::Query::push_back(Filter match, string query, bool inverted, int amount) {
    matches.push_back(match);
    queries.push_back(query);
    invertednesses.push_back(inverted);
    amounts[matches.size() - 1] = amount;

}

void Pattern::Query::push_back(Filter match, string query, bool inverted, bool unlimitedAmount) {
    matches.push_back(match);
    queries.push_back(query);
    invertednesses.push_back(inverted);
    amounts[matches.size() - 1] = -1;
}

Pattern::Pattern() {
    WHITESPACE = " \t\r\n";
    layer = 0;
    tally.push_back(0);
}

Pattern::~Pattern() {
    for (auto val : *this) {
        delete val;
    }
}

Substring *Pattern::add_substring(int start, int end, string tag="") {
    for (auto val : *this) {
        if (!val->tags.empty()) {
            if (val->Contains(start) || val->Contains(end)) {
                return nullptr;
            }
        }

        if (val->Contains(start) && val->Contains(end)) {
            bool success = val->push_back(start, end, tag);
            if (val->substrings.empty())
                return val;
            if (success) {
                if ( layer > (tally.size() - 1) )
                    tally.push_back(0);

                tally[layer]++;
            }
            return val->substrings[val->substrings.size() - 1];
        }
    }
    if (layer == 0) {
        if ( layer > (tally.size() - 1))
            tally.push_back(0);
        tally[layer]++;

        auto result2 = new Substring(start, end);
        result2->SetTag(tag);
        this->push_back(result2);

        return result2;
    }
    return nullptr;
}

void Pattern::new_layer() {
    for (Substring * substring : *this) {
        substring->new_layer();
    }
    this->layer++;
}

vector<string> Pattern::render_substrings() {
    auto result = vector<string>();

    for (Substring *substring : *this) {
        substring->render(&result, raw);
    }

    return result;
}

string Pattern::replace_all(string toReplaceWith) {
    string result = "";
    auto prevSubstringEnd = 0;

    for (auto substring : *this) {
        result += substring->_replace_all(toReplaceWith, raw, prevSubstringEnd);
    }

    if (prevSubstringEnd < raw->size())
        result += raw->substr(prevSubstringEnd, raw->size() - prevSubstringEnd);

    return result;
}

bool Pattern::MatchUpperCase(int cursor, string query, string *raw, bool inverted) {
    auto value = (*raw)[cursor];
    if (value >= 'a' && value <= 'z') {
        if (inverted)
            return false;
        return false;
    }
    else {
        if (inverted)
            return true;
        return true;
    }
}

bool Pattern::MatchLowerCase(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if (value >= 'A' && value <= 'Z') {
        if (inverted)
            return false;
        return false;
    }
    else {
        if (inverted)
            return true;
        return true;
    }
}

bool Pattern::MatchValue(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if (value == query[iteration - 1]) {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }

}

bool Pattern::MatchWhitespace(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if ((string("\n\t\r ")).find(value) != string::npos) {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchWord(int cursor, string query, string *raw, int iteration, bool inverted) {
    if (inverted)
        return !MatchWhitespace(cursor, query, raw, iteration, true);
    return MatchWhitespace(cursor, query, raw, iteration, true);
}

bool Pattern::MatchNumber(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if ((value >= '0' && value <= '9') || value == '.' || value == ',') {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchLetter(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || value == '-' || value == '_') {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchLength(int cursor, string query, string *raw, int iteration, bool inverted) {
    auto value = (*raw)[cursor];
    if (iteration <= query.size()) {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchUpperCase(char value, bool inverted) {
    if (value >= 'a' && value <= 'z') {
        if (inverted)
            return false;
        return false;
    }
    else {
        if (inverted)
            return true;
        return true;
    }
}

bool Pattern::MatchLowerCase(char value, bool inverted) {
    if (value >= 'A' && value <= 'Z') {
        if (inverted)
            return false;
        return false;
    }
    else {
        if (inverted)
            return true;
        return true;
    }
}

bool Pattern::MatchValue(char value, char query, bool inverted) {
    if (value == query) {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }

}

bool Pattern::MatchWhitespace(char value, bool inverted) {
    if ((string("\n\t\r ")).find(value) != string::npos) {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchWord(char value, bool inverted) {
    if (inverted)
        return !MatchWhitespace(value, true);
    return MatchWhitespace(value, true);
}

bool Pattern::MatchNumber(char value, bool inverted) {
    if ((value >= '0' && value <= '9') || value == '.' || value == ',') {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

bool Pattern::MatchLetter(char value, bool inverted) {
    if ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || value == '-' || value == '_') {
        if (inverted)
            return false;
        return true;
    }
    else {
        if (inverted)
            return true;
        return false;
    }
}

unsigned long Pattern::_FindAll(Filter filter, int cursor, string query, string *raw, int iteration, bool inverted) {

    if (iteration >= 4)
        return string::npos;

    auto toBeFiltered = (*filter)(cursor, query, raw, iteration, inverted);
    if (toBeFiltered)
        return cursor;
    else {
        iteration += 1; cursor += 1;

        return _FindAll(filter, cursor, query, raw, iteration);
    }
}

bool Pattern::FindAll(string query, bool includeGapsBeforeResults, bool includeGapsAfterResults) {
    bool hasFound = false;
    auto prevStart = string::npos;
    auto cursor = string::npos;

    for (int index = 0; index < raw->size(); index++) {
        cursor = _FindAll(MatchValue, index, query, raw, 1);

        if (cursor != string::npos) {
            if (cursor >= raw->length()) {
                new_layer();
                return hasFound;
            }
            else if (raw->substr(cursor, query.size()) == query) {
                hasFound = true;
                if (includeGapsAfterResults) {
                    if (prevStart != string::npos)
                        add_substring(prevStart, cursor - 1);
                } else if (includeGapsBeforeResults) {
                    if (prevStart != string::npos)
                        add_substring(prevStart + query.size(), cursor + query.size());
                } else {
                    add_substring(cursor, cursor + query.size());
                }
                index += query.size() - 1;
            }
        }

        prevStart = cursor;
    }

    new_layer();
    return hasFound;
}

bool Pattern::FindAllWords() {
    bool hasFound = false;
    auto prevStart = string::npos;
    bool wasWord = true;

    for (int index = 0; index < raw->size(); index++) {
        auto isWord = MatchWord(index, "", raw, 1, false);
        if (!isWord && wasWord) {
            if (prevStart != string::npos)
                add_substring(prevStart,  index);
            hasFound = true;

        }
        else if (isWord && !wasWord)
            prevStart = index;

        if (prevStart == string::npos)
            if (isWord)
                prevStart = index;

        wasWord = isWord;
    }
    if (wasWord && (prevStart != string::npos))
        add_substring(prevStart, raw->size());

    new_layer();
    return hasFound;
}

bool Pattern::FindAllWords(Pattern::Query query) {
    bool hasFound = false;
    auto prevStart = string::npos;
    bool wasWord = true;

    for (int index = 0; index < raw->size(); index++) {
        auto isWord = MatchWord(index, "", raw, 1, false);



        if (!isWord && wasWord) {
            if (prevStart != string::npos) {
                auto isMatch = query.Match(raw, prevStart, index - prevStart);

                if (isMatch)
                    add_substring(prevStart,  index, query.tag);
            }

            hasFound = true;

        }
        else if (isWord && !wasWord)
            prevStart = index;

        if (prevStart == string::npos)
            if (isWord)
                prevStart = index;

        wasWord = isWord;
    }
    if (wasWord && (prevStart != string::npos)) {
        auto isMatch = query.Match(raw, prevStart, raw->size() - prevStart);

        if (isMatch)
            add_substring(prevStart, raw->size(), query.tag);
    }

    if (query.tag == "")
        new_layer();
    return hasFound;
}

bool Pattern::FindAll(Pattern::Query &query, int limit) {
    int endPos = -1;
    bool hasFound = false;

    for (int index = 0; index < raw->size(); index++) {
        bool success = query.Match(raw, index, limit, endPos);

        if (success) {
            add_substring(index, endPos - 1, query.tag);

            index = endPos;
            hasFound = true;
        }
    }

    if (query.tag == "")
        new_layer();
    return hasFound;
}

Pattern Pattern::FromFindAll(string query, string *raw, bool includeGapsBeforeResults, bool includeGapsAfterResults) {
    Pattern result = Pattern();
    result.raw = raw;
    result.FindAll(query, includeGapsBeforeResults, includeGapsAfterResults);

    return result;
}

Pattern Pattern::FromFindAllWords(string *raw) {
    Pattern result = Pattern();
    result.raw = raw;
    result.FindAllWords();

    return result;
}

Pattern Pattern::FromFindAll(string *raw, Pattern::Query &query, int limit) {
    Pattern result = Pattern();
    result.raw = raw;
    result.FindAll(query, limit);

    return result;
}

bool Pattern::MatchAll(int cursor, string query, string *raw, int iteration, bool inverted) {
    return !inverted;
}

vector<string> Pattern::render_substrings(string tag) {

    if (tag == "") {
        return render_substrings();
    }
    else {
        auto result = vector<string>();

        for (Substring *substring : *this) {
            substring->render(&result, raw, tag);
        }

        return result;
    }

}

vector<pair<string, string>> Pattern::render_substrings_with_tags() {
    auto result = vector<pair<string, string>>();

    for (Substring *substring : *this) {
        substring->render_with_tags(&result, raw);
    }

    return result;
}
