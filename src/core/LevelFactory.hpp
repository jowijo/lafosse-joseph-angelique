#ifndef LEVELFACTORY_HPP
#define LEVELFACTORY_HPP

#include "Board.hpp"

class LevelFactory {
public:
    static Board* create_level(int level_id);
    
    // NOUVEAU : Permet au menu de savoir combien de niveaux afficher
    static int get_level_count() { return 5; }

private:
    static void build_level_1(Board* board);
    static void build_level_2(Board* board);
    static void build_level_3(Board* board);
    static void build_level_4(Board* board);
    static void build_level_5(Board* board);
};

#endif // LEVELFACTORY_HPP