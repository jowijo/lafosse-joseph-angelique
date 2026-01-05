#ifndef RULESYSTEM_HPP
#define RULESYSTEM_HPP

#include "Rule.hpp"
#include <vector>

class Board; 

class RuleSystem {
public:
    RuleSystem() = default;
    std::vector<Rule> find_active_rules(const Board& board);
    void apply_rules(Board& board, const std::vector<Rule>& active_rules);

private:
    std::vector<Rule> find_horizontal_rules(const Board& board);
    // methode pour verif verticale aussi
    std::vector<Rule> find_vertical_rules(const Board& board);
    
    bool is_subject(WordType word) const;
    bool is_property_predicate(WordType word) const;
};

#endif // RULESYSTEM_HPP