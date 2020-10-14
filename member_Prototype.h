//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_MEMBER_PROTOTYPE_H
#define KASAI_MEMBER_PROTOTYPE_H

#include <map>
#include "Prototype.h"
#include "Prototype_Engine.h"
#include "Event.h"

class member_Prototype : public Prototype {
public:
    MemberFunction value;

    member_Prototype(MemberFunction value);

    Prototype Run(Prototype* self, Prototype* input) {
        Event::Trigger(Prototype_Engine::prototypeEngine->name_by_id[self->identity]
            + "::"
            + Prototype_Engine::prototypeEngine->name_by_id[this->identity]
        , self
        );

        value(self, input);
    }
};


#endif //KASAI_MEMBER_PROTOTYPE_H
