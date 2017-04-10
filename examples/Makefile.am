bin_PROGRAMS=egfngf segclock cd95 fileModel

egfngf_SOURCES=application.cpp ../src/Models/Egfngf.cpp ../include
egfngf_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
egfngf_LDADD = ../src/libbltl.a ../src/libmining.a
egfngf_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 

segclock_SOURCES=application.cpp ../src/Models/Segclock.cpp ../include
segclock_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
segclock_LDADD = ../src/libbltl.a ../src/libmining.a
segclock_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 

cd95_SOURCES=application.cpp ../src/Models/Cd95.cpp ../include
cd95_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
cd95_LDADD = ../src/libbltl.a ../src/libmining.a
cd95_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 


fileModel_SOURCES=fileApplication.cpp ../src/Models/Cd95.cpp ../include
fileModel_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
fileModel_LDADD = ../src/libbltl.a ../src/libmining.a
fileModel_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v 


