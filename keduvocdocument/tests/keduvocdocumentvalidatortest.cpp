/*
 Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "keduvoctranslation.h"
#include "keduvocconjugation.h"
#include "keduvocdeclination.h"
#include "keduvocwordtype.h"


#include <KTemporaryFile>

#include <qtest_kde.h>

#include <qobject.h>
#include <qvalidator.h>

class KEduVocDocumentValidatorTest
  : public QObject
{
    Q_OBJECT

private slots:
    void testDocumentAboutInfo();
    void testLessons();
    void testWordTypes();
};

void KEduVocDocumentValidatorTest::testDocumentAboutInfo()
{
    KTemporaryFile temp;
    temp.setSuffix(".kvtml");
    temp.open();
    KUrl fileName = KUrl(temp.fileName());
    temp.close();

    const QString generator = QString::fromLatin1( "Validator Unit Tests" );
    const QString author = QString::fromLatin1( "Validator Test" );
    const QString license = QString::fromLatin1( "test license" );
    const QString comment = QString::fromLatin1( "comment" );
    const QString category = QString::fromLatin1( "test document" );
    const QString title = QString::fromLatin1( "Validator Test Title" );

    KEduVocDocument doc;
    doc.setAuthor( author );
    doc.setLicense( license );
    doc.setDocumentComment( comment );
    doc.setCategory( category );
    doc.setTitle( title );

    doc.saveAs(fileName, KEduVocDocument::Kvtml, generator);

    KEduVocDocument docRead;
    docRead.open(fileName);

    QCOMPARE( docRead.generator(), generator );
    QCOMPARE( docRead.author(), author );
    QCOMPARE( docRead.license(), license );
    QCOMPARE( docRead.documentComment(), comment );
    QCOMPARE( docRead.category(), category );
    QCOMPARE( docRead.title(), title );
}

void KEduVocDocumentValidatorTest::testLessons()
{
    QString lesson1 = QString::fromLatin1( "Lesson 1" );
    QString lesson1child1 = QString::fromLatin1( "Lesson 1.1" );
    QString lesson1child2 = QString::fromLatin1( "Lesson 1.2" );
    QString lesson2 = QString::fromLatin1( "Lesson 2" );
    QString lesson3 = QString::fromLatin1( "Lesson 3" );

    KEduVocDocument doc;
    doc.lesson()->appendChildContainer(new KEduVocLesson(lesson1, doc.lesson()));
    QCOMPARE(doc.lesson()->childContainerCount(), 1);

    /*
    QCOMPARE(doc.lesson(indexLesson1).name(), lesson1);
    QVERIFY(doc.lesson(indexLesson1).inPractice());

    int indexLesson2 = doc.appendLesson(lesson2, false);
    QCOMPARE(indexLesson2, 1);
    QCOMPARE(doc.lessonCount(), 2);
    QVERIFY(!doc.lesson(indexLesson2).inPractice());

    int indexLesson3 = doc.appendLesson(lesson3, false);
    QCOMPARE(indexLesson3, 2);
    QCOMPARE(doc.lessonCount(), 3);

    bool removed = doc.removeLesson(indexLesson2, KEduVocDocument::DeleteEmptyLesson); // only remove if empty
    QCOMPARE(doc.lessonCount(), 2);
    QVERIFY(removed);
    QCOMPARE(doc.lesson(2).name(), lesson3);

    doc.appendLesson(lesson2, true); */
// Not yet implemented:
//     doc.moveLesson(2, 1);
//     QCOMPARE(doc.lesson(2), lesson2);
}


void KEduVocDocumentValidatorTest::testWordTypes()
{
    KEduVocDocument doc;
    // create doc - has no word types yet
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 0);

    KEduVocWordType *noun;
    KEduVocWordType *nounMale;
    KEduVocWordType *nounFemale;
    KEduVocWordType *verb;

    noun = new KEduVocWordType("Noun", doc.wordTypeContainer());
    doc.wordTypeContainer()->appendChildContainer(noun);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 1);
    nounMale = new KEduVocWordType("Male", noun);
    noun->appendChildContainer(nounMale);
    nounFemale = new KEduVocWordType("Female", noun);
    noun->appendChildContainer(nounFemale);
    verb = new KEduVocWordType("Verb", doc.wordTypeContainer());
    doc.wordTypeContainer()->appendChildContainer(verb);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 2);
    QCOMPARE(doc.wordTypeContainer()->childContainer(0)->childContainerCount(), 2);

    // create some entries
    for(int i = 0; i < 20; i++) {
        KEduVocExpression *e =
            new KEduVocExpression(QStringList() << QString("lang1 %1").arg(i) << QString("lang2 %1").arg(i));
        doc.lesson()->appendEntry(e);
        e->translation(0)->setWordType(noun);
        e->translation(1)->setWordType(noun);
    }
    QCOMPARE(doc.lesson()->entryCount(), 20);
    QCOMPARE(noun->entryCount(), 20);
    doc.lesson()->entry(0)->translation(0)->setWordType(verb);
    // translation 1 is still noun, so it needs to be in both now
    QCOMPARE(noun->entryCount(), 20);
    QCOMPARE(verb->entryCount(), 1);
    doc.lesson()->entry(0)->translation(1)->setWordType(verb);
    QCOMPARE(noun->entryCount(), 19);
    QCOMPARE(verb->entryCount(), 1);

    // delete word type
    doc.wordTypeContainer()->deleteChildContainer(1);
    // the word type is set to 0 when removed
    QVERIFY(doc.lesson()->entry(0)->translation(0)->wordType() == 0);
    QVERIFY(doc.lesson()->entry(0)->translation(1)->wordType() == 0);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 1);
}

QTEST_KDEMAIN_CORE( KEduVocDocumentValidatorTest )

#include "keduvocdocumentvalidatortest.moc"
