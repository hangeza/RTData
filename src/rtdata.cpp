#include "rtdata.h"
#include "ui_rtdata.h"
#include <QtGui>

#include <QPainter>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include "RenderArea.h"
#include "importwizard_impl.h"
#include "func.h"

/* RTData::RTData(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RTData)
{
    ui->setupUi(this);
}

RTData::~RTData()
{
    delete ui;
} */

using namespace std;

RTData::RTData(QWidget *parent)
{
//   datawidget = new RTDataWidget;
//   setCentralWidget(datawidget);

//   importwizard=new ImportWizard(this);

   setCentralWidget(new QWidget);

   this->setMinimumWidth ( 300 );
   this->setMinimumHeight ( 200 );

   createActions();
   createMenus();
   createToolBars();
   createStatusBar();

   mapwindow=new Histo2dWidget;
   diagramwindow=new DiagramWidget;
   fImportwizard=NULL;
//   diagramwindow->setBgColor(Qt::white);
   //diagramwindow->show();

   statusBox = new QTextEdit(this);
   statusBox->setMaximumHeight(100);
   statusBox->setReadOnly(true);
   tabWidget = new QTableWidget(this);
   QGroupBox* groupbox1 = new QGroupBox(this);
   QGroupBox* groupbox2 = new QGroupBox(this);
   valueRadioGroup = new QGroupBox(this);
   plotColumnSelectComboBox = new QComboBox(groupbox1);
   plotColumnSelectComboBox->addItem("Date/Time");
   plotColumnSelectComboBox->addItem("X");
   plotColumnSelectComboBox->addItem("Y");
   valueRadioButton = new QRadioButton("values", valueRadioGroup);
   refRadioButton = new QRadioButton("ref", valueRadioGroup);
   diffRadioButton = new QRadioButton("difference", valueRadioGroup);
   diffRadioButton->setChecked(true);
   valueRadioGroup->setEnabled(true);

   refIntervalSpinBox = new QSpinBox(valueRadioGroup);
   refIntervalSpinBox->setMinimum(2);
   refIntervalSpinBox->setMaximum(10000);
   refIntervalSpinBox->setValue(3);

   QLabel* label1 = new QLabel("plot Values vs.",groupbox1);
   QLabel* label2 = new QLabel("plot 2D Map",groupbox2);
   button1 = new QPushButton("plot", groupbox1);
   button2 = new QPushButton("plot", groupbox2);
   button1->setEnabled(false);
   button2->setEnabled(false);
   connect(button1,SIGNAL(clicked()),this,SLOT(plot1d()));
   connect(button2,SIGNAL(clicked()),this,SLOT(plot2d()));

   // 1-d plot groupbox
   QGridLayout* layout1 = new QGridLayout;
   layout1->addWidget(label1, 0, 0);
   layout1->addWidget(plotColumnSelectComboBox, 0, 1);
   layout1->addWidget(button1, 1, 1);
   groupbox1->setLayout(layout1);

   // 2-d histo groupbox
   QGridLayout* layout2 = new QGridLayout;
   layout2->addWidget(label2, 0, 0);
   layout2->addWidget(button2, 0, 1);
   groupbox2->setLayout(layout2);

   // value select groupbox
   QGridLayout* layout3 = new QGridLayout;
   layout3->addWidget(valueRadioButton, 0, 0);
   layout3->addWidget(refRadioButton, 0, 1);
   layout3->addWidget(diffRadioButton, 0, 2);
   layout3->addWidget(refIntervalSpinBox, 1, 1);
   valueRadioGroup->setLayout(layout3);

   QGridLayout* layout = new QGridLayout;
   layout->setColumnStretch(0, 1);
   layout->setRowStretch(0, 1);
   layout->addWidget(tabWidget, 0, 0);
   layout->addWidget(groupbox1, 1, 0);
   layout->addWidget(groupbox2, 2, 0);
   layout->addWidget(valueRadioGroup, 3, 0);
//   layout->addWidget(label1, 1, 0);
//   layout->addWidget(plotColumnSelectComboBox, 1, 0);
   layout->addWidget(statusBox, 5, 0);
   centralWidget()->setLayout(layout);

/*
   const double xbins=300;
   const double ybins=200;
   imview = new ImageView;
   hgz::Function<double,double> gauss = hgz::Function<double,double>::Gauss(0.,5.);
   hgz::Array<double, 2> dataarray(hgz::extends(xbins,ybins));
   for (int i=0; i<ybins; i++)
      for (int j=0; j<xbins; j++)
      {
         double x=(double)j-(double)xbins/2.;
         double y=(double)i-(double)ybins/2.;
         dataarray[i][j]=gauss(sqrt(x * x + y * y));
      }


   imview->setData(dataarray);
   imview->show();
*/
   fVerbose=0;

// signals/slots mechanism in action
//    connect( pushButton, SIGNAL( clicked() ), this, SLOT( draw() ) );
//    connect( groupBox, SIGNAL( paint() ), this, SLOT( draw() ));
}



RTData::~ RTData()
{
   clearData();
   delete mapwindow;
   delete diagramwindow;
//   delete imview;
}

void RTData::draw()
{
    //update();
/*
    QPainter paint( this->groupBox );
    paint.setPen( Qt::blue );
    paint.drawLine(10,10,120,70 );
*/
}


void RTData::clear()
{
}

void RTData::about()
{
      QMessageBox::about(this, tr("RT-Data Viewing and Preprocessing Application"),
            tr("Application for preprocessing and viewing raw data obtained with "
                  "the Radiotelescope at the public Observatory in Radebeul (Dresden)"
                  ));
}

void RTData::createActions()
{
      newAct = new QAction(tr("&New"), this);
      newAct->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
      newAct->setShortcut(tr("Ctrl+N"));
      newAct->setStatusTip(tr("Create a new file"));
      connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

      openAct = new QAction(tr("&Open..."), this);
      openAct->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
      openAct->setShortcut(tr("Ctrl+O"));
      openAct->setStatusTip(tr("Open an existing file"));
      connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

      saveAct = new QAction(tr("&Save"), this);
      saveAct->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
      saveAct->setShortcut(tr("Ctrl+S"));
      saveAct->setStatusTip(tr("Save the document to disk"));
      connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

      saveAsAct = new QAction(tr("Save &As..."), this);
      saveAsAct->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
      saveAsAct->setStatusTip(tr("Save the document under a new name"));
      connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

      exitAct = new QAction(tr("E&xit"), this);
      exitAct->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
      exitAct->setShortcut(tr("Ctrl+Q"));
      exitAct->setStatusTip(tr("Exit the application"));
      connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

      cutAct = new QAction(tr("Cu&t"), this);
      cutAct->setShortcut(tr("Ctrl+X"));
      cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                              "clipboard"));

      copyAct = new QAction(tr("&Copy"), this);
      copyAct->setShortcut(tr("Ctrl+C"));
      copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                              "clipboard"));

      pasteAct = new QAction(tr("&Paste"), this);
      pasteAct->setShortcut(tr("Ctrl+V"));
      pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));

      aboutAct = new QAction(tr("&About"), this);
      aboutAct->setStatusTip(tr("Show the application's About box"));
      aboutAct->setIcon(style()->standardIcon(QStyle::SP_TitleBarContextHelpButton));
      connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

      aboutQtAct = new QAction(tr("About &Qt"), this);
      aboutQtAct->setIcon(style()->standardIcon(QStyle::SP_TitleBarMenuButton));
      aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
      connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

      cutAct->setEnabled(false);
      copyAct->setEnabled(false);
}

void RTData::createMenus()
{
      fileMenu = menuBar()->addMenu(tr("&File"));
      fileMenu->addAction(newAct);
      fileMenu->addAction(openAct);
      fileMenu->addAction(saveAct);
      fileMenu->addAction(saveAsAct);
      fileMenu->addSeparator();
      fileMenu->addAction(exitAct);

      editMenu = menuBar()->addMenu(tr("&Edit"));
      editMenu->addAction(cutAct);
      editMenu->addAction(copyAct);
      editMenu->addAction(pasteAct);

      menuBar()->addSeparator();

      helpMenu = menuBar()->addMenu(tr("&Help"));
      helpMenu->addAction(aboutAct);
      helpMenu->addAction(aboutQtAct);
}

void RTData::createToolBars()
{
      fileToolBar = addToolBar(tr("File"));
//      fileToolBar->addAction(newAct);
      fileToolBar->addAction(openAct);
//      fileToolBar->addAction(saveAct);

/*      editToolBar = addToolBar(tr("Edit"));
      editToolBar->addAction(cutAct);
      editToolBar->addAction(copyAct);
      editToolBar->addAction(pasteAct); */
}

void RTData::createStatusBar()
{
      statusBar()->showMessage(tr("Ready"));
}

void RTData::open()
{
   if (fImportwizard) { delete fImportwizard; fImportwizard=NULL; }
   fImportwizard = new ImportWizard(this);
   if (fImportwizard->exec()) {
      connect(fImportwizard, SIGNAL(dataChanged()), this, SLOT(replot()));
      statusBox->append("opening file "+fImportwizard->filename());
      if (fVerbose) std::cout<<"dialog ok"<<std::endl;
      clearData();
      //fDataVector=fImportwizard->dataVector();
      statusBox->append("read "+QString::number(fImportwizard->dataVector().size())+" entries");
      if (fVerbose) std::cout<<"dataVector.size()="<<fImportwizard->dataVector().size()<<std::endl;
      if (!fImportwizard->dataVector().size()) return;
      if (fImportwizard->dataVector()[0]->z() && (fImportwizard->dataVector()[0]->dateTime() || fImportwizard->dataVector()[0]->x() || fImportwizard->dataVector()[0]->y())) button1->setEnabled(true);
      if (fImportwizard->dataVector()[0]->z() && fImportwizard->dataVector()[0]->x() && fImportwizard->dataVector()[0]->y()) button2->setEnabled(true);
      // populate tab widget
      tabWidget->setColumnCount(4);
      tabWidget->setRowCount(fImportwizard->dataVector().size());

      for (int i=0; i<fImportwizard->dataVector().size(); i++) {
         if (fImportwizard->dataVector()[i]->dateTime()) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(fImportwizard->dataVector()[i]->dateTime()->toSecsSinceEpoch()));
            tabWidget->setItem(i, 0, item);
         }
         else {
            QTableWidgetItem *item = new QTableWidgetItem("N/A");
            tabWidget->setItem(i, 0, item);
         }
         if (fImportwizard->dataVector()[i]->x()) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(*(fImportwizard->dataVector()[i]->x())));
            tabWidget->setItem(i, 1, item);
         }
         if (fImportwizard->dataVector()[i]->y()) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(*(fImportwizard->dataVector()[i]->y())));
            tabWidget->setItem(i, 2, item);
         }
         if (fImportwizard->dataVector()[i]->z()) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(*(fImportwizard->dataVector()[i]->z())));
            tabWidget->setItem(i, 3, item);
         }
      }
      if (fImportwizard->refVector().size()) valueRadioGroup->setEnabled(true);
      else valueRadioGroup->setEnabled(false);
   }
   else if (fVerbose) std::cout<<"dialog canceled"<<std::endl;
   //importwizard->show();

/*    QString s = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                "/home/zhg/c/local/radio/daten");*/
//    datawidget->loadData(s);
}

void RTData::closeEvent( QCloseEvent * event )
{
   if (mapwindow) delete mapwindow;
   if (diagramwindow) delete diagramwindow;
//   imview->close();
   event->accept();
}

void RTData::clearData()
{
/*   while (fDataVector.size()) {
      if (fDataVector.last()!=NULL) {
         fDataVector.last()->release();
         delete fDataVector.last();
      }
      fDataVector.pop_back();
   }*/
}

void RTData::replot()
{
   if (fVerbose>1) std::cout<<"*** replot()"<<std::endl;
   if (mapwindow->isVisible()) plot2d();
   if (diagramwindow->isVisible()) plot1d();
}

double RTData::getInterpolRef(const QVector< DataItem* >& refvector, const QDateTime& t)
{
   int i=0;
   int n=refvector.size();
   if (n==1) return *(refvector[0]->z());
   while (i<n && *(refvector[i]->dateTime())<t) i++;
   i--;
   if (i<0) i=0;
   if (i>n-2) i=n-2;
   double t1=refvector[i]->dateTime()->toTime_t();
   double t2=refvector[i+1]->dateTime()->toTime_t();
   double v1=*(refvector[i]->z());
   double v2=*(refvector[i+1]->z());
   double u=(t.toTime_t()-t1)/(t2-t1);
   double v=v1+u*(v2-v1);
   //std::cout<<"t1="<<t1<<" t2="<<t2<<" v1="<<v1<<" v2="<<v2<<" t="<<t.toTime_t()<<" v="<<v<<" i="<<i<<std::endl;
   return v;
}


void RTData::calcLinRegression(const vector<double>& xvals, const vector<double>& yvals, double* slope, double* offset)
{
   int n=xvals.size();
   if (yvals.size()!=n) return;
   double sumx=0.;
   double sumy=0.;
   double sumxx=0.;
   double sumxy=0.;
   for (int i=0; i<n; i++) {
      sumx+=xvals[i];
      sumy+=yvals[i];
      sumxx+=xvals[i]*xvals[i];
      sumxy+=xvals[i]*yvals[i];
   }
   if (fVerbose>2)
   {
      cout<<"n="<<n<<endl;
      cout<<"sumx="<<sumx<<" sumy="<<sumy<<" sumxx="<<sumxx<<" sumxy="<<sumxy<<endl;
   }
   *slope=(sumxy*n-sumx*sumy)/(sumxx*n-sumx*sumx);
   *offset=sumy/n - *slope * sumx/n;
}


double RTData::getlinRegressRef(const QVector< DataItem* >& refvector, const QDateTime& t, int nrRefVals)
{
   int start=0;
   int n=refvector.size();
   int nrvals=nrRefVals;
   if (n==1) return *(refvector[0]->z());
   if (n<=nrvals) {
      nrvals=n;
      start=0;
   }
   else {
      while (start<n && *(refvector[start]->dateTime())<t) start++;
      //start--;
      start-=nrRefVals/2;
      if (start<0) {
         start=0;
      }
      if (start>n-nrvals) start=n-nrvals;
   }
   vector<double> x,y;
   double t0=refvector[start]->dateTime()->toTime_t();
   for (int i=0; i<nrvals; i++) {
      x.push_back(refvector[start+i]->dateTime()->toTime_t()-t0);
      y.push_back(*(refvector[start+i]->z()));
   }

   double slope, offs;
   calcLinRegression(x, y, &slope, &offs);

/*
   double t1=refvector[i]->dateTime()->toTime_t();
   double t2=refvector[i+1]->dateTime()->toTime_t();
   double v1=*(refvector[i]->z());
   double v2=*(refvector[i+1]->z());
   double u=(t.toTime_t()-t1)/(t2-t1);
   double v=v1+u*(v2-v1);
   //std::cout<<"t1="<<t1<<" t2="<<t2<<" v1="<<v1<<" v2="<<v2<<" t="<<t.toTime_t()<<" v="<<v<<" i="<<i<<std::endl;
*/
   double v=(t.toTime_t()-t0)*slope+offs;
   if (fVerbose>2)
   {
      cout<<"***lin. regression:"<<endl;
      cout<<" nrVals="<<nrvals<<endl;
      cout<<" slope="<<slope<<" , offset="<<offs<<endl;
      cout<<" val="<<v<<endl;
   }
   return v;
}


void RTData::plot1d()
{
   if (!fImportwizard->dataVector().size()) return;
   if (fVerbose>1) std::cout<<"*** plot1d()"<<std::endl;
   QVector<QPointF> data;
   double xmin=1e+38;
   double xmax=-1e+38;
   double ymin=1e+38;
   double ymax=-1e+38;
   if (fVerbose>2) std::cout<<"fImportwizard->dataVector().size()="<<fImportwizard->dataVector().size()<<std::endl;
   int N=0;
   if (fImportwizard->refVector().size() && refRadioButton->isChecked()) N=fImportwizard->refVector().size();
   else N=fImportwizard->dataVector().size();

   data.reserve(N);
   for (int i=0; i<N; i++) {
      QPointF point;
      if (fVerbose>2) std::cout<<"fImportwizard->dataVector()[i]="<<hex<<(fImportwizard->dataVector()[i])<<std::endl;
      if (fVerbose>2) std::cout<<"fImportwizard->dataVector()[i]->z()="<<hex<<(fImportwizard->dataVector()[i]->z())<<std::endl;
      if (fVerbose>3) std::cout<<"*fImportwizard->dataVector()[i]->z()="<<*(fImportwizard->dataVector()[i]->z())<<std::endl;
      double z;
      if (!fImportwizard->refVector().size() || valueRadioButton->isChecked())
      {
         z=*(fImportwizard->dataVector()[i]->z());
      } else
      if (fImportwizard->refVector().size() && refRadioButton->isChecked())
      {
         z=*(fImportwizard->refVector()[i]->z());
      } else
      if (fImportwizard->refVector().size() && diffRadioButton->isChecked() && fImportwizard->dataVector()[i]->dateTime())
      {
         z=*(fImportwizard->dataVector()[i]->z());
//         z-=getInterpolRef(fImportwizard->refVector(),*(fImportwizard->dataVector()[i]->dateTime()));
         z-=getlinRegressRef(fImportwizard->refVector(),*(fImportwizard->dataVector()[i]->dateTime()),refIntervalSpinBox->value());
      }

      point.setY(z);
      switch (plotColumnSelectComboBox->currentIndex())
      {
         case 0:  point.setX(i);
                  break;
         case 1:  if (fImportwizard->refVector().size() && refRadioButton->isChecked())
                    point.setX(*(fImportwizard->refVector()[i]->x()));
                  else point.setX(*(fImportwizard->dataVector()[i]->x()));
                  break;
         case 2:  if (fImportwizard->refVector().size() && refRadioButton->isChecked())
                    point.setX(*(fImportwizard->refVector()[i]->y()));
                  else point.setX(*(fImportwizard->dataVector()[i]->y()));
                  break;
      }
      if (fVerbose) {
         if (point.x()<xmin) xmin=point.x();
         if (point.x()>xmax) xmax=point.x();
         if (point.y()<ymin) ymin=point.y();
         if (point.y()>ymax) ymax=point.y();
      }

      data.push_back(point);
   }
   if (fVerbose) std::cout<<"xmin="<<xmin<<" xmax="<<xmax<<" ymin="<<ymin<<" ymax="<<ymax<<std::endl;
   // fill and open 1d-diagram window
//   diagramwindow->setBounds(QRectF(xmin,ymin,xmax-xmin,ymax-ymin));
   diagramwindow->setData(data);
   hgz::domain<double> x;
   hgz::Function<double,double> func=x;
   func=5.*func+13700.;
   //diagramwindow->setFunction(func);
   diagramwindow->show();
}

void RTData::plot2d()
{
   if (!fImportwizard->dataVector().size()) return;
   if (fVerbose>1) std::cout<<"*** plot2d()"<<std::endl;
   QVector<QPoint3F> data;
   if (fImportwizard->refVector().size() && refRadioButton->isChecked())
   {
      data.reserve(fImportwizard->refVector().size());
      for (int i=0; i<fImportwizard->refVector().size(); i++) {
         if (fImportwizard->refVector()[i]->x() && fImportwizard->refVector()[i]->y() && fImportwizard->refVector()[i]->z()) {
            double z=*(fImportwizard->refVector()[i]->z());
            data.push_back(QPoint3F(*(fImportwizard->refVector()[i]->x()), *(fImportwizard->refVector()[i]->y()), z));
         }
      }
   } else {
      data.reserve(fImportwizard->dataVector().size());
      for (int i=0; i<fImportwizard->dataVector().size(); i++) {
	// file>>dp.Ra>>dp.Dec>>dp.Value>>dummy>>dummy>>dummy>>dummy;
         if (fImportwizard->dataVector()[i]->x() && fImportwizard->dataVector()[i]->y() && fImportwizard->dataVector()[i]->z()) {
            double z;
	    if (!fImportwizard->refVector().size() || valueRadioButton->isChecked())
	    {
	       z=*(fImportwizard->dataVector()[i]->z());
	    } else
        if (fImportwizard->refVector().size() && diffRadioButton->isChecked() && fImportwizard->dataVector()[i]->dateTime())
        {
	       z=*(fImportwizard->dataVector()[i]->z());
//            z-=getInterpolRef(fImportwizard->refVector(),*(fImportwizard->dataVector()[i]->dateTime()));
	       z-=getlinRegressRef(fImportwizard->refVector(),*(fImportwizard->dataVector()[i]->dateTime()),refIntervalSpinBox->value());
	       if (fVerbose>2)
	       {
		  cout<<"x="<<*(fImportwizard->dataVector()[i]->x())<<", y="<<*(fImportwizard->dataVector()[i]->y())<<", val-ref="<<z<<endl;
	       }
	    }

	    data.push_back(QPoint3F(*(fImportwizard->dataVector()[i]->x()), *(fImportwizard->dataVector()[i]->y()), z));
	 }
      }
   }
   // fill and open 2d-map window
   mapwindow->setData(data);
   mapwindow->show();
}
