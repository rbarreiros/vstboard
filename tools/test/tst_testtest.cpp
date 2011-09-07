/**************************************************************************
#    Copyright 2010-2011 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include <QtCore/QString>
#include <QtTest/QtTest>

#include "audiobuffer.h"
#include "circularbuffer.h"

class TestTest : public QObject
{
    Q_OBJECT

public:
    TestTest();

    AudioBuffer *pinInBuf;
    AudioBuffer *pinOutBuf;
    CircularBuffer *ring;
    float *buf;
    float *bufOut;

    int bufSize;
    int ringSize;

private Q_SLOTS:
    void writeToRing();

};

TestTest::TestTest()
{
    bufSize = 200;
    ringSize = 450;

    pinInBuf = new AudioBuffer(false,false);
    pinInBuf->SetSize(bufSize);
    pinOutBuf = new AudioBuffer(false,false);
    pinOutBuf->SetSize(bufSize);

    ring = new CircularBuffer(ringSize);

    buf = new float[bufSize];
    for(int i=0; i<bufSize; ++i) {
        buf[i]=1+.01f*i;
    }
    bufOut = new float[bufSize];
    for(int i=0; i<bufSize; ++i) {
        bufOut[i]=.0f;
    }

    ring->Put( buf, bufSize );
    ring->Put( buf, bufSize );
//    ring->Get( bufOut, bufSize );
//    ring->Put( buf, bufSize );
}

void TestTest::writeToRing()
{
    QBENCHMARK {
        for(int i=0; i<100000; i++) {
            ring->Put( buf, bufSize );
            ring->Get( bufOut, bufSize );

        }
    }

//    for(int i=0; i<bufSize; i++) {
//        qDebug()<<buf[i]<<bufOut[i];
//    }
//    QVERIFY2(true, "Failure");
}
QTEST_APPLESS_MAIN(TestTest);

#include "tst_testtest.moc"
