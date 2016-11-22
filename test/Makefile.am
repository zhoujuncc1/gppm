check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = ../gtest/src/gtest-all.cc ../gtest/src/gtest_main.cc
libgtest_la_CPPFLAGS = -I$(top_srcdir)/gtest/include -I$(top_srcdir)/gtest/
libgtest_la_LDFLAGS = -pthread

check_PROGRAMS = bltl_test model_test miner_test

TESTS = bltl_test model_test miner_test


bltl_test_SOURCES = bltl_test.cpp ../include

bltl_test_LDADD = libgtest.la ../src/libbltl.a

bltl_test_LDFLAGS = -pthread

bltl_test_CPPFLAGS = $(BOOST_CPPFLAGS) -g -I$(top_srcdir)/gtest/include/ -I$(top_srcdir)/include -pthread


model_test_SOURCES = model_test.cpp ../src/Models/Egfngf.cpp

model_test_LDADD = libgtest.la

model_test_LDFLAGS = -pthread

model_test_CPPFLAGS = $(BOOST_CPPFLAGS) -g -I$(top_srcdir)/gtest/include/ -I$(top_srcdir)/include -pthread


miner_test_SOURCES = miner_test.cpp ../src/Mining/ParameterSet.cpp ../src/Mining/miner_utils.cpp ../src/Models/Egfngf.cpp ../include

miner_test_LDADD = libgtest.la ../src/libbltl.a

miner_test_LDFLAGS = -pthread

miner_test_CPPFLAGS = $(BOOST_CPPFLAGS) -g -I$(top_srcdir)/gtest/include/ -I$(top_srcdir)/include -pthread



