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
#include "container.h"

using namespace Connectables;

Object::Object(MainHost *host, int index, const ObjectInfo &info) :
    QObject(),
    listenProgramChanges(true),
    listAudioPinIn(new PinsList(host,this)),
    listAudioPinOut(new PinsList(host,this)),
    listMidiPinIn(new PinsList(host,this)),
    listMidiPinOut(new PinsList(host,this)),
    listBridgePinIn(new PinsList(host,this)),
    listBridgePinOut(new PinsList(host,this)),
    listParameterPinIn(new PinsList(host,this)),
    listParameterPinOut(new PinsList(host,this)),
    parked(false),
    solverNode(0),
    index(index),
    savedIndex(-2),
    sleep(true),
    currentProgram(0),
    closed(true),
    containerId(FixedObjId::noContainer),
    currentProgId(TEMP_PROGRAM),
    objInfo(info),
    myHost(host)
{
    setObjectName(QString("%1.%2").arg(objInfo.name).arg(index));

    //init pins lists
    ConnectionInfo i;
    i.objId=index;
    i.myHost=myHost;

    i.type=PinType::Audio;
    i.direction=PinDirection::Input;
    listAudioPinIn->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listAudioIn));
    i.direction=PinDirection::Output;
    listAudioPinOut->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listAudioOut));

    i.type=PinType::Midi;
    i.direction=PinDirection::Input;
    listMidiPinIn->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listMidiIn));
    i.direction=PinDirection::Output;
    listMidiPinOut->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listMidiOut));

    i.type=PinType::Bridge;
    i.direction=PinDirection::Input;
    listBridgePinIn->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listBridgeIn));
    i.direction=PinDirection::Output;
    listBridgePinOut->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listBridgeOut));

    i.type=PinType::Parameter;
    i.direction=PinDirection::Input;
    listParameterPinIn->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listParamIn));
    i.direction=PinDirection::Output;
    listParameterPinOut->SetInfo(this,i,ObjectInfo(NodeType::listPin, ObjType::listParamOut));

    pinLists.insert("audioin", listAudioPinIn);
    pinLists.insert("audioout", listAudioPinOut);
    pinLists.insert("midiin", listMidiPinIn);
    pinLists.insert("midiout", listMidiPinOut);
    pinLists.insert("bridgein", listBridgePinIn);
    pinLists.insert("bridgeout", listBridgePinOut);
    pinLists.insert("parameterin",listParameterPinIn);
    pinLists.insert("parameterout",listParameterPinOut);

    if(objInfo.nodeType != NodeType::container) {
        //editor pin
        listEditorVisible << "hide";
        listEditorVisible << "show";
        listParameterPinIn->AddPin(FixedPinNumber::editorVisible);

        //learning pin
        listIsLearning << "off";
        listIsLearning << "learn";
        listIsLearning << "unlearn";
        listParameterPinIn->AddPin(FixedPinNumber::learningMode);
    }
}


Object::~Object()
{
    pinLists.clear();
//    debug2(<< "delete Object" << objectName() << hex << (long)this)

    if(containerId!=FixedObjId::noContainer) {
        QSharedPointer<Object>cntPtr = myHost->objFactory->GetObjectFromId( containerId );
        if(cntPtr) {
            static_cast<Container*>(cntPtr.data())->OnChildDeleted(this);
        }
    }
    Close();
}


bool Object::Open()
{
    closed=false;
    errorMessage="";
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

    foreach(PinsList *lst, pinLists) {
        lst->Hide();
    }
/*
    QStandardItemModel *model = 0;
    if(parked)
        model = myHost->GetParkingModel();
    else
        model = myHost->GetModel();

    if(modelIndex.isValid()) {
        if(modelIndex.parent().isValid())
            model->removeRow(modelIndex.row(), modelIndex.parent());
        else
            model->removeRow(modelIndex.row());
    }
    modelIndex=QModelIndex();
    */
}

void Object::SetBridgePinsInVisible(bool visible)
{
    listBridgePinIn->SetVisible(visible);
}

void Object::SetBridgePinsOutVisible(bool visible)
{
    listBridgePinOut->SetVisible(visible);
}

void Object::setObjectName(const QString &name)
{
//    if(modelNode)
//        modelNode->setData(name,Qt::DisplayRole);
    if(modelIndex.isValid())
        myHost->GetModel()->setData(modelIndex, name, Qt::DisplayRole);

    //objInfo.name=QString("%1.%2").arg(objInfo.name).arg(index);
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

bool Object::IsDirty()
{
    if(!currentProgram)
        return false;
    return currentProgram->isDirty;
}

void Object::OnProgramDirty()
{
    if(!currentProgram)
        return;

    currentProgram->isDirty=true;


//    if(!modelIndex.isValid())
//        return;
//    QStandardItem *item=myHost->GetModel()->itemFromIndex(modelIndex);
//    if(!item)
//        return;
//    item->setData(true,UserRoles::isDirty);


//    QSharedPointer<Object>objPtr = myHost->objFactory->GetObj(modelIndex.parent());
//    if(!objPtr)
//        return;
//    static_cast<Container*>(objPtr.data())->SetProgramDirty();
}

void Object::UnloadProgram()
{
    currentProgId=EMPTY_PROGRAM;
}

void Object::SaveProgram()
{
    if(!currentProgram)
        return;

    if(!currentProgram->isDirty)
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

void Object::CopyCurrentProgram(int dest)
{
    if(listPrograms.contains(dest)) {
        debug("Object::CopyCurrentProgram dest already exists")
        return;
    }
    ObjectProgram *cpy = new ObjectProgram( *currentProgram );
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
    foreach(Pin *pin, listAudioPinIn->listPins) {
        static_cast<AudioPinIn*>(pin)->NewRenderLoop();
    }
}


Pin * Object::GetPin(const ConnectionInfo &pinInfo)
{
    Pin* pin=0;
    bool autoCreate=false;

    if(objInfo.objType == ObjType::dummy || !errorMessage.isEmpty())
        autoCreate=true;

    foreach(PinsList *lst, pinLists) {
        if(lst->connInfo.type == pinInfo.type && lst->connInfo.direction == pinInfo.direction) {
            pin=lst->GetPin(pinInfo.pinNumber,autoCreate);
            if(pin)
                return pin;
        }
    }

    return 0;
}


QStandardItem *Object::GetParkingItem()
{
    QStandardItem *modelNode=new QStandardItem();
    modelNode->setData(index,UserRoles::value);
    modelNode->setData(objectName(), Qt::DisplayRole);
    return modelNode;
}

QStandardItem *Object::GetFullItem()
{
    QStandardItem *modelNode = new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index, UserRoles::value);
    modelNode->setData(objectName(), Qt::DisplayRole);
    modelNode->setData(errorMessage, UserRoles::errorMessage);
    return modelNode;
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
    modelNode->setData(objectName(), Qt::DisplayRole);
//    modelNode->setData(hasEditor, UserRoles::hasEditor);
//    modelNode->setData(canLearn,UserRoles::canLearn);
    modelNode->setData(errorMessage, UserRoles::errorMessage);

    if(parentIndex.isValid()) {
        myHost->GetModel()->itemFromIndex(parentIndex)->appendRow(modelNode);
    } else {
        myHost->GetModel()->appendRow(modelNode);
    }

    modelIndex=modelNode->index();
}

void Object::SetContainerId(quint16 id)
{
    containerId = id;

    foreach(PinsList *lst, pinLists) {
        lst->SetContainerId(containerId);
    }
}

void Object::UpdateModelNode()
{
    if(!modelIndex.isValid())
        return;

    QStandardItem *modelNode = myHost->GetModel()->itemFromIndex(modelIndex);
    if(!modelNode) {
        debug("Object::UpdateModelNode node not found")
        return;
    }

    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(errorMessage, UserRoles::errorMessage);

    foreach(PinsList *lst, pinLists) {
        lst->UpdateModelNode(modelNode);
    }
}

void Object::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    //editor pin
    if(pinInfo.pinNumber==FixedPinNumber::editorVisible) {
        int val = static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::editorVisible))->GetIndex();
        if(val)
            //QTimer::singleShot(0, this, SLOT(OnShowEditor()));
            OnShowEditor();
        else
            OnHideEditor();
            //QTimer::singleShot(0, this, SLOT(OnHideEditor()));
    }
}

void Object::ToggleEditor(bool visible)
{
    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::editorVisible))->ChangeValue(visible);
}

LearningMode::Enum Object::GetLearningMode()
{
    if(!listParameterPinIn->listPins.contains(FixedPinNumber::learningMode))
        return LearningMode::off;

    return (LearningMode::Enum)static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::learningMode))->GetIndex();
}

void Object::SetContainerAttribs(const ObjectContainerAttribs &attr)
{
    if(!modelIndex.isValid())
        return;

    QStandardItem *item = myHost->GetModel()->itemFromIndex(modelIndex);

    item->setData(attr.position, UserRoles::position);
    item->setData(attr.editorPosition, UserRoles::editorPos);
    item->setData(attr.editorSize, UserRoles::editorSize);
    item->setData(attr.editorHScroll, UserRoles::editorHScroll);
    item->setData(attr.editorVScroll, UserRoles::editorVScroll);
}

void Object::GetContainerAttribs(ObjectContainerAttribs &attr)
{
    if(!modelIndex.isValid())
        return;

    attr.position = modelIndex.data(UserRoles::position).toPointF();
    attr.editorPosition = modelIndex.data(UserRoles::editorPos).toPoint();
    attr.editorSize = modelIndex.data(UserRoles::editorSize).toSize();
    attr.editorHScroll = modelIndex.data(UserRoles::editorHScroll).toInt();
    attr.editorVScroll = modelIndex.data(UserRoles::editorVScroll).toInt();
}

Pin* Object::CreatePin(const ConnectionInfo &info)
{
    switch(info.direction) {
        case PinDirection::Input :
            switch(info.type) {
                case PinType::Audio : {
                    AudioPinIn *newPin = new AudioPinIn(this,info.pinNumber);
                    newPin->buffer->SetSize(myHost->GetBufferSize());
                    return newPin;
                }

                case PinType::Midi : {
                    MidiPinIn *newPin = new MidiPinIn(this,info.pinNumber);
                    return newPin;
                }

                case PinType::Bridge : {
                    BridgePinIn *newPin = new BridgePinIn(this,info.pinNumber,info.bridge);
                    return newPin;
                }

                case PinType::Parameter : {
                    switch(info.pinNumber) {
                        case FixedPinNumber::editorVisible : {
                            ParameterPin *newPin = new ParameterPinIn(this,FixedPinNumber::editorVisible,"hide",&listEditorVisible,false,tr("Editor"));
                            newPin->SetLimitsEnabled(false);
                            return newPin;
                        }
                        case FixedPinNumber::learningMode : {
                            ParameterPin *newPin = new ParameterPinIn(this,FixedPinNumber::learningMode,"off",&listIsLearning,false,tr("Learn"));
                            newPin->SetLimitsEnabled(false);
                            return newPin;
                        }
                    }

                    break;
                }
                default :
                    return 0;
            }
            break;

        case PinDirection::Output :
            switch(info.type) {
                case PinType::Audio : {
                    AudioPinOut *newPin = new AudioPinOut(this,info.pinNumber);
                    newPin->buffer->SetSize(myHost->GetBufferSize());
                    return newPin;
                }

                case PinType::Midi : {
                    MidiPinOut *newPin = new MidiPinOut(this,info.pinNumber);
                    return newPin;
                }

                case PinType::Bridge : {
                    BridgePinOut *newPin = new BridgePinOut(this,info.pinNumber,info.bridge);
                    return newPin;
                }

                default :
                    return 0;
            }
            break;

        default :
            return 0;
    }

    return 0;
}

QDataStream & Object::toStream(QDataStream & out) const
{
    const quint16 file_version = 1;
    out << file_version;

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
    quint16 file_version;
    in >> file_version;

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
