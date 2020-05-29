//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

enum class Sound {A, B, C, D, E, F, G };

class Game{
private:
	std::shared_ptr<sf::Sound> soundA;
	std::shared_ptr<sf::Sound> soundB;
	std::shared_ptr<sf::Sound> soundC;
	std::shared_ptr<sf::Sound> soundD;
	std::shared_ptr<sf::Sound> soundE;
	std::shared_ptr<sf::Sound> soundF;
	std::shared_ptr<sf::Sound> soundG;

	std::unique_ptr<sf::SoundBuffer> bufferA;
	std::unique_ptr<sf::SoundBuffer> bufferB;
	std::unique_ptr<sf::SoundBuffer> bufferC;
	std::unique_ptr<sf::SoundBuffer> bufferD;
	std::unique_ptr<sf::SoundBuffer> bufferE;
	std::unique_ptr<sf::SoundBuffer> bufferF;
	std::unique_ptr<sf::SoundBuffer> bufferG;

public:
	///Pour charger les sons
	std::shared_ptr<sf::Sound> loadSound(std::string path);

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