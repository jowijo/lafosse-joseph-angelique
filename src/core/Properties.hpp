#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <set>
#include <string>

enum class PropertyType {
    YOU,     
    WIN,     
    PUSH,    
    STOP,    
};

class Properties {
private:
    std::set<PropertyType> active_properties;

public:
    Properties() = default;

    void add(PropertyType type) { active_properties.insert(type); }
    void remove(PropertyType type) { active_properties.erase(type); }
    bool has(PropertyType type) const { return active_properties.count(type) > 0; }
    
    // helpers
    bool has_you() const { return has(PropertyType::YOU); }
    bool has_push() const { return has(PropertyType::PUSH); }
};

#endif // PROPERTIES_HPP