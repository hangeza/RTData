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
#ifndef QPOINT3_H
#define QPOINT3_H

#include <QPointF>
#include <QtGlobal>
//class QPointF;

class QPoint3F : public QPointF
{
   public:
      QPoint3F()
         : QPointF(0.,0.),fz(0.) {}
      QPoint3F(double x, double y, double z)
         : QPointF(x,y),fz(z)  {}
      void setZ(qreal Z) { fz=Z; }
      qreal z() const { return fz; }
      
   private:
      double fz;
};

#endif
