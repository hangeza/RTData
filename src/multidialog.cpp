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
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include "multidialog.h"

#include <cassert>

MultiIntInputDialog::MultiIntInputDialog(QWidget * parent, int nrInputs)
   : QDialog(parent), fNrBoxes(nrInputs)
{
   QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel,
                                    Qt::Horizontal, this);

   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
   assert(fNrBoxes>=1);
   QGridLayout* layout = new QGridLayout;
   layout->setColumnStretch(1, 1);
   for (int i=0; i<fNrBoxes; i++)
   {
//      fLabels[i] = new QLabel("Value "+QString::number(i),this);
      fLabels.push_back( new QLabel("Value "+QString::number(i),this) );
//      QLabel* label = new QLabel("Value "+QString::number(i),this);
      fSpinBoxes.push_back( new QSpinBox(this) );
//      layout->addWidget((QLabel*)fLabels[i], i, 0);
      layout->addWidget(fLabels[i], i, 0);
      layout->addWidget(fSpinBoxes[i], i, 1);
      connect(fSpinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(updateValues(int)));
      fValues.push_back(fSpinBoxes[i]->value());
   }
   layout->addWidget(buttonBox, fNrBoxes, 1);
   setLayout(layout);
   setWindowTitle(tr("Value Dialog"));
}

MultiIntInputDialog::~ MultiIntInputDialog()
{
   
   while (fSpinBoxes.size())
   {
      if (fSpinBoxes.last()) delete fSpinBoxes.last();
      fSpinBoxes.pop_back();
   }
   while (fLabels.size())
   {
      if (fLabels.last()) delete fLabels.last();
      fLabels.pop_back();
   }
}

void MultiIntInputDialog::updateValues(int value)
{
   bool changed=false;
   for (int i=0; i<fNrBoxes; i++)
   {
      if (fValues[i]!=fSpinBoxes[i]->value()) {
         changed = true;
         fValues[i]=fSpinBoxes[i]->value();
      }
   }
   if (changed) emit valuesChanged(fValues);
}

void MultiIntInputDialog::setLabelText(int i, const QString & text)
{
   if (i<fNrBoxes) fLabels[i]->setText(text);
}

int MultiIntInputDialog::value(int i) const
{
    return fSpinBoxes[i]->value();
}

int MultiIntInputDialog::maximum(int i) const
{
    return fSpinBoxes[i]->maximum();
}

int MultiIntInputDialog::minimum(int i) const
{
    return fSpinBoxes[i]->minimum();
}

void MultiIntInputDialog::setMaximum(int i, int value)
{
    fSpinBoxes[i]->setMaximum(value);
}

void MultiIntInputDialog::setMinimum(int i, int value)
{
    fSpinBoxes[i]->setMinimum(value);
}

void MultiIntInputDialog::setMaximum(int value)
{
   foreach (QSpinBox* sb, fSpinBoxes)
      sb->setMaximum(value);
}

void MultiIntInputDialog::setMinimum(int value)
{
   foreach (QSpinBox* sb, fSpinBoxes)
      sb->setMinimum(value);
}


void MultiIntInputDialog::setValue(int i, int value)
{
   fValues[i]=value;
   disconnect(fSpinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(updateValues(int)));
   fSpinBoxes[i]->setValue(value);
   connect(fSpinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(updateValues(int)));
}

void MultiIntInputDialog::setValues(QVector< int > values)
{
   for (int i=0; i<fNrBoxes; i++)
   {
      setValue(i, values[i]);
   }
}




MultiDoubleInputDialog::MultiDoubleInputDialog(QWidget * parent, int nrInputs)
   : QDialog(parent), fNrBoxes(nrInputs)
{
   QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel,
                                    Qt::Horizontal, this);

   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
   if (fNrBoxes<1) exit(-1);
   QGridLayout* layout = new QGridLayout;
   layout->setColumnStretch(1, 1);
   for (int i=0; i<fNrBoxes; i++)
   {
//      fLabels[i] = new QLabel("Value "+QString::number(i),this);
      fLabels.push_back( new QLabel("Value "+QString::number(i),this) );
//      QLabel* label = new QLabel("Value "+QString::number(i),this);
      fSpinBoxes.push_back( new QDoubleSpinBox(this) );
//      layout->addWidget((QLabel*)fLabels[i], i, 0);
      layout->addWidget(fLabels[i], i, 0);
      layout->addWidget(fSpinBoxes[i], i, 1);
      connect(fSpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(updateValues(double)));
      fValues.push_back(fSpinBoxes[i]->value());
   }
   layout->addWidget(buttonBox, fNrBoxes, 1);
   setLayout(layout);
   setWindowTitle(tr("Value Dialog"));
}

MultiDoubleInputDialog::~ MultiDoubleInputDialog()
{
   
   while (fSpinBoxes.size())
   {
      if (fSpinBoxes.last()) delete fSpinBoxes.last();
      fSpinBoxes.pop_back();
   }
   while (fLabels.size())
   {
      if (fLabels.last()) delete fLabels.last();
      fLabels.pop_back();
   }
}

void MultiDoubleInputDialog::updateValues(double value)
{
   bool changed=false;
   for (int i=0; i<fNrBoxes; i++)
   {
      if (fValues[i]!=fSpinBoxes[i]->value()) {
         changed = true;
         fValues[i]=fSpinBoxes[i]->value();
      }
   }
   if (changed) emit valuesChanged(fValues);
}

void MultiDoubleInputDialog::setLabelText(int i, const QString & text)
{
   if (i<fNrBoxes) fLabels[i]->setText(text);
}

double MultiDoubleInputDialog::value(int i) const
{
    return fValues[i];
}

double MultiDoubleInputDialog::maximum(int i) const
{
    return fSpinBoxes[i]->maximum();
}

double MultiDoubleInputDialog::minimum(int i) const
{
    return fSpinBoxes[i]->minimum();
}

void MultiDoubleInputDialog::setMaximum(int i, double value)
{
    fSpinBoxes[i]->setMaximum(value);
}

void MultiDoubleInputDialog::setMinimum(int i, double value)
{
    fSpinBoxes[i]->setMinimum(value);
}

void MultiDoubleInputDialog::setMaximum(double value)
{
   foreach (QDoubleSpinBox* sb, fSpinBoxes)
      sb->setMaximum(value);
}

void MultiDoubleInputDialog::setMinimum(double value)
{
   foreach (QDoubleSpinBox* sb, fSpinBoxes)
      sb->setMinimum(value);
}


void MultiDoubleInputDialog::setValue(int i, double value)
{
   fValues[i]=value;
   disconnect(fSpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(updateValues(double)));
   fSpinBoxes[i]->setValue(value);
   connect(fSpinBoxes[i], SIGNAL(valueChanged(double)), this, SLOT(updateValues(double)));
}

void MultiDoubleInputDialog::setValues(QVector< double > values)
{
   for (int i=0; i<fNrBoxes; i++)
   {
      setValue(i, values[i]);
   }
}
