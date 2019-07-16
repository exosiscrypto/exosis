// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/test/uritests.h>

#include <qt/guiutil.h>
#include <qt/walletmodel.h>

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("exosis:fExDspm4Jxk6NcLmwm2gDBREYngUn4QhbA?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
