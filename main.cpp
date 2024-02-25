#include <iostream>
#include<vector>

#include <pagmo/algorithm.hpp>
#include <pagmo/algorithms/gwo.hpp>
#include <pagmo/algorithms/sga.hpp>
#include <pagmo/algorithms/gaco.hpp>
#include <pagmo/algorithms/ihs.hpp>
#include <pagmo/archipelago.hpp>
#include <pagmo/problem.hpp>
#include<pagmo/types.hpp>

using namespace pagmo;

typedef std::vector<int> vector_int;

struct problem_v0 {
    
    vector_double fitness(const vector_double &dv) const {

        vector_int int_dv(dv.begin(),dv.end());

        unsigned short human_stun_remaining = 0;
        unsigned int human_hp = 100;
        unsigned int human_ammo = 30;
        bool is_human_dead = false;
        unsigned int human_dead_turn = 1001;

        unsigned short zombie_stun_remaining = 0;
        unsigned int zombie_hp = 999;
        bool is_zombie_dead =  false;
        unsigned int zombie_dead_turn = 1001;

        int turn_idx = 1;

        for(;turn_idx<=1000;turn_idx++){
            // human turn first
            if(human_stun_remaining>0){
                human_stun_remaining--;
            }
            else{
                // if dv = 1, shoot
                if(dv[turn_idx-1]==1){
                    if(human_ammo>0){
                        zombie_hp-=29;
                        human_ammo--;
                        if(zombie_hp<=0){
                            is_zombie_dead=true;
                            zombie_dead_turn=turn_idx;
                            break;
                        }
                    }
                }
                // if dv = 2, reload
                else if(dv[turn_idx-1]==2){
                    human_ammo=30;
                    human_stun_remaining=2;
                }
                // if dv = 3, flash grenade to zombie
                else if(dv[turn_idx-1]==3){
                    zombie_stun_remaining=3;
                }
                // if dv = 4, regenerade human hp
                else if(dv[turn_idx-1]==4){
                    human_hp+=50;
                    human_stun_remaining=1;
                }
            }

            // after that, zombie
            if(zombie_stun_remaining>0){
                zombie_stun_remaining--;
            }
            else{
                // attack that human
                human_hp-=4;
                if(human_hp<=0){
                    is_human_dead=true;
                    human_dead_turn=turn_idx;
                }
            }
        }

        return {zombie_dead_turn+(1001-human_dead_turn)*1000+zombie_hp};
    }

    std::pair<vector_double, vector_double> get_bounds() const {
        vector_double lb(1000,1);
        vector_double ub(1000,4);
        return {lb, ub};
    }

    vector_double::size_type get_nix() const
    {
        return 1000;
    }
};

int main()

{

    // 1 - Instantiate a pagmo problem constructing it from a UDP

    problem prob{problem_v0()};

    // 2 - Instantiate a pagmo algorithm (Improved Harmony Search for 200 iterations)

    algorithm algo{ihs(200u)};

    // 3 - Instantiate an archipelago with 1 islands having each 100 individuals.

    archipelago archi{1u, algo, prob, 100u};

    // 4 - Run the evolution in parallel on the 1 separate islands 1 times.

    archi.evolve(1);

    // 5 - Wait for the evolutions to finish.

    archi.wait_check();


    // 6 - Print the fitness of the best solution in each island.

    for (const auto &isl : archi) {

        std::cout << isl.get_population() << '\n';
        std::cout << isl.get_population().champion_f()[0] << '\n';
        std::cout << isl.get_population().champion_x()[0] << '\n';

    }

    return 0;

}