SRC	=	src/BuildingBuilder.cpp \
		src/CityBuilder.cpp \
		src/ShipBuilder.cpp \
		src/main.cpp \
		src/systems/Console.cpp \
		src/systems/Move.cpp \
		src/systems/Time.cpp

OBJ	=	$(SRC:.cpp=.o)

NAME =	patrician

CXXFLAGS += -Wextra -Wall -std=c++11
CXXFLAGS += -Ilib -Isrc
LDXXFLAGS += -lpthread

RM	= rm -f

all: $(NAME)

$(NAME): $(OBJ)
	g++ $(OBJ) -o $(NAME) $(LDXXFLAGS) lib/ECS.so

debug:	CXXFLAGS+=-DDEBUG -g
debug:	re

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all debug clean fclean re
