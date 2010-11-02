//#ifndef HOSTLAYER_H
//#define HOSTLAYER_H

//#include "precomp.h"
//#include "globals.h"
//#include "projectfile/program.h"

//class ProgChangeEvent : public QEvent
//{
//public:
//    ProgChangeEvent(Project::Program *prog) : QEvent(Event::ProgChange),prog(prog) {}
//    Project::Program *prog;
//};

//class MainHost;
//class HostLayer : public QObject
//{
//Q_OBJECT
//public:
//    explicit HostLayer(int layer, QObject *parent = 0);
//    ~HostLayer();

//    void SaveProgram();
//    void Clear();
////    void Show();
////    void Hide();

//    Connectables::Object * AddObject(ObjType::Enum type,int containerId,int id=0,QString name="");
//    void RemoveObject(Connectables::Object *obj);

//    void AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
//    void RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
//    void RemoveCableFromPin(const ConnectionInfo &pin);
//    void RemoveCableFromObj(int objId);

//    inline int GetId() {return layer;}
//    inline int GetCurrentProgId() {return workingCopyOfProgram.progIndex;}

//    bool event(QEvent *event);

//    QDataStream & toStream(QDataStream& stream) const;
//    QDataStream & fromStream(QDataStream& stream);

//protected:
//    Connectables::Object * CreateObject(ObjType::Enum type,int containerId,int id=0,QString name="");
//    void UnloadProgram();
//    void LoadProgram(Project::Program *prog);

//    int layer;
//    bool hidden;
//    //working copy of the program
//    Project::Program workingCopyOfProgram;
//    //program will be saved there :
//    Project::Program *programPointer;

//    QList<Connectables::Object*>listObjects;

////    friend class MainHost;

//signals:
////    void ProgramSaving();
//    void ProgramLoading(int prog, int layer);

//public slots:
//    void OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
//    void OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
//};

//QDataStream & operator<< (QDataStream& stream, const HostLayer& layer);
//QDataStream & operator>> (QDataStream& stream, HostLayer& layer);

//#endif // HOSTLAYER_H
