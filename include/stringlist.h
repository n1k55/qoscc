//
// C++ Interface: stringlist.h
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <string>
#include <vector>

class stringlist {
public:
    stringlist();
    ~stringlist();
    std::string getString(unsigned int index);
    void addString(const std::string &text);
    unsigned int count();

private:
    stringlist(const stringlist &);
    std::vector<std::string> storage {};
};

#endif
