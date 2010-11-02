//#include "hostlayer.h"
//#include "connectables/objectfactory.h"
//#include "mainhost.h"
//#include "projectfile/projectfile.h"

//HostLayer::HostLayer(int layer, QObject *parent) :
//        QObject(parent),
//        layer(layer),
//        hidden(false),
//        workingCopyOfProgram(EMPTY_PROGRAM),
//        programPointer(0)
//{
//    debug("add host layer %d",layer);

//    // program -> layer
//    connect(&workingCopyOfProgram,SIGNAL(CableAdded(ConnectionInfo,ConnectionInfo)),
//            this,SLOT(OnCableAdded(ConnectionInfo,ConnectionInfo)));
//    connect(&workingCopyOfProgram,SIGNAL(CableRemoved(ConnectionInfo,ConnectionInfo)),
//            this,SLOT(OnCableRemoved(ConnectionInfo,ConnectionInfo)));
//}

//HostLayer::~HostLayer()
//{
////    Clear();
//}

//bool HostLayer::event(QEvent *event)
//{
//    if (event->type() == Event::ProgChange) {
//        ProgChangeEvent *e = static_cast<ProgChangeEvent *>(event);
//        LoadProgram(e->prog);
//        return true;
//    }

//    return QObject::event(event);
//}

//void HostLayer::LoadProgram(Project::Program *prog)
//{
//    //unload if needed
//    UnloadProgram();

//    programPointer = prog;
//    workingCopyOfProgram.CopyFrom(*prog);

//    foreach(Connectables::Object *obj, listObjects)
//        obj->LoadProgram(workingCopyOfProgram.progIndex);

//    emit ProgramLoading(prog->progIndex, layer);

//    workingCopyOfProgram.Load();

//}

//void HostLayer::UnloadProgram()
//{
//    if(!programPointer)
//        return;

//    //save if needed
//    SaveProgram();
//    programPointer = 0;
//    workingCopyOfProgram.Unload();
//}

//void HostLayer::SaveProgram()
//{
//    if(!programPointer)
//        return;

//    foreach(Connectables::Object *obj, listObjects)
//        obj->SaveProgram();

////    emit ProgramSaving();

//    if(workingCopyOfProgram.dirty)
//        programPointer->CopyFrom(workingCopyOfProgram);
//}

//void HostLayer::AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
//{
//    workingCopyOfProgram.AddCable(outputPin,inputPin);
//}

//void HostLayer::RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
//{
//    workingCopyOfProgram.RemoveCable(outputPin,inputPin);
//}

//void HostLayer::RemoveCableFromPin(const ConnectionInfo &pin)
//{
//    workingCopyOfProgram.RemoveCableFromPin(pin);
//}

//void HostLayer::RemoveCableFromObj(int objId)
//{
//    workingCopyOfProgram.RemoveCableFromObj(objId);
//}

//void HostLayer::OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
//{
//    MainHost::Get()->OnCableAdded(outputPin,inputPin);
//}

//void HostLayer::OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
//{
//    MainHost::Get()->OnCableRemoved(outputPin,inputPin);
//}

////void HostLayer::Show()
////{
////    if(!hidden)
////        return;
////
////    hidden=false;
////    workingCopyOfProgram.Load();
////}

////void HostLayer::Hide()
////{
////    if(hidden)
////        return;
////
////    hidden=true;
////    workingCopyOfProgram.Unload();
////}

//Connectables::Object * HostLayer::AddObject(ObjType::Enum type,int containerId, int identity,QString identityString)
//{
//    Connectables::Object *obj = CreateObject(type,containerId,identity,identityString);

//    if(!obj)
//        return 0;

//    obj->LoadProgram(workingCopyOfProgram.progIndex);
//    return obj;
//}

//void HostLayer::RemoveObject(Connectables::Object *obj)
//{
//    if(!listObjects.contains(obj))
//        return;

//    if(obj->GetContainerId()!=-1) {
//        Connectables::Object *container = Connectables::ObjectFactory::Get()->GetObjectFromId(obj->GetContainerId());
//        container->RemoveChildObject(obj->GetIndex());
//    }
//    listObjects.removeAll(obj);
////    obj->SetSleep(true);
//    obj->Close();

//    MainHost::Get()->OnObjectRemoved(obj->GetIndex(),layer, obj->GetContainerId());
//}

//Connectables::Object * HostLayer::CreateObject(ObjType::Enum type,int containerId, int identity,QString identityString)
//{
//    Connectables::Object *obj = Connectables::ObjectFactory::Get()->NewObject(layer,containerId,type,identity,identityString,this);

//    if(!obj)
//        return 0;

//    if(!obj->Open()) {
//        debug("HostLayer::CreateObject : open error")
//        obj->deleteLater();
//        return 0;
//    }

//    listObjects << obj;

//    if(containerId!=-1) {
//        Connectables::Object *container = Connectables::ObjectFactory::Get()->GetObjectFromId(containerId);
//        container->AddChildObject(obj->GetIndex());
//    }
//    MainHost::Get()->OnObjectAdded(obj,layer,containerId);
//    return obj;
//}

//void HostLayer::Clear()
//{
//    debug("clear host layer %d",layer)

//    //remove objects
//    foreach(Connectables::Object *obj, listObjects) {
//        MainHost::Get()->RemoveObject(obj->GetIndex());
//    }
//    listObjects.clear();

//    //remove cables
//    workingCopyOfProgram.dirty=false;
//    workingCopyOfProgram.Unload();
//    programPointer = 0;
//}

//QDataStream & HostLayer::toStream(QDataStream& stream) const
//{
//    stream << (qint32)listObjects.size();
//    foreach(Connectables::Object *obj, listObjects) {
//        stream << (quint16)obj->type;
//        stream << (qint16)obj->GetContainerId();
//        stream << obj->GetIdentity();
//        stream << obj->GetIdentityString();
//        stream << *obj;
//    }
//    return stream;
//}

//QDataStream & HostLayer::fromStream(QDataStream& stream)
//{
//    qint32 nbObj;
//    stream >> nbObj;
//    for(qint32 i=0;i<nbObj;i++) {
//        quint16 objType;
//        stream >> objType;

//        qint16 containerId;
//        stream >> containerId;
//        if(containerId!=-1) {
//            containerId = Connectables::ObjectFactory::Get()->IdFromSavedId(containerId);
//        }

//        qint32 identity;
//        stream >> identity;

//        QString identityString;
//        stream >> identityString;

//        Connectables::Object *obj = CreateObject((ObjType::Enum)objType,containerId,identity,identityString);
//        if(!obj)
//            return stream;

//        stream >> *obj;
//    }
//    return stream;
//}

//QDataStream & operator<< (QDataStream& stream, const HostLayer& layer)
//{
//    return layer.toStream(stream);
//}

//QDataStream & operator>> (QDataStream& stream, HostLayer& layer)
//{
//    return layer.fromStream(stream);
//}
