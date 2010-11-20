/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "object.h"
#include "../globals.h"
#include "../mainhost.h"
#include "../pathsolver.h"

using namespace Connectables;

Object::Object(int index, const ObjectInfo &info) :
    QObject(),
//    position(QPointF(0,0)),
//    size(QSize(0,0)),
//    editorVisible(false),
    hasEditor(false),
    canLearn(false),
//    modelNode(0),
    listenProgramChanges(true),
    parked(false),
    solverNode(0),
    index(index),
    savedIndex(-2),
    sleep(true),
//    parameterLearning(0),
    currentProgram(0),
    progIsDirty(false),
    closed(true),
    containerId(-1),
    modelAudioIn(QModelIndex()),
    modelAudioOut(QModelIndex()),
    modelMidiIn(QModelIndex()),
    modelMidiOut(QModelIndex()),
    modelParamIn(QModelIndex()),
    modelParamOut(QModelIndex()),
    modelBridgeIn(QModelIndex()),
    modelBridgeOut(QModelIndex()),
//    modelEditor(0),
//    modelLearningMode(0),
    currentProgId(TEMP_PROGRAM),
    objInfo(info)
{
    setObjectName(QString("%1.%2").arg(objInfo.name).arg(index));
}

Object::~Object()
{
    Close();
}


bool Object::Open()
{
    closed=false;
    emit Loaded();
    return true;
}

bool Object::Close()
{
    if(closed)
        return false;

    SetSleep(true);
    Hide();
    closed=true;

    foreach(ObjectProgram *prg, listPrograms) {
        delete prg;
    }
    listPrograms.clear();

    return true;
}

void Object::Hide() {
//    SetSleep(true);
    modelAudioIn=QModelIndex();
    foreach(AudioPinIn *pin, listAudioPinIn)
        pin->Close();
    modelAudioOut=QModelIndex();
    foreach(AudioPinOut *pin, listAudioPinOut)
        pin->Close();
    modelMidiIn=QModelIndex();
    foreach(MidiPinIn *pin, listMidiPinIn)
        pin->Close();
    modelMidiOut=QModelIndex();
    foreach(MidiPinOut *pin, listMidiPinOut)
        pin->Close();
    modelBridgeIn=QModelIndex();
    foreach(BridgePinIn *pin, listBridgePinIn)
        pin->Close();
    modelBridgeOut=QModelIndex();
    foreach(BridgePinOut *pin, listBridgePinOut)
        pin->Close();
    modelParamIn=QModelIndex();
    foreach(ParameterPin *pin, listParameterPinIn)
        pin->Close();
    modelParamOut=QModelIndex();
    foreach(ParameterPin *pin, listParameterPinOut)
        pin->Close();

    QStandardItemModel *model = 0;
    if(parked)
        model = MainHost::GetParkingModel();
    else
        model = MainHost::GetModel();

    if(modelIndex.isValid()) {
        if(modelIndex.parent().isValid())
            model->removeRow(modelIndex.row(), modelIndex.parent());
        else
            model->removeRow(modelIndex.row());
    }
    modelIndex=QModelIndex();
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
//    if(modelNode)
//        modelNode->setData(name,Qt::DisplayRole);
    if(modelIndex.isValid())
        MainHost::GetModel()->setData(modelIndex, name, Qt::DisplayRole);

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

void Object::CopyProgram(int ori, int dest)
{
    if(!listPrograms.contains(ori)) {
        debug("Object::CopyProgram ori not found")
        return;
    }
    if(listPrograms.contains(dest)) {
        debug("Object::CopyProgram dest already exists")
        return;
    }
    ObjectProgram *cpy = new ObjectProgram( *listPrograms.value(ori) );
    listPrograms.insert(dest,cpy);
}

void Object::RemoveProgram(int prg)
{
    if(!listPrograms.contains(prg)) {
        debug("Object::RemoveProgram not found")
        return;
    }
    listPrograms.remove(prg);
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
        }

    return 0;
}

void Object::SetParentModeIndex(const QModelIndex &parentIndex)
{
    if(modelIndex.isValid()) {
        if(modelIndex.parent() == parentIndex)
            return;
        else
            Hide();
    }

    parked=false;

    QStandardItem *modelNode = new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index, UserRoles::value);
    modelNode->setData(objInfo.name, Qt::DisplayRole);
    modelNode->setData(hasEditor, UserRoles::hasEditor);
    modelNode->setData(canLearn,UserRoles::canLearn);

    if(parentIndex.isValid()) {
        MainHost::GetModel()->itemFromIndex(parentIndex)->appendRow(modelNode);
    } else {
        MainHost::GetModel()->appendRow(modelNode);
    }

    modelIndex=modelNode->index();
}

void Object::SetContainerId(quint16 id)
{
    containerId = id;

    foreach(AudioPinIn* pin, listAudioPinIn) {
        pin->SetContainerId(containerId);
    }
    foreach(AudioPinOut* pin, listAudioPinOut) {
        pin->SetContainerId(containerId);
    }
    foreach(MidiPinIn* pin, listMidiPinIn) {
        pin->SetContainerId(containerId);
    }
    foreach(MidiPinOut* pin, listMidiPinOut) {
        pin->SetContainerId(containerId);
    }
    hashListParamPin::iterator i = listParameterPinIn.begin();
    while(i!=listParameterPinIn.end()) {
        i.value()->SetContainerId(containerId);
        ++i;
    }
    hashListParamPin::iterator j = listParameterPinOut.begin();
    while(j!=listParameterPinOut.end()) {
        j.value()->SetContainerId(containerId);
        ++j;
    }
    foreach(BridgePinIn* pin, listBridgePinIn) {
        pin->SetContainerId(containerId);
    }
    foreach(BridgePinOut* pin, listBridgePinOut) {
        pin->SetContainerId(containerId);
    }
}

void Object::SetParkingIndex(const QModelIndex &parentIndex)
{
//    if(modelNode)
//        Hide();
    if(modelIndex.isValid())
        Hide();

    parked=true;

    QStandardItem *modelNode=new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index,UserRoles::value);
    modelNode->setData(objInfo.name, Qt::DisplayRole);
    modelNode->setData(true,UserRoles::parking);
    containerId=-1;

    if(parentIndex.isValid())
        MainHost::GetParkingModel()->itemFromIndex(parentIndex)->appendRow(modelNode);
    else
        MainHost::GetParkingModel()->appendRow(modelNode);

    modelIndex=modelNode->index();
}

void Object::UpdateModelNode()
{
    if(!modelIndex.isValid())
        return;

    QStandardItem *modelNode = MainHost::GetModel()->itemFromIndex(modelIndex);

    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);

//    if(hasEditor) {
//        modelNode->setData(editorVisible,UserRoles::editorVisible);
//    }
//    if(canLearn) {
//        modelNode->setData(parameterLearning,UserRoles::paramLearning);
//    }

    //audio in
    if(!modelAudioIn.isValid() && listAudioPinIn.size()>0) {
        QStandardItem *item = new QStandardItem("AudioIn");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listAudioIn)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelAudioIn=item->index();
    }
    foreach(AudioPinIn* pin, listAudioPinIn) {
        pin->SetParentModelIndex(modelAudioIn);
    }

    //audio out
    if(!modelAudioOut.isValid() && listAudioPinOut.size()>0) {
        QStandardItem *item = new QStandardItem("AudioOut");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listAudioOut)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelAudioOut=item->index();
    }
    foreach(AudioPinOut* pin, listAudioPinOut) {
        pin->SetParentModelIndex(modelAudioOut);
    }

    //midi in
    if(!modelMidiIn.isValid() && listMidiPinIn.size()>0) {
        QStandardItem *item = new QStandardItem("MidiIn");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listMidiIn)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelMidiIn=item->index();
    }
    foreach(MidiPinIn* pin, listMidiPinIn) {
        pin->SetParentModelIndex(modelMidiIn);
    }

    //midi out
    if(!modelMidiOut.isValid() && listMidiPinOut.size()>0) {
        QStandardItem *item = new QStandardItem("MidiOut");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listMidiOut)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelMidiOut=item->index();
    }
    foreach(MidiPinOut* pin, listMidiPinOut) {
        pin->SetParentModelIndex(modelMidiOut);
    }

    //param in
    if(!modelParamIn.isValid() && listParameterPinIn.size()>0) {
        QStandardItem *item = new QStandardItem("ParamIn");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listParamIn)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelParamIn=item->index();
    }
    hashListParamPin::iterator i = listParameterPinIn.begin();
    while(i!=listParameterPinIn.end()) {
        i.value()->SetParentModelIndex(modelParamIn);
        ++i;
    }

    //param out
    if(!modelParamOut.isValid() && listParameterPinOut.size()>0) {
        QStandardItem *item = new QStandardItem("ParamOut");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listParamOut)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelParamOut=item->index();
    }
    hashListParamPin::iterator j = listParameterPinOut.begin();
    while(j!=listParameterPinOut.end()) {
        j.value()->SetParentModelIndex(modelParamOut);
        ++j;
    }

    //bridge in
    if(!modelBridgeIn.isValid() && listBridgePinIn.size()>0) {
        QStandardItem *item = new QStandardItem("BridgeIn");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listBridgeIn)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelBridgeIn=item->index();
    }
    foreach(BridgePinIn* pin, listBridgePinIn) {
        pin->SetParentModelIndex(modelBridgeIn);
    }

    //bridge out
    if(!modelBridgeOut.isValid() && listBridgePinOut.size()>0) {
        QStandardItem *item = new QStandardItem("BridgeOut");
        item->setData( QVariant::fromValue(ObjectInfo(NodeType::listPin, ObjType::listBridgeOut)) , UserRoles::objInfo);
        modelNode->appendRow(item);
        modelBridgeOut=item->index();
    }
    foreach(BridgePinOut* pin, listBridgePinOut) {
        pin->SetParentModelIndex(modelBridgeOut);
    }
}

bool Object::GetLearningMode()
{
    if(!modelIndex.isValid())
        return false;
    return MainHost::GetModel()->data(modelIndex, UserRoles::paramLearning).toBool();
}

void Object::SetContainerAttribs(const ObjectConatinerAttribs &attr)
{
    if(!modelIndex.isValid())
        return;

    QStandardItem *item = MainHost::GetModel()->itemFromIndex(modelIndex);

    item->setData(attr.position, UserRoles::position);
    item->setData(attr.size, UserRoles::size);
    item->setData(attr.editorVisible, UserRoles::editorVisible);
    item->setData(attr.editorPosition, UserRoles::editorPos);
    item->setData(attr.editorSize, UserRoles::editorSize);
    item->setData(attr.paramLearning, UserRoles::paramLearning);
}

void Object::GetContainerAttribs(ObjectConatinerAttribs &attr)
{
    if(!modelIndex.isValid())
        return;

    attr.position = modelIndex.data(UserRoles::position).toPointF();
    attr.size = modelIndex.data(UserRoles::size).toSizeF();
    attr.editorVisible = modelIndex.data(UserRoles::editorVisible).toBool();
    attr.editorPosition = modelIndex.data(UserRoles::editorPos).toPoint();
    attr.editorSize = modelIndex.data(UserRoles::editorSize).toSize();
    attr.paramLearning = modelIndex.data(UserRoles::paramLearning).toBool();
}

//void Object::OnEditorVisibilityChanged(bool visible)
//{
//    editorVisible = visible;
//}

//void Object::SetLearningMode(bool learning)
//{
////    if(modelLearningMode)
////        modelLearningMode->setData(learning,UserRoles::value);

//    if(learning) {
//        UnLearningModeChanged(false);
//        parameterLearning = 1;
//    } else {
//        parameterLearning=0;
//    }

////    if(modelNode)
////        modelNode->setData(learning,UserRoles::paramLearning);
//}

//void Object::SetUnLearningMode(bool unlearning)
//{
//    if(unlearning) {
////        modelLearningMode->setData(false,UserRoles::value);
//        if(modelNode)
//            modelNode->setData(false,UserRoles::paramLearning);

//        LearningModeChanged(false);
//        parameterLearning = -1;
//    } else {
//        parameterLearning=0;
//    }
//}

//void Object::SetEditorVisible(bool visible)
//{
//    if(editorVisible==visible)
//        return;

//    editorVisible=visible;
////    if(modelEditor)
////        modelEditor->setData(visible ,UserRoles::value);
//    if(modelNode)
//        modelNode->setData(editorVisible, UserRoles::editorVisible);
//}

//void Object::SetPosition(const QPointF &pos)
//{
//    position=pos;
//    if(modelNode)
//        modelNode->setData(pos,UserRoles::position);
//}

//void Object::SetSize(QSizeF s)
//{
//    size=s;
//    if(modelNode)
//        modelNode->setData(s,UserRoles::size);
//}

QDataStream & Object::toStream(QDataStream & out) const
{
    out << (qint16)index;
    out << sleep;
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
    in >> listenProgramChanges;

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
