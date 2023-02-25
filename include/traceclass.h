#ifndef TRACECLASS_H
#define TRACECLASS_H

#include <string>
#include <mutex>

#include "dbuffer.h"
#include "deviceinterface.h"
#include "traceinterface.h"
#include "controllerclass.h"

class TraceClass : public TraceInterface
{
public:
  TraceClass(ControllerClass*);
  ~TraceClass();
  TraceClass(const TraceClass&) = delete; // block copying

  void dump(FILE *file) override;

  std::string getName() override;
  void setname(std::string newname) override;
  int setParentName(std::string newname) override;
  std::string getParentName() override;
  DeviceInterface *getParent() override;

  void recalc_stringrefs() override;

  int setBuffer(const dbuffer&) override;
  const dbuffer& getFBuf() override;
  const dbuffer& getTBuf() override;

  double getXShift() override;
  double getYShift() override;
  unsigned int getDevChannel() override;
  std::string getColor() override;
  unsigned int getBufferSize() override;
  bool getPerfectBuffer() override;
  double getDc() override;
  dbuffer::fftWinType getFftWinType() override;

  void setYZoom(double) override;
  void setXShift(double) override;
  void setYShift(double) override;
  void setDevChannel(unsigned int newchannel) override;
  void setColor(const std::string & newcolor) override;
  int setBufferSize(unsigned int) override;
  void setPerfectBuffer(bool enablePerfectBuffer) override;
  void setFftWinType(dbuffer::fftWinType) override;

private:
  ControllerClass* parentController;
  double xshift, yshift;        // shift of trace in div's

  unsigned int dev_channel;     // who am i in parent device ?

  std::string color;                 // color of trace
  double peak;                  // peak value
  double peakpos;               // peak position

  std::string name;                  // symbolic name of trace
  DeviceInterface *parent;          // pointer to parent
  std::string parentname;            // parents name
  bool running;                 // indicates if trace is in use
  std::shared_mutex rwMutex;

  dbuffer buf;
  dbuffer buf_fft;
  dbuffer buf_perfect;
    
  unsigned int buffersize;
  unsigned int bufferPos;
  bool PerfectBuffer;
};

#endif // TRACECLASS_H
