## Process this file with automake to produce Makefile.in
bin_PROGRAMS = resonance
resonance_SOURCES = AeroButton.cpp BinauralTone.cpp drawerwidget.cpp FMOD_Sound.cpp fsimagemodel.cpp main.cpp QTranscluentWidget.cpp ResoModule.cpp Resonance.cpp ResoStyle.cpp ResoTree.cpp moc_AeroButton.cpp moc_drawerwidget.cpp moc_QTranscluentWidget.cpp moc_Resonance.cpp moc_ResoTree.cpp moc_BinauralTone.cpp moc_FMOD_Sound.cpp moc_ResoModule.cpp moc_ResoStyle.cpp moc_fsimagemodel.cpp 

AM_CXXFLAGS = @LIBQTCORE_CFLAGS@ @LIBQTGUI_CFLAGS@ @LIBQTXML_CFLAGS@ -I/usr/include/qxt/QxtCore -I/usr/include/qxt/QxtGui -I/usr/local/include/fmodex @LIBQTCORE_LIBS@ @LIBQTGUI_LIBS@ @LIBQTXML_LIBS@ -lQxtCore -lQxtGui -L/usr/local/lib -lfmodex64-4.34.20
AM_LIBS     = @LIBQTCORE_LIBS@ @LIBQTGUI_LIBS@ @LIBQTXML_LIBS@ -lQxtCore -lQxtGui -L/usr/local/lib -lfmodex

moc_%.cpp:	%.h
	moc $< -o $@
