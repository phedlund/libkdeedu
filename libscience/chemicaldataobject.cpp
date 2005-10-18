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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
#include "chemicaldataobject.h"

ChemicalDataObject::ChemicalDataObject( QVariant v, BlueObelisk type )
{
	m_value = v;
	m_type = type;
};

ChemicalDataObject::ChemicalDataObject() 
{
}

QString ChemicalDataObject::valueAsString()
{
	return m_value.toString();
}

bool ChemicalDataObject::operator==( const int v )
{
	if ( m_value.type() != QVariant::Int )	
		return false;
	
	if ( m_value.toInt() == v )
		return true;
	
	return false;
}

bool ChemicalDataObject::operator==( const QString& v )
{
	if ( m_value.type() != QVariant::String )	
		return false;

	if ( m_value.toString() == v )
		return true;

	return false;
}