#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Position.hpp" 
#include "Properties.hpp" 
#include <string>

enum class Direction : int; 

class Entity {
protected: 
    const std::string name_; 
private: 
    Position pos_; 
    Properties current_properties_; 
    Direction orientation_ = (Direction)3; // RIGHT

public:
    Entity(const std::string& name, int x, int y) : name_(name), pos_(x, y) {}
    virtual ~Entity() {} 

    const Position& get_position() const { return pos_; }
    const Properties& get_properties() const { return current_properties_; }
    const std::string& get_name() const { return name_; }
    Direction get_orientation() const { return orientation_; }
    
    void set_orientation(Direction dir) { orientation_ = dir; }
    void set_position(const Position& new_pos) { pos_ = new_pos; }

    void add_property(PropertyType type) { current_properties_.add(type); }
    void remove_property(PropertyType type) { current_properties_.remove(type); }
    
    virtual Entity* clone() const = 0; 
    virtual bool is_solid() const { return false; }
};

// Classes concrètes

class Rock : public Entity {
public:
    Rock(int x, int y) : Entity("ROCK", x, y) { add_property(PropertyType::PUSH); }
    Entity* clone() const override { return new Rock(*this); }
    bool is_solid() const override { return true; }
};

class Baba : public Entity {
public:
    Baba(int x, int y) : Entity("BABA", x, y) { add_property(PropertyType::YOU); }
    Entity* clone() const override { return new Baba(*this); }
    bool is_solid() const override { return true; }
};

class Flag : public Entity {
public:
    Flag(int x, int y) : Entity("FLAG", x, y) { add_property(PropertyType::WIN); }
    Entity* clone() const override { return new Flag(*this); }
    bool is_solid() const override { return false; }
};

class Wall : public Entity {
public:
    Wall(int x, int y) : Entity("WALL", x, y) { add_property(PropertyType::STOP); }
    Entity* clone() const override { return new Wall(*this); }
    bool is_solid() const override { return false; }
};

#endif // ENTITY_HPP