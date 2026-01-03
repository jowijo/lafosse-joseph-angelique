CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

# Inclusions
INCLUDES = -Isrc/core -Isrc/view -Isrc/interfaces -Isrc

# SFML via pkg-config (Trouvera la 3.0 sur Arch)
SFML_CFLAGS = $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)
SFML_LIBS = $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)

# Liste des sources
SRCS = src/main.cpp \
       src/core/Board.cpp \
       src/core/RuleWord.cpp \
       src/core/RuleSystem.cpp \
       src/core/LevelFactory.cpp \
       src/view/GameView.cpp \
       src/view/Menu.cpp

# Génération des noms d'objets dans le dossier obj/
OBJS = $(SRCS:src/%.cpp=obj/%.o)

TARGET = baba_is_you

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(SFML_LIBS)

# Règle de compilation
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SFML_CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -rf obj $(TARGET)

.PHONY: all clean