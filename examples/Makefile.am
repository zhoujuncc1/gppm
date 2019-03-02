bin_PROGRAMS=fileModel gpuModel modelChecker

gpuModel_SOURCES=gpuApplication.cpp ../src/Models/Cd95.cpp ../include
gpuModel_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
gpuModel_LDADD = ../src/libbltl.a ../src/libmining.a
gpuModel_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 


fileModel_SOURCES=fileApplication.cpp ../src/Models/Cd95.cpp ../include
fileModel_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
fileModel_LDADD = ../src/libbltl.a ../src/libmining.a
fileModel_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 


modelChecker_SOURCES=modelChecker.cpp ../include
modelChecker_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
modelChecker_LDADD = ../src/libbltl.a ../src/libmining.a
modelChecker_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 
