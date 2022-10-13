#include "GameController.h"

//STATIC VARIABLE DEFINITIONS
int GameController::_counter = 0;
GameController* GameController::_instance = nullptr;

#pragma region Private

#pragma region SINGLETON FUNCTIONS

void GameController::AddReference()
{
    //Increase our instance counter by one
    _counter++;
}

void GameController::RemoveReference()
{
    //Decrease our instance counter by one
    _counter--;
}

GameController::GameController()
{
    /* Create some empty vectors to contain our main game object types */
    _balls = std::vector<Ball>();
    _bricks = std::vector<Brick>();
    _players = std::vector<Player>();

    /* Initialize Game Variables */
    _game_state         = helpers::GAMESTATE::TITLE;
    _ball_launched      = false;
    _high_score         = 0;
    _total_score        = 0;
    _set_new_high_score = false;
}

GameController::~GameController()
{
    /*Destructor called after window closed and while our game 
       is exiting, make sure we delete all created pointers */

    /* Delete all brick texture pointers */
    for (sf::Texture* texture : _brick_textures)
    {
        delete texture;
        texture = nullptr;
    }

    /* Delete our background texture pointer */
    delete _background_texture;
    _background_texture = nullptr;
}

#pragma endregion


#pragma region COLLISION FUNCTIONS

void GameController::TestForCollisions()
{
    /* We will use our ball or balls if we add multiple for future 
        game modes to be the first object to call collision tests */
    for (Ball& ball : _balls)
    {
        /* For each ball we have check if it is colliding with any brick */
        for (Brick& brick : _bricks)
        {
            /* Use our collider engine to see if there is a ball to brick collision,
                if there is our function sets the bricks destroy flag to true,
                if there was a collision we will play the brick breaking sound */
            if (ColliderEngine::CheckForCollision(&ball, &brick))
                PlayBrickSound();
        }

        /* For each ball we have check if it is colliding with any player
            we may add other players in the future */
        for (Player& player : _players)
        {
            /* Use our collider engine to see if there is a ball to player collision,
                if there is our function handles the bouncing of the ball we just need to
                play the bouncing sound here */
            if (ColliderEngine::CheckForCollision(&ball, &player))
                PlayBounceSound();
        }

    }

}

void GameController::TestForDestroys()
{
    /* Checks each ball to see if it is flagged to destroy, if so
        we erase it from our vector */
    for (int i = 0; i < _balls.size(); i++)
    {
        if (_balls[i].ShouldDestroy())
        {
            _balls.erase(_balls.begin() + i);
        }
    }

    /* If there are now no balls, then the game is over!
        We will display a message based on certain conditions */
    if (_balls.size() == 0)
    {
        /* Set our gamestate to change to GAMEOVER */
        _game_state = helpers::GAMESTATE::GAMEOVER;

        /* Create a string stream to write our game over message to */
        std::stringstream game_over_text;

        /* Create our game over message based on if a new high score was achieved */
        game_over_text << "GAME OVER!\nFINAL SCORE: " << _total_score;
        if (_set_new_high_score)
        {
            game_over_text << "\nTHAT'S A NEW HIGH SCORE!";
            _high_score = _total_score;
        }
        else
        {
            game_over_text << "\nHIGH SCORE: " << _high_score;
        }
        game_over_text << "\nPRESS SPACE TO PLAY AGAIN\nOR ESC TO EXIT"; 

        /* Set our game over text object to the string of our created string stream */
        _game_over_text.setString(game_over_text.str());
    }
        

    /* Checks each brick to see if it is flagged to destroy, if so
        we erase it from our vector and update our score text */
    for (int i = 0; i < _bricks.size(); i++)
    {
        if (_bricks[i].ShouldDestroy())
        {
            /* Create a string stream to more easily create our updated score text, then
            update the score text object */
            std::stringstream updatedScore;
            _total_score += _bricks[i].GetScore();

            updatedScore << "SCORE: " << _total_score;
            _score_text.setString(updatedScore.str());

            /* Erase brick from vector */
            _bricks.erase(_bricks.begin() + i);
        }

    }

    /* If there are now no bricks, then the game is over!
        We will display a message based on certain conditions */
    if (_bricks.size() == 0)
    {
        _game_state = helpers::GAMESTATE::GAMEOVER;
        std::stringstream game_over_text;
        game_over_text << "GAME COMPLETE!\nFINAL SCORE: " << _total_score;
        if (_set_new_high_score)
        {
            game_over_text << "\nTHAT'S A NEW HIGH SCORE!";
            _high_score = _total_score;
        }
        else
        {
            game_over_text << "\nHIGH SCORE: " << _high_score;
        }
        game_over_text << "\nPRESS SPACE TO PLAY AGAIN\nOR ESC TO EXIT";
        _game_over_text.setString(game_over_text.str());
    }
}
#pragma endregion


#pragma region UPDATE FUNCTIONS

void GameController::UpdateObjects(sf::Vector2f constraints)
{
    /*Loop throug all balls and players and call their update methods */
    for (Ball& ball : _balls)
    {
        ball.Update(sf::Vector2f(0, 0), constraints);
    }

    for (Player& player : _players)
    {
        /*We are passing custom bounds here that are 0 and the width of the screen 
            with the players size as an offset */
        player.Update(sf::Vector2f(0, constraints.x - player.GetBounds().width));
    }
}

void GameController::UpdateGame(sf::RenderWindow* render_window)
{
    /*Creates a vector for constraints set to the window size */
    sf::Vector2f constraints;
    constraints.x = render_window->getSize().x;
    constraints.y = render_window->getSize().y;

    /* Calling collision detection functions */
    TestForCollisions();

    /* Checking to see if any objects need to be destroyed now that we have 
        checked for collisions */
    TestForDestroys();

    /* Update the Player, Ball, and Brick objects */
    UpdateObjects(constraints);

    /* Update our high score text, if the current score is greater than the high score
        then we will update the text! */
    UpdateHighScore(render_window );

    /* Draws objects to the render window */
    DrawObjects(render_window);
}

void GameController::UpdateHighScore(sf::RenderWindow* render_window)
{
    /* If the current score is greater than the high score we will:
        Update the high score, update the highscore text variable,
        Update the text variables positions as well to stay on the 
        Screen */
    if (_total_score > _high_score)
    {
        /* Set the new high score and set a flag showing this game set a new high score! */
        _high_score = _total_score;
        _set_new_high_score = true;

        /* String stream used to more easily create a display string for our high score text
            then set that string to our text object */
        std::stringstream high_score_string;
        high_score_string << "HIGH SCORE: " << _high_score;
        _high_score_text.setString(high_score_string.str());

        /* Create an offset vector based on the size of our updated text objects size */
        sf::Vector2f offset = sf::Vector2f(
            _high_score_text.getGlobalBounds().width,
            _high_score_text.getGlobalBounds().height
        );

        /* Additional margin added to offset. ADJUST IF DESIRED to move 
            high scores display location */
        offset.x = offset.x + 15;
        offset.y = offset.y + 5;

        /* Set the updated position of our high score text object */
        _high_score_text.setPosition(sf::Vector2f(render_window->getSize().x
            - offset.x, 5));
    }
}

#pragma endregion


#pragma region CREATION FUNCTIONS

void GameController::CreateBall(Ball ball)
{
    /* Adds a ball based on passed ball object to our controller */
    this->_balls.push_back(Ball(ball));
}

void GameController::CreateBrick(Brick brick)
{
    /* Adds a brick based on passed brick object to our controller */
    this->_bricks.push_back(Brick(brick));
}

void  GameController::CreateBricks(sf::RenderWindow* render_window)
{
    int             ROWS = 5;                                   //Number of rows of bricks
    int             COLUMNS = 15;                               //Numver of columns of bricks
    int             score = 0;                                  //Score value var for new brick
    sf::Vector2f    brick_size(50, 20);                         //Size of new brick
    sf::Vector2u    window_size = render_window->getSize();     //Vector for window size
    sf::Vector2f    base_position(0 + (window_size.x / 16.f),   //Position of first brick
                                  0 + (window_size.y / 16.f));
    sf::Vector2f    new_position;                               //Vector for new position
    

    /*Loop through each column within each row*/
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            /*Set the new bricks position based on current loop positions and the 
                original base position */
            new_position.x = base_position.x + (j * brick_size.x * 1.4);
            new_position.y = base_position.y + (i * brick_size.y * 2);

            /* Create a brick object used to set the values of a new object in our bricks vector */
            Brick new_brick(new_position, brick_size, score, sf::Color::White);

            /* Set the texture of the new brick to a random texture based on our random 
                texture function, will also update our score to be used */
            new_brick.SetTexture(RandomTexture(&_brick_textures, &score));

            /*Set the score of our new brick, this score was assigned based on the random color
                that was selected for this brick */
            new_brick.SetScore(score);

            /* Create the new brick in our game object container */
            CreateBrick(new_brick);
        }
    }
}

void GameController::CreateGameOverTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT)
{
    /* Creates our Gameover text object */
    std::stringstream game_over_text;
    game_over_text << "GAME OVER!\n\nFINAL SCORE: " << _total_score << "\nPRESS SPACE TO PLAY AGAIN\nOR ESC TO EXIT";
    _game_over_text.setFont(_font);
    _game_over_text.setString(game_over_text.str());
    _game_over_text.setCharacterSize(24);
    _game_over_text.setFillColor(sf::Color::White);

    /* The offset for our game over text, used for centering the text on the screen */
    sf::Vector2f title_offset = sf::Vector2f(_game_over_text.getGlobalBounds().width,
        _game_over_text.getGlobalBounds().height);

    title_offset.x = title_offset.x / 2;
    title_offset.y = title_offset.y / 2;

    /* Set position of our object to the center */
    _game_over_text.setPosition(sf::Vector2f((WINDOW_WIDTH / 2) - title_offset.x, (WINDOW_HEIGHT / 2) - title_offset.y));
}

void GameController::CreateHighScoreTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT)
{
    /* Creates our Highscore text object */
    std::stringstream high_score_string;
    high_score_string << "HIGH SCORE: " << _high_score;
    _high_score_text.setFont(_font);
    _high_score_text.setString(high_score_string.str());
    _high_score_text.setCharacterSize(24);
    _high_score_text.setFillColor(sf::Color::White);

    /* The offset for our highscore text, used for setting in upper right  */
    sf::Vector2f offset = sf::Vector2f(
        _high_score_text.getGlobalBounds().width,
        _high_score_text.getGlobalBounds().height
    );

    offset.x = offset.x + 15;
    offset.y = offset.y + 5;

    /* Set position of our object to the upper right */
    _high_score_text.setPosition(sf::Vector2f(WINDOW_WIDTH - offset.x, 5));


}

void GameController::CreateInstructionsTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT)
{
    /* Creates our Instructions text object */
    std::stringstream instructions_string;
    instructions_string << "Press Space To Launch Ball Randomly!";
    _instructions_text.setFont(_font);
    _instructions_text.setString(instructions_string.str());
    _instructions_text.setCharacterSize(32);
    _instructions_text.setFillColor(sf::Color::White);

    /* The offset for our Instructions text, used for centering the text on the screen */
    sf::Vector2f offset = sf::Vector2f(
        _instructions_text.getGlobalBounds().width,
        _instructions_text.getGlobalBounds().height
    );

    offset.x = offset.x / 2;
    offset.y = offset.y + 5;

    /* Set position of our Instructions object to the center */
    _instructions_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - offset.x,
        WINDOW_HEIGHT / 2 - offset.y));
}

void GameController::CreatePlayer(Player player)
{
    /* Adds a player based on passed player object to our controller */
    this->_players.push_back(Player(player));
}

void GameController::CreateScoreTextObject()
{
    /* Creates our Instructions text object */
    _score_text.setFont(_font);
    _score_text.setString("SCORE: 0");
    _score_text.setCharacterSize(24);
    _score_text.setFillColor(sf::Color::White);

    /* Set position of our Instructions object to the upper left */
    _score_text.setPosition(sf::Vector2f(15, 5));
}

sf::Texture* GameController::RandomTexture(std::vector<sf::Texture*>* textures, int* score)
{
    /* Get a random number within the indices of our textures vector */
    int index = helpers::RandomNumber(0, textures->size() - 1);

    /* If a score pointer was passed then set the score based on random index */
    if (score != nullptr)
        *score = 15 + 10 * index;

    /* Return the texture at the random index */
    return textures->at(index);
}

void  GameController::CreateTitleTextObject(float WINDOW_WIDTH, float WINDOW_HEIGHT)
{
    /* Creates our title text object */
    _title_text.setFont(_font);
    _title_text.setString("BRICK BREAKER (CLONE) \n\nPRESS SPACE TO START!");
    _title_text.setCharacterSize(24);
    _title_text.setFillColor(sf::Color::White);

    /* The offset for our Instructions text, used for centering the text on the screen */
    sf::Vector2f title_offset = sf::Vector2f(_title_text.getGlobalBounds().width,
        _title_text.getGlobalBounds().height);

    title_offset.x = title_offset.x / 2;
    title_offset.y = title_offset.y / 2;

    /* Set position of our Title object to the center */
    _title_text.setPosition(sf::Vector2f((WINDOW_WIDTH / 2) - title_offset.x, (WINDOW_HEIGHT / 2) - title_offset.y));
}

#pragma endregion


#pragma region EVENT POLLING FUNCTIONS
/*EVENT POLLING FUNCTIONS*/
void GameController::PollGameKeyEvents(sf::RenderWindow* render_window,
    sf::Event* event)
{
    /* Check if window should close, close if so */
    if (event->type == sf::Event::Closed ||
        event->type == sf::Event::KeyPressed && event->key.code ==
        sf::Keyboard::Escape)
    {
        _game_state = helpers::GAMESTATE::EXIT;
    }
    /* If window should stay open */
    else
    {
        /* If the ball is launched (most likely so we check first) process player input */
        if (_ball_launched)
        {
            for (Player& player : _players)
            {
                player.ProcessControls(event);
            }
        }
        /* If the ball has not launched yet game is not fully started. Prompt user to 
            hit space to launch the ball and start playing! */
        else
        {
            /* If player pressed space then flag the ball as launched and set 
                the ball(s) to move at a random upward direction
                30-80 degree angle or a 100-170 degree angle
                SEE SetRandomUpwardDirection to change angles */
            if (event->type == sf::Event::KeyPressed && event->key.code ==
                sf::Keyboard::Space)
            {
                _ball_launched = true;
                for (Ball& ball : _balls)
                {
                    ball.SetRandomUpwardDirection();
                }
            }
        }

    }
}

void GameController::PollGameOverKeyEvents(sf::RenderWindow* render_window, sf::Event* event)
{
    /* If the user wants to close we set game state to exit which will
        trigger the closing of the window */
    if (event->type == sf::Event::Closed ||
        event->type == sf::Event::KeyPressed && event->key.code ==
        sf::Keyboard::Escape)
    {
        _game_state = helpers::GAMESTATE::EXIT;
    }

    /* If the player hits the space bar we will change state to
        PLAY and reset, the restart the game! */
    else if (event->type == sf::Event::KeyPressed && event->key.code ==
        sf::Keyboard::Space)
    {
        ResetGame(render_window);
        _game_state = helpers::GAMESTATE::PLAY;
    }
}

void GameController::PollTitleKeyEvents(sf::RenderWindow* render_window,
    sf::Event* event)
{
    /* If the user wants to close we set game state to exit which will
        trigger the closing of the window */
    if (event->type == sf::Event::Closed ||
        event->type == sf::Event::KeyPressed && event->key.code ==
        sf::Keyboard::Escape)
    {
        _game_state = helpers::GAMESTATE::EXIT;
    }

    /* If the player hits the space bar we will change state to
        PLAY and start the game! */
    else if (event->type == sf::Event::KeyPressed && event->key.code ==
        sf::Keyboard::Space)
    {
        _game_state = helpers::GAMESTATE::PLAY;
    }
}

#pragma endregion


#pragma region GAMESTATE RUN FUNCTIONS
/* GAME STATE RUN FUNCTIONS */

/*PLAY'S GAMESTATE RUN FUNCTION*/
void GameController::RunGame(sf::RenderWindow* render_window,
    unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT,
    sf::Event* primary_event)
{

    while (render_window->pollEvent(*primary_event))
    {
        PollGameKeyEvents(render_window, primary_event);

    }


    render_window->clear();

    if (_ball_launched)
    {
        //Updates AND DRAWS our objects
        UpdateGame(render_window);
    }
    else
    {
        //JUST draws our objects with no input or movement
        DrawObjects(render_window);

        /* Lets the user know how to launch the ball */
        render_window->draw(_instructions_text);
    }

    render_window->display();


}

/*GAMEOVER'S GAMESTATE RUN FUNCTION*/
void GameController::RunGameOverScreen(sf::RenderWindow* render_window,
    sf::Event* primary_event)
{
    sf::Event event;
    while (render_window->pollEvent(*primary_event))
    {
        PollGameOverKeyEvents(render_window, primary_event);
    }

    render_window->clear();



    render_window->draw(_game_over_text);

    render_window->display();
}

/*TITLE'S GAMESTATE RUN FUNCTION*/
void GameController::RunTitleScreen(sf::RenderWindow* render_window,
    sf::Event* primary_event)
{

    //Creating our event variable
    sf::Event event;
    while (render_window->pollEvent(*primary_event))
    {
        PollTitleKeyEvents(render_window, primary_event);
    }

    render_window->clear();

    render_window->draw(_title_text);

    render_window->display();

}


#pragma endregion

#pragma region GAME SETUP / RESET FUNCTIONS

void GameController::SetupGame(sf::RenderWindow* render_window,
    unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
{
    /* Creates our score text object */
    CreateScoreTextObject();

    /* Creates our title text object */
    CreateTitleTextObject(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Creates our game over text object */
    CreateGameOverTextObject(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Creates our high score text object */
    CreateHighScoreTextObject(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Creates our insturctions text object */
    CreateInstructionsTextObject(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Creates a variable we will use to set the player's position */
    sf::Vector2f    player_position = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40);

    /* Creates a variable we will use to set the player's size */
    sf::Vector2f    player_size = sf::Vector2f(70, 20);

    /* Creates a variable we will use to set the player's speed */
    float           player_speed = 15;

    /* Creates a variable we will use to set the balls's position */
    sf::Vector2f    ball_position = player_position;

    /* Creates a variable we will use to set the balls's radius */
    float           ball_radius = 7;

    /* Creates a variable we will use to set the balls's speed */
    float           ball_speed = 13;

    /* Creates a variable we will use to set the balls's direction */
    sf::Vector2f    ball_direction = sf::Vector2f(0.45, -0.33);

    /* Offsetting the balls initial position to center it on top of our player */
    ball_position.y -= ball_radius * 2;
    ball_position.x -= ball_radius;

    /* Creates our player game object*/
    CreatePlayer(Player(player_position, player_speed,
        player_size));

    /* Creates our ball game object */
    CreateBall(Ball(ball_position,
        ball_direction, ball_speed, ball_radius));

    /* Creates all of our brick game objects */
    CreateBricks(render_window);
}

void GameController::ResetGame(sf::RenderWindow* render_window)
{
    /* Get the windows height and width */
    unsigned int    WINDOW_WIDTH = render_window->getSize().x;
    unsigned int    WINDOW_HEIGHT = render_window->getSize().y;

    _total_score        = 0;        //Reset our score to 0
    _ball_launched      = false;    //Reset the ball to not be launched
    _players.clear();               //Empty our players vector
    _balls.clear();                 //Empty our balls vector
    _bricks.clear();                //Empty our bricks vector
    _set_new_high_score = false;    //Reset our flag for achieving a new high score

    /* Calls setup game to finish out resetting our game */
    SetupGame(render_window, WINDOW_WIDTH, WINDOW_HEIGHT);
}

#pragma endregion


void GameController::PlayBrickSound() {
    /* Load our explosion wav file into our sound buffer */
    if (!_sound_buffer.loadFromFile("Assets/explosion.wav")) {}

    /* Makes sure the brick explosion sound object has the buffer */
    _brick_explosion.setBuffer(_sound_buffer);

    /*Plays the sound*/
    _brick_explosion.play();
}

void GameController::PlayBounceSound() {
    /* Load our bounce wav file into our sound buffer */
    if (!_sound_buffer.loadFromFile("Assets/bounce.wav")) {}

    /* Makes sure the ball bounce sound object has the buffer */
    _ball_bounce.setBuffer(_sound_buffer);

    /* Plays the sound */
    _ball_bounce.play();
}

/*Function to draw game objects to screen */
void  GameController::DrawObjects(sf::RenderWindow* render_window)
{
    /* We draw the background object first so everything else is drawn on top of it */
    render_window->draw(_background);

    /*Loop through all objects and call their draw functions*/
    for (Brick& brick : _bricks)
    {
        brick.Draw(render_window);
    }

    for (Ball& ball : _balls)
    {
        ball.Draw(render_window);
    }

    for (Player& player : _players)
    {
        player.Draw(render_window);
    }

    /* Draw our score and high score text objects */
    render_window->draw(_score_text);
    render_window->draw(_high_score_text);
}

#pragma endregion

#pragma region Public

#pragma region SINGLETON FUNCTIONS

/* SINGLETON FUNCTIONS THAT ARE NEEDED */
GameController* GameController::GetInstance()
{
    //If there is not a current instance of the singleton, create one
    if (_instance == nullptr)
    {
        //Logging For Debug Purposes
        std::cout << "Created instance\n\n\n";
        _instance = new GameController();
    }

    //Increase our counter as somewhere someone accessed this pointer
    AddReference();

    //Return the pointer for the singleton
    return _instance;
}

void GameController::ReleaseInstance()
{
    //We are releasing an instance, reduce our counter
    RemoveReference();

    //If the counter is at 0 now AND the instance is not
    //A null pointer then delete our instance and set to nullptr
    if (_counter == 0 && _instance != nullptr)
    {
        delete _instance;

        _instance = nullptr;

        //Logging For Debug Purposes
        std::cout << "Deleted instance\n\n\n";
    }
}
#pragma endregion

/*GAME STATE AND GAME INITIALIZERS*/
void GameController::InitializeGame(unsigned int WINDOW_WIDTH,
    unsigned int WINDOW_HEIGHT)
{
    /********************************************************
    *                LOADING RESOURCE FILES
    *********************************************************/


    /* Attempt to load our font, if successful set
      up our score text object */
    if (!_font.loadFromFile("Assets/font.ttf"))
        throw - 1;

    /*Attempt to load in our brick textures, if they all load
      without throwing an error then we can create new
      pointers to them */
    sf::Texture brick_texture1;
    if (!brick_texture1.loadFromFile("Assets/brick1.png"))
        throw - 1;

    sf::Texture brick_texture2;
    if (!brick_texture2.loadFromFile("Assets/brick2.png"))
        throw - 1;

    sf::Texture brick_texture3;
    if (!brick_texture3.loadFromFile("Assets/brick3.png"))
        throw - 1;

    sf::Texture brick_texture4;
    if (!brick_texture4.loadFromFile("Assets/brick4.png"))
        throw - 1;

    sf::Texture background_texture;
    if (!background_texture.loadFromFile("Assets/bg.png"))
        throw - 1;

    /********************************************************
    *              SETTING GAME CONTROLLER
    *            RESOURCES TO LOADED RESOURCES
    *********************************************************/

    /* Assigning pointers to our textures and adding them
        to our vector of brick textures */
    _brick_textures.push_back(new sf::Texture(brick_texture1));
    _brick_textures.push_back(new sf::Texture(brick_texture2));
    _brick_textures.push_back(new sf::Texture(brick_texture3));
    _brick_textures.push_back(new sf::Texture(brick_texture4));

    /* Assigning a pointer to our background texture image */
    _background_texture = new sf::Texture(background_texture);

    /* Setup our background image sprite */
    _background.setTexture(*_background_texture);
    _background.setTextureRect(sf::IntRect(0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT));
    _background.setPosition(0, 0);

}


/* OVERALL GAME CONTROLLER FUNCTION */
void GameController::RunGameControl(sf::RenderWindow* render_window,
    unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
{
    /********************************************************
    *             INSTANTIATING GAME OBJECTS
    *********************************************************/

    SetupGame(render_window, WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Our primary window loop */
    while (render_window->isOpen())
    {
        /* We will switch on the current gamestate
            to determine what aspect of the game we
            want to run*/

        /*Event object passed to each gamestates game loop function */
        sf::Event primary_event;

        switch (_game_state)
        {
        case helpers::GAMESTATE::TITLE:
            RunTitleScreen(render_window, &primary_event);
            break;
        case helpers::GAMESTATE::PLAY:
            RunGame(render_window, WINDOW_WIDTH, WINDOW_HEIGHT, &primary_event);
            break;
        case helpers::GAMESTATE::GAMEOVER:
            RunGameOverScreen(render_window, &primary_event);
            break;
        case helpers::GAMESTATE::EXIT:
            render_window->close();
            break;
        default:
            break;
        }

    }
}

#pragma endregion

