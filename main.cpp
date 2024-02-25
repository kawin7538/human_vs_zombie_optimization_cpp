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

        unsigned short human_stun_turns = 0;
        unsigned int human_hp = 100;
        unsigned int human_ammo = 30;
        bool is_human_dead = false;
        unsigned int human_dead_turn = 1001;

        unsigned short zombie_stun_turns = 0;
        unsigned int zombie_hp = 999;
        bool is_zombie_dead =  false;
        unsigned int zombie_dead_turn = 1001;

        int turn_idx = 1;

        for(;turn_idx<=1000;turn_idx++){
            // human turn first

            // after that, zombie
        }

        return {dv[0]};
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

    // (i.e., a user-defined problem, in this case the 30-dimensional

    // generalised Schwefel test function).

    problem prob{problem_v0()};

    // 2 - Instantiate a pagmo algorithm (self-adaptive differential

    // evolution, 100 generations).

    algorithm algo{ihs(200u)};
    // algo.set_verbosity(1);


    // 3 - Instantiate an archipelago with 16 islands having each 20 individuals.

    archipelago archi{1u, algo, prob, 100u};


    // 4 - Run the evolution in parallel on the 16 separate islands 10 times.

    archi.evolve(1);


    // 5 - Wait for the evolutions to finish.

    archi.wait_check();


    // 6 - Print the fitness of the best solution in each island.

    for (const auto &isl : archi) {

        std::cout << isl.get_population() << '\n';
        std::cout << isl.get_algorithm() << "\n";

    }

    return 0;

}