#ifndef RTDATA_H
#define RTDATA_H

#include <QMainWindow>

#include <QCloseEvent>
#include <vector>
class QAction;
class QMenu;
class QCanvasView;
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class ImportWizard;
class Histo2dWidget;
class DiagramWidget;
class DataItem;
class RawWindow;
class QTableWidget;
class QTextEdit;
class QGridLayout;
class QComboBox;
class ImageView;
class QPushButton;
class ImportWizard;
class QRadioButton;
class QGroupBox;
class QSpinBox;

QT_BEGIN_NAMESPACE
namespace Ui { class RTData; }
QT_END_NAMESPACE

class RTData : public QMainWindow
{
    Q_OBJECT

public:
    RTData(QWidget *parent = nullptr);
    ~RTData();

   protected:
      void closeEvent(QCloseEvent *event);

   public slots:
      void clear();
      void about();
      void draw();
      void plot1d();
      void plot2d();
      void replot();

   private slots:
      void open();

   private:
      void createActions();
      void createMenus();
      void createToolBars();
      void createStatusBar();

      void clearData();

      double getInterpolRef(const QVector< DataItem* >& refvector, const QDateTime& t);
      double getlinRegressRef(const QVector< DataItem* >& refvector, const QDateTime& t, int nrRefVals);
      void calcLinRegression(const std::vector<double>& xvals, const std::vector<double>& yvals, double* slope, double* offset);


      Histo2dWidget* mapwindow;
      DiagramWidget* diagramwindow;
      ImportWizard* fImportwizard;
      //QVector<DataItem*> fDataVector;
      int fVerbose;
      //ImageView* imview;

      QTableWidget* tabWidget;
      QTextEdit* statusBox;
      QComboBox* plotColumnSelectComboBox;
      QGroupBox* valueRadioGroup;
      QRadioButton* valueRadioButton;
      QRadioButton* refRadioButton;
      QRadioButton* diffRadioButton;
      QSpinBox* refIntervalSpinBox;
      QGridLayout* layout;
      QPushButton* button1;
      QPushButton* button2;
      QMenu *fileMenu;
      QMenu *editMenu;
      QMenu *helpMenu;
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

private:
    Ui::RTData *ui;
};
#endif // RTDATA_H
