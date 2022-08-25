NAME	:= gtest.a
AR		:= ar
ARFLAGS	:= scr
DIR		:= googletest
SRCS	= $(LIBDIR)/googletest/src/gtest_main.cc $(SRCDIR)/gtest-all.cc
OBJS	= $(notdir $(SRCS:.cc=.o))
SRCDIR	:= $(DIR)/gtest
LIBDIR	:= $(DIR)/googletest-release-1.11.0

.PHONY: all
all		: $(NAME)

$(NAME)	: $(OBJS)
	$(AR) $(ARFLAGS) $(DIR)/$(NAME) $^
	$(RM) $(OBJS)

$(LIBDIR):
	mkdir -p $(DIR)
	wget https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz -C $(DIR)
	rm  release-1.11.0.tar.gz

.PHONY: clean
clean:
	$(RM) $(NAME)

fclean: clean
	$(RM) -r $(DIR)

$(SRCDIR): $(LIBDIR)
	python3 $(LIBDIR)/googletest/scripts/fuse_gtest_files.py $(DIR)

$(OBJS)	: $(SRCDIR)
	clang++ -c -std=c++11 $(LIBDIR)/googletest/src/gtest_main.cc $(SRCDIR)/gtest-all.cc -I$(DIR)
