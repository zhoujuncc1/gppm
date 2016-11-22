bin_PROGRAMS=egfngf
egfngf_SOURCES=egfngf.cpp ../src/Models/Egfngf.cpp ../include
egfngf_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
egfngf_LDADD = ../src/libbltl.a ../src/libmining.a
egfngf_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS)