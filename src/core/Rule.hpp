#ifndef RULE_HPP
#define RULE_HPP

#include "RuleWord.hpp"
#include <string>

struct Rule {
    WordType subject;     
    WordType verb;        
    WordType predicate;   
    bool is_horizontal; 

    std::string to_string() const;
};

#endif // RULE_HPP