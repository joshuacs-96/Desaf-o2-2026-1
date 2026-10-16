TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        RedSocialMensajes.cpp \
        RedSocialPersistencia.cpp \
        main.cpp \
        MatrizAmistades.cpp \
        AlgoritmosAmistad.cpp \
        FechaHora.cpp \
        MedidorRecursos.cpp \
        Usuario.cpp \
        SolicitudAmistad.cpp \
        Foto.cpp \
        Album.cpp \
        RedSocial.cpp \
        Mensaje.cpp \
        persistencia.cpp \
        utilcsv.cpp

HEADERS += \
    ArregloDinamico.h \
    MatrizAmistades.h \
    AlgoritmosAmistad.h \
    FechaHora.h \
    MedidorRecursos.h \
    Usuario.h \
    SolicitudAmistad.h \
    Foto.h \
    Album.h \
    RedSocial.h \
    Mensaje.h \
    persistencia.h \
    utilcsv.h
