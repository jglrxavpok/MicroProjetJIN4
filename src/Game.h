//
// Created by jglrxavpok on 28/05/2020.
//
#pragma once

#ifndef GAMEH
#define GAMEH

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "Sound.h"


///Pour charger les buffer pour les sons
std::unique_ptr<sf::SoundBuffer> loadBuffer(std::string path);

///Pour charger les sons
std::shared_ptr<sf::Sound> loadSound(std::unique_ptr<sf::SoundBuffer>& buffer);

///Pour charger des images
std::shared_ptr<sf::Texture> loadImage(std::string path);



class Game{
public: 
	///Pour rien afficher sur le clavier utiliser cette texture pour le sprite "keySprite"
	std::shared_ptr<sf::Texture> noKeyImage = loadImage("resources/no_key.png");

private:
	std::vector<std::string> names = { "A", "B", "C", "D", "E", "F", "G" };

	///Musique 1
	///std::vector<Sound> music{ Sound::G, Sound::A, Sound::B, Sound::G, Sound::G, Sound::F, Sound::E, Sound::C, Sound::D, Sound::E, Sound::F, Sound::E, Sound::F, Sound::G, Sound::G, Sound::F, Sound::E, Sound::D, Sound::E, Sound::F, Sound::D, Sound::E, Sound::C, Sound::E, Sound::G, Sound::G, Sound::A, Sound::B, Sound::G, Sound::G, Sound::F, Sound::E, Sound::C, Sound::D, Sound::E, Sound::F, Sound::E, Sound::F, Sound::G, Sound::G, Sound::F, Sound::E, Sound::D, Sound::E, Sound::F, Sound::D, Sound::E, Sound::C, Sound::E, Sound::G };

	///Image de fond du clavier dans sa globalité
	std::shared_ptr<sf::Texture> keyboardImage;

	std::map<std::string, std::shared_ptr<Sound>> notes;

	///Dernière image de note utilisée
	std::shared_ptr<Sound> lastPressKey;

	sf::Sprite keySprite;
	sf::Sprite keyBoardSprite;

public:
	///initialisation chargement des textures
	Game();

	void playSound(std::string note);

	/// Appelé toutes les TARGET_UPDATE_PERIOD ms pour mettre à jour l'état du jeu
	void update();

	void playMusic();

	/// Appelé autant que possible pour afficher le jeu
	/// @param renderTarget fenêtre sur laquelle on dessine
	/// @param partialTick nombre entre 0 et 1 pour donner la progression entre deux ticks
	/// (permet d'avoir des animations propres même si update est appelé à un rythme pas suffisant pour cela, par exemple 10Hz)
	void render(sf::RenderWindow& renderTarget, float partialTick);
};

#endif // !GAMEH