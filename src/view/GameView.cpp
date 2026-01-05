#include "GameView.hpp"
#include "RuleWord.hpp" 
#include <iostream>

bool GameView::load_texture_only(sf::Texture& texture, const std::string& path) {
    if (!texture.loadFromFile(path)) { 
        std::cerr << "WARNING: Could not load asset: " << path << std::endl;
        return false; 
    }
    texture.setSmooth(true); 
    return true;
}

void GameView::scale_sprite_to_tile(sf::Sprite& sprite) {

    const sf::Texture& tex = sprite.getTexture();
    
  
    // Pas de div par 0
    if (tex.getSize().x == 0 || tex.getSize().y == 0) return;

    float scaleX = TILE_SIZE / static_cast<float>(tex.getSize().x);
    float scaleY = TILE_SIZE / static_cast<float>(tex.getSize().y);
 
    sprite.setScale({scaleX, scaleY});
}

void GameView::load_assets() {
    load_texture_only(rock_texture_, "utils/rock.png");
    load_texture_only(wall_texture_, "utils/wall.png");
    load_texture_only(flag_texture_, "utils/flag.png");

    load_texture_only(baba_textures_[Direction::UP],    "utils/baba_up.png");
    load_texture_only(baba_textures_[Direction::DOWN],  "utils/baba_down.png");
    load_texture_only(baba_textures_[Direction::LEFT],  "utils/baba_left.png");
    load_texture_only(baba_textures_[Direction::RIGHT], "utils/baba_right.png");

    load_texture_only(word_textures_[WordType::BA],   "utils/text_baba.png");
    load_texture_only(word_textures_[WordType::RO],   "utils/text_rock.png");
    load_texture_only(word_textures_[WordType::WA],   "utils/text_wall.png");
    load_texture_only(word_textures_[WordType::FL],   "utils/text_flag.png");
    load_texture_only(word_textures_[WordType::IS],   "utils/text_is.png");
    load_texture_only(word_textures_[WordType::YOU],  "utils/text_you.png");
    load_texture_only(word_textures_[WordType::PUSH], "utils/text_push.png");
    load_texture_only(word_textures_[WordType::STOP], "utils/text_stop.png");
    load_texture_only(word_textures_[WordType::WIN],  "utils/text_win.png");
    
    if (font_.openFromFile("utils/font.ttf")) {
        has_font_ = true;
        win_text_.setFont(font_);
        win_text_.setString("NIVEAU REUSSI !");
        win_text_.setCharacterSize(50);
        win_text_.setFillColor(sf::Color::Green);
        win_text_.setOutlineColor(sf::Color::Black);
        win_text_.setOutlineThickness(3);
        
        next_button_text_.setFont(font_);
        next_button_text_.setString("NIVEAU SUIVANT >");
        next_button_text_.setCharacterSize(25);
        next_button_text_.setFillColor(sf::Color::White);
    } else {
        std::cerr << "WARNING: Font not found at utils/font.ttf" << std::endl;
    }
}

GameView::GameView(const Board& board, const std::string& title) 
    : board_(board),
      sprite_generic_(rock_texture_), 
      win_text_(font_),
      next_button_text_(font_)
{
    unsigned int pw = static_cast<unsigned int>(board_.get_width() * TILE_SIZE);
    unsigned int ph = static_cast<unsigned int>(board_.get_height() * TILE_SIZE);
    window_.create(sf::VideoMode({pw, ph}), title);
    window_.setFramerateLimit(30);
    load_assets();

    next_button_.setSize({250.f, 60.f});
    next_button_.setFillColor(sf::Color(0, 100, 255));
    next_button_.setOutlineColor(sf::Color::White);
    next_button_.setOutlineThickness(2);
    float btnX = (pw / 2.0f) - 125;
    float btnY = (ph / 2.0f) + 50; 
    next_button_.setPosition({btnX, btnY});
    
    if (has_font_) {
        sf::FloatRect textRect = next_button_text_.getLocalBounds();
        next_button_text_.setOrigin({textRect.position.x + textRect.size.x/2.0f, textRect.position.y + textRect.size.y/2.0f});
        next_button_text_.setPosition({btnX + 125, btnY + 30});
    }
}

void GameView::draw_entity(const Entity& entity) {
    float x = entity.get_position().x * TILE_SIZE;
    float y = entity.get_position().y * TILE_SIZE;
    sprite_generic_.setPosition({x, y});
    sprite_generic_.setOrigin({0, 0});
    sprite_generic_.setScale({1, 1});
    sprite_generic_.setColor(sf::Color::White);

    const RuleWord* rw = dynamic_cast<const RuleWord*>(&entity);
    if (rw) {
        WordType wt = rw->get_word_type();
        if (word_textures_.count(wt) && word_textures_[wt].getSize().x > 0) {
            sprite_generic_.setTexture(word_textures_[wt], true);
            scale_sprite_to_tile(sprite_generic_);
            window_.draw(sprite_generic_);
        } else {
            sf::RectangleShape r({TILE_SIZE-4, TILE_SIZE-4});
            r.setPosition({x+2, y+2});
            r.setOutlineThickness(2); r.setOutlineColor(sf::Color::Black);
            if(wt==WordType::IS) r.setFillColor(sf::Color::White);
            else r.setFillColor(sf::Color::Cyan);
            window_.draw(r);
        }
        return;
    }

    bool drawn_texture = false;
    if (entity.get_name() == "BABA") {
        Direction d = entity.get_orientation();
        if (baba_textures_.find(d) == baba_textures_.end()) d = Direction::DOWN;
        if (baba_textures_.count(d) && baba_textures_[d].getSize().x > 0) {
            sprite_generic_.setTexture(baba_textures_[d], true);
            drawn_texture = true;
        }
    }
    else if (entity.get_name() == "ROCK" && rock_texture_.getSize().x > 0) {
        sprite_generic_.setTexture(rock_texture_, true);
        drawn_texture = true;
    }
    else if (entity.get_name() == "FLAG" && flag_texture_.getSize().x > 0) {
        sprite_generic_.setTexture(flag_texture_, true);
        drawn_texture = true;
    }
    else if (entity.get_name() == "WALL" && wall_texture_.getSize().x > 0) {
        sprite_generic_.setTexture(wall_texture_, true);
        drawn_texture = true;
    }

    if (drawn_texture) {
        scale_sprite_to_tile(sprite_generic_);
        window_.draw(sprite_generic_);
        return;
    }

    sf::RectangleShape rect({TILE_SIZE - 2, TILE_SIZE - 2});
    rect.setPosition({x + 1, y + 1});
    if (entity.get_properties().has_you()) rect.setFillColor(sf::Color::White); 
    else if (entity.get_properties().has_push()) rect.setFillColor(sf::Color(150, 75, 0)); 
    else if (entity.get_properties().has(PropertyType::WIN)) rect.setFillColor(sf::Color::Yellow); 
    else if (entity.get_properties().has(PropertyType::STOP)) rect.setFillColor(sf::Color(100, 100, 100));
    else rect.setFillColor(sf::Color::Blue);
    window_.draw(rect);
}

void GameView::update() {}

void GameView::handle_input(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        is_running_ = false;
        current_result_ = GameResult::EXIT;
        window_.close();
    }
    if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left && board_.is_won()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
            if (next_button_.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})) {
                is_running_ = false;
                current_result_ = GameResult::NEXT_LEVEL;
                window_.close();
            }
        }
    }
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (!board_.is_won() && keyPressed->code == sf::Keyboard::Key::R) {
            const_cast<Board&>(board_).restart();
            return;
        }
        if (board_.is_won()) return; 
        Direction dir = Direction::NONE;
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Up:    dir = Direction::UP; break;
            case sf::Keyboard::Key::Down:  dir = Direction::DOWN; break;
            case sf::Keyboard::Key::Left:  dir = Direction::LEFT; break;
            case sf::Keyboard::Key::Right: dir = Direction::RIGHT; break;    
            case sf::Keyboard::Key::Z:     
                const_cast<Board&>(board_).undo(); 
                break;
            default: break;
        }
        if (dir != Direction::NONE) const_cast<Board&>(board_).attempt_move(dir); 
    }
}

GameResult GameView::run() {
    while (window_.isOpen() && is_running_) {
        while (const auto event = window_.pollEvent()) {
            handle_input(*event);
        }
        window_.clear(sf::Color::Black);
        for (int y = 0; y < board_.get_height(); ++y) {
            for (int x = 0; x < board_.get_width(); ++x) {
                try {
                    const auto& entities = board_[{x, y}];
                    for (const Entity* e : entities) draw_entity(*e);
                } catch (...) {}
            }
        }
        if (board_.is_won()) {
            sf::RectangleShape overlay({static_cast<float>(window_.getSize().x), static_cast<float>(window_.getSize().y)});
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window_.draw(overlay);
            if (has_font_) {
                sf::FloatRect textRect = win_text_.getLocalBounds();
                win_text_.setOrigin({textRect.position.x + textRect.size.x/2.0f, textRect.position.y + textRect.size.y/2.0f});
                win_text_.setPosition({window_.getSize().x/2.0f, window_.getSize().y/2.0f - 50});
                window_.draw(win_text_);
                window_.draw(next_button_);
                window_.draw(next_button_text_);
            }
        }
        window_.display();
    }
    return current_result_;
}

