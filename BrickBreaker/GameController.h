#ifndef ALL_GAME_OBJECTS_H
#define ALL_GAME_OBJECTS_H

#include "Ball.h"
#include "Brick.h"
#include "SFML/Audio.hpp"
#include "ColliderEngine.h"
#include "HelperFunctions.h"
#include "Player.h"
#include <vector>
#include <sstream>

/*Class will be a singleton that acts as our game controller. It will contain
  all game objects, it will also control gamestates and rendering */
class GameController {
private:

#pragma region MEMBER VARIABLES

#pragma region SINGLETON VARIABLES
    //The static pointer to the singleton instance
    static GameController*      _instance;

    /*A static counter variable used to track how many users there are of the
    Singleton to help prevent premature deletion */
    static int                  _counter;
#pragma endregion


#pragma region GAMEOBJECT VARIABLES
    /* A few vectors that can hold balls, bricks or player objects*/
    std::vector<Ball>           _balls;
    std::vector<Brick>          _bricks;
    std::vector<Player>         _players;

    /* The sound object used for a brick breaking */
    sf::Sound                   _brick_explosion;
    sf::Sound                   _ball_bounce;

    /* Various text display objects for the player to see*/
    sf::Text                    _score_text;
    sf::Text                    _title_text;
    sf::Text                    _game_over_text;
    sf::Text                    _high_score_text;
    sf::Text                    _instructions_text;

    /* The game object for our background */
    sf::Sprite                  _background;
#pragma endregion


#pragma region GLOBAL GAME VARIABLES
    /*As we are only using two sounds, a single sound
        buffer here will suffice */
    sf::SoundBuffer _sound_buffer;

    unsigned int			    _high_score;            //Holds the games high score
    unsigned int                _total_score;           //Holds the current score
    bool                        _ball_launched;         //Shows if the ball has been launched
    bool                        _set_new_high_score;    //Determines if a new high score was hit
    sf::Font                    _font;                  //Holds the font our game uses
    helpers::GAMESTATE          _game_state;            //Holds the games gamestate

#pragma endregion


#pragma region TEXTURE VARIABLES

    /* Holds the possible textures used on our brick objects*/
    std::vector<sf::Texture*>   _brick_textures;

    /* Holds the texture for the background objects */
    sf::Texture* _background_texture;
#pragma endregion 
    
#pragma endregion

#pragma region MEMBER FUNCTIONS

#pragma region SINGLETON FUNCTIONS
    /*Function to add a reference, called when GetInstance is called and
      increases the counter by one */
    static void AddReference();

    /*Function to remove a reference, called when ReleaseInstance is called and
      decreases the counter by one */
    static void RemoveReference();

    /*Constructor is private so no objects can be created,
this is a singleton class */
    GameController();

    /*Deconstructor private as we will be explicitly deleting reference*/
    ~GameController();

#pragma endregion


#pragma region COLLISION FUNCTIONS
    /* Function that will test for collisions of objects */
    void TestForCollisions();

    /* Function will test objects to see if they need to be destroyed
        if one does, then do certain things if needed. IE: Trigger Game Over when
        ball is destroyed, trigger a varied game over(beaten) when all bricks are
        destroyed. Handle what message is shown on game over screen based on if the 
        game was beat, new high score, or just the score*/
    void TestForDestroys();
#pragma endregion


#pragma region UPDATE FUNCTIONS

    /* Updates the core game objects (Player and Ball) This includes calling their
        update functions processing movement, etc */
    void UpdateObjects(sf::Vector2f constraints);

    /* Function will call the draw method of all gameobjects that the
      singleton holds */
    void UpdateGame(sf::RenderWindow* render_window);

    /* Function will check if the high score has been beaten and update the text object if so */
    void UpdateHighScore(sf::RenderWindow* render_window);

#pragma endregion


#pragma region CREATION FUNCTIONS
    /* Function to create and add a ball game object to the singleton */
    void CreateBall(Ball ball);

    /* Function will create and add a brick game object to the singleton */
    void CreateBrick(Brick brick);

    /* Function will create our grid of brick and add them to the singleton */
    void CreateBricks(sf::RenderWindow* render_window);

    /* Function will create and add our game over text object to the singleton */
    void CreateGameOverTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT);

    /* Function will create and add our high score text object to the singleton */
    void CreateHighScoreTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT);

    /* Function will create and add our instructions text object to the singleton */
    void CreateInstructionsTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT);

    /* Function will create and add a player game object to the singleton */
    void CreatePlayer(Player player);

    /* Function will create and add our score text object to the singleton */
    void CreateScoreTextObject();

    /* Function will create and add our title text object to the singleton */
    void CreateTitleTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT);

    /* Function will return a random texture from our textures, this is used in our 
    CreateBricks function to randomize the looks of the bricks optionally takes a 
    pointer to and int intended to randomly assign a score value along with the colors
    ie all blue will be 15, etc */
    sf::Texture* RandomTexture(std::vector<sf::Texture*>* textures, int* score = nullptr);

#pragma endregion


#pragma region EVENT POLLING FUNCTIONS
    /*Event polling function for our PLAY game state */
    void PollGameKeyEvents(sf::RenderWindow* render_window, sf::Event* event);

    /*Event polling function for our GAMEOVER game state */
    void PollGameOverKeyEvents(sf::RenderWindow* render_window, sf::Event* event);

    /*Event polling function for our TITLE game state */
    void PollTitleKeyEvents(sf::RenderWindow* render_window, sf::Event* event);
#pragma endregion


#pragma region GAMESTATE RUN FUNCTIONS
    
    /* Game loop function for our PLAY game state */
    void RunGame(sf::RenderWindow* render_window,
        unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT,
        sf::Event* primary_event);

    /* Game loop function for our GAMEOVER game state */
    void RunGameOverScreen(sf::RenderWindow* render_window,
        sf::Event* primary_event);

    /* Game loop function for our TITLE game state */
    void RunTitleScreen(sf::RenderWindow* render_window,
        sf::Event* primary_event);

#pragma endregion


#pragma region GAME SETUP / RESET FUNCTIONS

    /* Resets important game variables then calls SetupGame to 
        be ready for a new game! */
    void ResetGame(sf::RenderWindow* render_window);

    /* Function that will initialize game variables, called after our overall 
        game initialization it will instantiated game objects needed */
    void SetupGame(sf::RenderWindow* render_window,
        unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

#pragma endregion

    /*Function plays the sound of the ball hitting the player */
    void PlayBounceSound();

    /*Function plays the sound of the ball hitting the brick */
    void PlayBrickSound();
    
    /* Function to draw game objects to the render window */
    void DrawObjects(sf::RenderWindow* render_window);

#pragma endregion

public:

#pragma region SINGLETON FUNCTIONS
    /*Returns a pointer to the instance of this singleton,
   if none currently exists, then creates one */
    static GameController* GetInstance();

    /*Function to release the instance, USE WHEN DONE with an instance */
    static void ReleaseInstance();
#pragma endregion

    /* Function initializes our game by loading and assigning assets */
    void InitializeGame(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

    /* Primary Game Function! This functions is our game state manager and will
        call the proper functions based on the game state. */
    void RunGameControl(sf::RenderWindow* render_window, 
        unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

};

#endif
