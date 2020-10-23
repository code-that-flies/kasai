//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_MEMBERPROTOTYPE_H
#define KASAI_MEMBERPROTOTYPE_H

#include <map>
#include "Prototype.h"
#include "PrototypeEngine.h"
#include "EventEngine.h"

class MemberPrototype : public Prototype {
public:
    MemberFunction value;

    MemberPrototype(MemberFunction value);

    Prototype Run(Prototype* self, Prototype* input) {
        EventEngine::Trigger(PrototypeEngine::prototypeEngine->name_by_id[self->identity]
                             + "::"
                             + PrototypeEngine::prototypeEngine->name_by_id[this->identity]
        , self
        );

        value(self, input);
    }
};


#endif //KASAI_MEMBERPROTOTYPE_H
