bin_PROGRAMS=egfngf segclock cd95 train_egfngf train_segclock train_cd95

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

train_egfngf_SOURCES=training.cpp ../src/Models/Egfngf.cpp ../include
train_egfngf_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
train_egfngf_LDADD = ../src/libbltl.a ../src/libmining.a
train_egfngf_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v

train_segclock_SOURCES=training.cpp ../src/Models/Segclock.cpp ../include
train_segclock_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
train_segclock_LDADD = ../src/libbltl.a ../src/libmining.a
train_segclock_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v

train_cd95_SOURCES=training.cpp ../src/Models/Cd95.cpp ../include
train_cd95_CPPFLAGS=-I$(top_srcdir)/include $(BOOST_CPPFLAGS) -g -std=c++11
train_cd95_LDADD = ../src/libbltl.a ../src/libmining.a
train_cd95_LDFLAGS = -L/usr/local/lib -lgsl $(GSL_LIBS) -v