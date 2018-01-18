SRC	=	src/BuildingBuilder.cpp \
		src/CityBuilder.cpp \
		src/Duration.cpp \
		src/Exceptions.cpp \
		src/PlayerBuilder.cpp \
		src/ShipBuilder.cpp \
		src/main.cpp \
		src/gui/AElement.cpp \
		src/gui/Button.cpp \
		src/gui/Game.cpp \
		src/gui/Hud.cpp \
		src/gui/Popin.cpp \
		src/gui/Text.cpp \
		src/gui/TextRender.cpp \
		src/systems/Console.cpp \
		src/systems/Sdl.cpp \
		src/systems/Time.cpp

OBJ	=	$(SRC:.cpp=.o)

NAME =	patrician

CXXFLAGS += -Wextra -Wall -std=c++11
CXXFLAGS += -Ilib -Isrc -I.
CXXFLAGS += `sdl2-config --cflags`
LDXXFLAGS += -lpthread -lm
LDXXFLAGS += -lSDL2 -lSDL2_ttf

RM	= rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) lib/ECS.so $(LDXXFLAGS)

debug:	CXXFLAGS+=-DDEBUG -g
debug:	re

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all debug clean fclean re
