/***************************************************************************
 * RTData - The Radiotelescope Data Visualization Tool
 * A simple program for 1d and 2d visualization of fieldstrength vs. coordinate data series
 * recorded with a radio telescope.
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

#include "rtdata.h"

//#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RTData w;
    w.show();
    return a.exec();
}
