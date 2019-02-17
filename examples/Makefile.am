bin_PROGRAMS=fileModel

fileModel_SOURCES=fileApplication.cpp ../src/Models/Cd95.cpp ../include
fileModel_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
fileModel_LDADD = ../src/libbltl.a ../src/libmining.a
fileModel_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 


