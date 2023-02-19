//
// C++ Implementation: stringlist.cpp
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "stringlist.h"

#include <cstdio>
#include "stringlist.h"

#include <stdexcept>

stringlist::stringlist() {
}

// copy constructor
stringlist::stringlist(const stringlist &oldlist) {
}

stringlist::~stringlist() {
}

// string getString(unsigned int)
// return string of given index
std::string stringlist::getString(unsigned int index) {
    try {
        return storage.at(index);
    } catch (std::out_of_range const& exc) {
        return "";
    }
}

// void addString(const string&)
// add a string to list
void stringlist::addString(const std::string &text) {
    storage.push_back(text);
}

// unsigned int count()
// return number of strings
unsigned int stringlist::count() {
    return static_cast<unsigned int>(storage.size());
}
