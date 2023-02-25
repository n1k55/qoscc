#ifndef SCOPECLASS_H
#define SCOPECLASS_H

#include <cstdio>
#include <string>
#include <mutex>
#include <vector>
#include <memory>


#include "traceinterface.h"
#include "scopeinterface.h"
#include "controllerclass.h"

class ScopeClass : public ScopeInterface 
{
public:
  ScopeClass(ControllerClass*);
  ~ScopeClass();  
  ScopeClass(const ScopeClass&) = delete;

  std::string getName() override;
  int setName(const std::string &) override;

  double getfreq(int);
  double gettime(int);
  double getValue(int y);


  int addTrace(TraceInterface *) override;

  void dump(FILE *file) override;

  unsigned int getTraceNum() const;

  void setHeight(int) override;
  void setWidth(int) override;
  void setXPos(int) override;
  void setYPos(int) override;

  TraceInterface *getTrace(const char *) override;
  TraceInterface *getTrace(const std::string &) override;
  TraceInterface *getTrace(unsigned int) override;

  int removeTrace(TraceInterface *) override;
  int removeTrace(const std::string &) override;

  double calcx(int, int); 

  int setTriggerSource(std::string) override;
  void recalc_stringrefs() override;
  int setXYSourceX(std::string) override;
  int setXYSourceY(std::string) override;
  std::string getTriggerName() override;
  std::string getXYSourceXName() override;
  std::string getXYSourceYName() override;
  void getTraceList(stringlist *) override;
  bool triggerLocked() override;

  int getMode() override;
  double getSweep() override;
  unsigned int getHDivs() override;
  unsigned int getVDivs() override;
  int getTriggerEdge() override;
  double getTriggerLevel() override;

  bool getHold() override;
  bool getDispLog() override;
  unsigned int getDispFMin() override;
  unsigned int getDispFMax() override;
  std::string getInfoTraceName() override;
  bool getDispDb() override;
  double getDispDbRef() override;
  double getVDiv() override {
    return vDiv;
  }
  double getDbMin() override {
    return dbMin;
  }
  double getDbMax() override {
    return dbMax;
  }
  /* FIXME needs qfont */
  std::string getFont() override{
   // return font.toString();
    return font;
  }

  void setMode(int) override;
  void setSweep(double) override;
  void setHDivs(unsigned int) override;
  void setVDivs(unsigned int) override;
  void setTriggerEdge(int) override;
  void setTriggerLevel(double) override;
  void setHold(bool) override;
  void setDispLog(bool) override;
  void setDispFMin(unsigned int) override;
  void setDispFMax(unsigned int) override;
  void setInfoTraceName(const std::string&) override;
  void setDispDb(bool) override;
  void setDispDbRef(double) override;
  void setVDiv(double) override;
  void setDbMin(double) override;
  void setDbMax(double) override;
  void setFont(const std::string&) override;

  int storeValues(FILE *, double, double, int) override;
  double getMaxDspRate() override;
    
  void notifyTraceUpdate(const std::string&) override;  
  void registerObserver(const ScopeObserver* newObserver) override;
  void removeObserver(const ScopeObserver* delObserver) override;
  void notifyObserver() override;  
private:
  std::vector<std::shared_ptr<ScopeObserver>> _scopeObserver;
  ControllerClass* parentController;
  int maxy(int a, int b, int trace);
  void getpeak(int index);
  void initVars(); 

  int do_trigger(unsigned int);   

  int getFftPeakNear(unsigned int trace, int x, int y, double *freq, double *value);
  int getYtPeakNear(unsigned int trace, int x, int y, double *time, double *value);  
  unsigned int tracenum;
//    double **fbuf;
  dbuffer *buf;
//    unsigned int *buflen;
  std::string name;

  TraceInterface **traces;           // list of all classes for this scope

  TraceInterface *trigger_source;
  std::string trigger_source_name;
  TraceInterface *xysource_x;       // the sources traces (indices) for the xy-mode
  std::string xysource_x_name;       // and the corresponding names...
  TraceInterface *xysource_y;
  std::string xysource_y_name;

  int trigger_pos;
    
  unsigned int hdivs, vdivs;     // horizontal and vertial divisions  

  double sweep;                  // sweep (time to pass one div, in secs)  

  int mode;

  int trigger_edge;              // settings for trigger
  double trigger_level;
  bool trigger_locked;           // true if triggering was sucessful....

  bool hold;

  //std::recursive_mutex 
  std::shared_mutex mutex;

  bool dispLog;
  bool dispDb;
  double dispDbRef; 

  unsigned int dispFMin, dispFMax;

  std::string infoTraceName;

  double infoPeakFT;
  double infoPeakVal;
  double infoDC;

  double vDiv;
  double dbMin, dbMax;

  std::string font;
};
//todo better as enum
// scope display modes:
#define M_NONE      0x00
#define M_YT        0x01
#define M_XY        0x02
#define M_FFT       0x03

// trigger edge selection:
#define TE_NONE   0x00 /* = disabled */
#define TE_RISE   0x01
#define TE_FALL   0x02
#endif // SCOPECLASS_H
