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

#ifndef RTDATA_H
#define RTDATA_H

#include <QMainWindow>

#include <QCloseEvent>
#include <vector>
#include <memory>

#include "importwizard_impl.h"
#include "RenderArea.h"

class QAction;
class QMenu;
class QCanvasView;
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class ImportWizard;
class DataItem;
class RawWindow;
class QTableWidget;
class QTextEdit;
class QGridLayout;
class QComboBox;
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
      void about();
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


      std::unique_ptr<Histo2dWidget> mapwindow;
      std::unique_ptr<DiagramWidget> diagramwindow;
      std::unique_ptr<ImportWizard> importwizard;
      int fVerbose { 0 };

      QTableWidget* tabWidget { nullptr };
      QTextEdit* statusBox { nullptr };
      QComboBox* plotColumnSelectComboBox { nullptr };
      QGroupBox* valueRadioGroup { nullptr };
      QRadioButton* valueRadioButton { nullptr };
      QRadioButton* refRadioButton { nullptr };
      QRadioButton* diffRadioButton { nullptr };
      QSpinBox* refIntervalSpinBox { nullptr };
      QGridLayout* layout { nullptr };
      QPushButton* button1 { nullptr };
      QPushButton* button2 { nullptr };
      QMenu *fileMenu { nullptr };
      QMenu *editMenu { nullptr };
      QMenu *helpMenu { nullptr };
      QToolBar *fileToolBar { nullptr };
      QToolBar *editToolBar { nullptr };
      QAction *newAct { nullptr };
      QAction *openAct { nullptr };
      QAction *saveAct { nullptr };
      QAction *saveAsAct { nullptr };
      QAction *exitAct { nullptr };
      QAction *cutAct { nullptr };
      QAction *copyAct { nullptr };
      QAction *pasteAct { nullptr };
      QAction *aboutAct { nullptr };
      QAction *aboutQtAct { nullptr };

private:
    Ui::RTData *ui;
};
#endif // RTDATA_H
