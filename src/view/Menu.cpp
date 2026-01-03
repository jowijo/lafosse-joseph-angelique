#include "Menu.hpp"
#include "LevelFactory.hpp" // Pour connaître le nombre de niveaux
#include <iostream>

Menu::Menu() :
    background_sprite_(background_texture_),
    play_button_text_(font_),
    select_button_text_(font_),
    back_text_(font_)
{
    window_.create(sf::VideoMode({800, 600}), "Baba Is You - Menu");
    window_.setFramerateLimit(60);

    // 1. Chargement Ressources
    if (!font_.openFromFile("utils/font.ttf")) { 
        if (!font_.openFromFile("assets/fonts/font.ttf")) std::cerr << "ERREUR: Police introuvable." << std::endl;
    }

    bool loaded = false;
    if (background_texture_.loadFromFile("utils/babaisyou.png")) loaded = true;
    else if (background_texture_.loadFromFile("utils/babaisyou.jpg")) loaded = true;
    else if (background_texture_.loadFromFile("assets/images/babaisyou.png")) loaded = true;

    if (loaded) {
        background_sprite_.setTexture(background_texture_, true);
        float scaleX = 800.0f / static_cast<float>(background_texture_.getSize().x);
        float scaleY = 600.0f / static_cast<float>(background_texture_.getSize().y);
        background_sprite_.setScale({scaleX, scaleY});
    } else {
        // Fallback procédural
        sf::Image img; img.resize({800, 600}, sf::Color(75, 0, 130));
        for(int i=0; i<100; ++i) img.setPixel({(unsigned)(rand()%800), (unsigned)(rand()%600)}, sf::Color::White);
        if(background_texture_.loadFromImage(img)) background_sprite_.setTexture(background_texture_, true);
    }

    // 2. Init Bouton "JOUER" (Ecran Principal)
    play_button_.setSize({200.f, 60.f});
    play_button_.setPosition({300.f, 350.f}); // Un peu plus haut
    play_button_.setFillColor(sf::Color(255, 0, 128)); 
    play_button_.setOutlineColor(sf::Color::White);
    play_button_.setOutlineThickness(3);

    play_button_text_.setString("JOUER");
    play_button_text_.setCharacterSize(30);
    play_button_text_.setFillColor(sf::Color::White);
    sf::FloatRect pr = play_button_text_.getLocalBounds();
    play_button_text_.setOrigin({pr.position.x + pr.size.x/2.0f, pr.position.y + pr.size.y/2.0f});
    play_button_text_.setPosition({400.f, 380.f});

    // 3. Init Bouton "SELECT LEVEL" (Ecran Principal)
    select_button_.setSize({200.f, 60.f});
    select_button_.setPosition({300.f, 450.f});
    select_button_.setFillColor(sf::Color(0, 100, 255)); // Bleu
    select_button_.setOutlineColor(sf::Color::White);
    select_button_.setOutlineThickness(3);

    select_button_text_.setString("NIVEAUX");
    select_button_text_.setCharacterSize(30);
    select_button_text_.setFillColor(sf::Color::White);
    sf::FloatRect sr = select_button_text_.getLocalBounds();
    select_button_text_.setOrigin({sr.position.x + sr.size.x/2.0f, sr.position.y + sr.size.y/2.0f});
    select_button_text_.setPosition({400.f, 480.f});

    // 4. Init Écran de sélection
    init_level_buttons();
}

void Menu::init_level_buttons() {
    int count = LevelFactory::get_level_count();
    
    float startX = 50.f;
    float startY = 100.f;
    float gapX = 150.f;
    float gapY = 100.f;

    for (int i = 0; i < count; ++i) {
        // Création du bouton carré
        sf::RectangleShape btn({100.f, 80.f});
        
        // Calcul position grille (ex: 4 par ligne)
        float x = startX + (i % 4) * gapX;
        float y = startY + (i / 4) * gapY;
        
        btn.setPosition({x, y});
        btn.setFillColor(sf::Color(50, 50, 50, 200)); // Gris semi-transparent
        btn.setOutlineColor(sf::Color::White);
        btn.setOutlineThickness(2);
        
        level_buttons_.push_back(btn);

        // Texte "Level X"
        sf::Text txt(font_); // Constructeur SFML 3
        txt.setString("Niv " + std::to_string(i + 1));
        txt.setCharacterSize(20);
        txt.setFillColor(sf::Color::White);
        
        sf::FloatRect tr = txt.getLocalBounds();
        txt.setOrigin({tr.position.x + tr.size.x/2.0f, tr.position.y + tr.size.y/2.0f});
        txt.setPosition({x + 50.f, y + 40.f}); // Centre du bouton
        
        level_texts_.push_back(txt);
    }

    // Bouton RETOUR
    back_button_.setSize({150.f, 50.f});
    back_button_.setPosition({50.f, 500.f});
    back_button_.setFillColor(sf::Color(200, 0, 0));
    back_button_.setOutlineColor(sf::Color::White);
    back_button_.setOutlineThickness(2);

    back_text_.setString("RETOUR");
    back_text_.setCharacterSize(24);
    back_text_.setFillColor(sf::Color::White);
    sf::FloatRect br = back_text_.getLocalBounds();
    back_text_.setOrigin({br.position.x + br.size.x/2.0f, br.position.y + br.size.y/2.0f});
    back_text_.setPosition({125.f, 525.f});
}

int Menu::run() {
    while (window_.isOpen() && is_running_) {
        while (const auto event = window_.pollEvent()) {
            handle_input(*event);
        }
        render();
    }
    return selected_level_;
}

void Menu::handle_input(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        is_running_ = false;
        selected_level_ = 0; // Quitter
        window_.close();
    }
    
    if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
            sf::Vector2f mPosF = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};

            if (!in_level_select_) {
                // --- ECRAN ACCUEIL ---
                // Clic sur JOUER (Niveau 1 par défaut)
                if (play_button_.getGlobalBounds().contains(mPosF)) {
                    selected_level_ = 1;
                    is_running_ = false;
                    window_.close();
                }
                // Clic sur NIVEAUX
                else if (select_button_.getGlobalBounds().contains(mPosF)) {
                    in_level_select_ = true;
                }
            } else {
                // --- ECRAN SELECTION ---
                // Clic sur RETOUR
                if (back_button_.getGlobalBounds().contains(mPosF)) {
                    in_level_select_ = false;
                }
                // Clic sur un NIVEAU
                for (size_t i = 0; i < level_buttons_.size(); ++i) {
                    if (level_buttons_[i].getGlobalBounds().contains(mPosF)) {
                        selected_level_ = static_cast<int>(i + 1);
                        is_running_ = false;
                        window_.close();
                        return;
                    }
                }
            }
        }
    }
}

void Menu::render() {
    window_.clear(sf::Color::Black);
    
    // Fond commun
    window_.draw(background_sprite_);

    if (!in_level_select_) {
        // Menu Principal
        window_.draw(play_button_);
        window_.draw(play_button_text_);
        
        window_.draw(select_button_);
        window_.draw(select_button_text_);
    } else {
        // Sélection Niveau
        // Fond semi-transparent pour lisibilité
        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window_.draw(overlay);

        for (const auto& btn : level_buttons_) window_.draw(btn);
        for (const auto& txt : level_texts_) window_.draw(txt);
        
        window_.draw(back_button_);
        window_.draw(back_text_);
    }

    window_.display();
}