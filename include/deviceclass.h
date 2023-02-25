#ifndef DEVICECLASS_H
#define DEVICECLASS_H

#include <string>
#include <mutex>
#include <atomic>

#include "dbuffer.h"
#include "stringlist.h"
#include "dspcommon.h"
#include "deviceinterface.h"
#include "controllerclass.h"

class DeviceClass : public DeviceInterface
{
public:
    DeviceClass(ControllerClass* _parentController);
    ~DeviceClass();

    void dump(FILE *file) override;

    int start() override;   // open device and start reading
    int stop() override;    // close device and stop reading
    int setDspSize(int) override;
    int getDspSize() override;

    std::string getName() override;
    void setname(std::string) override;
    std::string getdevname() override;
    void setdevname(std::string) override;

    unsigned int getChNum() override;

    void setBuffersize(unsigned int) override;
    int setChannels(unsigned int) override;
    int setDspRate(double) override;
    double getAdjust() override;

    unsigned int getBuffersize() override;
    unsigned int getChannels() override;
    double getDspRate() override;
    int getDspType() override;

    void getDspChannelList(stringlist *) override;
    void getDspRateList(stringlist *) override;
    void getDspSizeList(stringlist *) override;
    void getDspNameList(stringlist *) override;

    void run() override;

    int setDeviceType(int) override;
    int setAdjust(double) override;

    bool isRunning() override;
private:
    ControllerClass* parentController;
    dspCommon *dsp;
    int type;

//    unsigned int buffersize;
//    double **fbuf;                   // buffers for all channels
    dbuffer *buf;

    std::string name;                  // symbolic name of device

    void readbuffers();

    std::shared_mutex rwMutex;
    std::thread* deviceThread;

    bool readerRunning;
    std::atomic<bool> close;

    // FIXME: we should get that running with a friend?!?
    void *deviceHandler(); // thread handling function...

};

#endif // DEVICECLASS_H

