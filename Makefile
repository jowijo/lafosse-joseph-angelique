CXX = g++
# permet d'indiquer au compilateur où chercher les fichiers .hpp
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Isrc/core -Isrc/view -Isrc/interfaces -Isrc

# recup options de compilation pour SFML par la commande pkg-config
SFML_FLAGS = $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)
SFML_LIBS = $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)

# nom de l'exécutable
TARGET = baba_is_you

# règle par défaut (celle qui se lance quand on tape juste 'make')
all: $(TARGET)

# on assemble tous les .o pour faire l'exécutable
$(TARGET): main.o Board.o RuleWord.o RuleSystem.o LevelFactory.o GameView.o Menu.o
	$(CXX) $(CXXFLAGS) main.o Board.o RuleWord.o RuleSystem.o LevelFactory.o GameView.o Menu.o -o $(TARGET) $(SFML_LIBS)

# Règles de compilations par fichiers : 

# Main
main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/main.cpp -o main.o

# Core  (modèle)
Board.o: src/core/Board.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/core/Board.cpp -o Board.o

RuleWord.o: src/core/RuleWord.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/core/RuleWord.cpp -o RuleWord.o

RuleSystem.o: src/core/RuleSystem.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/core/RuleSystem.cpp -o RuleSystem.o

LevelFactory.o: src/core/LevelFactory.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/core/LevelFactory.cpp -o LevelFactory.o

# View (vue)
GameView.o: src/view/GameView.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/view/GameView.cpp -o GameView.o

Menu.o: src/view/Menu.cpp
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) -c src/view/Menu.cpp -o Menu.o

# nettoyage (make clean)
clean:
	rm -f *.o $(TARGET)