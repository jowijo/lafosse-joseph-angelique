#include "Board.hpp"
#include "RuleSystem.hpp"
#include "RuleWord.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

Position Board::get_next_position(const Position& current_pos, Direction direction) const {
    Position next_pos = current_pos;
    switch (direction) {
        case Direction::UP:    next_pos.y -= 1; break;
        case Direction::DOWN:  next_pos.y += 1; break;
        case Direction::LEFT:  next_pos.x -= 1; break;
        case Direction::RIGHT: next_pos.x += 1; break;
        case Direction::NONE: break;
    }
    return next_pos;
}

void Board::clear_grid(std::vector<std::vector<std::list<Entity*>>>& grid_to_clear) {
    for (auto& row : grid_to_clear) {
        for (auto& list : row) {
            for (Entity* entity : list) {
                delete entity; 
            }
            list.clear();
        }
    }
}

BoardState Board::copy_state() const {
    BoardState state;
    state.grid.resize(height_, std::vector<std::list<Entity*>>(width_));
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            for (Entity* entity : grid_[y][x]) {
                state.grid[y][x].push_back(entity->clone());
            }
        }
    }
    return state;
}

void Board::save_state() { history_.push(copy_state()); }

bool Board::undo() {
    if (history_.empty()) return false;
    clear_grid(grid_);
    grid_ = history_.top().grid;
    history_.pop();
    is_win_ = false;
    notify();
    return true;
}

void Board::set_initial_state() {
    if (has_initial_state_) clear_grid(initial_state_.grid);
    initial_state_ = copy_state();
    has_initial_state_ = true;
}

void Board::restart() {
    if (!has_initial_state_) return;
    clear_grid(grid_);
    while (!history_.empty()) { clear_grid(history_.top().grid); history_.pop(); }
    grid_.resize(height_, std::vector<std::list<Entity*>>(width_));
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            for (Entity* entity : initial_state_.grid[y][x]) {
                grid_[y][x].push_back(entity->clone());
            }
        }
    }
    is_win_ = false;
    notify();
}

bool Board::can_push_chain(const Position& start_pos, Direction direction, std::list<Entity*>& entities_to_move) {
    if (start_pos.x < 0 || start_pos.x >= width_ || start_pos.y < 0 || start_pos.y >= height_) return false; 
    const std::list<Entity*>& current_entities = (*this)[start_pos];
    if (current_entities.empty()) return true; 

    bool chain_continues = false;
    for (Entity* entity : current_entities) {
        if (entity->get_properties().has(PropertyType::STOP)) return false; 
        if (entity->get_properties().has(PropertyType::PUSH)) {
            chain_continues = true;
            entities_to_move.push_back(entity);
        }
        else if (!entity->get_properties().has(PropertyType::YOU) && !entity->get_properties().has(PropertyType::WIN)) {
            if (entity->is_solid()) return false; 
        }
    }
    if (chain_continues) {
        Position next_pos = get_next_position(start_pos, direction);
        return can_push_chain(next_pos, direction, entities_to_move);
    }
    return true; 
}

bool Board::move_entity_to(Entity* entity, const Position& target_pos) {
    if (target_pos.x < 0 || target_pos.x >= width_ || target_pos.y < 0 || target_pos.y >= height_) return false;
    Position current_pos = entity->get_position();
    grid_[current_pos.y][current_pos.x].remove(entity);
    entity->set_position(target_pos);
    grid_[target_pos.y][target_pos.x].push_back(entity);
    return true;
}

const std::list<Entity*>& Board::operator[](const Position& pos) const {
    if (pos.x < 0 || pos.x >= width_ || pos.y < 0 || pos.y >= height_) throw std::out_of_range("Position hors limites.");
    return grid_[pos.y][pos.x];
}

std::vector<Entity*> Board::operator[](PropertyType type) const {
    std::vector<Entity*> result;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            for (Entity* entity : grid_[y][x]) {
                if (entity->get_properties().has(type)) result.push_back(entity);
            }
        }
    }
    return result;
}

void Board::check_win_condition() {
    std::vector<Entity*> you_entities = (*this)[PropertyType::YOU];
    for (Entity* player : you_entities) {
        try {
            const auto& entities_at_pos = (*this)[player->get_position()];
            for (Entity* other : entities_at_pos) {
                if (other != player && other->get_properties().has(PropertyType::WIN)) {
                    is_win_ = true;
                    std::cout << "VICTOIRE !" << std::endl;
                    return;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur check_win: " << e.what() << std::endl;
        }
    }
}

Board::Board(int width, int height) : width_(width), height_(height), grid_(height, std::vector<std::list<Entity*>>(width)) {
    rule_system_ = new RuleSystem(); 
}

Board::~Board() { 
    clear_grid(grid_);
    if(has_initial_state_) clear_grid(initial_state_.grid);
    delete rule_system_;
    while (!history_.empty()) { clear_grid(history_.top().grid); history_.pop(); }
}

void Board::attach(IObserver& observer) { observers_.push_back(&observer); }
void Board::detach(IObserver& observer) { observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end()); }
void Board::add_entity(Entity* entity) {
    if (!entity) return;
    const Position& pos = entity->get_position();
    if (pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_) grid_[pos.y][pos.x].push_back(entity); 
    else { delete entity; }
}
const std::list<Entity*>& Board::get_entities_at(const Position& pos) const { 
    try { 
        return (*this)[pos]; 
    } catch (...) { 
        static std::list<Entity*> e; 
        return e; 
    }
}

bool Board::attempt_move(Direction direction) {
    if (is_win_) return false; 
    std::vector<Entity*> you_entities = (*this)[PropertyType::YOU];
    if (you_entities.empty()) return false; 
    
    save_state();
    bool at_least_one_moved = false;

    for (Entity* player : you_entities) {
        Position current_pos = player->get_position();
        Position next_pos = get_next_position(current_pos, direction);
        player->set_orientation(direction);

        std::list<Entity*> push_chain;
        if (can_push_chain(next_pos, direction, push_chain)) {
            for (Entity* pushed_entity : push_chain) {
                Position target_pos = get_next_position(pushed_entity->get_position(), direction);
                move_entity_to(pushed_entity, target_pos);
            }
            move_entity_to(player, next_pos);
            at_least_one_moved = true;
        }
    }

    if (at_least_one_moved) {
        notify(); 
        check_win_condition();
        return true;
    } else {
        history_.pop(); 
        return false;
    }
}

void Board::move_player_entity(Direction direction) { attempt_move(direction); }

void Board::notify() { 
    try {
        std::vector<Rule> active_rules = rule_system_->find_active_rules(*this);
        rule_system_->apply_rules(*this, active_rules);
    } catch (const std::exception& e) {
        std::cerr << "Erreur regles: " << e.what() << std::endl;
    }
    for (IObserver* observer : observers_) observer->update(); 
}