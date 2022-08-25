##############
# Test rules #
##############
TESTER			 := tester
TESTDIR			 := gtest
GTESTDIR		 := $(TESTDIR)/googletest
GTESTLIB		 := $(GTESTDIR)/gtest.a
GTEST_INCLUDES	 := -I$(GTESTDIR)/gtest $(INCLUDES)
GTEST_FLAGS		 := -fsanitize=address -std=c++11
GTEST_LIBS		 := -lpthread

TARGET_OBJDIR	  = $(shell python3 print_newer.py $(OBJDIR) $(SAN_OBJDIR))
TARGET_OBJDIRS	  = $(SRCDIRS:$(SRCDIR)%=$(TARGET_OBJDIR)%)
TARGET_OBJS		  = $(SRCS:$(SRCDIR)%.cpp=$(TARGET_OBJDIR)%.o)
TEST_TARGET		 := webserv.a

TESTCASE_DIR	 := $(TESTDIR)/testcases
TESTCASE_DIRS	  = $(shell find $(TESTCASE_DIR) -type d)
TESTCASE_SRCS	  = $(shell find $(TESTCASE_DIR) -name '*test.cpp')
TESTCASE_OBJS	  = $(TESTCASE_SRCS:%.cpp=$(BUILDDIR)%.o)
TESTCASE_OBJDIRS  = $(TESTCASE_DIRS:%=$(BUILDDIR)%)
TESTCASE_DEPS	  = $(TESTCASE_SRCS:%.cpp=$(BUILDDIR)%.d)
.PHONY: $(TESTCASE_DEPS)

$(GTESTLIB)	:
	$(MAKE) -C $(TESTDIR)

$(TESTCASE_OBJDIRS):
	@mkdir -p $@

$(BUILDDIR)gtest/%.o: gtest/%.cpp
	@printf "$(PURPLE)$(ITALIC)"
	$(CXX) $(GTEST_FLAGS) $(GTEST_INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

$(TEST_TARGET): $(TARGET_OBJDIRS) $(TARGET_OBJS	)
	ar -rcs $@ $(TARGET_OBJS)

$(TESTER)	: $(GTESTLIB) $(TEST_TARGET) $(TESTCASE_OBJDIRS) $(TESTCASE_OBJS)
	clang++ $(GTEST_FLAGS) $(GTEST_INCLUDES) $(GTESTLIB) $(TESTCASE_OBJS) $(TEST_TARGET) $(GTEST_LIBS) -o $@

GTEST_OPT	:= $(subst $() ,*:*,$(filter-out gtest,$(MAKECMDGOALS)))
gtest		: $(TESTER) FORCE
	./$< --gtest_filter='*$(GTEST_OPT)*'
	@rm $(TEST_TARGET)

%:;@:

-include $(TESTCASE_DEPS)
