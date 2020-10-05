//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_INT_PROTOTYPE_H
#define KASAI_INT_PROTOTYPE_H

#include "typed_Prototype.h"

class int_Prototype : public typed_Prototype<int> {
public:
    void load(string raw) override;
};


#endif //KASAI_INT_PROTOTYPE_H
