//
// C++ Interface: dspmm.h
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef DSPMM_H
#define DSPMM_H

#include "dspcommon.h"

#include <sys/time.h>
#include <string>

#include "dbuffer.h"

class dspMM : public dspCommon {
public:
    dspMM();
    ~dspMM() override;

    int openDevice() override;
    int closeDevice() override;
    int setDeviceName(const std::string&) override;
    int setDspRate(double) override;
    int setDspSize(int) override;
    int type() override;
    int readdsp(dbuffer *) override;
    int setChannels(unsigned int channels) override;
    void setBufferSize(unsigned int size) override;
    int setAdjust(double) override;

    void getDspChannelList(stringlist *) override;
    void getDspRateList(stringlist *) override;
    void getDspSizeList(stringlist *) override;
    void getDspNameList(stringlist *liste) override;

private:

    int fd;
    struct timeval starttime;
    struct timezone tz;
};

#endif
