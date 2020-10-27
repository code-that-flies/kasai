//
// Created by Nathaniel Blair on 27/10/20.
//

#ifndef KASAI_INDENTATIONTALLYTAG_H
#define KASAI_INDENTATIONTALLYTAG_H


#include "TallyTag.h"

class IndentationTallyTag : public TallyTag<char> {
    static unsigned int SPACES_PER_TAB;

    unsigned int Tally(const vector<char>& raw) override {
        float result = 0;

        for (auto character: raw) {
            if (character == ' '){
                result += 1.0f / SPACES_PER_TAB;
            }
            else if (character == '\t') {
                result++;
            }
            else {
                return result;
            }
        }

        return result;
    }
};


#endif //KASAI_INDENTATIONTALLYTAG_H
