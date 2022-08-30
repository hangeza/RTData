# RTData - The Radiotelescope Data Visualization Tool
A simple program for 1d and 2d visualization of fieldstrength vs. coordinate data series recorded with a radio telescope.

## Description
A program which allows the selection of the following fields from an ASCII data file for visualization:
- date+time
- measurement value (fieldstrength)
- x-coordinate (i.e. RA or Az)
- y-coordinate (i.e. Dec or Alt)
- reference switch indicator (a bit indicating whether the sky signal is measured or the receiver's noise)

Consequently, the measurement values may be plotted wrt. time, x-ccordinate, y-coordinate (1d plots) or x and y coordinates (2d plot). When specifying the ref switch field, the data values are corrected for the interpolated receiver noise from the closest reference measurements around the signal measurement.

## Usage
Run qmake and make. Then execute the compiled binary "RTData".

## Prerequisites
Qt version 5.12 or higher
