/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
**
** This file is part of the QtCore module of the Qt Toolkit, plus some
** modifications by PySide team.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** Contact: http://www.qt-project.org/legal
**
****************************************************************************/

#undef QT_NO_STL
#undef QT_NO_STL_WCHAR
#define Q_BYTE_ORDER // used to enable QSysInfo.Endian detection on MacOSX

#include "/opt/local/include/QtCore/qnamespace.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QByteArray;

class QString;

#ifndef Q_MOC_OUTPUT_REVISION
#define Q_MOC_OUTPUT_REVISION 61
#endif

// macro for onaming members
#ifdef METHOD
#undef METHOD
#endif
#ifdef SLOT
#undef SLOT
#endif
#ifdef SIGNAL
#undef SIGNAL
#endif

Q_CORE_EXPORT const char *qFlagLocation(const char *method);

#define QTOSTRING_HELPER(s) #s
#define QTOSTRING(s) QTOSTRING_HELPER(s)
#ifndef QT_NO_DEBUG
# define QLOCATION "\0"__FILE__":"QTOSTRING(__LINE__)
# define METHOD(a)   qFlagLocation("0"#a QLOCATION)
# define SLOT(a)     qFlagLocation("1"#a QLOCATION)
# define SIGNAL(a)   qFlagLocation("2"#a QLOCATION)
#else
# define METHOD(a)   "0"#a
# define SLOT(a)     "1"#a
# define SIGNAL(a)   "2"#a
#endif

#ifdef QT3_SUPPORT
#define METHOD_CODE   0                        // member type codes
#define SLOT_CODE     1
#define SIGNAL_CODE   2
#endif

#define QMETHOD_CODE  0                        // member type codes
#define QSLOT_CODE    1
#define QSIGNAL_CODE  2

#define Q_ARG(type, data) QArgument<type >(#type, data)
#define Q_RETURN_ARG(type, data) QReturnArgument<type >(#type, data)

class QObject;
class QMetaMethod;
class QMetaEnum;
class QMetaProperty;
class QMetaClassInfo;


class Q_CORE_EXPORT QGenericArgument
{
public:
    inline QGenericArgument(const char *aName = 0, const void *aData = 0)
        : _data(aData), _name(aName) {}
    inline void *data() const { return const_cast<void *>(_data); }
    inline const char *name() const { return _name; }

private:
    const void *_data;
    const char *_name;
};

class Q_CORE_EXPORT QGenericReturnArgument: public QGenericArgument
{
public:
    inline QGenericReturnArgument(const char *aName = 0, void *aData = 0)
        : QGenericArgument(aName, aData)
        {}
};

template <class T>
class QArgument: public QGenericArgument
{
public:
    inline QArgument(const char *aName, const T &aData)
        : QGenericArgument(aName, static_cast<const void *>(&aData))
        {}
};


template <typename T>
class QReturnArgument: public QGenericReturnArgument
{
public:
    inline QReturnArgument(const char *aName, T &aData)
        : QGenericReturnArgument(aName, static_cast<void *>(&aData))
        {}
};

struct Q_CORE_EXPORT QMetaObject
{
    const char *className() const;
    const QMetaObject *superClass() const;

    QObject *cast(QObject *obj) const;

#ifndef QT_NO_TRANSLATION
    // ### Qt 4: Merge overloads
    QString tr(const char *s, const char *c) const;
    QString trUtf8(const char *s, const char *c) const;
    QString tr(const char *s, const char *c, int n) const;
    QString trUtf8(const char *s, const char *c, int n) const;
#endif // QT_NO_TRANSLATION

    int methodOffset() const;
    int enumeratorOffset() const;
    int propertyOffset() const;
    int classInfoOffset() const;

    int constructorCount() const;
    int methodCount() const;
    int enumeratorCount() const;
    int propertyCount() const;
    int classInfoCount() const;

    int indexOfConstructor(const char *constructor) const;
    int indexOfMethod(const char *method) const;
    int indexOfSignal(const char *signal) const;
    int indexOfSlot(const char *slot) const;
    int indexOfEnumerator(const char *name) const;
    int indexOfProperty(const char *name) const;
    int indexOfClassInfo(const char *name) const;

    QMetaMethod constructor(int index) const;
    QMetaMethod method(int index) const;
    QMetaEnum enumerator(int index) const;
    QMetaProperty property(int index) const;
    QMetaClassInfo classInfo(int index) const;
    QMetaProperty userProperty() const;

    static bool checkConnectArgs(const char *signal, const char *method);
    static QByteArray normalizedSignature(const char *method);
    static QByteArray normalizedType(const char *type);

    // internal index-based connect
    static bool connect(const QObject *sender, int signal_index,
                        const QObject *receiver, int method_index,
                        int type = 0, int *types = 0);
    // internal index-based disconnect
    static bool disconnect(const QObject *sender, int signal_index,
                           const QObject *receiver, int method_index);
    // internal slot-name based connect
    static void connectSlotsByName(QObject *o);

    // internal index-based signal activation
    static void activate(QObject *sender, int signal_index, void **argv);
    static void activate(QObject *sender, int from_signal_index, int to_signal_index, void **argv);
    static void activate(QObject *sender, const QMetaObject *, int local_signal_index, void **argv);
    static void activate(QObject *sender, const QMetaObject *, int from_local_signal_index, int to_local_signal_index, void **argv);
    // internal guarded pointers
    static void addGuard(QObject **ptr);
    static void removeGuard(QObject **ptr);
    static void changeGuard(QObject **ptr, QObject *o);

    static bool invokeMethod(QObject *obj, const char *member,
                             Qt::ConnectionType,
                             QGenericReturnArgument ret,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument());

    static inline bool invokeMethod(QObject *obj, const char *member,
                             QGenericReturnArgument ret,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, Qt::AutoConnection, ret, val0, val1, val2, val3,
                val4, val5, val6, val7, val8, val9);
    }

    static inline bool invokeMethod(QObject *obj, const char *member,
                             Qt::ConnectionType type,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, type, QGenericReturnArgument(), val0, val1, val2,
                                 val3, val4, val5, val6, val7, val8, val9);
    }

    static inline bool invokeMethod(QObject *obj, const char *member,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, Qt::AutoConnection, QGenericReturnArgument(), val0,
                val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }

    QObject *newInstance(QGenericArgument val0 = QGenericArgument(0),
                         QGenericArgument val1 = QGenericArgument(),
                         QGenericArgument val2 = QGenericArgument(),
                         QGenericArgument val3 = QGenericArgument(),
                         QGenericArgument val4 = QGenericArgument(),
                         QGenericArgument val5 = QGenericArgument(),
                         QGenericArgument val6 = QGenericArgument(),
                         QGenericArgument val7 = QGenericArgument(),
                         QGenericArgument val8 = QGenericArgument(),
                         QGenericArgument val9 = QGenericArgument()) const;

    enum Call {
        InvokeMetaMethod,
        ReadProperty,
        WriteProperty,
        ResetProperty,
        QueryPropertyDesignable,
        QueryPropertyScriptable,
        QueryPropertyStored,
        QueryPropertyEditable,
        QueryPropertyUser,
        CreateInstance
    };

    int static_metacall(Call, int, void **) const;

#ifdef QT3_SUPPORT
    QT3_SUPPORT const char *superClassName() const;
#endif

    struct { // private data
        const QMetaObject *superdata;
        const char *stringdata;
        const uint *data;
        const void *extradata;
    } d;
};

struct QMetaObjectExtraData
{
    const QMetaObject **objects;
    int (*static_metacall)(QMetaObject::Call, int, void **);
};

inline const char *QMetaObject::className() const
{ return d.stringdata; }

inline const QMetaObject *QMetaObject::superClass() const
{ return d.superdata; }

#ifdef QT3_SUPPORT
inline const char *QMetaObject::superClassName() const
{ return d.superdata ? d.superdata->className() : 0; }
#endif

QT_END_NAMESPACE

QT_END_HEADER

#define qdoc

#if 0
  #define Q_WS_X11
#elif 1
  #define Q_WS_MAC
#elif 0
  #include "pysidewtypes.h"
  #define Q_WS_WIN
#elif 0
  #define Q_WS_SIMULATOR
#endif

// There are symbols in Qt that exist in Debug but
// not in release
#define QT_NO_DEBUG

#include "/opt/local/include/QtCore/QtCore"
#if 1 || 0
  // Workaround to parse the QApplication header
  #define Q_INTERNAL_QAPP_SRC
  #undef qdoc
#endif
#include "/opt/local/include/QtGui/QtGui"
#include "qpytextobject.h"  // PySide class
#if 0
  #include "/opt/local/include/QtGui/QX11Info"
  #include "/opt/local/include/QtGui/QX11EmbedContainer"
  #include "/opt/local/include/QtGui/QX11EmbedWidget"
#elif 1
  #include "/opt/local/include/QtGui/qmacstyle_mac.h"
#endif
#include "/opt/local/include/QtXml/QtXml"
#include "/opt/local/include/QtUiTools/QtUiTools"
#include "/opt/local/include/QtNetwork/QtNetwork"
#include "/opt/local/include/QtScript/QtScript"
#include "/opt/local/include/QtScriptTools/QtScriptTools"
#include "/opt/local/include/QtMultimedia/QtMultimedia"
#include <QtMaemo5/QtMaemo5>
#include "/opt/local/include/QtDeclarative/QtDeclarative"

// QT_GUI_LIB must be defined to QSqlRelationalDelegate become visible
#define QT_GUI_LIB
#undef Q_DECLARE_INTERFACE
#include "/opt/local/include/QtSql/QtSql"
#include "/opt/local/include/QtSvg/QtSvg"

#if 1
#  include "/opt/local/include/QtXmlPatterns/QtXmlPatterns"
#endif

#if 1
#  include "/opt/local/include/QtWebKit/QtWebKit"
#endif

#if 1
#  include "/opt/local/include/QtTest/QtTest"
#if 8 > 5
#  include "pysideqtesttouch.h"
#endif
#endif

// Phonon
#include "phonon/pyside_phonon.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/abstractaudiooutput.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/abstractmediastream.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/abstractvideooutput.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/addoninterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/audiooutput.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/audiooutputinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/backendcapabilities.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/backendinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/effect.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/effectinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/effectparameter.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/effectwidget.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/mediacontroller.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/medianode.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/mediaobject.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/mediaobjectinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/mediasource.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/objectdescription.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/objectdescriptionmodel.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/path.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/phonon_export.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/phonondefs.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/phononnamespace.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/platformplugin.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/seekslider.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/streaminterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/videoplayer.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/videowidget.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/videowidgetinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/volumefadereffect.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/volumefaderinterface.h"
#include "QT_PHONON_INCLUDE_DIR-NOTFOUND/volumeslider.h"

//QtHelp need be included after QtSql
#include "/opt/local/include/QtHelp/QtHelp"

#ifndef QT_NO_OPENGL
#include </System/Library/Frameworks/AGL.framework/Headers/gl.h>
#include </opt/local/include/QtOpenGL/QtOpenGL>
#endif // QT_NO_OPENGL

