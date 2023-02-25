//
// C++ Interface: dspjack.h
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DSPJACK_H
#define DSPJACK_H

#include "dspcommon.h"

#include <jack/jack.h>
#include <string>

#include "dbuffer.h"

class stringlist;

class dspJACK : public dspCommon {
public:
    dspJACK();

    ~dspJACK() override;

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

    // oooh, shit. these have to be public because the
    // processing (callback) functions have to be able
    // to access them from "outside"
    jack_default_audio_sample_t **buffer;
    pthread_cond_t data_ready;
    pthread_mutex_t data_ready_lock;
    jack_port_t **inports;
    jack_port_t **outports;

    jack_nframes_t frames;

private:
    jack_client_t *client;

    static int jack_process(jack_nframes_t nframes, void *arg);
    static void jack_shutdown(void *arg);

    bool jackdRunning;
};

#endif
