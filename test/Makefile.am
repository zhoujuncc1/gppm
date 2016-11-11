check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = ../gtest/src/gtest-all.cc ../gtest/src/gtest_main.cc
libgtest_la_CPPFLAGS = -I$(top_srcdir)/gtest/include -I$(top_srcdir)/gtest/
libgtest_la_LDFLAGS = -pthread

check_PROGRAMS = bltl_test model_test

bltl_test_SOURCES = bltl_test.cpp ../include

bltl_test_LDADD = libgtest.la ../src/libbltl.a

bltl_test_LDFLAGS = -pthread

bltl_test_CPPFLAGS = -I$(top_srcdir)/gtest/include/ -I$(top_srcdir)/include -pthread


model_test_SOURCES = model_test.cpp ../src/Models/Egfngf.cpp

model_test_LDADD = libgtest.la

model_test_LDFLAGS = -pthread

model_test_CPPFLAGS = -I/usr/local/include/ -I$(top_srcdir)/gtest/include/ -I$(top_srcdir)/include -pthread

TESTS = bltl_test model_test
