#include "LevelFactory.hpp"
#include "Entity.hpp"
#include "RuleWord.hpp"
#include <iostream>

Board* LevelFactory::create_level(int level_id) {
    int width = 18; int height = 12;
    Board* board = new Board(width, height);
    switch (level_id) {
        case 1: build_level_1(board); break;
        case 2: build_level_2(board); break;
        case 3: build_level_3(board); break;
        case 4: build_level_4(board); break;
        default: delete board; return nullptr;
    }
    return board;
}

void LevelFactory::build_level_1(Board* board) {
    for(int x=0; x<board->get_width(); ++x) { board->add_entity(new Wall(x,0)); board->add_entity(new Wall(x,board->get_height()-1)); }
    for(int y=1; y<board->get_height()-1; ++y) { board->add_entity(new Wall(0,y)); board->add_entity(new Wall(board->get_width()-1,y)); }
    for(int y=1; y<5; ++y) board->add_entity(new Wall(10,y));

    board->add_entity(new RuleWord(WordType::BA,2,2)); board->add_entity(new RuleWord(WordType::IS,3,2)); board->add_entity(new RuleWord(WordType::YOU,4,2));
    board->add_entity(new RuleWord(WordType::WA,6,1)); board->add_entity(new RuleWord(WordType::IS,7,1)); board->add_entity(new RuleWord(WordType::STOP,8,1));
    board->add_entity(new RuleWord(WordType::FL,11,2)); board->add_entity(new RuleWord(WordType::IS,12,2)); board->add_entity(new RuleWord(WordType::WIN,13,2));
    board->add_entity(new RuleWord(WordType::RO,2,8)); board->add_entity(new RuleWord(WordType::IS,3,8)); board->add_entity(new RuleWord(WordType::PUSH,4,8));

    board->add_entity(new Baba(3,5)); board->add_entity(new Rock(6,5)); board->add_entity(new Flag(12,8)); 
    board->add_entity(new RuleWord(WordType::WIN,6,8));
    
    board->notify();
    board->set_initial_state();
}

void LevelFactory::build_level_2(Board* board) {
    for(int x=0; x<board->get_width(); ++x) { board->add_entity(new Wall(x,0)); board->add_entity(new Wall(x,board->get_height()-1)); }
    for(int y=1; y<board->get_height()-1; ++y) { board->add_entity(new Wall(0,y)); board->add_entity(new Wall(board->get_width()-1,y)); }
    for(int x=4; x<=12; ++x) {board->add_entity(new Wall(x,3)); board->add_entity(new Wall(x,9)); }
    for(int y=4; y<=8; ++y) { board->add_entity(new Wall(4,y)); board->add_entity(new Wall(12,y)); }

    board->add_entity(new RuleWord(WordType::BA,6,4)); board->add_entity(new RuleWord(WordType::IS,7,4)); board->add_entity(new RuleWord(WordType::YOU,8,4));
    board->add_entity(new RuleWord(WordType::WA,6,6)); board->add_entity(new RuleWord(WordType::IS,7,6)); board->add_entity(new RuleWord(WordType::STOP,8,6));
    board->add_entity(new RuleWord(WordType::FL,1,1)); board->add_entity(new RuleWord(WordType::IS,2,1)); board->add_entity(new RuleWord(WordType::WIN,3,1));

    board->add_entity(new Baba(8,8)); board->add_entity(new Flag(14,6));
    board->notify(); board->set_initial_state();
}

void LevelFactory::build_level_3(Board* board) {
    for(int x=0; x<board->get_width(); ++x) { board->add_entity(new Wall(x,0)); board->add_entity(new Wall(x,board->get_height()-1)); }
    for(int y=1; y<board->get_height()-1; ++y) { board->add_entity(new Wall(0,y)); board->add_entity(new Wall(board->get_width()-1,y)); }
    for(int y=1; y<board->get_height()-1; ++y) board->add_entity(new Rock(7,y));

    board->add_entity(new RuleWord(WordType::BA,1,1)); board->add_entity(new RuleWord(WordType::IS,2,1)); board->add_entity(new RuleWord(WordType::YOU,3,1));
    board->add_entity(new RuleWord(WordType::FL,11,1)); board->add_entity(new RuleWord(WordType::IS,12,1)); board->add_entity(new RuleWord(WordType::WIN,13,1));
    board->add_entity(new RuleWord(WordType::RO,2,5)); board->add_entity(new RuleWord(WordType::IS,3,5)); board->add_entity(new RuleWord(WordType::STOP,4,5));
    board->add_entity(new RuleWord(WordType::PUSH,2,8));

    board->add_entity(new Baba(2,3)); board->add_entity(new Flag(12,6));
    board->notify(); board->set_initial_state();
}

void LevelFactory::build_level_4(Board* board) {
    for(int x=0; x<board->get_width(); ++x) { board->add_entity(new Wall(x,0)); board->add_entity(new Wall(x,board->get_height()-1)); }
    for(int y=1; y<board->get_height()-1; ++y) { board->add_entity(new Wall(0,y)); board->add_entity(new Wall(board->get_width()-1,y)); }
    for(int y=1; y<board->get_height()-1; ++y) board->add_entity(new Wall(9,y));

    board->add_entity(new Baba(3,6));
    board->add_entity(new RuleWord(WordType::BA,2,2)); board->add_entity(new RuleWord(WordType::IS,3,2)); board->add_entity(new RuleWord(WordType::YOU,4,2));
    board->add_entity(new RuleWord(WordType::WA,2,10)); board->add_entity(new RuleWord(WordType::IS,3,10)); board->add_entity(new RuleWord(WordType::STOP,4,10));
    board->add_entity(new RuleWord(WordType::RO,6,4)); board->add_entity(new RuleWord(WordType::IS,7,5)); board->add_entity(new RuleWord(WordType::YOU,6,8));

    for(int x=12; x<16; ++x) board->add_entity(new Wall(x,4));
    for(int x=10; x<14; ++x) board->add_entity(new Wall(x,8));

    board->add_entity(new Rock(11,6)); 
    board->add_entity(new Flag(16,2)); 
    board->add_entity(new RuleWord(WordType::FL,11,10)); board->add_entity(new RuleWord(WordType::IS,12,10)); board->add_entity(new RuleWord(WordType::WIN,13,10));

    board->notify(); board->set_initial_state();
}
