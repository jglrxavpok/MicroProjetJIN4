#ifndef SOUNDH
#define SOUNDH

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <memory>

enum class keyState { blinking, right, wrong, nothing };

class Sound {
private:
	std::unique_ptr<sf::SoundBuffer> buffer;

	///Tous les sons possible à l'anglaise : do = C, Ré = D, ... (une seule gamme sans la note doublée finale)
	std::shared_ptr<sf::Sound> sound;

	///Image de la touche si le joueur appuie sur la bonne touche
	std::shared_ptr<sf::Texture> successKeyImage;

	///Image de la touche si le joueur appuie sur cette touche alors qu'il ne fallait pas
	std::shared_ptr<sf::Texture> wrongKeyImage;

	///Image de la touche quand le joueur doit appuyer sur cette touche
	std::shared_ptr<sf::Texture> pressKeyImage;

	///Utilisé pour onPress pour faire clignoter la touche
	int blink = 0;
	keyState state;

public:
	Sound(std::string note);
	void play();
	void render(sf::RenderWindow& renderTarget, float partialTick, sf::Sprite& keySprite, std::shared_ptr<sf::Texture> noKeyImage);
	void setState(keyState state);
	keyState getState();

	~Sound() = default;
};


#endif // !SOUNDH