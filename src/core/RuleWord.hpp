#ifndef RULEWORD_HPP
#define RULEWORD_HPP

#include "Entity.hpp"
#include <map>

enum class WordType {
    BA, RO, WA, FL, // Sujets
    IS,             // Verbe
    YOU, PUSH, STOP, WIN // Propriétés
};

class RuleWord : public Entity {
private:
    WordType word_type_;

public:
    RuleWord(WordType type, int x, int y);
    virtual ~RuleWord() = default;

    WordType get_word_type() const { return word_type_; }

    static PropertyType word_to_property(WordType word);
    static PropertyType word_to_entity_property(WordType word);

    Entity* clone() const override; 
    bool is_solid() const override { return true; }
};

// Mappings globaux
extern const std::map<WordType, std::string> WORD_NAMES;
extern const std::map<WordType, PropertyType> ENTITY_WORD_MAPPING;

#endif // RULEWORD_HPP