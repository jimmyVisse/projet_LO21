QT += widgets
QT += xml

SOURCES += \
    main.cpp \
    UTProfiler.cpp \
    UVEditeur.cpp \
    supprUV.cpp \
    supprFormation.cpp \
    CursusEditeur.cpp \
    mainwindow.cpp \
    DossierUI.cpp \
    uv.cpp \
    formation.cpp \
    dossier.cpp \
    strategie.cpp \
    solution.cpp \
    completer.cpp

HEADERS += \
    UTProfiler.h \
    UVEditeur.h \
    supprUV.h \
    supprFormation.h \
    CursusEditeur.h \
    mainwindow.h \
    DossierUI.h \
    uv.h \
    formation.h \
    dossier.h \
    strategie.h \
    inscription.h \
    solution.h \
    completer.h

OTHER_FILES += \
    UV_XML.xml \
    UV_Cursus2.xml \
    logo_utc.png \
    utc2.png \
    ../build-projet_lo21-Desktop_Qt_5_2_1_MinGW_32bit-Debug/UV_Cursus.xml

FORMS += \
    mainwindow.ui
