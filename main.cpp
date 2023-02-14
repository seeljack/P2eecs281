// AD48FB4835AF347EB0CA8009E24C3B13F8519882

// QUESTIONS
//-------------
//P2random.h is not returning the correct output
// Pop certain elements from priority queue (Do you pop the Jedi)
// Troops lost once you see that the number of troops is 0, then you pop it from the qeueue. You can pop the looser. Make sure because it could happen for
// Make struct of Planets?


//./galaxy_debug -v < large-DL-in.txt
//./galaxy_debug -v < spec-DL-in.txt
//./galaxy_debug -v < large-PR-in.txt


// Did I implement the priority queue right?
// Does my setup look right for the rest of the project?
// What is random.h
// Did i set up my getmode right?
//is there a way to optimize deployment?
//need help with structure of code and how the layout should be



//For when doing the battles make sure it keeps going until there are no more battles, instead of only seeing if batle when adding one


#include <vector>
#include <_types/_uint32_t.h>
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

#include "P2random.h"
using namespace std;


struct Deployment{
    uint32_t time;
    uint32_t force;
    mutable uint32_t quantity;
    uint32_t id;
    char side;
    uint32_t planet;
    uint32_t general;
    
};


struct  Jedi_comparing{
    //comparing Jedi force (want the smallest one to be top)
    bool operator()( const Deployment &j1,  const Deployment &j2) {
        return j1.force > j2.force;
    }
};

struct  Sith_comparing{
    //comparing Sith force (want the biggest one on top)
    bool operator()( const Deployment &s1,  const Deployment &s2) {
        return s1.force < s2.force;
    }
};

struct Planet{
    //priority queue Jedi (The lowest force is the top one)
    priority_queue<Deployment, vector<Deployment>, Jedi_comparing> Jedi;

    //priority queue Sith (The highest force is the top one)
    priority_queue<Deployment, vector<Deployment>, Sith_comparing> Sith;

    uint32_t total_count_lost = 0;

    uint32_t  battles_on_planet = 0;

};





// Galaxy class, Planet class, General Class


//Galaxy Class
class Galaxy{
    private:

        string mode;

        vector<Planet> planets;

        uint32_t id_help_counter = 0;

        uint32_t CURRENT_TIMESTAMP = 0;


    public:

        void resize_vector(int size){
            planets.resize(static_cast<uint32_t>(size));
        }

        // Reads in data one line at a time and puts all the data into the private member variables
        void interpret(string data, Deployment Person){


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
            Person.id += id_help_counter;
            id_help_counter += 1;



            if(Person.side == 'J'){
                planets[Person.planet].Jedi.push(Person);

                // cout << Person.time << ' ' << Person.side << " " << Person.general << " " << Person.planet << " " << Person.force << " " << Person.id << " " << Person.quantity << "\n";
                battle(planets[Person.planet], Person.time);

            }
            else{
                planets[Person.planet].Sith.push(Person);

                // cout << Person.time << ' ' << Person.side << " " << Person.general << " " << Person.planet << " " << Person.force << " " << Person.id << " " << Person.quantity << "\n";
                battle(planets[Person.planet], Person.time);
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

        
        void battle(Planet& da_planet, uint32_t time){
            if(time != CURRENT_TIMESTAMP){
                for(uint32_t i = 0; i < planets.size(); i++){
                    if(planets[i].battles_on_planet > 0){
                        cout << "Median troops lost on planet " << i << " at time " <<  CURRENT_TIMESTAMP << " is " << planets[i].total_count_lost / planets[i].battles_on_planet << "\n";
                        planets[i].total_count_lost = 0;
                        planets[i].battles_on_planet = 0;
                    }
                }
                CURRENT_TIMESTAMP = time;
            }

            while((!da_planet.Jedi.empty()) && (!da_planet.Sith.empty()) && ((da_planet.Jedi.top().force) <= (da_planet.Sith.top().force))){
                
                if(da_planet.Jedi.top().quantity < da_planet.Sith.top().quantity){
                    uint32_t total_troops_lost = 2 * da_planet.Jedi.top().quantity;

                    //the sith attacked
                    if(da_planet.Sith.top().id < da_planet.Jedi.top().id){
                        cout << "General " << da_planet.Sith.top().general << " battalion attacked" << " general " << da_planet.Jedi.top().general << " battalion on planet ";
                        cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost. " << " \n";
                    }

                    //the jedi attacked
                    else{
                        cout << "General " << da_planet.Jedi.top().general << " battalion attacked" << " general " << da_planet.Sith.top().general << " battalion on planet ";
                        cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost. " << " \n";
                    }

                    //Changing the quantity and poping the losing side
                    da_planet.Sith.top().quantity = da_planet.Sith.top().quantity - da_planet.Jedi.top().quantity;
                    da_planet.Jedi.pop();

                    //Add data for the median
                    da_planet.battles_on_planet += 1;
                    da_planet.total_count_lost += total_troops_lost;
                }
                else if(da_planet.Sith.top().quantity < da_planet.Jedi.top().quantity){
                    uint32_t total_troops_lost = 2 * da_planet.Sith.top().quantity;

                    //The Sith attacked
                    if(da_planet.Sith.top().id < da_planet.Jedi.top().id){
                        cout << "General " << da_planet.Sith.top().general << " battalion attacked" << " general " << da_planet.Jedi.top().general << " battalion on planet ";
                        cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost. " << " \n";
                    }
                    //The Jedi attacked
                    else{
                        cout << "General " << da_planet.Jedi.top().general << " battalion attacked" << " general " << da_planet.Sith.top().general << " battalion on planet ";
                        cout << da_planet.Sith.top().planet << ". "  << total_troops_lost << " troops were lost. " << " \n";
                    }
                    //Changing the quantity and poping the losing side
                    da_planet.Jedi.top().quantity = da_planet.Jedi.top().quantity - da_planet.Sith.top().quantity;
                    da_planet.Sith.pop();

                    //Add data for the median
                    da_planet.battles_on_planet += 1;
                    da_planet.total_count_lost += total_troops_lost;
                }
                else{
                    uint32_t total_troops_lost = 2 * da_planet.Sith.top().quantity;
                    cout << "Lost " << total_troops_lost << "troops \n";
                    da_planet.Jedi.pop();
                    da_planet.Sith.pop();
                    da_planet.battles_on_planet += 1;
                    da_planet.total_count_lost += total_troops_lost;
                }
            }
            // if(planets[static_cast<uint32_t>(0)].Jedi.top().time){
            //     cout << planets[static_cast<uint32_t>(0)].Jedi.top().time;
            // for(int i = 0; static_cast<uint32_t>(i) < planets.size(); i++){
            //     cout << planets[static_cast<uint32_t>(i).Jedi.top().quantity];
            //     // cout << planets[static_cast<uint32_t>(i)].Jedi.top().quantity;
            // }
               
            }

};




//Does the deployment seperating DL and PR and sending it into function
void deployment2(){
        // Note to self implement istream instead of cin and other one. So its just that function for both modes (for cin or sstream).
        stringstream ss;
        
        
        string junk;
        getline(cin, junk);

        //get mode
        string mode;
        string junk2;
        cin >> junk2;
        getline(cin, mode);
        cout << mode << "\n";

        //get num generals
        string general_num;
        cin >> junk2;
        getline(cin, general_num);
        cout << general_num << "\n";

        //get num planets
        string num_planets;
        cin >> junk2;
        getline(cin, num_planets);
        cout << num_planets << "\n";

        if(mode == "PR"){
            cin >> junk2;
            string random_seed;
            getline(cin, random_seed);
            cin >> junk2;
            string num_deployment;
            getline(cin, num_deployment);
            string arrival_rate;
            getline(cin, arrival_rate);
            P2random::PR_init(ss, static_cast<uint32_t>(stoi(random_seed)), static_cast<uint32_t>(stoi(general_num)), static_cast<uint32_t>(stoi(num_planets)), static_cast<uint32_t>(stoi(num_deployment)), static_cast<uint32_t>(stoi(arrival_rate)));
            Deployment person;
            string data;
            Galaxy the_galaxy;
            while(getline(ss,data)){
                the_galaxy.interpret(data,person);
            } 
        }
        else{
            Deployment person;
            string data;
            Galaxy the_galaxy;
            the_galaxy.resize_vector(stoi(num_planets));
            while(getline(cin,data)){
                the_galaxy.interpret(data,person);
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

    while ((choice = getopt_long(argc, argv, "vmgw", long_options, &index)) != -1) {
        switch (choice) {
            case 'v':
            {
                cout << "verbose" << "\n";
                deployment2();

            }
            break;
            case 'm':
            {
                cout << "median" << "\n";
            }
            break;
            case 'g':
            {
                cout << "general-eval" << "\n";
            }
            break;
            case 'w':
            {
                cout << "watcher" << "\n";
            }
            break;
        }
    }
}


int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    getMode(argc, argv);
    


    return 0;
}
