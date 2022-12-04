MAKEFILES_DIR := makefiles/
-include $(MAKEFILES_DIR)color.mk

#############
# Variables #
#############

NAME	:= webserv
SRCDIR  := srcs/
SRCDIRS := $(shell find $(SRCDIR) -type d)
SRCS	:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
INCLUDES := $(addprefix -I,$(SRCDIRS))

CXX		:= c++
CXXFLAGS:= -Wall -Werror -Wextra -std=c++98 -pedantic -O2

BUILDDIR:= build/
OBJDIR	:= $(BUILDDIR)release/
OBJDIRS := $(SRCDIRS:$(SRCDIR)%=$(OBJDIR)%)
OBJS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.o)
DEPS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.d)
.PHONY: $(DEPS)

DSTRCTR	:= ./destructor.c

#################
# General rules #
#################

.PHONY: all
all: $(NAME)

$(NAME): $(OBJDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

clean: FORCE
	$(RM) -rf $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(NAME).dSYM
	$(RM) $(NAME).a
	$(RM) gtest/googletest/gtest.a

.PHONY: re
re: fclean all

run: $(NAME)
	./$(NAME)

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
-include $(MAKEFILES_DIR)debug.mk

##############
# Test rules #
##############
-include $(MAKEFILES_DIR)test.mk
-include ./test/Makefile
