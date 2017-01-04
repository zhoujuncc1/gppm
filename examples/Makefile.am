bin_PROGRAMS=egfngf segclock

egfngf_SOURCES=application.cpp ../src/Models/Egfngf.cpp ../include
egfngf_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
egfngf_LDADD = ../src/libbltl.a ../src/libmining.a
egfngf_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v

segclock_SOURCES=application.cpp ../src/Models/Segclock.cpp ../include
segclock_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
segclock_LDADD = ../src/libbltl.a ../src/libmining.a
segclock_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v