#ifndef KDEEDUGLOSSARY_H
#define KDEEDUGLOSSARY_H
/***************************************************************************

    copyright            : (C) 2005 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <khtml_part.h>
#include <kdialogbase.h>

class QChar;
class QDomDocument;
class QListViewItem;
class KListView;
class KListViewSearchLine;
class KActionCollection;
class GlossaryItem;

/**
 * @class Glossary
 * @author Carsten Niehaus
 *
 * This class stores all items to be displayed. It also
 * has access-methods to the items
 */
class Glossary
{
	public:
		Glossary(){};
		~Glossary(){};

		/**
		 * add the item @p item to the glossary
		 */
		void addItem( GlossaryItem* item ){
			m_itemlist.append( item );
		}

		/**
		 * clear the Glossary
		 */
		void clear(){
			m_itemlist.clear();
		}

		/**
		 * Every glossary can have a name. It will be
		 * set to @p name
		 */
		void setName( const QString& name ){
			m_name = name;
		}

		/**
		 * @returns the name of the glossary
		 */
		QString name()const{
			return m_name;
		}
		
		/**
		 * @param path The path where to look for the file
		 * @param filename The file to be loaded
		 * @return true if the file was successfully loaded
		 */
		bool loadLayout( QDomDocument&, const QString& path, const QString& filename );
	
	private:
		QValueList<GlossaryItem*> readItems( QDomDocument &itemDocument );
		
		QValueList<GlossaryItem*> m_itemlist;
		
		QString m_name;
};

/**
 * @class GlossaryItem
 * @author Carsten Niehaus
 *
 * A GlossaryItem stores the information of the content of
 * the item and its name. Furthermore, every item can have 
 * a number of pictures or references associated to it.
 * These are stored as QStringLists.
 */
class GlossaryItem
{
	public:
		GlossaryItem();
		~GlossaryItem();

		void setName( const QString& s ){
			m_name = s;
		}

		void setDesc( const QString& s){
			m_desc = s;
		}

		void setRef( const QStringList& s){
			m_ref = s;
		}
	
		void setPictures( const QString& s ){
			m_pic = s;
		}

		QString name() const {
			return m_name;
		}
		
		QString desc() const {
			return m_desc;
		}
		
		QStringList ref() const {
			return m_ref;
		}
		
		QStringList pictures() const {
			return m_pic;
		}
		
		/**
		 * @return the formated HTML code for current item.
		 **/
		QString toHtml() const;

		/**
		 * This method parses the references.
		 * @return the HTML code with the references as HTML links
		 */
		QString parseReferences() const;

	private:
		QString m_name;
		QString m_desc;
		QStringList m_ref;
		QStringList m_pic;
};

class GlossaryDialog : public KDialogBase
{
	Q_OBJECT

	public:
		GlossaryDialog( QWidget *parent=0, const char *name=0);
		~GlossaryDialog();

		void keyPressEvent(QKeyEvent*);

		/**
		 * sets the glossary
		 */
		void setGlossary( const Glossary& glossary ){
			m_glossary = glossary;
		}

	private:
		Glossary m_glossary;
		
		KHTMLPart *m_htmlpart;
		QString m_htmlbasestring;
		QString m_picbasestring;
		KListView *m_glosstree;

		KActionCollection* m_actionCollection;

		void populateTree();
		QListViewItem* findTreeWithLetter( const QChar&, QListViewItem* );

		KListViewSearchLine *m_search;

	private slots:
		void slotClicked( QListViewItem * );
		/**
		 * The user clicked on a href. Emit the corresponding item
		 */
		void displayItem( const KURL& url, const KParts::URLArgs& args );

	protected slots:
		virtual void slotClose();
	
	signals:
		void closed();
};

#endif // KDEEDUGLOSSARY_H
