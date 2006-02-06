/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include "spectrum.h"

#include "element.h"

#include <klocale.h>

#include <math.h>

double Spectrum::minPeak()
{
	double value = ( *m_peaklist.begin() )->wavelength;
	QList<peak*>::const_iterator it = m_peaklist.begin();
	const QList<peak*>::const_iterator itEnd = m_peaklist.end();
	for (;it!=itEnd;++it)
	{
		if ( value > ( *it )->wavelength )
			value = ( *it )->wavelength;
	}
	return value;
}

double Spectrum::maxPeak()
{
	double value = ( *m_peaklist.begin() )->wavelength;
	QList<peak*>::const_iterator it = m_peaklist.begin();
	const QList<peak*>::const_iterator itEnd = m_peaklist.end();
	for (;it!=itEnd;++it)
	{
		if ( value < ( *it )->wavelength )
			value = ( *it )->wavelength;
	}
	return value;
}


Spectrum* Spectrum::adjustToWavelength( double min, double max )
{
	Spectrum *spec = new Spectrum();

	QList<peak*>::const_iterator it = m_peaklist.begin();
	const QList<peak*>::const_iterator itEnd = m_peaklist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->wavelength < min || ( *it )->wavelength > max )
			continue;

		spec->addPeak( *it );
	}

	spec->adjustMinMax();

	return spec;
}

void Spectrum::adjustIntensities()
{
	int maxInt = 0;
	QList<peak*>::Iterator it = m_peaklist.begin();
	const QList<peak*>::Iterator itEnd = m_peaklist.end();

	//find the highest intensity
	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->intensity > maxInt )
			maxInt = ( *it )->intensity;
	}

	//check if an adjustment is needed or not
	if ( maxInt == 1000 ) return;

	double max = ( double ) maxInt;

	//now adjust the intensities.
	it = m_peaklist.begin();
	for ( ; it != itEnd; ++it )
	{
		double curInt = ( ( double )( *it )->intensity );
		
		double newInt = max*1000/curInt;
		( *it )->intensity = ( int ) round( newInt );
	}
}

QList<double> Spectrum::wavelengths( double min, double max )
{
	QList<double> list;
	
	QList<peak*>::const_iterator it = m_peaklist.begin();
	const QList<peak*>::const_iterator itEnd = m_peaklist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->wavelength < min || ( *it )->wavelength > max )
			continue;

		list.append( ( *it )->wavelength );
	}

	return list;
}

