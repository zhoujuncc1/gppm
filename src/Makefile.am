BUILT_SOURCES = Bltl/parser.hpp
AM_YFLAGS = -d
noinst_LIBRARIES = libbltl.a libmining.a

libbltl_a_SOURCES = Bltl/bltl_parser.cpp Bltl/Bltl.cpp Bltl/Parameter.cpp Bltl/Prd.cpp Bltl/TimeVariable.cpp Bltl/parser.ypp Bltl/lexer.lpp \
					$(top_srcdir)/include/Bltl/Bltl.h $(top_srcdir)/include/Bltl/ModelChecker.h \
					$(top_srcdir)/include/Bltl/Node.h $(top_srcdir)/include/Bltl/Bltl.h \
					$(top_srcdir)/include/Bltl/Parameter.h $(top_srcdir)/include/Bltl/Prd.h \
					$(top_srcdir)/include/Bltl/TimeVariable.h   
libbltl_a_CPPFLAGS= -g -std=c++11 -I$(top_srcdir)/include 

libmining_a_SOURCES = Mining/miner_utils.cpp Mining/ModelChecking.cpp Mining/ParameterSet.cpp Mining/SA.cpp \
                    ../include/miner_utils.h ../include/Mining.h ../include/ModelChecking.h \
                    ../include/ParameterSet.h ../include/Model.h ../include/State.h
libmining_a_CPPFLAGS= -g -std=c++11 -I$(top_srcdir)/include $(BOOST_CPPFLAGS) $(GSL_CFLAGS)

