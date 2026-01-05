#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Menu {
private:
    sf::RenderWindow window_;
    
    // ressources
    sf::Font font_;
    sf::Texture background_texture_;
    sf::Sprite background_sprite_;
    
    // écran principal
    sf::RectangleShape play_button_;
    sf::Text play_button_text_;

    sf::RectangleShape select_button_; 
    sf::Text select_button_text_;
    
    // écran selection level
    bool in_level_select_ = false; // état pour savoir sur quel écran on est
    std::vector<sf::RectangleShape> level_buttons_;
    std::vector<sf::Text> level_texts_;
    sf::RectangleShape back_button_; // bouton retour
    sf::Text back_text_;
    
    bool is_running_ = true;
    int selected_level_ = 0; // 0 = pas de sélection (ou quitter), >0 = ID du niveau

public:
    Menu();
    // rend l'ID du niveau choisi 1 ou 0 si on quitte
    int run();

private:
    void handle_input(const sf::Event& event);
    void render();
    void init_level_buttons(); //helper

};
#endif // MENU_HPP