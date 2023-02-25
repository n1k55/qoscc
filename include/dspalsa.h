//
// C++ Interface: dspalsa.h
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DSPALSA_H
#define DSPALSA_H

#include "dspcommon.h"

#include <alsa/asoundlib.h>

#include "stringlist.h"
#include "dbuffer.h"

class dspALSA : public dspCommon {
public:
    dspALSA();

    ~dspALSA() override;

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

    unsigned int getChannels();
    double getDspRate();
    unsigned int getDspSize();

    void getDspChannelList(stringlist *) override;
    void getDspRateList(stringlist *) override;
    void getDspSizeList(stringlist *) override;
    void getDspNameList(stringlist *liste) override;

private:
    snd_pcm_t *pcm_handle;
    snd_pcm_stream_t stream;
    snd_pcm_hw_params_t *hwparams;

    stringlist devicelist;
};

#endif

