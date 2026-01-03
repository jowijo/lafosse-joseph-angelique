#include "Board.hpp"
#include "GameView.hpp"
#include "Menu.hpp"
#include "LevelFactory.hpp"
#include <iostream>

int main() {
    try {
        Menu menu;
        // Le menu retourne maintenant l'ID du niveau choisi (ex: 4), ou 0 si on quitte
        int start_level = menu.run(); 

        // Si l'utilisateur a fermé ou cliqué sur croix sans choisir
        if (start_level <= 0) return 0; 

        int current_level_id = start_level;
        bool game_running = true;

        while (game_running) {
            std::cout << "Chargement du Niveau " << current_level_id << "..." << std::endl;
            Board* game_board = LevelFactory::create_level(current_level_id);

            if (game_board == nullptr) {
                std::cout << "Plus de niveaux disponibles ou fin du jeu !" << std::endl;
                // Optionnel : Retourner au menu au lieu de quitter ?
                // Pour l'instant on quitte.
                break; 
            }

            {
                GameView view(*game_board, "Baba Is You - Niveau " + std::to_string(current_level_id)); 
                GameResult result = view.run();

                if (result == GameResult::NEXT_LEVEL) {
                    std::cout << "Niveau gagne ! Passage au suivant." << std::endl;
                    current_level_id++; 
                } else {
                    std::cout << "Retour au menu ou Quitter." << std::endl;
                    game_running = false; 
                    // Ici, on pourrait relancer le menu si on voulait
                }
            } 
            delete game_board; 
            game_board = nullptr;
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur critique: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Erreur inconnue." << std::endl;
        return 1;
    }
    return 0;
}