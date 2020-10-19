//
// Created by Nathaniel Blair on 18/10/20.
//

#include "Query.h"

template<class T>
bool Query<T>::Range::In(T queryValue, T rawValue) {
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
Query<T>::Range::Range(T min, T max, bool relative, T defaultValue)
: min(min), max(max), relative(relative), defaultValue(defaultValue) {
}

template<class T>
T Query<T>::Range::Reverse() {
    return defaultValue;
}
