#ifndef SCOPEVIEW_H
#define SCOPEVIEW_H

#include <mutex>

#include <QWidget>
#include <QFont>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPixmap>

#include "scopeinterface.h"

class ScopeView : public QWidget, public ScopeObserver
{
  Q_OBJECT
public:  
  ScopeView(ScopeInterface* _modelScope, QWidget *parent = nullptr, const char *name = nullptr);
  ~ScopeView() override;
  ScopeView(const ScopeView&) = delete;

  void triggerRedrawScope();
  void redrawScope();
  unsigned int getMouseX() const;
  unsigned int getMouseY() const;
  void setMouseX(unsigned int);
  void setMouseY(unsigned int);
  int getScreenPos(double);

  bool update(int value) override;

  std::string getGridCol();
  std::string getBCol();
  std::string getMarkCol();
  std::string getTextCol();

  void setGridCol(const std::string&);
  void setBCol(const std::string&);
  void setMarkCol(const std::string&);
  void setTextCol(const std::string&);

private:
  ScopeInterface* modelScope;
  
  void mousePressEvent(QMouseEvent *e) override;
  void resizeEvent(QResizeEvent *e) override;
  void paintEvent(QPaintEvent *e) override;
  bool event(QEvent *e) override; 

  inline void drawmark(int x, int y);
  inline void draw_triglvl(int trace);
  inline void grid();
  inline void grid_fft();
  inline double calcx(int buffpos, int trace);
  QPainter *painter;
  QPixmap *pm;
  QFont font;
  std::string bcol, gridcol, markcol, textcol;

  std::mutex bltMutex;

  // position of mouse pointer
  unsigned int mouse_x;
  unsigned int mouse_y;
};

#endif // SCOPEVIEW_H
