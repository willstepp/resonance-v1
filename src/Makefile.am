## Process this file with automake to produce Makefile.in
bin_PROGRAMS = resonance
resonance_SOURCES = AeroButton.cpp BinauralTone.cpp drawerwidget.cpp FMOD_Sound.cpp main.cpp QTranscluentWidget.cpp ResoModule.cpp Resonance.cpp ResoStyle.cpp ResoTree.cpp moc_AeroButton.cpp moc_drawerwidget.cpp moc_QTranscluentWidget.cpp moc_Resonance.cpp moc_ResoTree.cpp moc_BinauralTone.cpp moc_FMOD_Sound.cpp moc_ResoModule.cpp moc_ResoStyle.cpp

AM_CXXFLAGS = @QT_CXXFLAGS@
MOC         = @QT_MOC@

moc_%.cc:	%.h
	$(MOC) $< -o $@
