# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lsinke <lsinke@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/06/23 19:36:26 by lsinke        #+#    #+#                  #
#    Updated: 2022/06/23 19:36:26 by lsinke        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME := PmergeMe

CC = c++

CFLAGS += -Wall -Werror -Wextra -I.
CFLAGS += -std=c++98 #-O3 -flto -march=native # optimization flags?! sure

SRCS :=	main.cpp				\
		PmergeMe.cpp

OBJD :=	obj/
OBJS :=	$(SRCS:.cpp=.o)
OBJP :=	$(addprefix $(OBJD), $(OBJS))

INCS :=	PmergeMe.hpp			\
		PairIterator.hpp		\
		PairIterator.tpp

all: $(NAME)

$(OBJD)%.o: %.cpp $(INCS)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@c++ $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJP)
	@echo "Compiling main executable"
	@c++ $(CFLAGS) $(OBJP) -o $(NAME)

clean:
	@rm -rf $(OBJD)
	@echo "Done cleaning $(CURDIR)/$(OBJD)"

fclean: clean
	@rm -f $(NAME)
	@echo "Done cleaning executable $(CURDIR)/$(NAME)"

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re