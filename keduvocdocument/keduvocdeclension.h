/***************************************************************************

    C++ Interface: keduvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KEDUVOCDECLENSION_H
#define KEDUVOCDECLENSION_H

#include "keduvoctext.h"

/**
A declension contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocDeclension{
public:
    enum DeclensionNumber {
        Singular = 1,
        Dual,
        Plural
    };

    enum DeclensionCase {
        Nominative = 1,
        Genitive,
        Dative,
        Accusative,
        Ablative,
        Locative,
        Vocative,
        DeclensionCaseMAX
    };


    /**
     * The constructor without arguments
     */
    explicit KEduVocDeclension();

    /** copy constructor
     * @param other comparison object to copy
     */
    KEduVocDeclension( const KEduVocDeclension &other );

    ~KEduVocDeclension();

    /** equality operator
     * @param a object to compare to
     * @returns true if comparisons are the same, false otherwise
     */
//     will probably not be necessary
//     bool operator == ( const KEduVocDeclension& a ) const;

    /** assignment operator for d-pointer copying
     * @param other object to copy from
     * @returns reference to this object
     */
    KEduVocDeclension& operator= ( const KEduVocDeclension& other );

    /**
     * The grammatical number, there is singular and plural for english, some languages have dual for exactly two items.
     * @param gender
     * @param decCase
     * @return
     */
    KEduVocText& declension(DeclensionNumber number, DeclensionCase decCase);

    /**
     * Set a declension
     * @param
     * @param number
     * @param decCase
     */
    void setDeclension(const KEduVocText& declension, DeclensionNumber number, DeclensionCase decCase);

    bool isEmpty();

private:
    int indexOf(DeclensionNumber number, DeclensionCase decCase);

    class Private;
    Private * const d;
};

#endif
