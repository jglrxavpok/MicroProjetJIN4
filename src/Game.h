//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

enum Sound {A, B, C, D, E, F, G };

class Game{
private:
	std::vector<Sound> music{ Sound::G, Sound::A, Sound::B, Sound::G, Sound::G, Sound::F, Sound::E, Sound::C, Sound::D, Sound::E, Sound::F, Sound::E, Sound::F, Sound::G, Sound::G, Sound::F, Sound::E, Sound::D, Sound::E, Sound::F, Sound::D, Sound::E, Sound::C, Sound::E, Sound::G, Sound::G, Sound::A, Sound::B, Sound::G, Sound::G, Sound::F, Sound::E, Sound::C, Sound::D, Sound::E, Sound::F, Sound::E, Sound::F, Sound::G, Sound::G, Sound::F, Sound::E, Sound::D, Sound::E, Sound::F, Sound::D, Sound::E, Sound::C, Sound::E, Sound::G };
	std::map<Sound, std::shared_ptr<sf::Sound>> sounds;

	std::map<Sound, std::unique_ptr<sf::SoundBuffer>> buffers;

public:
	///Pour charger les buffer pour les sons
	std::unique_ptr<sf::SoundBuffer> loadBuffer(std::string path);

	///Pour charger les sons
	std::shared_ptr<sf::Sound> loadSound(std::unique_ptr<sf::SoundBuffer>& buffer);

	///Pour charger des images
	std::shared_ptr<sf::Image> Game::loadImage(std::string path);

	///initialisation chargement des textures
	Game();

	/// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
	void update();

	///fonction temporaire
	void playSound(Sound sound);

	/// Appelé autant que possible pour afficher le jeu
	/// @param renderTarget fenêtre sur laquelle on dessine
	/// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
	/// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
	void render(sf::RenderWindow& renderTarget, float partialTick);
};