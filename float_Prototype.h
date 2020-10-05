//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_FLOAT_PROTOTYPE_H
#define KASAI_FLOAT_PROTOTYPE_H

#include "typed_Prototype.h"

class float_Prototype : public typed_Prototype<float> {
public:

    void load(string raw) override;

};


#endif //KASAI_FLOAT_PROTOTYPE_H
