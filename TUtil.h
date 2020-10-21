//
// Created by Nathaniel Blair on 20/10/20.
//

#ifndef KASAI_TUTIL_H
#define KASAI_TUTIL_H

#include "Utility.h"
#include "IReversible.h"
#include <iterator>
#include <unordered_set>

#include <string>
#include <vector>

using std::vector;
using std::string;
using std::unordered_set;

template <class T>
class TUtil {
public:
    static void Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length);

    static void Copy(const vector<T> raw, vector<T>& result);

    static void Merge(vector<T>& result, vector<T>& input);

    // Source: https://leetcode.com/problems/intersection-of-two-arrays/discuss/82001/8ms-concise-C%2B%2B-using-unordered_set
    static vector<T> Intersection(const vector<T>& left, const vector<T>& right);

    static bool Has(const vector<T>& raw, const T& query);

    static bool Intersects(const vector<T>& left, const vector<T>& right);

    static string ToString(const vector<T>& value);

    static vector<char> FromString(const string& input);

    class Range : public IReversible {
    public:
        T min;
        T max;
        bool relative;
        T defaultValue;

        Range(T min, T max, bool relative, T defaultValue);

        Range();

        bool In(T queryValue, T rawValue);

        bool In(T value);

        // Returns the reverse of In
        // ( AKA a 'built' [default] value as *output* )
        T Reverse();
    };
};


template<class T>
void TUtil<T>::Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length) {
    std::copy(std::begin(raw) + start, std::begin(raw) + start + length, std::begin(result));
}

template<class T>
void TUtil<T>::Copy(const vector<T> raw, vector<T>& result) {
    std::copy(std::begin(raw), std::begin(raw), std::begin(result));

}

template<class T>
void TUtil<T>::Merge(vector<T> &result, vector<T> &input) {
    result.insert(result.end(), input.begin(), input.end());
}

template<class T>
vector<char> TUtil<T>::FromString(const string &input) {
    return vector<char>(input.begin(), input.end());
}

template<class T>
string TUtil<T>::ToString(const vector<T> &value) {
    return string(value.begin(), value.end());
}

template<class T>
vector<T> TUtil<T>::Intersection(const vector<T> &left, const vector<T> &right) {
    unordered_set<T> m(left.begin(), left.end());
    vector<T> result;

    for (auto a : right) {
        if (m.count(a)) {
            result.push_back(a);
            m.erase(a);
        }
    }

    return result;
}

template<class T>
bool TUtil<T>::Intersects(const vector<T>& left, const vector<T>& right) {
    if (Intersection(left, right).size() > 0)
        return true;
    else
        return false;
}

template<class T>
bool TUtil<T>::Has(const vector<T>& raw, const T& query) {
    return std::find(raw.begin(), raw.end(), query) != raw.end();
}

template<class T>
bool TUtil<T>::Range::In(T queryValue, T rawValue) {
    if (relative) {
        if ((queryValue - min) <= rawValue && (max + queryValue) >= rawValue) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (rawValue >= min && rawValue <= max) {
            return true;
        }
        else {
            return false;
        }
    }
}

template<class T>
TUtil<T>::Range::Range(T min, T max, bool relative, T defaultValue)
        : min(min), max(max), relative(relative), defaultValue(defaultValue) {
}

template<class T>
T TUtil<T>::Range::Reverse() {
    return defaultValue;
}

template<class T>
TUtil<T>::Range::Range() : relative(false) {
}

template<class T>
bool TUtil<T>::Range::In(T value) {
    return index >= this->min && index <= this->max;
}


#endif //KASAI_TUTIL_H
