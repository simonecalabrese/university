#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h> 

using std::string, std::cout, std::ostream, std::cin, std::isdigit;

bool isNumeric(string);
/* Return whether a string is numeric or not */
bool isNumeric(string str) {
    for (int i = 0; i < str.length(); i++)
        if (isdigit(str[i]) == false)
            return false;
    return true;
}

/* Enum representing all available games */
enum class Game {
    RPS, // Rock-Paper-Scissors
    DiceRoll
};

/* This class represents a registered player that has selected a game to play.
*  During the registration process, If the user enters a name that matches 
*  another registered user, that user will be selected for the game */
class Player {
    private:
        string id; // unique id
        string name;
        int score;
        int games; // total games played

    public:
        Player(const string name) {
            this->name = name;
            this->score = 0;
            this->games = 0;
            timeval time;
            srand(time.tv_usec);
            this->id = std::to_string(rand()) + "_" + name;
        }

        Player(const string name, const int score, const int games) {
            this->name = name;
            this->score = score;
            this->games = games;
            timeval time;
            srand(time.tv_usec);
            this->id = std::to_string(rand()) + "_" + name;
        }

        Player(const string id, const string name, const int score, const int games) : id(id), name(name), score(score), games(games) {}

        string getId() {
            return id;
        }
        
        string getName() {
            return name;
        }

        const int getScore() {
            return score;
        }
        
        const int getGames() {
            return games;
        }

        void addScore(const int score) {
            this->score += score;
        }
        
        void addGame() {
            games++;
        }
};

/* This class represents the Rock-Paper-Scissors game with two players
*  required and five rounds to win. */
class RPS {
    public:
        static const int required_players = 2;
        static const int max_players = 2;
        static const int game_score = 5;
        static const short int game_rounds = 5;
        
        static Player *play(std::vector<Player> players) {
            if(players.size() == 2) {
                short int i = 0;
                short int p1_won_rounds = 0;
                short int p2_won_rounds = 0;
                Player *winner;
                Player p1(players[0]), p2(players[1]);
                while(i < game_rounds) {
                    i++;
                    cout << "\n" << p1.getName() << "'s won rounds: " << p1_won_rounds << "\n";
                    cout << p2.getName() << "'s won rounds: " << p2_won_rounds << "\n";
                    cout << "-- Round " << i << " --\n";
                    cout << p1.getName() << "'s turn: choose between rock, paper and scissors\n";
                    string p1_answer;
                    cin >> p1_answer;
                    cout << p2.getName() << "'s turn: choose between rock, paper and scissors\n";
                    string p2_answer;
                    cin >> p2_answer;
                    if (
                        (p1_answer.compare("paper") == 0 && p2_answer.compare("scissors") == 0) ||
                        (p1_answer.compare("scissors") == 0 && p2_answer.compare("rock") == 0) ||
                        (p1_answer.compare("rock") == 0 && p2_answer.compare("paper") == 0)
                    ) {
                        p2_won_rounds++;
                    } else if (
                        (p2_answer.compare("paper") == 0 && p1_answer.compare("scissors") == 0) ||
                        (p2_answer.compare("scissors") == 0 && p1_answer.compare("rock") == 0) ||
                        (p2_answer.compare("rock") == 0 && p1_answer.compare("paper") == 0)
                    ) {
                        p1_won_rounds++;
                    }
                }
                if(p1_won_rounds > p2_won_rounds) {
                    /* Player 1 win */
                    winner = new Player(p1.getId(), p1.getName(), p1.getScore()+game_score, p1.getGames()+1);
                    return winner;
                } else if(p2_won_rounds > p1_won_rounds) {
                    /* Player 2 win */
                    winner = new Player(p2.getId(), p2.getName(), p2.getScore()+game_score, p2.getGames()+1);
                    return winner;
                } else {
                    /* Draw */
                    return nullptr;
                }
            } else {
                cout << "Only 2 players can play at RPS.\n";
                return nullptr;
            }
        }
};

/* This class represents a dice roll game where at least two players
*  are required. Whoever gets the highest score wins */
class DiceRoll {
    public:
        static const int min_required_players = 2;
        static const int game_score = 1;
        static const short int game_rounds = 3;
        
        static Player *play(std::vector<Player> players) {
            short int i = 0;
            int won_rounds[players.size()] = {0};
            int rolls[players.size()] = {0};
            Player *winner;
            bool draw = true; // draw flag
            while(i < game_rounds) {
                i++;
                cout << "\n";
                for(int j = 0; j < players.size(); j++) {
                    cout << players[j].getName() << "'s won rounds: " << won_rounds[j] << "\n";
                }
                cout << "-- Round " << i << " --\n";
                cout << "Simulating dice rolls...\n";
                for(int j = 0; j < players.size(); j++) {
                    int roll = (rand() % 6) + 1;
                    cout << players[j].getName() << " has " << (roll) << "\n";
                    rolls[j] = roll;
                }
                int max = rolls[0];
                int maxIndex = 0; // player index with highest roll
                for(int j = 0; j < players.size(); j++) {
                    if(rolls[j] > max) {
                        max = rolls[j];
                        maxIndex = j;
                    }
                    if(rolls[j] != max) {
                        draw = false;
                    }
                }
                won_rounds[maxIndex]++;
            }
            if(draw) {
                /* Draw */
                return nullptr;
            } else {
                int max = won_rounds[0];
                int maxIndex = 0; // player index with highest won rounds
                for(int j = 0; j < players.size(); j++) {
                    if(won_rounds[j] > max) {
                        max = won_rounds[j];
                        maxIndex = j;
                    }
                }
                winner = new Player(players[maxIndex].getId(), players[maxIndex].getName(), players[maxIndex].getScore()+game_score, players[maxIndex].getGames()+1);
                return winner;
            }
        }
};

class GameRoom {
    private:
        std::vector<Player> players;
        std::vector<DiceRoll> games;

    public:
        GameRoom() {
            cout << "Welcome!\n";
            askForChoice();
        }

        void printWinner(Player *winner) {
            cout << "\n-------------------------------------------------\n";
            cout << "The winner is " << winner->getName() << "! \n";
            cout << "-------------------------------------------------\n";
        }

        void printLeaderboard() {
            if(players.size() == 0) {
                cout << "\nThere are no registered players at the moment\n";
            } else {
                cout << "\nCurrent leaderboard:\n";
                for(Player p: players) {
                    cout << "- " << p.getName() << " (" << p.getScore() << " points)\n";
                }
                cout << "\n";
            }
        }
        
        /* Return true if a player with a given name has already been registered. */
        bool playerExists(const string &name) {
            bool found = false;
            for(Player &p : players) {
                if(p.getName().compare(name) == 0) {
                    found = true;
                }
            }
            return found;
        }

        /* Return a registered Player that matches a given name. */
        Player *findPlayer(const string &name) {
            Player *found = nullptr;
            for(Player &p : players) {
                if(p.getName().compare(name) == 0) {
                    found = &p;
                }
            }
            return found;
        }

        /* Update players stats after a completed match. If a Draw has occurred 
        *  only the Player's total games will be updated. */
        bool updatePlayer(const string &id, const string &new_name, const int new_score, const int new_games) {
            bool updated = false;
            for(int i = 0; i < players.size(); i++) {
                if(players[i].getId().compare(id) == 0) {
                    Player p(new_name, new_score, new_games);
                    players[i] = p;
                    updated = true;
                }
            }
            return updated;
        }

        /* Register a certain number of users, ask the user for each player's name and
        * check if a player with that name has already been registered, otherwise it will
        * be added to the players list. */
        std::vector<Player> registerPlayers(const int num) {
            int i = 0;
            std::vector<Player> registered;
            while(i < num) {
                i++;
                string name;
                cout << "Insert the name of the player n" << i << ": \n";
                cin >> name;
                if(!playerExists(name)) {
                    Player p(name);
                    players.push_back(p);
                    registered.push_back(p);
                } else {
                    Player *found = findPlayer(name);
                    registered.push_back(*found);
                }
            }
            return registered;
        }

        Player *play(const Game game) {
            if(game == Game::RPS) {
                cout << "\nRock-Paper-Scissors selected.\n";
                std::vector<Player> registered = registerPlayers(2); // required players to register
                RPS rps;
                Player *winner = rps.play(registered);
                if(winner != nullptr) {
                    updatePlayer(winner->getId(), winner->getName(), winner->getScore(), winner->getGames());
                } else {
                    /* Draw has occurred, updating only players's total games */
                    for(Player &p : registered ) {
                        updatePlayer(p.getId(), p.getName(), p.getScore(), p.getGames()+1);
                    }
                }
                return winner;
            } else if(game == Game::DiceRoll) {
                cout << "\nDice Roll selected.\n";
                DiceRoll diceroll;
                askforinput:
                cout << "How many players do you want to register? (Min. " << diceroll.min_required_players << " required)\n";
                string p_total;
                cin >> p_total;
                if(isNumeric(p_total)) {
                    if(stoi(p_total) < diceroll.min_required_players) {
                        cout << "Operation canceled: You need to register at least " << diceroll.min_required_players << " players.\n";
                        goto askforinput;
                    } else {
                        std::vector<Player> registered = registerPlayers(stoi(p_total)); // required players to register
                        Player *winner = diceroll.play(registered);
                        if(winner != nullptr) {
                            updatePlayer(winner->getId(), winner->getName(), winner->getScore(), winner->getGames());
                        } else {
                            /* Draw has occurred, updating only players's total games */
                            for(Player &p : registered ) {
                                updatePlayer(p.getId(), p.getName(), p.getScore(), p.getGames()+1);
                            }
                        }
                        return winner;
                    }
                } else {
                    cout << "Operation canceled: You need to register at least " << diceroll.min_required_players << " players.\n";
                    goto askforinput;
                }
            } else {
                return nullptr;
            }
        }

        /* Ask the user to choose a game to play with */
        void askForChoice() {
            cout << "\nSelect a game to play with.\n";
            cout << "1 - Rock-Paper-Scissors (2 players required)\n";
            cout << "2 - Dice roll (Min. 2 players required)\n";
            cout << "\n\n8 - Watch leaderboard\n";
            cout << "0 - Exit from program\n";
            string choice;
            cin >> choice;
            if(isNumeric(choice)) {
                int c = std::stoi(choice);
                switch(c) {
                    /* Rock-Paper-Scissors */
                    case 1: {
                        Player *winner = play(Game::RPS);
                        if(winner != nullptr) printWinner(winner);
                        else cout << "Draw\n";
                        delete winner;
                        break;
                    }
                    /* Dice roll */
                    case 2: {
                        Player *winner = play(Game::DiceRoll);
                        if(winner != nullptr) printWinner(winner);
                        else cout << "Draw\n";
                        delete winner;
                        break;
                    }
                    /* Show leaderboard */
                    case 8: {
                        printLeaderboard();
                        askForChoice();
                        break;
                    }
                    /* Exit from program */
                    case 0: {
                        delete this;
                        exit(0);
                    }
                    default: {
                        cout << "Invalid option.\n";
                        askForChoice();
                    }
                }
                askForChoice();
            } else {
                cout << "Invalid option.\n";
                askForChoice();
            }
        }
};

int main() {
    GameRoom *r = new GameRoom();
    return 0;
}