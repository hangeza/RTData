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

#ifndef DUALDIALOG_H
#define DUALDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QSpinBox;
class QDoubleSpinBox;
class QLabel;
class QGridLayout;


class MultiIntInputDialog : public QDialog
{
Q_OBJECT
   public:
      MultiIntInputDialog(QWidget* parent=0, int nrInputs=1);
      ~MultiIntInputDialog();

      int value(int i) const;
      int maximum(int i) const;
      int minimum(int i) const;
   public slots:
      void setLabelText(int i, const QString& text);
      void setValue(int i, int value);
      void setValues(QVector<int> values);
      void setMaximum(int i, int value);
      void setMinimum(int i, int value);
      void setMaximum(int value);
      void setMinimum(int value);
   signals:
      void valuesChanged(QVector<int> values);
   private slots:
      void updateValues(int value);
   private:
      int fNrBoxes;
      QVector<QSpinBox*> fSpinBoxes;
      QVector<QLabel*> fLabels;
      QVector<int> fValues;
};

class MultiDoubleInputDialog : public QDialog
{
Q_OBJECT
   public:
      MultiDoubleInputDialog(QWidget* parent=0, int nrInputs=1);
      ~MultiDoubleInputDialog();

      double value(int i) const;
      double maximum(int i) const;
      double minimum(int i) const;
   public slots:
      void setLabelText(int i, const QString& text);
      void setValue(int i, double value);
      void setValues(QVector<double> values);
      void setMaximum(int i, double value);
      void setMinimum(int i, double value);
      void setMaximum(double value);
      void setMinimum(double value);
   signals:
      void valuesChanged(QVector<double> values);
   private slots:
      void updateValues(double value);
   private:
      int fNrBoxes;
      QVector<QDoubleSpinBox*> fSpinBoxes;
      QVector<QLabel*> fLabels;
      QVector<double> fValues;
};


#endif
