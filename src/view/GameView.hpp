#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include "IObserver.hpp" 
#include "Board.hpp"
#include "RuleWord.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

enum class GameResult { EXIT, NEXT_LEVEL };

class GameView : public IObserver {
private:
    const Board& board_; 
    sf::RenderWindow window_;
    const float TILE_SIZE = 50.0f; 

    // --- RESSOURCES (Doivent être déclarées AVANT les sprites/textes) ---
    sf::Texture rock_texture_; 
    sf::Texture wall_texture_;
    sf::Texture flag_texture_;
    sf::Font font_; // La police doit exister avant les textes
    
    // Maps de textures (pas de problème d'initialisation ici)
    std::map<Direction, sf::Texture> baba_textures_;
    std::map<WordType, sf::Texture> word_textures_;

    // --- OBJETS GRAPHIQUES (Dépendent des ressources) ---
    sf::Sprite sprite_generic_; // Sprite réutilisable
    sf::Text win_text_;
    sf::RectangleShape next_button_;
    sf::Text next_button_text_;
    
    bool has_font_ = false;
    GameResult current_result_ = GameResult::EXIT;
    bool is_running_ = true;

private:
    void draw_entity(const Entity& entity);
    void handle_input(const sf::Event& event);
    void load_assets();
    void scale_sprite_to_tile(sf::Sprite& sprite); 
    bool load_texture_only(sf::Texture& texture, const std::string& path);

public:
    GameView(const Board& board, const std::string& title);
    void update() override; 
    GameResult run(); 
};

#endif // GAMEVIEW_HPP