#include "RuleSystem.hpp"
#include "Board.hpp" 
#include "RuleWord.hpp"
#include <algorithm>
#include <iostream>

// Implémentation de Rule::to_string (parfois placée ici ou dans Rule.cpp si existant, 
// mais pour ce projet on l'a souvent mise avec RuleWord ou ici. 
// Pour éviter les erreurs "undefined reference", mettons-la ici si elle n'est pas dans RuleWord.cpp)
// VERIFICATION: Dans la structure actuelle, Rule::to_string est souvent oubliée. 
// Ajoutons-la ici pour être sûr.

std::string Rule::to_string() const {
    // Note: WORD_NAMES est accessible via RuleWord.hpp
    return WORD_NAMES.at(subject) + " IS " + WORD_NAMES.at(predicate) + (is_horizontal ? " (H)" : " (V)");
}

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
                    rules.push_back({word1->get_word_type(), WordType::IS, word3->get_word_type(), true});
                }
            }
        }
    }
    return rules;
}

std::vector<Rule> RuleSystem::find_active_rules(const Board& board) {
    return find_horizontal_rules(board);
}

void RuleSystem::apply_rules(Board& board, const std::vector<Rule>& active_rules) {
    // Nettoyage
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
    // Application
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