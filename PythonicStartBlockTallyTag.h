//
// Created by Nathaniel Blair on 28/10/20.
//

#ifndef KASAI_PYTHONICSTARTBLOCKTALLYTAG_H
#define KASAI_PYTHONICSTARTBLOCKTALLYTAG_H


#include "TallyTag.h"

class PythonicStartBlockTallyTag : public TallyTag<char> {
    PythonicStartBlockTallyTag() : TallyTag("Start Block") {

    }

    int Tally(const vector<char>& raw) override {
        float result = 0;

        for (int i = raw.size() - 1; i >= 0; i--) {
            if (raw[i] == '\n'){
                // Ignore
            }
            else if (raw[i] == ':') {
                result++;
            }
            else {
                return result;
            }
        }

        return 0; // If the colon is not followed up by text, it is not a start block in the line - thus, 0 is returned
    }
};




#endif //KASAI_PYTHONICSTARTBLOCKTALLYTAG_H
