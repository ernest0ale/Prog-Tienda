TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    App/app.cpp \
    Data/database.cpp \
    Interface/consola.cpp \
    Interface/entradaDatos.cpp \
    Interface/menu.cpp \
    Services/productoService.cpp \
    Services/reporteService.cpp \
    Services/ventaService.cpp \
    Utils/SQLite/sqlite3.c \
    Licencia/licencia.cpp
    

HEADERS += \
    App/app.h \
    Data/database.h \
    Global/colors.h \
    Global/definition.h \
    Global/utils.h \
    Interface/consola.h \
    Interface/entradaDatos.h \
    Interface/menu.h \
    Models/producto.h \
    Models/venta.h \
    Services/productoService.h \
    Services/reporteService.h \
    Services/ventaService.h \
    Utils/SQLite/sqlite3.h \
    Licencia/licencia.h
