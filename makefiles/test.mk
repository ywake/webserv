##############
# Test rules #
##############
TESTER		 := tester
TESTDIR		 := gtest
GTESTDIR	 := $(TESTDIR)/googletest
GTESTLIB	 := $(GTESTDIR)/gtest.a
GTEST_INCLUDES := -I$(GTESTDIR)/gtest $(INCLUDES)
GTEST_FLAGS	 := -fsanitize=address -std=c++11

TESTOBJS_DIR  = $(shell python3 print_newer.py $(OBJDIR) $(SAN_OBJDIR))
TESTOBJS_DIRS = $(SRCDIRS:$(SRCDIR)%=$(TESTOBJS_DIR)%)
TESTOBJS	  = $(SRCS:$(SRCDIR)%.cpp=$(TESTOBJS_DIR)%.o)
TEST_TARGET	 := webserv.a
TESTLIBS	 :=

TESTCASE_DIR := $(TESTDIR)/testcases
TESTCASES	  = $(shell find $(TESTCASE_DIR) -name '*test.cpp')
TESTCASE_DIRS = $(shell find $(TESTCASE_DIR) -type d)
TESTCASE_OBJS = $(TESTCASES:%.cpp=$(BUILDDIR)%.o)
TESTCASE_OBJDIRS = $(TESTCASE_DIRS:%=$(BUILDDIR)%)
TESTCASE_DEPS = $(TESTCASES:%.cpp=$(BUILDDIR)%.d)

$(GTESTLIB)	:
	$(MAKE) -C $(TESTDIR)

$(TESTCASE_OBJDIRS):
	@mkdir -p $@

$(BUILDDIR)gtest/%.o: gtest/%.cpp
	@printf "$(PURPLE)$(ITALIC)"
	$(CXX) $(GTEST_FLAGS) $(GTEST_INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

$(TEST_TARGET): $(TESTOBJS_DIRS) $(TESTOBJS)
	ar -rcs $@ $(TESTOBJS)

$(TESTER)	: $(GTESTLIB) $(TEST_TARGET) $(TESTCASE_OBJDIRS) $(TESTCASE_OBJS)
	clang++ $(GTEST_FLAGS) $(GTEST_INCLUDES) $(GTESTLIB) $(TESTCASE_OBJS) $(TEST_TARGET) $(TESTLIBS) -o $@

gtest    : $(TESTER) FORCE
ifdef FILTER
	./$< --gtest_filter=$(FILTER)
else
	./$<
endif
	@rm $(TEST_TARGET)

-include $(TESTCASE_DEPS)
