#include "RuleWord.hpp"
#include "Rule.hpp" 
#include <stdexcept>

// var globales declarées extern dans le .hpp
const std::map<WordType, std::string> WORD_NAMES = {
    {WordType::BA, "BABA"}, {WordType::RO, "ROCK"}, {WordType::WA, "WALL"}, {WordType::FL, "FLAG"},
    {WordType::IS, "IS"}, {WordType::YOU, "YOU"}, {WordType::PUSH, "PUSH"}, {WordType::STOP, "STOP"}, {WordType::WIN, "WIN"}
};

const std::map<WordType, PropertyType> ENTITY_WORD_MAPPING = {
    {WordType::BA, PropertyType::YOU}, {WordType::RO, PropertyType::PUSH}
};

RuleWord::RuleWord(WordType type, int x, int y) 
    : Entity(WORD_NAMES.at(type), x, y), word_type_(type) 
{
    // tous les mots sont PUSH par défaut
    add_property(PropertyType::PUSH);
}

Entity* RuleWord::clone() const {
    return new RuleWord(word_type_, get_position().x, get_position().y);
}

PropertyType RuleWord::word_to_property(WordType word) {
    switch (word) {
        case WordType::YOU: return PropertyType::YOU;
        case WordType::PUSH: return PropertyType::PUSH;
        case WordType::STOP: return PropertyType::STOP;
        case WordType::WIN: return PropertyType::WIN;
        default: throw std::runtime_error("Word is not a property.");
    }
}

PropertyType RuleWord::word_to_entity_property(WordType word) {
    if (word == WordType::BA) return PropertyType::YOU;
    if (word == WordType::RO) return PropertyType::PUSH;
    if (word == WordType::FL) return PropertyType::WIN;
    if (word == WordType::WA) return PropertyType::STOP;
    throw std::runtime_error("Word is not a known entity subject.");
}