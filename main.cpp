#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>

#include "Pokemon.hpp"
#include "Trainer.hpp"

using namespace std;

class League {
private:
  vector<Trainer*> gym_leaders;
  vector<Trainer*> defeated;
public:  
  void get_gym_leaders(ifstream *gym_input) {
    int num_gym_leaders;
    string ignore;
    *gym_input >> num_gym_leaders;
    *gym_input >> ignore; // read in the word "leaders"

    for(int i = 0; i < num_gym_leaders; ++i) {
      gym_leaders.push_back(Trainer_factory(*gym_input));
    }
  }
  void fight(Trainer *player) {
    for(int i = 0; i < gym_leaders.size(); ++i) {
      player->reset();
      Trainer *current_gym_leader = gym_leaders[i];

      std::cout << "-----" << *player << " vs. " << *current_gym_leader << "-----" << endl;
      int num_wins = 0;
      for(int i = 0; i < 5; ++i) {
        Pokemon enemy = current_gym_leader->choose_pokemon();
        std::cout << *current_gym_leader << " chooses " << enemy << endl;
        Pokemon p = player->choose_pokemon(enemy.get_type());
        std::cout << *player << " chooses " << p << endl;
        if (Pokemon_battle(p, enemy)) {
          std::cout << p << " defeats " << enemy << endl << endl;
          ++num_wins;
        }
        else {
          std::cout << enemy << " defeats " << p << endl << endl;
        }
      }
       cout << "Result: " << *player << "=" << num_wins << ", "
         << *current_gym_leader << "=" << 5 - num_wins << endl;

    if (num_wins >= 3) {
      defeated.push_back(current_gym_leader);
    }

    if (num_wins == 0 || num_wins == 5) {
      cout << "It's a clean sweep!" << endl;
    }

    cout << endl;
  }

   cout << *player << " won " << defeated.size() << " matches by defeating:" << endl;
    for(int i = 0; i < defeated.size(); ++i) {
      cout << *defeated[i] << endl;
    }
  }
  ~League() {
    for(size_t i = 0; i < gym_leaders.size(); ++i) {
      delete gym_leaders[i];
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: battle.exe PLAYER_FILE GYM_LEADERS_FILE" << endl;
    return 1;
  }
  std::string player_in_name = argv[1];
  std::string gym_leaders_in_name = argv[2];

  std::ifstream player_in(player_in_name);
  if ( !player_in.is_open() ) {
    std::cout << "Unable to open " << player_in_name << endl;
    return 1;
  }
  std::ifstream gym_in(gym_leaders_in_name);
  if ( !gym_in.is_open() ) {
    std::cout << "Unable to open " << gym_leaders_in_name << endl;
    return 1;
  }
  Trainer *player = Trainer_factory(player_in);
  League opponents;
  opponents.get_gym_leaders(&gym_in);
  opponents.fight(player);  
  delete player;
  player_in.close();
  gym_in.close();
}