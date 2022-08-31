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

#ifndef IMPORTWIZARD_IMPL_H
#define IMPORTWIZARD_IMPL_H

#include <iostream>
#include <fstream>
#include "ui_importwizard.h"
#include <QDateTime>
#include <QMap>

class QString;
class QDateTime;
class QTimer;
//class RTDataWidget;

class DataItem {
   public:
      DataItem()
         : fDateTime(NULL), fX(NULL), fY(NULL), fZ(NULL) {}
      DataItem(QDateTime* datetime, double* x, double* y, double* z)
         : fDateTime(datetime), fX(x), fY(y), fZ(z) {}
      /** destructor
       * the memory is released automatically once the object is destroyed. To release the occupied memory manually use the release method
      */
      ~DataItem()
      {
         release();
      }

      void release() {
         if (fDateTime) delete fDateTime;
         if (fX) delete fX;
         if (fY) delete fY;
         if (fZ) delete fZ;
         fDateTime=NULL;
         fX=fY=fZ=NULL;
      }

      void setDateTime(QDateTime* datetime) { fDateTime=datetime; }
      void setX(double* x) { fX=x; }
      void setY(double* y) { fY=y; }
      void setZ(double* z) { fZ=z; }

      const QDateTime* dateTime() const { return fDateTime; }
      const double* x() const { return fX; }
      const double* y() const { return fY; }
      const double* z() const { return fZ; }

      void print() const
      {
         if (fDateTime) std::cout<<"Date/Time: "<<fDateTime->toString().toStdString();
         if (fX) std::cout<<" x="<<(*fX);
         if (fY) std::cout<<" y="<<(*fY);
         if (fZ) std::cout<<" z="<<(*fZ);
         std::cout<<std::endl;
      }

   private:
      QDateTime* fDateTime;
      double* fX;
      double* fY;
      double* fZ;
};

class ImportWizard: public QDialog, private Ui::importDialog {
Q_OBJECT
public:
    ImportWizard(QWidget *parent = 0);
    ~ImportWizard();
    const QVector< DataItem* >& dataVector() { return fDataVector; fChanged=false; }
    const QVector< DataItem* >& refVector() { return fRefVector; fChanged=false; }
    const QString& filename() const { return fFilename; }
    bool fileChanged() const { return fChanged; }
public slots:
    void reset();

private slots:
      void assignFile();
      void validateFile(const QString& filename);
      void readRawData();
      void validateMapping(int dummy);
      void parseData();
      void reloadData();
   signals:
      void dataChanged();
private: 
// private methods
      void clearData();
      QMap<int,int> getColStat(const QString& filename);
// private fields
      int fVerbose;
      QString fFilename;
      std::ifstream fFile;
      //QWidget* fParent;
      QStringList fStringList;
      QVector<DataItem*> fDataVector;
      QVector<DataItem*> fRefVector;
      QTimer* fTimer;

      QString fSeperator;
      int fFirstRow;
      int fDateTimeIndex;
      QString fDateTimePattern;
      int fXIndex;
      int fYIndex;
      int fRefIndex;
      int fValueIndex;
      int fOldSize;
      bool fChanged;
};

#endif
