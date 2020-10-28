//
// Created by Nathaniel Blair on 27/10/20.
//

#ifndef KASAI_TAG_H
#define KASAI_TAG_H

#include "Utility.h"

class Tag : public string {
public:

    Tag* meta;


    ~Tag();

    Tag(string value) : string(value), meta(nullptr) {

    }

    Tag(Tag meta, string value) : string(value), meta(new Tag(meta)) {

    }

};

#endif //KASAI_TAG_H
