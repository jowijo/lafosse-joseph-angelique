#ifndef BOARD_HPP
#define BOARD_HPP

#include "ISubject.hpp" 
#include "Entity.hpp"
#include "Position.hpp"
#include <vector>
#include <list>
#include <stack>
#include <string>

class RuleSystem; 

enum class Direction { UP, DOWN, LEFT, RIGHT, NONE };

struct BoardState {
    std::vector<std::vector<std::list<Entity*>>> grid;
};

class Board : public ISubject {
private:
    const int width_;
    const int height_;
    
    std::vector<std::vector<std::list<Entity*>>> grid_;
    std::vector<IObserver*> observers_; 
    std::stack<BoardState> history_; 
    BoardState initial_state_;
    bool has_initial_state_ = false;
    RuleSystem* rule_system_; 
    bool is_win_ = false;

private:
    bool move_entity_to(Entity* entity, const Position& target_pos);
    void clear_grid(std::vector<std::vector<std::list<Entity*>>>& grid_to_clear);
    BoardState copy_state() const;
    bool can_push_chain(const Position& start_pos, Direction direction, std::list<Entity*>& entities_to_move);
    Position get_next_position(const Position& current_pos, Direction direction) const;
    void check_win_condition();

public:
    Board(int width, int height);
    ~Board();

    int get_width() const { return width_; }
    int get_height() const { return height_; }
    bool is_won() const { return is_win_; }

    const std::list<Entity*>& get_entities_at(const Position& pos) const;
    void add_entity(Entity* entity);
    bool attempt_move(Direction direction);
    void move_player_entity(Direction direction);

    void save_state();
    bool undo();
    void set_initial_state();
    void restart();

    const std::list<Entity*>& operator[](const Position& pos) const;
    std::vector<Entity*> operator[](PropertyType type) const;

    void attach(IObserver& observer) override; 
    void detach(IObserver& observer) override; 
    void notify() override;
};

#endif // BOARD_HPP