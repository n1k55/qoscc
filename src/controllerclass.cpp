//
// C++ Implementation: globalClass.cpp
//
// Description:
//
//
// Author: Sven Queisser, <tincan@svenqueisser.de, (C) 2001 - 2005
//         MajorX234, <majorx234@gmail.com (C) 2022
// Copyright: See COPYING file that comes with this distribution
//
//

#include "controllerclass.h"

#include <cstring>
#include <cstdio>
#include <string>
#include <mutex>
#include <thread>
#include <fftw3.h>
#include <cerrno>
#include <unistd.h>
#include <iostream>

#include "deviceclass.h"
#include "scopeclass.h"
#include "traceclass.h"
#include "stringlist.h"
#include "dspcommon.h"
#include "dbuffer.h"

ControllerClass::ControllerClass() {
    devnum = 0;
    tracenum = 0;
    scopenum = 0;
    delay = 40000; // that would be 25Hz at absolutely no load. good start value.
}

ControllerClass::~ControllerClass() {
    delete [] traces;
    delete [] devices;
    delete [] scopes;
    // this produces a sigsegv, so leave it!
    //    fftw_cleanup();
}

// addDevice
// ads specified device to array
int ControllerClass::addDevice(DeviceInterface *newdev) {
    // bail out if device exists already
    if(getDevice(newdev->getName()) != nullptr) {
        return -1;
    }

    {
        writeLock lock(rwMutex);
        //    printf("ControllerClass::addDevice(%s): now have %d devices\n", newdev->getname(), devnum+1);
        //    fflush(stdout);
        DeviceInterface **newlist;  // create new, larger array
        newlist = new DeviceInterface*[devnum + 1];
        // copy over old devices
        for(unsigned int i = 0; i < devnum; i++) {
            newlist[i] = devices[i];
        }
        // add new device at end
        newlist[devnum] = newdev;

        // delete old array if it existed
        if (devnum != 0U) {
            delete [] devices;
        }
        // copy back new list
        devices = newlist;
        // increase number of devices
        devnum ++;
    }

    return 0;
}

// removeDevice
// removes specified class from array and clears memory
int ControllerClass::removeDevice(DeviceInterface *devptr) {
    // bail out if we got a zero pointer
    if ((devptr == nullptr) || devnum < 1) {
      return -1;
    }

    {
        writeLock lock(rwMutex);
        int removed = 0; // number of traces we removed ....
        for(unsigned int i = 0; i < devnum; i++) {
            if(devptr == devices[i]) {
                removed ++;
                //            delete devices[i];
            } else {
                devices[i - removed] = devices[i];
            }
        }
        devnum -= removed;
    }

    return 0;
}

// DeviceInterface *getDevice(char*)
// looks for device with name and returns its pointer
// else it returns the NULL-pointer
DeviceInterface *ControllerClass::getDevice(const std::string &name) {
    readLock lock(rwMutex);
    for (unsigned int i = 0; i < devnum; i++) {
        if(name == devices[i]->getName()) {
            return devices[i];
        }
    }
    return nullptr;
}

// int addTrace(TraceInterface *)
// adds new trace to global list
int ControllerClass::addTrace(TraceInterface *newtrace) {
    // bail out if trace name exists already
    if (getTrace(newtrace->getName()) != nullptr) {
        return -1;
    }

    {
        readLock lock(rwMutex);
        //    printf("ControllerClass::addTrace(%s): now have %d traces\n", newtrace->getname(), tracenum+1);
        //    fflush(stdout);
        TraceInterface **newlist;  // create new, larger array
        newlist = new TraceInterface*[tracenum + 1];
        // copy over old traces
        for (unsigned int i = 0; i < tracenum; i++) {
            newlist[i] = traces[i];
        }
        // add new device at end
        newlist[tracenum] = newtrace;

        // delete old array if it existed
        if (tracenum != 0U) {
            delete [] traces;
        }
        // copy back new list
        traces = newlist;
        // increase number of devices
        tracenum ++;
    }

    return 0;
}

// int removeTrace(TraceInterface*)
// removes trace identified by pointer from class
int ControllerClass::removeTrace(TraceInterface *deltrace) {
    // bail out if we got a zero pointer
    if((deltrace == nullptr) || tracenum < 1) {
        return -1;
    }

    {
        writeLock lock(rwMutex);
        int removed = 0; // number of traces we removed ....
        for(unsigned int i = 0; i < tracenum; i++) {
            if(deltrace == traces[i]) {
                removed ++;
            } else {
                traces[i - removed] = traces[i];
            }
        }

        tracenum -= removed;
    }

    return 0;
}

// TraceInterface *getTrace(char*)
// looks for trace with name and returns its pointer
// else it returns the NULL-pointer
TraceInterface *ControllerClass::getTrace(const std::string &name) {
    readLock lock(rwMutex);
    for(unsigned int i = 0; i < tracenum; i++) {
        if (name == traces[i]->getName()) {
            return traces[i];
        }
    }

    return nullptr;
}

// int getTraceNum()
// returns number of existing traces
unsigned int ControllerClass::getTraceNum() const {
    return tracenum;
}

int ControllerClass::addScope(ScopeInterface *newscope) {
    // bail out if scope name exists already
    if(getScope(newscope->getName()) != nullptr) {
        return -1;
    }

    {
        writeLock lock(rwMutex);
        //    printf("ControllerClass::addScope(%s): now have %d scopes\n", newscope->getName().c_str(), scopenum+1);
        //    fflush(stdout);
        ScopeInterface **newlist;  // create new, larger array
        newlist = new ScopeInterface*[scopenum + 1];
        // copy over old scopes
        for(unsigned int i = 0; i < scopenum; i++) {
            newlist[i] = scopes[i];
        }
        // add new scope at end
        newlist[scopenum] = newscope;

        // delete old array if it existed
        if(scopenum != 0U) {
            delete [] scopes;
        }
        // copy back new list
        scopes = newlist;
        // increase number of devices
        scopenum ++;
    }
    return 0;
}

// int removeScope(ScopeInterface*)
// removes Scope of given name
int ControllerClass::removeScope(ScopeInterface *scope) {
    // bail out if we got a zero pointer
    if((scope == nullptr) || scopenum < 1) {
        return -1;
    }

    {
        writeLock lock(rwMutex);
        int removed = 0; // number of traces we removed ....
        for(unsigned int i = 0; i < scopenum; i++) {
            if(scope == scopes[i]) {
                removed ++;
                delete scopes[i];
            } else {
                scopes[i - removed] = scopes[i];
            }
        }
        scopenum -= removed;
    }

    return 0;
}

// int removeScope(string)
// removes Scope of given name
int ControllerClass::removeScope(const std::string &name) {
    return removeScope(getScope(name));
}

// scope *getScope(string)
// returns pointer so scope of given name
// returns nullptr if none matches
ScopeInterface *ControllerClass::getScope(const std::string &name) {
    readLock lock(rwMutex);
    for(unsigned int i = 0; i < scopenum; i++) {
        if(name == scopes[i]->getName()) {
            return scopes[i];
        }
    }
    return nullptr;
}

// stringlist getDeviceList()
// return list of devices
void ControllerClass::getDeviceList(stringlist *liste) {
    std::shared_lock<std::shared_mutex> lock(rwMutex);
    // compose list...
    for(unsigned int i = 0; i < devnum; i++) {
        liste->addString(devices[i]->getName());
    }
}

// stringlist getTraceList()
// return list of Traces
void ControllerClass::getTraceList(stringlist *liste) {
    readLock lock(rwMutex);
    // compose list...
    for(unsigned int i = 0; i < tracenum; i++) {
        liste->addString(traces[i]->getName());
    }
}

// stringlist getScopeList()
// return list of scope wins
void ControllerClass::getScopeList(stringlist *liste) {
    readLock lock(rwMutex);
    // compose list...
    for(unsigned int i = 0; i < scopenum; i++) {
        liste->addString(scopes[i]->getName());
    }
}

int ControllerClass::readconfig(const std::string &file) {
    FILE *conffile;
    char *confline = nullptr;
    size_t conflen = 0;
    int fstat;

    enum conftypes{C_NONE = 0, C_DEV, C_TRACE, C_SCOPE};
    conftypes conftype = C_NONE;

    char thelp[81];

    conffile = fopen(file.c_str(), "r");
    if(conffile == nullptr) {
        std::cerr << "\nreadconfig(): could not open config file" << file <<"! (" << errno << ")" << std::endl;
        return 1;
    }

    TraceInterface *currtrace; // pointers for current device / trace / scope
    DeviceInterface *currdev;
    ScopeInterface *currscope;

    // read configfile, transfer parameters to structs
    do {
        fstat = getline(&confline, &conflen, conffile);
        if(fstat > 1 && (strncmp("#", confline, 1) != 0)) {   /* 0 would mean EOF, 1 would mean an empty line, "#" is a comment */
            confline[fstat - 1] = 0;      /* close string properly by replacing LF by zero */
            //            printf("%s\n", confline);
            //            fflush(stdout);
            if(strncmp("[dev]", confline, 5) == 0) {
                currdev = new DeviceClass(this);
                if(currdev == nullptr) {
                    std::cerr << "readconfig: could not allocate memory (" << errno << std::endl;
                    exit(-1);
                }
                addDevice(currdev);
                conftype = C_DEV;
            } else if(strncmp("[trace]", confline, 7) == 0) {
                currtrace = new TraceClass(this);
                if(currtrace == nullptr) {
                    std::cerr << "readconfig: could not allocate memory (" << errno << ")" << std::endl;
                    exit(-1);
                }
                addTrace(currtrace);
                conftype = C_TRACE;
            } else if(strncmp("[scope]", confline, 8) == 0) {
                currscope = new ScopeClass(this);
                if(currscope == nullptr) {
                    std::cerr << "readconfig: could not allocate memory (" << errno << ")" << std::endl;
                    exit(-1);
                }
                addScope(currscope);
                conftype = C_SCOPE;
            } else if(conftype == C_DEV) {
                if(strncmp("name = ", confline, 7) == 0) {
                    currdev->setname(getsparam(confline));
                } else if(strncmp("file = ", confline, 7) == 0) {
                    currdev->setdevname(getsparam(confline));
                } else if(strncmp("channels = ", confline, 11) == 0) {
                    currdev->setChannels(getiparam(confline));
                } else if(strncmp("dsp_rate = ", confline, 11) == 0) {
                    currdev->setDspRate(getfparam(confline));
                } else if(strncmp("devicetype = ", confline, 13) == 0) {
                    if(strncmp("none", getsparam(confline), 4) == 0) {
                        currdev->setDeviceType(PCM_NONE);
                    } else if(strncmp("oss", getsparam(confline), 3) == 0) {
                        currdev->setDeviceType(PCM_OSS);
                    } else if(strncmp("alsa", getsparam(confline), 4) == 0) {
                        currdev->setDeviceType(PCM_ALSA);
                    } else if(strncmp("jack", getsparam(confline), 4) == 0) {
                        currdev->setDeviceType(PCM_JACK);
                    } else if(strncmp("mm", getsparam(confline), 4) == 0) {
                        currdev->setDeviceType(PCM_MM);
                    } else {
                        std::cerr << "readconfig(): no such option for devicetype for " << currdev->getName() << std::endl;
                    }
                } else if(strncmp("dsp_size = ", confline, 11) == 0) {
                    if(currdev->setDspSize(getiparam(confline)) != 0) {
                        std::cerr << "readconfig(): invalid dsp_size for " << currdev->getName() << std::endl;
                    }
                } else if(strncmp("buffersize = ", confline, 13) == 0) {
                    currdev->setBuffersize(getiparam(confline));
                } else if(strncmp("adjust = ", confline, 9) == 0) {
                    currdev->setAdjust(getfparam(confline));
                } else {
                    printf("no such option in [dev]: %s\n", confline);
                }

            } else if(conftype == C_TRACE) {
                if(strncmp("name = ", confline, 7) == 0) {
                    currtrace->setname(getsparam(confline));
                } else if(strncmp("channel = ", confline, 10) == 0) {
                    currtrace->setDevChannel(getiparam(confline));
                } else if(strncmp("color = ", confline, 8) == 0) {
                    currtrace->setColor(getsparam(confline));
                } else if(strncmp("xshift = ", confline, 9) == 0) {
                    currtrace->setXShift(getfparam(confline));
                } else if(strncmp("yshift = ", confline, 9) == 0) {
                    currtrace->setYShift(getfparam(confline));
                } else if(strncmp("buffersize = ", confline, 13) == 0) {
                    currtrace->setBufferSize(getiparam(confline));
                } else if(strncmp("perfectbuffer = ", confline, 16) == 0) {
                    if(strncmp("true", getsparam(confline), 4) == 0) {
                        currtrace->setPerfectBuffer(true);
                    } else if(strncmp("false", getsparam(confline), 5) == 0) {
                        currtrace->setPerfectBuffer(false);
                    } else {
                        std::cerr << "readconfig(): no such option for perfectbuffer: " << std::string(getsparam(confline)) << std::endl;
                    }
                } else if(strncmp("parent = ", confline, 9) == 0) {
                    if(currtrace->setParentName(getsparam(confline)) != 0) {
                        std::cerr << "readconfig(): Unable to set parent name " << std::string(getsparam(confline)) << " for trace " << currtrace->getName() << std::endl;
                        fflush(stderr);
                    }
                } else if(strncmp("fftwin = ", confline, 9) == 0) {
                    if(strncmp("rectangular", getsparam(confline), 11) == 0) {
                        currtrace->setFftWinType(dbuffer::winRect);
                    } else if(strncmp("hanning", getsparam(confline), 7) == 0) {
                        currtrace->setFftWinType(dbuffer::winHanning);
                    } else if(strncmp("hamming", getsparam(confline), 7) == 0) {
                        currtrace->setFftWinType(dbuffer::winHamming);
                    } else if(strncmp("blackman-harris-a", getsparam(confline), 17) == 0) {
                        currtrace->setFftWinType(dbuffer::winBlackmanHarrisA);
                    } else if(strncmp("blackman-harris-b", getsparam(confline), 17) == 0) {
                        currtrace->setFftWinType(dbuffer::winBlackmanHarrisB);
                    } else {
                        printf("no such option for fftwin: %s\n", getsparam(confline));
                    }
                }
                else {
                    printf("no such option in [trace]: %s\n", confline);
                }

            } else if(conftype == C_SCOPE) {
                if(strncmp("name = ", confline, 7) == 0) {
                    currscope->setName(getsparam(confline));
                } else if(strncmp("mode = ", confline, 7) == 0) {
                    strcpy(thelp, getsparam(confline));
                    if(strncmp(thelp, "yt", 2) == 0) {
                        currscope->setMode(M_YT);
                    } else if(strncmp(thelp, "xy", 2) == 0) {
                        currscope->setMode(M_XY);
                    } else if(strncmp(thelp, "fft", 3) == 0) {
                        currscope->setMode(M_FFT);
                    } else {
                        printf("no such parameter for mode in [scope]: %s\n", thelp);
                    }
                } else if(strncmp("sweep = ", confline, 8) == 0) {
                    currscope->setSweep(getfparam(confline));
//ToFix                else if(!strncmp("bcol = ", confline, 7))
//ToFix                    currscope->setBCol(getsparam(confline));
//ToFix                else if(!strncmp("gridcol = ", confline, 10))
//ToFix                    currscope->setGridCol(getsparam(confline));
//ToFix                else if(!strncmp("markcol = ", confline, 10))
//ToFix                    currscope->setMarkCol(getsparam(confline));
//ToFix                else if(!strncmp("textcol = ", confline, 10))
//ToFix                    currscope->setTextCol(getsparam(confline));
                } else if(strncmp("font = ", confline, 7) == 0) {
                    currscope->setFont(getsparam(confline));
                } else if(strncmp("width = ", confline, 8) == 0) {
                    currscope->setWidth(getiparam(confline));
                } else if(strncmp("height = ", confline, 9) == 0) {
                    currscope->setHeight(getiparam(confline));
                } else if(strncmp("hdivs = ", confline, 8) == 0) {
                    currscope->setHDivs(getiparam(confline));
                } else if(strncmp("vdivs = ", confline, 8) == 0) {
                    currscope->setVDivs(getiparam(confline));
                } else if(strncmp("trace = ", confline, 8) == 0) {
                    if(currscope->addTrace(getTrace(getsparam(confline))) != 0) {
                        std::cerr << "readconfig(): cannot add Trace " << std::string(getsparam(confline)) << std::endl;
                    }
                } else if(strncmp("trigger_source = ", confline, 17) == 0) {
                    currscope->setTriggerSource(getsparam(confline));
                } else if(strncmp("trigger_level = ", confline, 16) == 0) {
                    currscope->setTriggerLevel(getfparam(confline));
                } else if(strncmp("trigger_edge = ", confline, 15) == 0) {
                    strcpy(thelp, getsparam(confline));
                    if(strncmp(thelp, "fall", 4) == 0) {
                        currscope->setTriggerEdge(TE_FALL);
                    } else if(strncmp(thelp, "rise", 4) == 0) {
                        currscope->setTriggerEdge(TE_RISE);
                    } else if(strncmp(thelp, "none", 4) == 0) {
                        currscope->setTriggerEdge(TE_NONE);
                    } else {
                        printf("no such parameter for trigger_edge in [scope]: %s\n", thelp);
                    }
                } else if(strncmp("xysource_x = ", confline, 13) == 0) {
                    currscope->setXYSourceX(getsparam(confline));
                } else if(strncmp("xysource_y = ", confline, 13) == 0) {
                    currscope->setXYSourceY(getsparam(confline));
                } else if(strncmp("disp_f_min = ", confline, 13) == 0) {
                    currscope->setDispFMin(getiparam(confline));
                } else if(strncmp("disp_f_max = ", confline, 13) == 0) {
                    currscope->setDispFMax(getiparam(confline));
                } else if(strncmp("infotrace = ", confline, 12) == 0) {
                    currscope->setInfoTraceName(getsparam(confline));
                } else if(strncmp("disp_log = ", confline, 11) == 0) {
                    if(strcmp(getsparam(confline), "true") == 0) {
                        currscope->setDispLog(true);
                    } else {
                        currscope->setDispLog(false);
                    }
                } else if(strncmp("disp_db = ", confline, 10) == 0) {
                    if(strcmp(getsparam(confline), "true") == 0) {
                        currscope->setDispDb(true);
                    } else {
                        currscope->setDispDb(false);
                    }
                } else if(strncmp("disp_db_ref = ", confline, 14) == 0) {
                    currscope->setDispDbRef(getfparam(confline));
                } else if(strncmp("v_div = ", confline, 8) == 0) {
                    currscope->setVDiv(getfparam(confline));
                } else if(strncmp("db_min = ", confline, 9) == 0) {
                    currscope->setDbMin(getfparam(confline));
                } else if(strncmp("db_max = ", confline, 9) == 0) {
                    currscope->setDbMax(getfparam(confline));
                } else if(strncmp("x = ", confline, 4) == 0) {
                    currscope->setXPos(getiparam(confline));
                } else if(strncmp("y = ", confline, 4) == 0) {
                    currscope->setYPos(getiparam(confline));
                } else {
                    printf("no such option in [scope]: %s\n", confline);
                }
            }
        }
    } while(fstat != -1);

    fclose(conffile);

    if(conflen > 0) {  // there's no mem allocated else
        free(confline);
    }

    // print out all config infos
    /*
    for(unsigned int i = 0; i < global->devnum; i++)
        global->devices[i]->dump(stdout);
    for(unsigned int i = 0; i < global->tracenum; i++)
        global->traces[i]->dump(stdout);
    for(unsigned int i = 0; i < global->scopenum; i++)
        global->scopes[i]->dump(stdout);
    */

    return 0;
}

int ControllerClass::getiparam(char *text) {
    unsigned int start;
    for(start = 0; (strncmp("= ", text + start, 2) != 0) && start < strlen(text); start++) {
        ;
    }
    start += 2;
    return atoi(text + start);
}

char *ControllerClass::getsparam(char *text) {
    int start;

    for(start = 0; strncmp("= ", text + start, 2) != 0; start++) {
        ;
    }
    start += 2;

    return text + start;
}

float ControllerClass::getfparam(char *text) {
    unsigned int start;
    for(start = 0; (strncmp("= ", text + start, 2) != 0) && start < strlen(text); start++) {
        ;
    }
    start += 2;
    return atof(text + start);
}

// writeconfig()
// writes configuration data to file
int ControllerClass::writeconfig(const std::string &filename) {
    unsigned int i;
    FILE *file;

    file = fopen(filename.c_str(), "w");
    if(file == nullptr) {
        fprintf(stderr, "writeconfig(): Could not open file %s (%s)\n", filename.c_str(), strerror(errno));
        return -1;
    }

    // print out devices
    for(i = 0; i < devnum; i++) {
        fprintf(file, "[dev]\n");
        devices[i]->dump(file);
        fprintf(file, "\n");
    }

    // print out traces
    for(i = 0; i < tracenum; i++) {
        fprintf(file, "[trace]\n");
        traces[i]->dump(file);
        fprintf(file, "\n");
    }

    // print out scope sections
    for(i = 0; i < scopenum; i++) {
        fprintf(file, "[scope]\n");
        scopes[i]->dump(file);
        fprintf(file, "\n");
    }
    return 0;
}

bool ControllerClass::hasTrace(TraceInterface *trace) {
    readLock lock(rwMutex);
    for(unsigned int i = 0; i < tracenum; i++) {
        if(traces[i] == trace) {
            return true;
        }
    }
    return false;
}

// void quit
void ControllerClass::quit() {
    {
        writeLock lock(rwMutex);
        // kill scope-redrawer thread
        //    pthread_cancel(scope_thread);
        //    pthread_join(scope_thread, NULL);

        // destroy traces, scopes and devices
        for(unsigned int i = 0; i < scopenum; i++) {
            delete scopes[i];
        }
        scopenum = 0;
    }
    // only set read lock while stopping traces and devices
    // they also need to read from us, so we cannot
    // set a write lock
    {
        readLock lock(rwMutex);
        for(unsigned int i = 0; i < devnum; i++) {
            devices[i]->stop();
        }
    }
    {
        writeLock lock(rwMutex);
        for(unsigned int i = 0; i < devnum; i++) {
            delete devices[i];
    }
        devnum = 0;

        for(unsigned int i = 0; i < tracenum; i++) {
            delete traces[i];
        }
        tracenum = 0;
    }
}

void ControllerClass::notifyTraceUpdate(const std::string& devicename){
    readLock lock(rwMutex);
    for(unsigned int i = 0; i < scopenum; i++) {
        scopes[i]->notifyTraceUpdate(devicename);
    }
}
