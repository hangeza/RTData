/***************************************************************************
 *   Copyright (C) 2007 by Hans-Georg Zaunick   *
 *   hg.zaunick@physik.tu-dresden.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "imageview.h"

#include <QtGui>
#include <QAction>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QGridLayout>
#include "imageview.h"

ImageView::ImageView(QWidget * parent)
{
//   setCentralWidget(new QWidget);
//   centralWidget()->setVisible(false);
   fImage = NULL;
   this->setMinimumWidth ( 200 );
   this->setMinimumHeight ( 200 );
       
   layout = new QGridLayout;
   layout->setRowStretch(0,1);
   createActions();
   createMenus();
   createToolBars();
   createStatusBar();

   layout->setContentsMargins(0,menuBar->height(),0,0);
   layout->setVerticalSpacing(0);
   setLayout(layout);
   
   fNormalize = true;
   fVerbose = 2;
}

ImageView::~ ImageView()
{
   if (fImage) delete fImage;
}

void ImageView::closeEvent(QCloseEvent * event)
{
}

void ImageView::paintEvent(QPaintEvent * event)
{
   if (!fImage) return;
   if (fVerbose>1) std::cout<<"*** ImageView::paintEvent(QPaintEvent *)"<<std::endl;
   QPainter painter(this);
//   painter.drawPixmap(QRect(0,0,this->width(),this->height()),fPixmap);
   QRect rect=QRect(0,menuBar->height(),this->width(),this->height()-menuBar->height()-statusBar->height());
   painter.drawImage(rect,*fImage);
}

void ImageView::createActions()
{
      newAct = new QAction(QIcon(":/filenew.xpm"), tr("&New"), this);
      newAct->setShortcut(tr("Ctrl+N"));
      newAct->setStatusTip(tr("Create a new file"));
      connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

      openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
      openAct->setShortcut(tr("Ctrl+O"));
      openAct->setStatusTip(tr("Open an existing file"));
      connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

      saveAct = new QAction(QIcon(":/filesave.xpm"), tr("&Save"), this);
      saveAct->setShortcut(tr("Ctrl+S"));
      saveAct->setStatusTip(tr("Save the document to disk"));
      connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

      saveAsAct = new QAction(tr("Save &As..."), this);
      saveAsAct->setStatusTip(tr("Save the document under a new name"));
      connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

      exitAct = new QAction(tr("E&xit"), this);
      exitAct->setShortcut(tr("Ctrl+Q"));
      exitAct->setStatusTip(tr("Exit the application"));
      connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

      cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
      cutAct->setShortcut(tr("Ctrl+X"));
      cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                              "clipboard"));

      copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
      copyAct->setShortcut(tr("Ctrl+C"));
      copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                              "clipboard"));
      
      pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
      pasteAct->setShortcut(tr("Ctrl+V"));
      pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));

      aboutAct = new QAction(tr("&About"), this);
      aboutAct->setStatusTip(tr("Show the application's About box"));
      connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

      aboutQtAct = new QAction(tr("About &Qt"), this);
      aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
      connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

      cutAct->setEnabled(false);
      copyAct->setEnabled(false);
}

void ImageView::createMenus()
{
   menuBar = new QMenuBar(this);
//   menuBar->setFrameStyle(QFrame::Panel | QFrame::Raised);
   //menuBar->setStyle(new QMotifStyle());
//   layout->addWidget(menuBar,0,0);
   //layout->setRowStretch(0,0);
   
   fileMenu = menuBar->addMenu(tr("&File"));
   fileMenu->addAction(newAct);
   fileMenu->addAction(openAct);
   fileMenu->addAction(saveAct);
   fileMenu->addAction(saveAsAct);
   fileMenu->addSeparator();
   fileMenu->addAction(exitAct);

   editMenu = menuBar->addMenu(tr("&Edit"));
   editMenu->addAction(cutAct);
   editMenu->addAction(copyAct);
   editMenu->addAction(pasteAct);

   menuBar->addSeparator();

   helpMenu = menuBar->addMenu(tr("&Help"));
   helpMenu->addAction(aboutAct);
   helpMenu->addAction(aboutQtAct);
}

void ImageView::createToolBars()
{
/*      fileToolBar = addToolBar(tr("File"));
      fileToolBar->addAction(newAct);
      fileToolBar->addAction(openAct);
      fileToolBar->addAction(saveAct);

      editToolBar = addToolBar(tr("Edit"));
      editToolBar->addAction(cutAct);
      editToolBar->addAction(copyAct);
      editToolBar->addAction(pasteAct);*/
}

void ImageView::createStatusBar()
{
   statusBar = new QStatusBar(this);
   //statusBar->setMaximumHeight(16);
   //statusBar->setMinimumHeight(16);
   layout->addWidget(statusBar,1,0);
   layout->setRowStretch(1,0);
   statusBar->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
   statusBar->showMessage(tr("Ready"));
   //statusBar->addWidget(new QFrame(this));
   //statusBar->addWidget(new QProgressBar);
   statusBar->setSizeGripEnabled(true);
}

void ImageView::draw()
{
   this->setUpdatesEnabled(false);
   double Offset,Range;
   
   double max=-1e+38;
   double min=1e+38;
   for (int i=0; i < fArray.rows(); i++)
   {
      for (int j=0; j < fArray.cols(); j++)
      {
         double val=fArray[i][j];
         if (val<min) min=val;
         if (val>max) max=val;
      }
   }
   if (fVerbose) std::cout<<"min="<<min<<" max="<<max<<std::endl;
   if (fVerbose) std::cout<<"cols="<<fArray.cols()<<" rows="<<fArray.rows()<<std::endl;
   if (fImage) delete fImage;
   fImage = new QImage(fArray.cols(),fArray.rows(),QImage::Format_RGB32);
   if (fVerbose>1) std::cout<<"*** ImageView::draw() "<<std::endl;

   fImage->fill(Qt::black);
   for (int i=0; i < fArray.rows(); i++)
   {
      for (int j=0; j < fArray.cols(); j++)
      {
         double val=fArray[i][j];
         //short col = ColormapSize*((val-fOffset)/fRange);
         double col = (val-min)/(max-min);
         fImage->setPixel(j,i,getFalseColor(col).rgb());
      }
   }
   
   this->setUpdatesEnabled(true);
   update();
}

QColor ImageView::getFalseColor(double val)
{
   static const int maxintensity=1791;
   int b,g,r;
   int v=(int)((double)maxintensity*val);
   if (v<0) v=0;
//   if (v>maxintensity) v=maxintensity;
      
   if (v<=255){
      g=0;
      r=0;
      b=v;
   }
   else if (v>255 && v<=511){
      b=255;
      r=0;
      g=v-256;
   }
   else if (v>511 && v<=767){
      r=0;
      g=255;
      b=767-v;
   }
   else if (v>767 && v<=1023){
      g=255;
      b=0;
      r=v-768;
   }
   else if (v>1023 && v<=1279){
      g=1279-v;
      b=0;
      r=255;
   }
   else if (v>1279 && v<=1535){
      r=255;
      g=0;
      b=v-1280;
   }
   else if (v>1535 && v<=1791){
      r=255;
      g=v-1536;
      b=255;
   }
   else { r=g=b=255; }
      
   return QColor(r,g,b);
}

void ImageView::setData(const hgz::Array< double, 2 > & data)
{
   fArray=data;
   draw();
}

void ImageView::about()
{
      QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
                  "write modern GUI applications using Qt, with a menu bar, "
                  "toolbars, and a status bar."));
}
