##
## Makefile for spider in /home/matteo/rendu/spider
## 
## Made by matteo
## Login   <matteo.margueritte@epitech.eu>
## 
## Started on  Mon Oct  2 18:05:28 2017 matteo
## Last update Sun Oct  8 20:28:06 2017 matteo
##

BLUE		=	\033[1;34m

CYAN		=	\033[1;36m

RED		=	\033[1;31m

RESET		=	\033[0m

SRCDIR=	src/

SRC=		$(SRCDIR)BoostServer.cpp \
		$(SRCDIR)Server.cpp \
		$(SRCDIR)Session.cpp \
		$(SRCDIR)Core.cpp \
		$(SRCDIR)CommandHandler.cpp \
		$(SRCDIR)main.cpp \
		$(SRCDIR)Parser.cpp \
		$(SRCDIR)DataHandler.cpp \
		$(SRCDIR)DataWriter.cpp \
		$(SRCDIR)KeystrokeData.cpp \
		$(SRCDIR)MouseData.cpp \
		$(SRCDIR)StatusData.cpp \

OBJS=		$(SRC:.cpp=.o)

RM=		rm -f

CXXFLAGS+=	-W -Wall -Wextra -std=c++17

LDFLAGS=	-lboost_system -lssl -lcrypto -lpthread

GXX=		g++

NAME=		server

ECHO=		echo -e

%.o: %.cpp
	@$(GXX) $(CXXFLAGS) -c -o $@ $<
	@$(ECHO) "${BLUE}[${CYAN}OK${BLUE}]" $@ "${RESET}"

all: $(NAME) $(CLIENT)

debug: CXXFLAGS += -g3 -Wfatal-errors
debug: fclean $(NAME)

$(NAME): $(OBJS)
	@$(GXX) -o $(NAME) $(OBJS) $(LDFLAGS)
	@$(ECHO) "${RED}${NAME} ${BLUE}Compiled${RESET}"

clean:
	@$(RM) $(OBJS)
	@$(ECHO) "${RED}Objects${BLUE} Cleaned${RESET}"

fclean: clean
	@$(RM) $(NAME)
	@$(ECHO) "${RED}${NAME} ${BLUE} Cleaned${RESET}"

re: fclean all

.PHONY: all clean fclean re
