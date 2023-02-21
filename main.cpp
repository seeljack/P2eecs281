// AD48FB4835AF347EB0CA8009E24C3B13F8519882

// QUESTIONS
//-------------
//P2random.h is not returning the correct output
// Pop certain elements from priority queue (Do you pop the Jedi)
// Troops lost once you see that the number of troops is 0, then you pop it from the qeueue. You can pop the looser. Make sure because it could happen for
// Make struct of Planets?


//./galaxy_debug -v < large-DL-in.txt
//./galaxy_debug -v < spec-DL-in.txt
//./galaxy_debug -vmgw < large-DL-in.txt
//./galaxy_debug -vmgw < spec-PR-in.txt

//./galaxy_debug -g < spec-DL-in.txt
//./galaxy_debug -m < spec-DL-in.txt
//./galaxy_debug -w < spec-DL-in.txt
//./galaxy_debug -vgwm < large-DL-in.txt
//./galaxy_debug -w < test-7-w.txt



//General 50/50 with having the correct one be attacking the other
//Median is wrong every so often
//Number of battles is wrong
//Number of troops lost is wrong (for general evaluation)
//Ambush is wrong


//For when doing the battles make sure it keeps going until there are no more battles, instead of only seeing if batle when adding one


#include <vector>
#include <iostream>
#include <getopt.h>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <deque>
#include <queue>
#include <vector>
#include <sstream>
#include <cmath>

#include "P2random.h"
using namespace std;


enum class State {Initial, SeenOne, SeenBoth, MaybeBetter};


struct General{
    uint32_t jedi_deployed = 0;
    uint32_t sith_deployed = 0;
    uint32_t survived_troops = 0;
};


struct Deployment{
    uint32_t time;
    uint32_t force;
    mutable uint32_t quantity;
    int id;
    char side;
    uint32_t planet;
    uint32_t general;
    
};


struct  Jedi_comparing{
    //comparing Jedi force (want the smallest one to be top)
    bool operator()( const Deployment &j1,  const Deployment &j2) {
        if(j1.force == j2.force){
            if(j1.time == j2.time){
                return j1.id > j2.id;
            }
            return j1.time > j2.time;
        }
        else{
            return j1.force > j2.force;
        }
    }
};

struct  Sith_comparing{
    //comparing Sith force (want the biggest one on top)
    bool operator()( const Deployment &s1,  const Deployment &s2) {
        if(s1.force == s2.force){
            if(s1.time == s2.time){
                return s1.id > s2.id;
            }
            return s1.time > s2.time;
        }
        else{
            return s1.force < s2.force;
        }
    }
};

struct  Median_lower_comparing{
    //comparing Jedi force (want the smallest one to be top)
    bool operator()( const int &m1,  const int &m2) {
        return m1 < m2;
    }
};

struct  Median_higher_comparing{
    //comparing Jedi force (want the smallest one to be top)
    bool operator()( const int &m1,  const int &m2) {
        return m1 > m2;
    }
};

struct Planet{
    //priority queue Jedi (The lowest force is the top one)
    priority_queue<Deployment, vector<Deployment>, Jedi_comparing> Jedi;

    //priority queue Sith (The highest force is the top one)
    priority_queue<Deployment, vector<Deployment>, Sith_comparing> Sith;

    //Median
    //Lower half of the median
    priority_queue<uint32_t, vector<uint32_t>, Median_lower_comparing> median_lower;

    //Higher half of the median
    priority_queue<uint32_t, vector<uint32_t>, Median_higher_comparing> median_higher;


    //Movie Watcher attack
    State state = State::Initial;
    uint32_t jedi_force;
    uint32_t maybe_jedi_force;
    uint32_t sith_force;
    uint32_t maybe_sith_force;

    uint32_t jedi_time;
    uint32_t maybe_jedi_time;

    uint32_t sith_time;

    bool at_exist = false;


    //Movie watcher ambush
    State state2 = State::Initial;
    uint32_t jedi_force2;
    uint32_t maybe_jedi_force2;
    uint32_t sith_force2;
    uint32_t maybe_sith_force2;

    uint32_t jedi_time2;
    uint32_t maybe_jedi_time2;

    uint32_t sith_time2;
    uint32_t maybe_sith_time2;

   bool ambush_exist = false;

};




//Galaxy Class
class Galaxy{
    private:


        vector<Planet> planets;

        vector<General> generals;

        int id_help_counter = 0;

        uint32_t CURRENT_TIMESTAMP = 0;

        uint32_t total_battles = 0;

    public:

        bool verb;
        bool general;
        bool watcher;
        bool med;

        void set_mode(bool verbose, bool general, bool watcher, bool median){
            this->verb = verbose;
            this->general = general;
            this->watcher = watcher;
            this->med = median;
        }

        void resize_vector(int p_size, int g_size ){
            planets.resize(static_cast<uint32_t>(p_size));
            generals.resize(static_cast<uint32_t>(g_size));
        }

        // Reads in data one line at a time and puts all the data into the private member variables
        void interpret(string data, Deployment Person, int general_num, int planet_num){

            vector<string> the_data;
            //splits the string up so you can read in all the numbers
            the_data = interpret_healper_splitString(data);

            // removes first element in string for data with uneeded first element
            the_data[2].erase(the_data[2].begin());
            the_data[3].erase(the_data[3].begin());
            the_data[4].erase(the_data[4].begin());
            the_data[5].erase(the_data[5].begin());


            Person.time = static_cast<uint32_t>(stoi(the_data[0]));
            Person.side = the_data[1][0];
            Person.general = static_cast<uint32_t>(stoi(the_data[2]));
            Person.planet = static_cast<uint32_t>(stoi(the_data[3]));
            Person.force = static_cast<uint32_t>(stoi(the_data[4]));
            Person.quantity = static_cast<uint32_t>(stoi(the_data[5])); 
            id_help_counter = 1;
            Person.id += id_help_counter;
            id_help_counter += 1;

            //Error Checking
            error_checking(Person, general_num, planet_num);

            if(Person.side == 'J'){
                planets[Person.planet].Jedi.push(Person);

                // Add the general that is deployed
                generals[Person.general].jedi_deployed += Person.quantity;
                generals[Person.general].survived_troops += Person.quantity;


                // cout << Person.time << ' ' << Person.side << " " << Person.general << " " << Person.planet << " " << Person.force << " " << Person.id << " " << Person.quantity << "\n";
                battle(planets[Person.planet], Person);

            }
            else{
                planets[Person.planet].Sith.push(Person);

                generals[Person.general].sith_deployed += Person.quantity;
                generals[Person.general].survived_troops += Person.quantity;
                // cout << Person.time << ' ' << Person.side << " " << Person.general << " " << Person.planet << " " << Person.force << " " << Person.id << " " << Person.quantity << "\n";
                battle(planets[Person.planet], Person);
            }

        }
        //Helper function for interpret, splits the string up so it can read all the numbers if there is more than 1
        vector<string> interpret_healper_splitString(string str, string delimiter = " "){
            uint32_t start = 0;
            uint32_t end = static_cast<uint32_t>(str.find(delimiter));
            vector<string> the_data;
            while (end != static_cast<uint32_t>(-1)) {
                the_data.push_back( str.substr(static_cast<uint32_t>(start), static_cast<uint32_t>(end) - static_cast<uint32_t>(start)));
                // cout << str.substr(static_cast<uint32_t>(start), static_cast<uint32_t>(end) - static_cast<uint32_t>(start)) << endl;
                start = static_cast<uint32_t>(end) + static_cast<uint32_t>(delimiter.size());
                end = static_cast<uint32_t>(str.find(delimiter, static_cast<uint32_t>(start)));
            }
            the_data.push_back(str.substr(static_cast<uint32_t>(start), static_cast<uint32_t>(end) - static_cast<uint32_t>(start)));
            return the_data;
        }

        void median(){
            uint32_t median;

            for(uint32_t i = 0; i < planets.size(); i++){
                    if((planets[i].median_higher.size() > 0) && (planets[i].median_higher.size() == planets[i].median_lower.size())){
                        median = (planets[i].median_higher.top() + planets[i].median_lower.top()) / 2;
                        cout << "Median troops lost on planet " << i << " at time " <<  CURRENT_TIMESTAMP << " is " << median << ".\n";
                    }
                    else if((planets[i].median_higher.size() > 0) && (planets[i].median_higher.size() > planets[i].median_lower.size())){
                        median = planets[i].median_higher.top();
                        cout << "Median troops lost on planet " << i << " at time " <<  CURRENT_TIMESTAMP << " is " << median << ".\n";
                    }
                    else if((planets[i].median_lower.size() > 0) && (planets[i].median_lower.size() > planets[i].median_higher.size())){
                        median = planets[i].median_lower.top();
                        cout << "Median troops lost on planet " << i << " at time " <<  CURRENT_TIMESTAMP << " is " << median << ".\n";
                    }
                }
                CURRENT_TIMESTAMP += 1;
        }

        void verbose(Planet& da_planet){
             while((!da_planet.Jedi.empty()) && (!da_planet.Sith.empty()) && ((da_planet.Jedi.top().force) <= (da_planet.Sith.top().force))){
                total_battles += 1;
                if(da_planet.Jedi.top().quantity < da_planet.Sith.top().quantity){
                    uint32_t total_troops_lost = 2 * da_planet.Jedi.top().quantity;

                    //the sith attacked
                    if(verb){
                        if(da_planet.Sith.top().id > da_planet.Jedi.top().id){
                            cout << "General " << da_planet.Jedi.top().general << "'s battalion attacked" << " General " << da_planet.Sith.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }

                        //the jedi attacked
                        else{
                            cout << "General " << da_planet.Sith.top().general << "'s battalion attacked" << " General " << da_planet.Jedi.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }
                    }

                    //Add data for the median

                    // Push into median higher if either empty
                    // Compare with top values of the median priority queus and if value is less than top of median_lower put it in median_lower, else put into median_higher
                    // Rebalance

                    if(med){
                        if((da_planet.median_lower.size() == 0) && (da_planet.median_higher.size() == 0)){
                            da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                        }
                        else{
                            if(total_troops_lost < da_planet.median_higher.top()){
                                da_planet.median_lower.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            else{
                                da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            if(da_planet.median_lower.size() > da_planet.median_higher.size() && da_planet.median_lower.size() - da_planet.median_higher.size() > 1){
                                da_planet.median_higher.push(da_planet.median_lower.top());
                                da_planet.median_lower.pop();
                            }
                            else if(da_planet.median_lower.size() < da_planet.median_higher.size() && da_planet.median_higher.size() - da_planet.median_lower.size() > 1){
                                da_planet.median_lower.push(da_planet.median_higher.top());
                                da_planet.median_higher.pop();
                            }
                        }
                    }

                    //Data for generals
                    if(general){
                        // generals[da_planet.Jedi.top().general].jedi_deployed += da_planet.Jedi.top().quantity;
                        generals[da_planet.Jedi.top().general].survived_troops -= da_planet.Jedi.top().quantity;
                        // generals[da_planet.Sith.top().general].sith_deployed += da_planet.Sith.top().quantity;
                        generals[da_planet.Sith.top().general].survived_troops -= da_planet.Jedi.top().quantity;
                    }


                    //Movie Watcher




                    //Changing the quantity and poping the losing side
                    da_planet.Sith.top().quantity = da_planet.Sith.top().quantity - da_planet.Jedi.top().quantity;
                    da_planet.Jedi.pop();
                    // total_battles += 1;

                    


                }
                else if(da_planet.Sith.top().quantity < da_planet.Jedi.top().quantity){
                    uint32_t total_troops_lost = 2 * da_planet.Sith.top().quantity;


                    if(verb){
                    //The Sith attacked
                        if(da_planet.Sith.top().id > da_planet.Jedi.top().id){
                            cout << "General " << da_planet.Jedi.top().general << "'s battalion attacked" << " General " << da_planet.Sith.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }
                        //The Jedi attacked
                        else{
                            cout << "General " << da_planet.Sith.top().general << "'s battalion attacked" << " General " << da_planet.Jedi.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }
                    }

                    //Add data for the median
                    if(med){
                        if((da_planet.median_lower.size() == 0) && (da_planet.median_higher.size() == 0)){
                            da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                        }
                        else{
                            if(total_troops_lost < da_planet.median_higher.top()){
                                da_planet.median_lower.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            else {
                                da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            if(da_planet.median_lower.size() > da_planet.median_higher.size() && da_planet.median_lower.size() - da_planet.median_higher.size() > 1){
                                da_planet.median_higher.push(da_planet.median_lower.top());
                                da_planet.median_lower.pop();
                            }
                            else if(da_planet.median_lower.size() < da_planet.median_higher.size() && da_planet.median_higher.size() - da_planet.median_lower.size() > 1){
                                da_planet.median_lower.push(da_planet.median_higher.top());
                                da_planet.median_higher.pop();
                            }
                        }
                    }

                    //Data for generals
                    if(general){
                        // generals[da_planet.Jedi.top().general].jedi_deployed += da_planet.Jedi.top().quantity;
                        generals[da_planet.Jedi.top().general].survived_troops -= da_planet.Sith.top().quantity;
                        // generals[da_planet.Sith.top().general].sith_deployed += da_planet.Sith.top().quantity;
                        generals[da_planet.Sith.top().general].survived_troops -= da_planet.Sith.top().quantity;
                    }




                    //Changing the quantity and poping the losing side
                    da_planet.Jedi.top().quantity = da_planet.Jedi.top().quantity - da_planet.Sith.top().quantity;
                    da_planet.Sith.pop();
                    // total_battles += 1;
                }
                else {
                    uint32_t total_troops_lost = 2 * da_planet.Sith.top().quantity;
                    if(verb){
                    //The Sith attacked
                        if(da_planet.Sith.top().id > da_planet.Jedi.top().id){
                            cout << "General " << da_planet.Jedi.top().general << "'s battalion attacked" << " General " << da_planet.Sith.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }
                        //The Jedi attacked
                        else{
                            cout << "General " << da_planet.Sith.top().general << "'s battalion attacked" << " General " << da_planet.Jedi.top().general << "'s battalion on planet ";
                            cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost." << "\n";
                        }
                    }

                    if(med){
                        if((da_planet.median_lower.size() == 0) && (da_planet.median_higher.size() == 0)){
                            da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                        }
                        else{
                            if(total_troops_lost < da_planet.median_higher.top()){
                                da_planet.median_lower.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            else {
                                da_planet.median_higher.push(static_cast<uint32_t>(total_troops_lost));
                            }
                            if(da_planet.median_lower.size() > da_planet.median_higher.size() && da_planet.median_lower.size() - da_planet.median_higher.size() > 1){
                                da_planet.median_higher.push(da_planet.median_lower.top());
                                da_planet.median_lower.pop();
                            }
                            else if(da_planet.median_lower.size() < da_planet.median_higher.size() && da_planet.median_higher.size() - da_planet.median_lower.size() > 1){
                                da_planet.median_lower.push(da_planet.median_higher.top());
                                da_planet.median_higher.pop();
                            }
                        }
                    }

                    //Data for generals
                    if(general){
                        // generals[da_planet.Jedi.top().general].jedi_deployed += da_planet.Jedi.top().quantity;
                        generals[da_planet.Jedi.top().general].survived_troops -= da_planet.Jedi.top().quantity;
                        // generals[da_planet.Sith.top().general].sith_deployed += da_planet.Sith.top().quantity;
                        generals[da_planet.Sith.top().general].survived_troops -= da_planet.Jedi.top().quantity;
                    }

                    da_planet.Jedi.pop();
                    da_planet.Sith.pop();
                }
            }
        }

        void at_watcher(Planet& da_planet, Deployment person){
            
            if(watcher){
                //Seen None State
                if(da_planet.state == State::Initial){
                    if(person.side == 'J'){
                        da_planet.state = State::SeenOne;
                        da_planet.jedi_force = person.force;
                        da_planet.jedi_time = person.time;
                    }
                }

                //Seen One State
                else if(da_planet.state == State::SeenOne){
                    if(person.side == 'S' && person.force >= da_planet.jedi_force){
                        da_planet.sith_force = person.force;
                        da_planet.at_exist = true;
                        da_planet.state = State::SeenBoth;
                        da_planet.sith_time = person.time;
                    }
                    if(person.side == 'J' && person.force < da_planet.jedi_force){
                        da_planet.jedi_force = person.force;
                        da_planet.jedi_time = person.time;
                    }
                }

                //Seen two State
                else if(da_planet.state == State::SeenBoth){
                    if(person.side == 'J' && person.force < da_planet.jedi_force){
                        da_planet.maybe_jedi_force = person.force;
                        da_planet.maybe_jedi_time = person.time;
                        da_planet.state = State::MaybeBetter;
                    }
                    if(person.side == 'S' && person.force > da_planet.sith_force){
                        da_planet.sith_force = person.force;
                        da_planet.sith_time = person.time;
                    }
                }

                //Maybe Better
                else if(da_planet.state == State::MaybeBetter){
                    if(person.side == 'S' && person.force >= da_planet.sith_force){
                        da_planet.sith_force = person.force;
                        da_planet.jedi_force = da_planet.maybe_jedi_force;
                        da_planet.sith_time = person.time;
                        da_planet.jedi_time = da_planet.maybe_jedi_time;
                        da_planet.state = State::SeenBoth;
                    }
                    if(person.side == 'J' && person.force < da_planet.maybe_jedi_force){
                        da_planet.maybe_jedi_force = person.force;
                        da_planet.maybe_jedi_time = person.time;
                    }                   
                }

            }
        }

        void am_watcher(Planet& da_planet, Deployment person){
            if(watcher){
                //Seen None State
                if(da_planet.state2 == State::Initial){
                    if(person.side == 'S'){
                        da_planet.state2 = State::SeenOne;
                        da_planet.sith_force2 = person.force;
                        da_planet.sith_time2 = person.time;
                    }
                }

                //Seen One State
                else if(da_planet.state2 == State::SeenOne){
                    if(person.side == 'J' && person.force < da_planet.sith_force2){
                        da_planet.jedi_force2 = person.force;
                        da_planet.ambush_exist = true;
                        da_planet.state2 = State::SeenBoth;
                        da_planet.jedi_time2 = person.time;
                    }
                    if(person.side == 'S' && person.force >= da_planet.jedi_force){
                        da_planet.sith_force2 = person.force;
                        da_planet.sith_time2 = person.time;
                    }
                }

                //Seen two State
                else if(da_planet.state == State::SeenBoth){
                    if(person.side == 'S' && person.force >= da_planet.jedi_force2){
                        da_planet.maybe_sith_force2 = person.force;
                        da_planet.maybe_sith_time2 = person.time;
                        da_planet.state2 = State::MaybeBetter;
                    }
                    if(person.side == 'J' && person.force < da_planet.jedi_force2){
                        da_planet.jedi_force2 = person.force;
                        da_planet.jedi_time2 = person.time;
                    }
                }

                //Maybe Better
                else if(da_planet.state == State::MaybeBetter){
                    if(person.side == 'J' && person.force < da_planet.jedi_force2){
                        da_planet.jedi_force2 = person.force;
                        da_planet.sith_force2 = da_planet.maybe_sith_force2;
                        da_planet.jedi_time2 = person.time;
                        da_planet.sith_time2 = da_planet.maybe_sith_time2;
                        da_planet.state2 = State::SeenBoth;
                    }
                    if(person.side == 'S' && person.force >= da_planet.maybe_jedi_force){
                        da_planet.maybe_sith_force2 = person.force;
                        da_planet.maybe_sith_time2 = person.time;
                    }                   
                }

            }
        }

        void battle(Planet& da_planet, Deployment person){
            if((person.time != CURRENT_TIMESTAMP) && (med == true)){
                median();
            }
            if(watcher){
                at_watcher(da_planet, person);
                am_watcher(da_planet,person);
            }
                verbose(da_planet);
            }
      
        void last_line(){
            cout << "---End of Day---" << "\n";
            cout << "Battles: " << total_battles << "\n";
        }

        void general_eval(){
            cout << "---General Evaluation---" << "\n";
            for(uint32_t i = 0; i < generals.size(); i++){
                cout << "General " << i << " deployed " << generals[i].jedi_deployed << " Jedi troops and ";
                cout << generals[i].sith_deployed << " Sith troops, and " << generals[i].survived_troops;
                cout << "/" << (generals[i].jedi_deployed + generals[i].sith_deployed) << " troops survived." << "\n";
            }
        }

        void watcher_eval(){
            cout << "---Movie Watcher---\n";
            for(uint32_t i = 0; i < planets.size(); i++){
                if(planets[i].ambush_exist == true){
                    cout << "A movie watcher would enjoy an ambush on planet " << i << " with Sith at time " << planets[i].sith_time2 << " and Jedi at time " << planets[i].jedi_time << " with a force difference of " << planets[i].sith_force2 - planets[i].jedi_force2 << ".\n";
                }
                else{
                    cout << "A movie watcher would not see an interesting ambush on planet " << i << ".\n";
                }
                if(planets[i].at_exist == true){
                    cout << "A movie watcher would enjoy an attack on planet " << i << " with Jedi at time " << planets[i].jedi_time << " and Sith at time " << planets[i].sith_time << " with a force difference of " << planets[i].sith_force - planets[i].jedi_force << ".\n";
                }
                else{
                    cout << "A movie watcher would not see an interesting attack on planet " << i << ".\n";
                }
            }
        }
        
        void error_checking(Deployment Person,int general_num, int planet_num){
            if((Person.general > static_cast<uint32_t>(general_num - 1))){
                    cout << "Error one of your generals was greater then num_generals or less than 0" << "\n";
                    exit(1);
                }
                if((Person.planet > static_cast<uint32_t>(planet_num -1))){
                    cout << "Error one of your planets was greater than num_planets or less than 0" << "\n";
                    exit(1);
                }
                if(Person.force <= 0){
                    cout << "Force sensitivity is less than 0" << "\n";
                    exit(1);
                }
                if(Person.quantity <= 0){
                    cout << "Troop count is less than 0" << "\n";
                    exit(1);
                }
                if(Person.time < CURRENT_TIMESTAMP){
                    cout << "Timestamps are not increasing" << "\n";
                    exit(1);
                }
        }
};





//Does the deployment seperating DL and PR and sending it into function
void deployment2(bool verbose, bool general, bool watcher, bool median){
        // Note to self implement istream instead of cin and other one. So its just that function for both modes (for cin or sstream).
        
        stringstream ss;
        
        
        string junk;
        getline(cin, junk);

        //get mode
        string mode;
        string junk2;
        cin >> junk2;
        getline(cin, mode);

        //get num generals
        string general_num;
        cin >> junk2;
        getline(cin, general_num);

        //get num planets
        string num_planets;
        cin >> junk2;
        getline(cin, num_planets);


        if(mode == " PR"){
            cin >> junk2;
            string random_seed;
            getline(cin, random_seed);
            cin >> junk2;
            string num_deployment;
            getline(cin, num_deployment);
            string arrival_rate;
            getline(cin, arrival_rate);
            arrival_rate.erase(arrival_rate.begin(),arrival_rate.begin() + 13);
            P2random::PR_init(ss, static_cast<uint32_t>(stoi(random_seed)), static_cast<uint32_t>(stoi(general_num)), static_cast<uint32_t>(stoi(num_planets)), static_cast<uint32_t>(stoi(num_deployment)), static_cast<uint32_t>(stoi(arrival_rate)));
            Deployment person;
            string data;
            Galaxy the_galaxy;
            the_galaxy.set_mode(verbose, general, watcher, median);
            the_galaxy.resize_vector(stoi(num_planets), stoi(general_num));
            while(getline(ss,data)){
                the_galaxy.interpret(data,person,stoi(general_num), stoi(num_planets));
             }
            if(median){
                the_galaxy.median();
            }
            the_galaxy.last_line();
            if(general){
                the_galaxy.general_eval();
            }
            if(watcher){
                the_galaxy.watcher_eval();
            }


        }
        else{
            Deployment person;
            string data;
            Galaxy the_galaxy;
            the_galaxy.set_mode(verbose, general, watcher, median);
            the_galaxy.resize_vector(stoi(num_planets), stoi(general_num));
            while(getline(cin,data)){
                the_galaxy.interpret(data,person, stoi(general_num), stoi(num_planets));
            } 
            if(median){
                the_galaxy.median();
            }
            the_galaxy.last_line();
            if(general){
                the_galaxy.general_eval();
            }
            if(watcher){
                the_galaxy.watcher_eval();
            }

        }
        

        //pass in function and if its PR use ss, if its not use cin. 

    }


void getMode(int argc, char * argv[]) {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int choice;
  int index = 0;
  option long_options[] = {
    {"verbose", no_argument, nullptr,'v'},
    {"median", no_argument, nullptr, 'm'},
    {"general-eval", no_argument, nullptr, 'g'},
    {"watcher", no_argument, nullptr, 'w'},
    { nullptr, 0, nullptr, '\0' },
    //{ADD IN HELP}
    };  // long_options[]
    bool verbose = false;
    bool general = false;
    bool watcher = false;
    bool median = false;
    while ((choice = getopt_long(argc, argv, "vmgw", long_options, &index)) != -1) {
        switch (choice) {
            case 'v':
            {
                verbose = true;
                break;
            }
            case 'm':
            {
                median = true;
                break;
            }
            case 'g':
            {
                general = true;
                break;
            }
            case 'w':
            {
                watcher = true;
                break;
            }
        }
    }
    cout << "Deploying troops..." << "\n";
    deployment2(verbose,general,watcher,median);
}


int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    getMode(argc, argv);
    


    return 0;
}
