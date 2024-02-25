#include <iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>

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

struct human_vs_zombie_problem {
    
    vector_double fitness(const vector_double &dv) const {

        vector_int int_dv(dv.begin(),dv.end());

        double human_stun_remaining = 0;
        double human_hp = 100;
        double human_ammo = 30;
        bool is_human_dead = false;
        double human_dead_turn = 1001;

        double zombie_stun_remaining = 0;
        double zombie_hp = 999;
        bool is_zombie_dead =  false;
        double zombie_dead_turn = 1001;

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
                            zombie_hp=0;
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
                    human_hp=std::min(human_hp,(double)100);
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

        return {zombie_dead_turn, zombie_hp, dv[0]-1, -(human_hp-1)};
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

    vector_double::size_type get_nec() const
    {
        return 2;
    }

    vector_double::size_type get_nic() const
    {
        return 1;
    }
};

int main()

{

    std::ofstream MyFile("output.txt");

    // 1 - Instantiate a pagmo problem constructing it from a UDP

    problem prob{human_vs_zombie_problem()};

    // 2 - Instantiate a pagmo algorithm (Improved Harmony Search for 200 iterations)

    algorithm algo{ihs(2000000,0.85,0.35,0.99,1e-5,1,50)};

    // 3 - Instantiate an archipelago with 1 islands having each 100 individuals.

    archipelago archi{1u, algo, prob, 100u};

    // 4 - Run the evolution in parallel on the 1 separate islands 1 times.

    archi.evolve(1);

    // 5 - Wait for the evolutions to finish.

    archi.wait_check();


    // 6 - Print the fitness of the best solution in each island.

    for (const auto &isl : archi) {

        std::cout << isl.get_population() << '\n';

        if((isl.get_population().champion_f()[1]==0)&&(isl.get_population().champion_f()[3]<0)){
            std::map<double, std::string> dv_to_str_solution;
            dv_to_str_solution[1]="Shoot That ZOMBIE";
            dv_to_str_solution[2]="Reload AK-47";
            dv_to_str_solution[3]="Flash Grenade";
            dv_to_str_solution[4]="Regenerated HP";

            MyFile << "Steps to kill that zombies" << '\n';
            int step_idx = 1;
            for (const auto &x : isl.get_population().champion_x()){
                MyFile << step_idx <<". " << dv_to_str_solution[x] << '\n';
                step_idx+=1;
                if(step_idx>isl.get_population().champion_f()[0]){
                    break;
                }
            }
            MyFile << "ZOMBIE WENT TO HELL!!!!" << '\n';
        }
    }

    return 0;

}