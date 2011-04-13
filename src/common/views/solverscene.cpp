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

#include "solverscene.h"
#include "../renderer/solvernode.h"
#include "../connectables/object.h"

using namespace View;

SolverScene::SolverScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void SolverScene::NewSolver(orderedNodes* nodes)
{
    qDeleteAll( items() );

    int currentLevel=0;

    float x=0;
    float y=0;

    float objHeight=18;

    float passWidth=0;

    orderedNodes::iterator i = nodes->begin();
    while(i!=nodes->end()) {
        if(currentLevel!=i.key()) {
            currentLevel=i.key();
            y=0;
            x+=passWidth;
            passWidth=0;
        }

        float lineHeight=objHeight+10;
        QGraphicsRectItem *renderLineView = new QGraphicsRectItem();
        addItem(renderLineView);

        float lineWidth=0;
        Connectables::Object* obj = i.value()->objectPtr.data();
        QRectF rectObj(0,0,10,objHeight);
            QGraphicsRectItem *objView = new QGraphicsRectItem(rectObj,renderLineView);
            QGraphicsTextItem *txt = new QGraphicsTextItem(QString::number(obj->GetIndex()),objView);
            txt=0; //unused
            objView->setPos(lineWidth,0);
            lineWidth+=10;

        renderLineView->setRect(0,0,lineWidth,lineHeight);
        passWidth = std::max(passWidth,lineWidth);
        renderLineView->setPos(x,y);

        y+=lineHeight;
        ++i;


    }
}

