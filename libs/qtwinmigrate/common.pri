infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtwinmigrate-uselib
TEMPLATE += fakelib
QTWINMIGRATE_LIBNAME = $$qtLibraryTarget(QtSolutions_MFCMigrationFramework-head)
TEMPLATE -= fakelib
TWINMIGRATE_LIBDIR = $$PWD/lib
unix:qtwinmigrate-uselib:!qtwinmigrate-buildlib:QMAKE_RPATHDIR += $$QTWINMIGRATE_LIBDIR
