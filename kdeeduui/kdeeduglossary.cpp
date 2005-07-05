/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kdeeduglossary.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <klistview.h>
#include <klistviewsearchline.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

#include <qfile.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qtoolbutton.h>

Glossary::Glossary()
{
	m_name = i18n( "Glossary" );
}

Glossary::~Glossary()
{
}

bool Glossary::loadLayout( QDomDocument &Document, const KURL& url )
{
        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
	{
		kdDebug() << "no such file: " << layoutFile.name() << endl;
		return false;
	}

        if (!layoutFile.open(IO_ReadOnly))
                return false;

        ///Check if document is well-formed
        if (!Document.setContent(&layoutFile))
        {
                kdDebug() << "wrong xml" << endl;
                layoutFile.close();
                return false;
        }
        layoutFile.close();

        return true;
}

bool Glossary::isEmpty() const
{
	return m_itemlist.count() == 0;
}


Glossary* Glossary::readFromXML( const KURL& url )
{
	QDomDocument doc( "document" );

	Glossary *glossary = new Glossary();

	if ( glossary->loadLayout( doc, url ) )
	{
		QValueList<GlossaryItem*> itemList;
		itemList = glossary->readItems( doc );
		glossary->setItemlist( itemList );
	}

	return glossary;
}

QValueList<GlossaryItem*> Glossary::readItems( QDomDocument &itemDocument )
{
	QValueList<GlossaryItem*> list;

	QDomNodeList itemList;
	QDomNodeList refNodeList;
	QDomElement itemElement;
	QStringList reflist;

	itemList = itemDocument.elementsByTagName( "item" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		reflist.clear();
		GlossaryItem *item = new GlossaryItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		
		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		QDomElement refNode = ( const QDomElement& ) itemElement.namedItem( "references" ).toElement();

		QString desc = descNode.toElement().text();
//TODO fix the pictures-tag
//X 		desc.replace("[img]", m_picbasestring );
//X 		desc.replace("[/img]", "\" />" );
		desc.replace("[b]", "<b>" );
		desc.replace("[/b]", "</b>" );
		desc.replace("[i]", "<i>" );
		desc.replace("[/i]", "</i>" );

//		item->setName( i18n( nameNode.toElement( ).text().utf8() ) );
//		item->setDesc( i18n( desc.utf8() ) );
		item->setName( nameNode.toElement().text() );
		item->setDesc( desc );
		refNodeList = refNode.elementsByTagName( "refitem" );
		for ( uint it = 0; it < refNodeList.count(); it++ )
		{
//			reflist << i18n( refNodeList.item( it ).toElement().text().utf8() );
			reflist << refNodeList.item( it ).toElement().text();
		}
		reflist.sort();
		item->setRef( reflist );
		
		list.append( item );
	}
	
	return list;
}



GlossaryDialog::GlossaryDialog( QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Glossary" ), Close, Close, parent, name, false )
{
	//XX TMP!!!
	QString baseHtml = "foo.png";
//X 	QString baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
//X 	baseHtml.append("kalzium/data/");
//X 	baseHtml.append("bg.jpg");

//X 	m_picbasestring = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
//X 	m_picbasestring.append("kalzium/data/knowledgepics/");
//X 	m_picbasestring.prepend( "<img src=\"" );

	m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	m_htmlbasestring.append( baseHtml );
	m_htmlbasestring.append("\">");
	
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
	vbox->activate();

	QHBoxLayout *hbox = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox->activate();

	QToolButton *clear = new QToolButton( plainPage() );
	clear->setIconSet( SmallIconSet( "locationbar_erase" ) );
	hbox->addWidget( clear );

	QLabel *lbl = new QLabel( plainPage() );
	lbl->setText( i18n( "Search:" ) );
	hbox->addWidget( lbl );

	m_search = new KListViewSearchLine( plainPage(), 0, "search-line" );
	hbox->addWidget( m_search );
	vbox->addLayout( hbox );
	setFocusProxy(m_search);
 
	QSplitter *vs = new QSplitter( plainPage() );
	vbox->addWidget( vs );
	
	m_glosstree = new KListView( vs, "treeview" );
	m_glosstree->addColumn( "entries" );
	m_glosstree->header()->hide();
	m_glosstree->setFullWidth( true );
	m_glosstree->setRootIsDecorated( true );
 
	m_search->setListView( m_glosstree );
 
	m_htmlpart = new KHTMLPart( vs, "html-part" );
 
//X 	m_actionCollection = new KActionCollection(this);
//X         KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);
//X 
//X 	QDomDocument doc( "foo" );
//X 	QString filename = "knowledge.xml";
//X 
//X 	if ( loadLayout( doc, filename ) )
//X 		m_itemList = readItems( doc );
//X 
//X 	QDomDocument doc2( "foo" );
//X 	filename = "tools.xml";
//X 
//X 	if ( loadLayout( doc2, filename ) )
//X 		m_toolList = readTools( doc2 );
//X 
//X 	populateTree();
//X 
	connect( m_htmlpart->browserExtension(), SIGNAL( openURLRequestDelayed( const KURL &, const KParts::URLArgs & ) ), this, SLOT( displayItem( const KURL &, const KParts::URLArgs & ) ) );
	connect( m_glosstree, SIGNAL(clicked( QListViewItem * )), this, SLOT(slotClicked( QListViewItem * )));
	connect( clear, SIGNAL(clicked()), m_search, SLOT(clear()));
 
	resize( 600, 400 );
}

GlossaryDialog::~GlossaryDialog()
{
}

void GlossaryDialog::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Return) {
		e->ignore();
	}
	KDialogBase::keyPressEvent(e);
}

void GlossaryDialog::displayItem( const KURL& url, const KParts::URLArgs& )
{
	// using the "host" part of a kurl as reference
	QString myurl = url.host().lower();
	m_search->setText( "" );
	m_search->updateSearch( "" );
	QListViewItem *found = 0;
	QListViewItemIterator it( m_glosstree );
	QListViewItem *item;
	while ( it.current() )
	{
		item = it.current();
		if ( item->text(0).lower() == myurl )
		{
			found = item;
			break;
		}
		++it;
	}
	slotClicked( found );
}

void GlossaryDialog::updateTree()
{
	//XXX hack hack hack hack
	m_glosstree->clear();

	QValueList<Glossary*>::iterator itGl = m_glossaries.begin();
	const QValueList<Glossary*>::iterator itGlEnd = m_glossaries.end();
	
	for ( ; itGl != itGlEnd ; ++itGl )
	{
		QValueList<GlossaryItem*> items = ( *itGl )->itemlist();
		QValueList<GlossaryItem*>::iterator it = items.begin();
		const QValueList<GlossaryItem*>::iterator itEnd = items.end();

		QListViewItem *main = new QListViewItem( m_glosstree, ( *itGl )->name() );
		main->setExpandable( true );
		main->setSelectable( false );
		//XXX TMP!!!
		bool foldinsubtrees = true;

		///FIXME The next line is crashing for some unkown reasons...
		for ( ; it != itEnd ; ++it )
		{
			if ( foldinsubtrees )
			{
				QChar thisletter = ( *it )->name().upper()[0];
				QListViewItem *thisletteritem = findTreeWithLetter( thisletter, main );
				if ( !thisletteritem )
				{
					thisletteritem = new QListViewItem( main, thisletter );
					thisletteritem->setExpandable( true );
					thisletteritem->setSelectable( false );
				}
				new QListViewItem( thisletteritem, ( *it )->name() );
			}
			else
				new QListViewItem( main, ( *it )->name() );
		}
		main->sort();
	}
}

void GlossaryDialog::addGlossary( Glossary* newgloss )
{
	if ( !newgloss ) return;
	if ( newgloss->isEmpty() ) return;
	m_glossaries.append( newgloss );

	kdDebug() << "Count of the new glossary: " << newgloss->itemlist().count() << endl;
	kdDebug() << "Number of glossaries: " << m_glossaries.count() << endl;

	updateTree();
}

QListViewItem* GlossaryDialog::findTreeWithLetter( const QChar& l, QListViewItem* i )
{
	QListViewItem *it = i->firstChild();
	while ( it )
	{
		if ( it->text(0)[0] == l )
			return it;
		it = it->nextSibling();
	}
	return 0;
}

void GlossaryDialog::slotClicked( QListViewItem *item )
{
	if ( !item )
		return;
	
	QString html = m_htmlbasestring;
	
	/**
	 * The next lines are searching for the correct KnowledgeItem
	 * in the m_itemList. When it is found the HTML will be
	 * generated
	 */
	QValueList<Glossary*>::iterator itGl = m_glossaries.begin();
	const QValueList<Glossary*>::iterator itGlEnd = m_glossaries.end();
	bool found = false;
	GlossaryItem *i = 0;
	while ( !found && itGl != itGlEnd )
	{
		QValueList<GlossaryItem*> items = ( *itGl )->itemlist();
		QValueList<GlossaryItem*>::iterator it = items.begin();
		const QValueList<GlossaryItem*>::iterator itEnd = items.end();
		while ( !found && it != itEnd )
		{
			if ( ( *it )->name() == item->text( 0 ) )
			{
				i = *it;
				found = true;
			}
			++it;
		}
		++itGl;
	}
	if ( found && i )
	{
		html += i->toHtml() + "</body></html>";
		m_htmlpart->begin();
		m_htmlpart->write( html );
		m_htmlpart->end();
		return;
	}
}

void GlossaryDialog::slotClose()
{
	emit closed();
	accept();
}

QString GlossaryItem::toHtml() const
{
	QString code = "<h1>" + m_name + "</h1>" + m_desc;

//	QString pic_path = locate("data", "kalzium/data/knowledgepics/");
	if ( !m_ref.isEmpty() )
	{
		QString refcode = parseReferences();
		code += refcode;
	}
	return code;
}

QString GlossaryItem::parseReferences() const
{
	QString htmlcode = "<h3>" + i18n( "References" ) + "</h3>";
	
	bool first = true;
	for ( uint i = 0; i < m_ref.size(); i++ )
	{
		if ( !first )
			htmlcode += "<br>";
		else
			first = false;
		htmlcode += QString( "<a href=\"item://%1\">%2</a>" ).arg( m_ref[i], m_ref[i] );
	}

	return htmlcode;
}


#include "kdeeduglossary.moc"
