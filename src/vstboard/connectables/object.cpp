/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "object.h"
#include "../globals.h"
#include "../mainhost.h"
#include "../pathsolver.h"
#include "../projectfile/program.h"

using namespace Connectables;

Object::Object(int index, const ObjectInfo &info) :
    QObject(),
    listenProgramChanges(true),
    solverNode(0),
//    type(type),
    index(index),
    savedIndex(-2),
//    identity(0),
//    identityString(QString::null),
    sleep(true),
    parameterLearning(0),
    currentProgram(0),
    progIsDirty(false),
    closed(true),
    containerId(-1),
    modelNode(0),
    modelAudioIn(0),
    modelAudioOut(0),
    modelMidiIn(0),
    modelMidiOut(0),
    modelParamIn(0),
    modelParamOut(0),
    modelBridgeIn(0),
    modelBridgeOut(0),
    modelEditor(0),
//    nodeType(NodeType::object),
    currentProgId(TEMP_PROGRAM),
    objInfo(info)
{
    setObjectName(QString("%1.%2").arg(objInfo.name).arg(index));
   //    debug(QString("create obj %1:%2").arg(GetIndex()).arg(objectName()).toAscii())
   // modelNode = new QStandardItem("tmpNode");

}

Object::~Object()
{
    Close();
//    debug(QString("delete obj %1:%2 %3").arg(GetIndex()).arg(objectName()).arg((long)this,0,16).toAscii())
}


bool Object::Open()
{
    closed=false;
    emit Loaded();
    return true;
}

//returns false if already close
bool Object::Close()
{
    if(closed)
        return false;

//debug(QString("object closed %1").arg((long)this,0,16).toAscii())

    SetSleep(true);
    Hide();
    closed=true;

    foreach(ObjectProgram *prg, listPrograms) {
        delete prg;
    }
    listPrograms.clear();


//    MainHost::modelProxy->Remove(index);
//    if(modelNode)
//    modelNode->parent()->removeRow(modelNode->row());

    return true;
}

void Object::Hide() {
    foreach(AudioPinIn *pin, listAudioPinIn)
        pin->Close();
    foreach(AudioPinOut *pin, listAudioPinOut)
        pin->Close();
    foreach(MidiPinIn *pin, listMidiPinIn)
        pin->Close();
    foreach(MidiPinOut *pin, listMidiPinOut)
        pin->Close();
    foreach(BridgePinIn *pin, listBridgePinIn)
        pin->Close();
    foreach(BridgePinOut *pin, listBridgePinOut)
        pin->Close();
    foreach(ParameterPin *pin, listParameterPinIn)
        pin->Close();
    foreach(ParameterPin *pin, listParameterPinOut)
        pin->Close();

    if(modelNode) {
        if(modelNode->parent())
            modelNode->model()->removeRow(modelNode->row(), modelNode->parent()->index());
        else
            modelNode->model()->removeRow(modelNode->row());
    }

    modelNode=0;
    modelAudioIn=0;
    modelAudioOut=0;
    modelMidiIn=0;
    modelMidiOut=0;
    modelParamIn=0;
    modelParamOut=0;
    modelBridgeIn=0;
    modelBridgeOut=0;
    modelEditor=0;
}

void Object::SetBridgePinsInVisible(bool visible)
{
    foreach(BridgePinIn *pin, listBridgePinIn)
        pin->SetVisible(visible);
}

void Object::SetBridgePinsOutVisible(bool visible)
{
    foreach(BridgePinOut *pin, listBridgePinOut)
        pin->SetVisible(visible);
}

void Object::setObjectName(const QString &name)
{
//    MainHost::modelProxy->Update(index,Qt::DisplayRole,name);
    if(modelNode)
        modelNode->setData(name,Qt::DisplayRole);
    QObject::setObjectName(name);
}

void Object::SetSleep(bool sleeping)
{
    objMutex.lock();
    sleep = sleeping;
    objMutex.unlock();
}

bool Object::GetSleep()
{
    QMutexLocker l(&objMutex);
    return sleep;
}

void Object::UnloadProgram()
{
    currentProgId=EMPTY_PROGRAM;
}

void Object::SaveProgram()
{
    if(!currentProgram)
        return;

    if(!progIsDirty)
        return;

    currentProgram->Save(listParameterPinIn,listParameterPinOut);
}

void Object::LoadProgram(int prog)
{
    //if prog is already loaded, update model

    if(prog==currentProgId && currentProgram) {
        UpdateModelNode();
        return;
    }

    //if a program is loaded, unload it without saving
    int progWas = currentProgId;
    if(currentProgId!=EMPTY_PROGRAM && currentProgram)
        UnloadProgram();

    currentProgId=prog;

    if(!listPrograms.contains(currentProgId))
        listPrograms.insert(currentProgId,new ObjectProgram(prog,listParameterPinIn,listParameterPinOut));

    currentProgram=listPrograms.value(currentProgId);
    currentProgram->Load(listParameterPinIn,listParameterPinOut);
    progIsDirty=false;
    UpdateModelNode();

    //if the loaded program was a temporary prog, delete it
    if(progWas==TEMP_PROGRAM)
        listPrograms.remove(TEMP_PROGRAM);
}

void Object::NewRenderLoop()
{
    foreach(AudioPinIn *pin, listAudioPinIn) {
        pin->NewRenderLoop();
    }
}


Pin * Object::GetPin(const ConnectionInfo &pinInfo)
{
    switch(pinInfo.type) {
        case PinType::Audio :
            if(pinInfo.direction==PinDirection::Input) {
                if(pinInfo.pinNumber >= listAudioPinIn.size()) {
                    if(objInfo.objType == ObjType::dummy) {
                        while(listAudioPinIn.size() <= pinInfo.pinNumber ) {
                            listAudioPinIn.append(new AudioPinIn(this, listAudioPinIn.size()));
                        }
                    } else
                        return 0;
                }
                return listAudioPinIn.at(pinInfo.pinNumber);
            } else {
                if(pinInfo.pinNumber >= listAudioPinOut.size()) {
                    if(objInfo.objType == ObjType::dummy) {
                        while(listAudioPinOut.size() <= pinInfo.pinNumber ) {
                            listAudioPinOut.append(new AudioPinOut(this, listAudioPinOut.size()));
                        }
                    } else
                        return 0;
                }
                return listAudioPinOut.at(pinInfo.pinNumber);
            }
        case PinType::Midi :
            if(pinInfo.direction==PinDirection::Input)
                return listMidiPinIn.at(pinInfo.pinNumber);
            else
                return listMidiPinOut.at(pinInfo.pinNumber);

        case PinType::Parameter :
            if(pinInfo.direction==PinDirection::Input)
                return listParameterPinIn.value(pinInfo.pinNumber);
            else
                return listParameterPinOut.value(pinInfo.pinNumber);

        case PinType::Bridge :
            if(pinInfo.direction==PinDirection::Input)
                return listBridgePinIn.at(pinInfo.pinNumber);
            else
                return listBridgePinOut.at(pinInfo.pinNumber);

//        default:
//            debug("object getpin : pin not found");
        }

    //i'm a dummy object : i create needed pins
//    if(objInfo.objType == ObjType::dummy) {

//    }

    return 0;
}

void Object::SetParentModelNode(QStandardItem* parent)
{
    if(modelNode && modelNode->parent() == parent)
        return;

//    debug("Object::SetParentModelNode %d",index)

    if(modelNode)
        Hide();

    modelNode = new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index, UserRoles::value);
    modelNode->setData(objInfo.name, Qt::DisplayRole);
//    modelNode->setData(listenProgramChanges,UserRoles::programmable);

    parent->appendRow(modelNode);
    containerId = parent->data(UserRoles::value).toInt();
    //UpdateModelNode();
}

void Object::SetParkingNode(QStandardItem* parent)
{
//    debug("Object::SetParkingNode %d",index)

    if(modelNode)
        Hide();

    modelNode=new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index,UserRoles::value);
    modelNode->setData(objInfo.name, Qt::DisplayRole);
    modelNode->setData(true,UserRoles::parking);
    containerId=-1;
    parent->appendRow(modelNode);
}

void Object::UpdateModelNode()
{
//    debug("Object::UpdateModelNode %d",index)

    if(!modelNode)
        return;

//    debug(QString("Object::UpdateModelNode %1 %2").arg(index).arg(objectName()).toAscii())

//    if(modelNode->parent()==0) {
//        debug("Object::UpdateModelNode no parent container")
//    } else {
//       containerId = modelNode->parent()->data(UserRoles::value).toInt();
//    }

//    MainHost::modelProxy->Update(index,Qt::DisplayRole,objectName());
//    modelNode->setData(objectName(),Qt::DisplayRole);
//    MainHost::modelProxy->Update(index,UserRoles::nodeType,nodeType);
    //modelNode->setData(objInfo.nodeType,UserRoles::nodeType);
//    modelNode->setData(listenProgramChanges,UserRoles::programmable);

    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);

    //audio in
    if(!modelAudioIn && listAudioPinIn.size()>0) {
        modelAudioIn = new QStandardItem("AudioIn");
        modelAudioIn->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listAudioIn)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelAudioIn,modelNode);
        modelNode->appendRow(modelAudioIn);
    }
    foreach(AudioPinIn* pin, listAudioPinIn) {
        pin->SetParentModelNode(modelAudioIn);
//        pin->SetVisible(true);
    }

    //audio out
    if(!modelAudioOut && listAudioPinOut.size()>0) {
        modelAudioOut = new QStandardItem("AudioOut");
        modelAudioOut->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listAudioOut)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelAudioOut,modelNode);
        modelNode->appendRow(modelAudioOut);
    }
    foreach(AudioPinOut* pin, listAudioPinOut) {
        pin->SetParentModelNode(modelAudioOut);
//        pin->SetVisible(true);
    }

    //midi in
    if(!modelMidiIn && listMidiPinIn.size()>0) {
        modelMidiIn = new QStandardItem("MidiIn");
        modelMidiIn->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listMidiIn)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelMidiIn,modelNode);
        modelNode->appendRow(modelMidiIn);
    }
    foreach(MidiPinIn* pin, listMidiPinIn) {
        pin->SetParentModelNode(modelMidiIn);
//        pin->SetVisible(true);
    }

    //midi out
    if(!modelMidiOut && listMidiPinOut.size()>0) {
        modelMidiOut = new QStandardItem("MidiOut");
        modelMidiOut->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listMidiOut)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelMidiOut,modelNode);
        modelNode->appendRow(modelMidiOut);
    }
    foreach(MidiPinOut* pin, listMidiPinOut) {
        pin->SetParentModelNode(modelMidiOut);
//        pin->SetVisible(true);
    }

    //param in
    if(!modelParamIn && listParameterPinIn.size()>0) {
        modelParamIn = new QStandardItem("ParamIn");
        modelParamIn->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listParamIn)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelParamIn,modelNode);
        modelNode->appendRow(modelParamIn);
    }
    hashListParamPin::iterator i = listParameterPinIn.begin();
    while(i!=listParameterPinIn.end()) {
        i.value()->SetParentModelNode(modelParamIn);
        ++i;
    }

    //param out
    if(!modelParamOut && listParameterPinOut.size()>0) {
        modelParamOut = new QStandardItem("ParamOut");
        modelParamOut->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listParamOut)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelParamOut,modelNode);
        modelNode->appendRow(modelParamOut);
    }
    hashListParamPin::iterator j = listParameterPinOut.begin();
    while(j!=listParameterPinOut.end()) {
        j.value()->SetParentModelNode(modelParamOut);
        ++j;
    }

    //bridge in
    if(!modelBridgeIn && listBridgePinIn.size()>0) {
        modelBridgeIn = new QStandardItem("BridgeIn");
        modelBridgeIn->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listBridgeIn)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelBridgeIn,modelNode);
        modelNode->appendRow(modelBridgeIn);
    }
    foreach(BridgePinIn* pin, listBridgePinIn) {
        pin->SetParentModelNode(modelBridgeIn);
//        pin->SetVisible(true);
    }

    //bridge out
    if(!modelBridgeOut && listBridgePinOut.size()>0) {
        modelBridgeOut = new QStandardItem("BridgeOut");
        modelBridgeOut->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listBridgeOut)) , UserRoles::objInfo);
//        MainHost::modelProxy->Add(-1,modelBridgeOut,modelNode);
        modelNode->appendRow(modelBridgeOut);
    }
    foreach(BridgePinOut* pin, listBridgePinOut) {
        pin->SetParentModelNode(modelBridgeOut);
//        pin->SetVisible(true);
    }
}

void Object::SetLearningMode(bool learning)
{
    if(learning) {
        UnLearningModeChanged(false);
        parameterLearning = 1;
    } else {
        parameterLearning=0;
    }
}

void Object::SetUnLearningMode(bool unlearning)
{
    if(unlearning) {
        LearningModeChanged(false);
        parameterLearning = -1;
    } else {
        parameterLearning=0;
    }
}

void Object::SetPosition(QPointF pos)
{
    position=pos;
//    MainHost::modelProxy->Update(index,UserRoles::position,position);
    modelNode->setData(position,UserRoles::position);
}

void Object::SetSize(QSizeF s)
{
    size=s;
//    MainHost::modelProxy->Update(index,UserRoles::size,size);
    modelNode->setData(size,UserRoles::size);
}

QDataStream & Object::toStream(QDataStream & out) const
{
    out << (qint16)index;
    out << sleep;
    out << (qint8)parameterLearning;
    out << listenProgramChanges;

    out << (quint16)listPrograms.size();
    hashPrograms::const_iterator i = listPrograms.constBegin();
    while(i!=listPrograms.constEnd()) {
        out << (quint16)i.key();
        out << *i.value();
        ++i;
    }

    if(currentProgram)
        out << (quint16)currentProgram->progId;
    else
        out << (quint16)EMPTY_PROGRAM;


    return out;
}

QDataStream & Object::fromStream(QDataStream & in)
{
    qint16 id;
    in >> id;
    savedIndex=id;
    in >> sleep;
    in >> (qint8&)parameterLearning;
    in >> listenProgramChanges;

//    UnloadProgram();
//    LoadProgram(TEMP_PROGRAM);

    quint16 nbProg;
    in >> nbProg;
    for(quint16 i=0; i<nbProg; i++) {
        quint16 progId;
        in >> progId;

        ObjectProgram *prog=0;
        if(listPrograms.contains(progId)) {
            prog=listPrograms.value(progId);
            delete prog;
        }

        prog = new ObjectProgram(progId);
        listPrograms.insert(progId,prog);

        in >> *prog;
    }

    quint16 progId;
    in >> progId;
//    UnloadProgram();
    //LoadProgram(progId);

//    listPrograms.remove(EMPTY_PROGRAM-1)
    return in;
}

QDataStream & operator<< (QDataStream & out, const Connectables::Object& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, Connectables::Object& value)
{
    return value.fromStream(in);
}
