//
// C++ Interface: dspnone.h
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DSPNONE_H
#define DSPNONE_H

#include "dspcommon.h"

#include "dbuffer.h"

class dspNONE : public dspCommon {
public:
    dspNONE();

    ~dspNONE() override;

    int openDevice() override;
    int setDeviceName(const std::string&) override;
    int closeDevice() override;
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
};

#endif
