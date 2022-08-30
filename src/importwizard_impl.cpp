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

#include <QtGui>
#include <QFileDialog>
#include <QPainter>

#include <iostream>
#include <fstream>
#include <ios>
#include <cmath>
#include <vector>
#include <map>
#include <complex>

#include "importwizard_impl.h"

const int TIMEOUT = 20000;

using namespace std;

ImportWizard::ImportWizard(QWidget *parent)
    :QDialog(parent), importDialog()
{
   setupUi(this);
   cancelButton->setText(tr("cancel"));
   continueButton->setText(tr("continue"));
   finishButton->setText(tr("finish"));
   connect( openFileButton, SIGNAL( clicked() ), this, SLOT(assignFile()));
   connect( continueButton, SIGNAL( clicked() ), this, SLOT(readRawData()));
   connect( finishButton, SIGNAL( clicked() ), this, SLOT(parseData()));
   connect( filenameEdit, SIGNAL( textChanged(const QString&) ), this, SLOT(validateFile(const QString&)));
   connect( dateTimeComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));
   connect( dateEditComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));
   connect( xComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));
   connect( yComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));
   connect( refComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));
   connect( valueComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT(validateMapping(int)));

   tabWidget->setTabEnabled(1,false);
   fTimer = new QTimer(this);
   connect(fTimer, SIGNAL(timeout()), this, SLOT(reloadData()));

   fOldSize = 0;
   fChanged = false;

   fVerbose=0;
}

void ImportWizard::assignFile()
{
   QString s=QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                ".");
   if (s.size()) filenameEdit->setText(s);
   fFilename=filenameEdit->text();
}

void ImportWizard::validateFile(const QString & filename)
{
   QFileInfo fileinfo(filename);
   if (fileinfo.exists()){
      continueButton->setEnabled(true);
      fileSizeLabel->setText(QString::number(fileinfo.size()));
      fileCreatedLabel->setText(fileinfo.birthTime().toString());
      fileModifiedLabel->setText(fileinfo.lastModified().toString());
      fileOwnerLabel->setText(fileinfo.owner());

      QString fSeperator=seperatorEdit->text();
      fFirstRow=startLineSpinBox->value();
   }
   else {
      continueButton->setEnabled(false);
      fileSizeLabel->setText("");
      fileCreatedLabel->setText("");
      fileModifiedLabel->setText("");
      fileOwnerLabel->setText("");
   }
}

void ImportWizard::readRawData()
{
   if (!fFilename.size()) return;
   QMap<int,int> colstat = getColStat(fFilename);
   int maxcols=0;
   int maxentries=0;
   QMap<int, int>::const_iterator i = colstat.constBegin();
   while (i != colstat.constEnd()) {
      if (i.value()>maxentries) maxcols=i.key();
      ++i;
   }
   if (fVerbose) cout<<"file organized in "<<maxcols<<" columns"<<endl;

   fFile.open(fFilename.toStdString().c_str());
   if (!fFile)
   {
      std::cerr<<"could not read file "<<fFilename.toStdString()<<std::endl;
      return;
   }

   char c;

   int actRow=0;
   for (int actRow=0; actRow<fFirstRow-1; actRow++) {
      char str[500];
      fFile.getline(str,500);
   }

   do
   {
      fFile>>c;
      if (c=='#')
      {
         char str[500];
         fFile.getline(str,500);
//         length--;
//         std::cout<<c<<std::endl;
      }
      else fFile.putback(c);
   } while (c=='#');

   fStringList.clear();
//   rawDataTableWidget->setRowCount(_data.size());

   while (!fFile.eof())
   {
      char str[500];
      fFile.getline(str,500);
      QString line(str);
      if (line.size()<3) continue;
      line.remove(QString("\n"));
      line.remove(QString("\r"));
      line.remove(QString("Timeout"));
      line.remove(QString("!"));
      if (fVerbose>1) std::cout<<"line "<<actRow<<": "<<line.toStdString()<<std::endl;
      QStringList strlist = line.split(" ",QString::SplitBehavior::SkipEmptyParts);
      if (strlist.size()!=maxcols) continue;
      fStringList.push_back(line);
      if (fVerbose>1) {
         for (int i=0; i<strlist.size();++i) std::cout<<strlist[i].toStdString()<<" ";
         std::cout<<std::endl;
      }
//      if (strlist.size()>rawDataTableWidget->columnCount()) rawDataTableWidget->setColumnCount(strlist.size());
//      rawDataTableWidget->setRowCount(rawDataTableWidget->rowCount()+1);
/*      for (int i=0; i<strlist.size();++i)
      {
//         QTableWidgetItem *newItem = new QTableWidgetItem(strlist[i]);
//         rawDataTableWidget->setItem(rawDataTableWidget->rowCount()-1, i, newItem);
//         rawDataTableWidget->setItem(actRow, i, newItem);
      }
      */
      actRow++;
   }
   fFile.close();
   if (fOldSize==fStringList.size()) {
      fChanged=false;
      return;
   }
   std::cout<<"read "<<fStringList.size()<<" entries."<<std::endl;
   fChanged = true;
   if (fVerbose) std::cout<<"detected change in file"<<std::endl;
   fOldSize=fStringList.size();
   //fFile.close();
   tabWidget->setTabEnabled(1,true);
   tabWidget->setCurrentIndex(1);
   tabWidget->setTabEnabled(0,false);
   continueButton->setEnabled(false);

   dateTimeComboBox->clear();
   xComboBox->clear();
   yComboBox->clear();
   valueComboBox->clear();
   refComboBox->clear();

   dateTimeComboBox->insertItem(0,"N/A");
   xComboBox->insertItem(0,"N/A");
   yComboBox->insertItem(0,"N/A");
   refComboBox->insertItem(0,"N/A");
   QStringList colStrings;
   for (int i=0; i<maxcols; i++) {
      colStrings.push_back(QString("Col "+QString::number(i+1)));
   }
   dateTimeComboBox->insertItems(1,colStrings);
   xComboBox->insertItems(1,colStrings);
   yComboBox->insertItems(1,colStrings);
   refComboBox->insertItems(1,colStrings);
   valueComboBox->insertItems(0,colStrings);

//   fFile.close();
}

void ImportWizard::validateMapping(int dummy)
{
   if (!this->isVisible()) return;
   if (dateTimeComboBox->currentIndex() || xComboBox->currentIndex() || yComboBox->currentIndex()) {
      finishButton->setEnabled(true);
      fDateTimePattern=dateEditComboBox->currentText();
      fDateTimeIndex=dateTimeComboBox->currentIndex();
      fXIndex=xComboBox->currentIndex();
      fYIndex=yComboBox->currentIndex();
      fRefIndex=refComboBox->currentIndex();
      fValueIndex=valueComboBox->currentIndex();
      if (fVerbose>2) std::cout<<"index values changed"<<std::endl;
   }
   else finishButton->setEnabled(false);
}

void ImportWizard::parseData()
{
   clearData();

   for (int line=0; line<fStringList.size(); line++)
   {
      QStringList strlist = fStringList[line].split(" ",QString::SplitBehavior::SkipEmptyParts);

      QDateTime* datetime = NULL;
      double* x = NULL;
      double* y = NULL;
      double* z = NULL;

//      DataItem* item = new DataItem(datetime, x, y, z);
      DataItem* item = new DataItem;

      if (fDateTimeIndex) {
        if (dateEditComboBox->currentIndex() > 0) {
            // remove the fractional part of the second, since this is prohibiting proper parsing of the QDateTime value
            /// ToDo: evaluate the fractional seconds as well
            strlist[fDateTimeIndex].remove(strlist[fDateTimeIndex].lastIndexOf("."),10);
            QString datestr(strlist[fDateTimeIndex-1]+" "+strlist[fDateTimeIndex]);
            if (fVerbose>2) std::cout<<"Date String: "<<datestr.toStdString()<<std::endl;
            if (fVerbose>2) std::cout<<"selected format: "<<fDateTimePattern.toStdString()<<std::endl;

            datetime = new QDateTime(QDateTime::fromString(datestr,fDateTimePattern));
	 
            if (fVerbose>2) std::cout<<"date fmt="<<fDateTimePattern.toStdString()<<std::endl;
            if (fVerbose>2) std::cout<<"Date: "<<datetime->toString().toStdString()<<std::endl;

            item->setDateTime(datetime);
            qDebug()<<"time="<<*datetime;
        } else {
            // assume that the time is in unix time format representation
            QString datestr(strlist[fDateTimeIndex-1]);
            bool ok { false };
            double unix_time { datestr.toDouble(&ok) };
            if (!ok) {
                // conversion of date from string failed
                // what should be done at this point?
                qDebug()<<"date conversion failed";
                continue;
            }
            qDebug() << "read unix timestamp (double): " << unix_time << " (qint64): " << static_cast<qint64>(unix_time);

            datetime = new QDateTime(QDateTime::fromSecsSinceEpoch(static_cast<qint64>(unix_time)));
            item->setDateTime(datetime);
            qDebug()<<"time="<<*datetime;
        }
      }

      if (fVerbose>2) std::cout<<"xComboBox->currentIndex()="<<fXIndex<<std::endl;
      if (fXIndex) {
         if (fVerbose>2) std::cout<<"strlist[xComboBox->currentIndex()-1]="<<strlist[fXIndex-1].toStdString()<<std::endl;
         x=new double(strlist[fXIndex-1].toDouble());
         if (fVerbose>2) std::cout<<"x="<<*x<<std::endl;
         item->setX(x);
      }
      if (fYIndex) {
         y=new double(strlist[fYIndex-1].toDouble());
         item->setY(y);
      }

      z=new double(strlist[fValueIndex].toDouble());
      item->setZ(z);

      int ref=0;
      if (fRefIndex) {
         ref=strlist[fRefIndex-1].toInt();
      }

      if (fVerbose>1) item->print();
      if (ref) fRefVector.push_back(item);
      else fDataVector.push_back(item);


      if (fVerbose>1) {
         for (int i=0; i<strlist.size();++i) std::cout<<strlist[i].toStdString()<<" ";
         std::cout<<std::endl;
      }
   }
   accept();
   fTimer->start(TIMEOUT);
   emit(dataChanged());
}

void ImportWizard::clearData()
{
   while (fDataVector.size()) {
      if (fDataVector.last()!=NULL) {
         fDataVector.last()->release();
         delete fDataVector.last();
      }
      fDataVector.pop_back();
   }
   while (fRefVector.size()) {
      if (fRefVector.last()!=NULL) {
         fRefVector.last()->release();
         delete fRefVector.last();
      }
      fRefVector.pop_back();
   }
}

ImportWizard::~ ImportWizard()
{
   clearData();
}

void ImportWizard::reloadData()
{
   if (!fFilename.size()) return;
   readRawData();
   if (!fChanged) return;
   clearData();
   parseData();
   if (fVerbose) std::cout<<"reload data"<<std::endl;
}

QMap< int, int > ImportWizard::getColStat(const QString & filename)
{
   QMap< int, int > colstat;
   if (!filename.size()) return colstat;
   fFile.open(filename.toStdString().c_str());
   if (!fFile)
   {
      std::cerr<<"could not read file "<<filename.toStdString()<<std::endl;
      return colstat;
   }

   char c;

   int actRow=0;
   for (int actRow=0; actRow<fFirstRow-1; actRow++) {
      char str[500];
      fFile.getline(str,500);
   }

   do
   {
      fFile>>c;
      if (c=='#')
      {
         char str[500];
         fFile.getline(str,500);
//         length--;
//         std::cout<<c<<std::endl;
      }
      else fFile.putback(c);
   } while (c=='#');

   int maxcols=0;
   while (!fFile.eof())
   {
      char str[500];
      fFile.getline(str,500);
      QString line(str);
      if (line.size()<3) continue;
      line.remove(QString("\n"));
      line.remove(QString("\r"));
      line.remove(QString("Timeout"));
      line.remove(QString("!"));
      if (fVerbose>1) std::cout<<"line "<<actRow<<": "<<line.toStdString()<<std::endl;
      QStringList strlist = line.split(" ",QString::SplitBehavior::SkipEmptyParts);
      colstat[strlist.size()]++;
      actRow++;
   }
   fFile.close();
   if (fVerbose) {
      cout<<"column statistics:"<<endl;
      QMap<int, int>::const_iterator i = colstat.constBegin();
      while (i != colstat.constEnd()) {
         cout << i.key() << ": " << i.value() << endl;
         ++i;
      }
   }
   return colstat;
}
