#############
# Variables #
#############

NAME	:= webserv
SRCDIR  := srcs/
SRCDIRS := $(shell find $(SRCDIR) -type d)
SRCS	:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
INCLUDES := $(addprefix -I,$(SRCDIRS))

CXX		:= c++
CXXFLAGS:= -g -Wall -Werror -Wextra -std=c++98 -pedantic

BUILDDIR:= build/
OBJDIR	:= $(BUILDDIR)release/
OBJDIRS := $(SRCDIRS:$(SRCDIR)%=$(OBJDIR)%)
OBJS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.o)
DEPS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.d)

DSTRCTR	:= ./destructor.c

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

clean: FORCE
	$(RM) -rf $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(NAME).dSYM

re: fclean all

run: $(NAME)
	./$(NAME)

norm: FORCE
	@printf "$(RED)"; norminette | grep -v ": OK!" \
	&& exit 1 \
	|| printf "$(GREEN)%s\n$(END)" "Norm OK!"

$(OBJDIRS):
	@mkdir -p $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@printf "$(THIN)$(ITALIC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

-include $(DEPS)

.PHONY: FORCE
FORCE:

###############
# Debug rules #
###############

SAN_OBJDIR	:= $(BUILDDIR)debug/
SAN_OBJDIRS := $(SRCDIRS:$(SRCDIR)%=$(SAN_OBJDIR)%)
SAN_OBJS	:= $(SRCS:$(SRCDIR)%.cpp=$(SAN_OBJDIR)%.o)
SAN_DEPS	:= $(SRCS:$(SRCDIR)%.cpp=$(SAN_OBJDIR)%.d)

-include $(SAN_DEPS)

$(SAN_OBJDIR)%.o: $(SRCDIR)%.cpp
	@printf "$(CYAN)$(ITALIC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

sani: CXXFLAGS += -fsanitize=address
sani: $(SAN_OBJDIRS) $(SAN_OBJS)
	$(CXX) $(CXXFLAGS)  $(SAN_OBJS) -o $(NAME) $(LIBS)

$(SAN_OBJDIRS):
	@mkdir -p $@

$(DSTRCTR):
	curl https://gist.githubusercontent.com/ywake/793a72da8cdae02f093c02fc4d5dc874/raw/destructor.c > $(DSTRCTR)

Darwin_leak: $(DSTRCTR) $(OBJDIRS) $(OBJS)
	$(CXX) -g -std=c++98 $(OBJS) $(DSTRCTR) -o $(NAME) $(LIBS)

Linux_leak: sani

leak: $(shell uname)_leak

##############
# Test rules #
##############

TESTER		 := tester
TESTDIR		 := gtest
GTESTDIR	 := $(TESTDIR)/googletest
GTESTLIB	 := $(GTESTDIR)/gtest.a
GTEST_INCLDS := -I$(GTESTDIR)/gtest $(INCLUDES)
GTEST_FLAGS	 := -fsanitize=address -std=c++11

TESTOBJS_DIR  = $(shell python3 print_newer.py $(OBJDIR) $(SAN_OBJDIR))
TESTOBJS_DIRS = $(SRCDIRS:$(SRCDIR)%=$(TESTOBJS_DIR)%)
TESTOBJS	  = $(SRCS:$(SRCDIR)%.cpp=$(TESTOBJS_DIR)%.o)
TEST_TARGET  := webserv.a
TESTLIBS	 := -lpthread

TESTCASE_DIR := $(TESTDIR)/testcases
TESTCASES	  = $(shell find $(TESTCASE_DIR) -name '*test.cpp')
TESTCASE_DIRS = $(shell find $(TESTCASE_DIR) -type d)
TESTCASE_OBJS = $(TESTCASES:%.cpp=$(BUILDDIR)%.o)
TESTCASE_OBJDIRS = $(TESTCASE_DIRS:%=$(BUILDDIR)%)

$(GTESTLIB)	:
	$(MAKE) -C $(TESTDIR)

$(TESTCASE_OBJDIRS):
	@mkdir -p $@

$(BUILDDIR)gtest/%.o: gtest/%.cpp
	@printf "$(PURPLE)$(ITALIC)"
	$(CXX) $(GTEST_FLAGS) $(GTEST_INCLDS) -MMD -MP -c $< -o $@
	@printf "$(END)"

$(TEST_TARGET): $(TESTOBJS_DIRS) $(TESTOBJS)
	@ar -rcs $@ $(TESTOBJS)

$(TESTER)	: $(GTESTLIB) $(TEST_TARGET) $(TESTCASE_OBJDIRS) $(TESTCASE_OBJS)
	clang++ $(GTEST_FLAGS) $(GTEST_INCLDS) $(GTESTLIB) $(TESTCASE_OBJS) $(TEST_TARGET) $(TESTLIBS) -o $@

gtest    : $(TESTER) FORCE
	./$<
	@rm $(TEST_TARGET)

-include ./test/Makefile

##########
# Colors #
##########

END		= \e[0m
BOLD	= \e[1m
THIN	= \e[2m
ITALIC	= \e[3m
U_LINE	= \e[4m
BLACK	= \e[30m
RED		= \e[31m
GREEN	= \e[32m
YELLOW	= \e[33m
BLUE	= \e[34m
PURPLE	= \e[35m
CYAN	= \e[36m
WHITE	= \e[37m
