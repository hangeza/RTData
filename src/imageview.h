/***************************************************************************
 * Copyright (C) 2010-2022  HG Zaunick, A. Großmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <vector>
#include <map>
#include "Array.h"

class QAction;
class QMenu;
class QCanvasView;
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QMenuBar;
//class ImportWizard;
//class Histo2dWidget;
//class DiagramWidget;
//class DataItem;
//class RawWindow;
//class QTableWidget;
//class QTextEdit;
class QGridLayout;
//class QComboBox;
class QCloseEvent;
class QPaintEvent;
class QImage;
class QToolBar;
class QStatusBar;

class ImageView : public QWidget
{
Q_OBJECT
   public:
      ImageView(QWidget* parent = NULL);
      ~ImageView();
   protected:
      void closeEvent(QCloseEvent *event);
      void paintEvent(QPaintEvent * event);
   public slots:
      void about();
      void setData(const hgz::Array<double,2>& data);
/*      void setData(const hgz::Array<int,2>& data);
      void setData(const std::vector<std::vector<double> >& data);
      void setData(const QVector<QVector<double> >& data);
      void setData(const std::vector<std::vector<int> >& data);
      void setData(const QVector<QVector<int> >& data);
      void setData(const std::map<int,std::map<int,double> >& data);
      void setData(const QMap<int,QMap<int,double> >& data);
      void setData(const std::map<int,std::map<int,int> >& data);
      void setData(const QMap<int,QMap<int,int> >& data);
      void setData(int xsize, int ysize, double** data);
      void setData(int xsize, int ysize, int** data);
      void setData(const QPixmap& data);
      void setData(const QImage& data);
      void setData(const QPicture& data);*/
   private slots:
      void draw();
   private:
      void createActions();
      void createMenus();
      void createToolBars();
      void createStatusBar();
      QColor getFalseColor(double val);
      
      int fVerbose;
      QImage* fImage;
      hgz::Array<double,2> fArray;
      bool fNormalize;
      
      QGridLayout *layout;
      QMenuBar *menuBar;
      QMenu *fileMenu;
      QMenu *editMenu;
      QMenu *helpMenu;
      QStatusBar* statusBar;
      QToolBar *fileToolBar;
      QToolBar *editToolBar;
      QAction *newAct;
      QAction *openAct;
      QAction *saveAct;
      QAction *saveAsAct;
      QAction *exitAct;
      QAction *cutAct;
      QAction *copyAct;
      QAction *pasteAct;
      QAction *aboutAct;
      QAction *aboutQtAct;
      
};

#endif // IMAGEVIEWWIDGET_H
