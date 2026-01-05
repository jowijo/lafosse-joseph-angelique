#include "RuleSystem.hpp"
#include "Board.hpp" 
#include "RuleWord.hpp"
#include <algorithm>
#include <iostream>

bool RuleSystem::is_subject(WordType word) const {
    return word == WordType::BA || word == WordType::RO || word == WordType::WA || word == WordType::FL;
}

bool RuleSystem::is_property_predicate(WordType word) const {
    return word == WordType::YOU || word == WordType::PUSH || word == WordType::STOP || word == WordType::WIN;
}

std::vector<Rule> RuleSystem::find_horizontal_rules(const Board& board) {
    std::vector<Rule> rules;
    for (int y = 0; y < board.get_height(); ++y) {
        for (int x = 0; x < board.get_width() - 2; ++x) {
            const auto& list1 = board[{x, y}];
            const auto& list2 = board[{x + 1, y}];
            const auto& list3 = board[{x + 2, y}];

            RuleWord* word1 = nullptr;
            for(Entity* e : list1) if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word1 = rw; break; }
            RuleWord* word2 = nullptr;
            for(Entity* e : list2) if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word2 = rw; break; }
            RuleWord* word3 = nullptr;
            for(Entity* e : list3) if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word3 = rw; break; }

            if (word1 && word2 && word3) {
                if (is_subject(word1->get_word_type()) && 
                    word2->get_word_type() == WordType::IS && 
                    is_property_predicate(word3->get_word_type())) 
                {
                    rules.push_back({word1->get_word_type(), WordType::IS, word3->get_word_type(), true}); // true = horizontal
                }
            }
        }
    }
    return rules;
}

//new : detection verticale
std::vector<Rule> RuleSystem::find_vertical_rules(const Board& board) {
    std::vector<Rule> rules;
    // on parcourt les colonnes x d'abord, puis les lignes y jusqu'à height-2
    for (int x = 0; x < board.get_width(); ++x) {
        for (int y = 0; y < board.get_height() - 2; ++y) {
            
            // on regarde (x, y), (x, y+1), (x, y+2)
            const auto& list1 =board[{x, y}];
            const auto& list2 =board[{x, y + 1}];
            const auto& list3 =board[{x, y + 2}];

            RuleWord* word1 = nullptr;
            for(Entity* e : list1) if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word1 = rw; break; }
            RuleWord* word2 = nullptr;
            for(Entity* e : list2)if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word2 = rw; break; }
            RuleWord* word3 = nullptr;
            for(Entity* e : list3) if (RuleWord* rw = dynamic_cast<RuleWord*>(e)) { word3 = rw; break; }

            if (word1 && word2 && word3) {
                if (is_subject(word1->get_word_type()) && 
                    word2->get_word_type()== WordType::IS && 
                    is_property_predicate(word3->get_word_type())) 
                {
                    rules.push_back({word1->get_word_type(), WordType::IS, word3->get_word_type(), false});
                }
            }
        }
    }
    return rules;
}

std::vector<Rule> RuleSystem::find_active_rules(const Board& board) {
    std::vector<Rule> rules = find_horizontal_rules(board);
    
    std::vector<Rule> vertical_rules =find_vertical_rules(board);
    
    rules.insert(rules.end(), vertical_rules.begin(), vertical_rules.end());
    
    return rules;
}

void RuleSystem::apply_rules(Board& board, const std::vector<Rule>& active_rules) {
    for (int y = 0; y < board.get_height(); ++y) {
        for (int x = 0; x < board.get_width(); ++x) {
            const auto& list = board[{x, y}];
            for (Entity* entity : list) {
                if (dynamic_cast<RuleWord*>(entity) == nullptr) {
                    entity->remove_property(PropertyType::YOU);
                    entity->remove_property(PropertyType::PUSH);
                    entity->remove_property(PropertyType::STOP);
                    entity->remove_property(PropertyType::WIN);
                }
            }
        }
    }
    for (const auto& rule : active_rules) {
        std::string subject_name = WORD_NAMES.at(rule.subject);
        PropertyType property_to_apply = RuleWord::word_to_property(rule.predicate);

        for (int y = 0; y < board.get_height(); ++y) {
            for (int x = 0; x < board.get_width(); ++x) {
                const auto& list = board[{x, y}];
                for (Entity* entity : list) {
                    if (dynamic_cast<RuleWord*>(entity) != nullptr) continue;
                    if (entity->get_name() == subject_name) {
                        entity->add_property(property_to_apply);
                    }
                }
            }
        }
    }
}