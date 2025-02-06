#include <iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

using namespace std;

//structure to store charger availability report

struct availability
{
    uint64_t start_time;
    uint64_t end_time;
    bool is_up;
};

//function to parse input file
void parse_input(const string& filename, map<uint32_t, set<uint32_t> >& station_chargers, map<uint32_t, vector<availability> >& charger_reports)
{
    ifstream file(filename);
    if(!file){
        cerr << "ERROR: Unable to open file" << endl;
        exit(1);
    }

    string line;
    bool reading_stations = true; //flag to check if we are reading station data or charger reports

    while(getline(file, line)){
        if(line.empty()) continue; //skip empty lines

        if(line == "[Charger Availability Reports]"){
            reading_stations = false;
            continue;
        }

        stringstream ss(line);
        if(reading_stations){
            //parsing station and charger mapping
            uint32_t station_id, charger_id;
            ss >> station_id;
            while (ss >> charger_id)
            {
                station_chargers[station_id].insert(charger_id);
            }
        }
        else{
            //parsing charger availability reports
            uint32_t charger_id;
            uint64_t start_time, end_time;
            string status;
            ss >> charger_id >> start_time >> end_time >> status;
            bool is_up = (status == "true");

            availability report = {start_time, end_time, is_up};
            charger_reports[charger_id].push_back(report);

        }
    }
    file.close();
}
//comparator function for sorting events
bool compareEvents(const pair<uint64_t, int>& a, const pair<uint64_t, int>& b) {
    return a.first == b.first ? a.second < b.second : a.first < b.first;
}

//function to calculate uptime percentage for each station
void calculate_uptime(const map<uint32_t, set<uint32_t> >& station_chargers, const map<uint32_t, vector<availability> > & charger_reports){
    map<uint32_t, double> station_uptime; //mao to store uptime percentage for each station

    for(map<uint32_t, set<uint32_t> >::const_iterator it = station_chargers.begin(); it != station_chargers.end(); ++it){
        uint32_t station_id = it ->first;
        const set<uint32_t> & chargers = it->second;

        uint64_t total_time = 0, up_time = 0;

        vector<pair<uint64_t, int> > events; //store events for sorting

        //iterate through chargers assigned to this station
        for(set<uint32_t>::const_iterator charger_it = chargers.begin(); charger_it != chargers.end(); ++charger_it){
            uint32_t charger_id = *charger_it;


            if(charger_reports.find(charger_id)!= charger_reports.end()){
                //process all availability reports for this charger
                for (std::vector<availability>::const_iterator it = charger_reports.at(charger_id).begin(); it != charger_reports.at(charger_id).end(); ++it) {
                    const availability& report = *it;
                    events.push_back(make_pair(report.start_time, report.is_up ? 1 : -1));
                    events.push_back(make_pair(report.end_time, report.is_up ? -1 : 1));
                    }
            }
        }
        //sort events based on time(start first and then end if times are equal)
        sort(events.begin(), events.end(),compareEvents);
        
        int active_chargers = 0;
        uint64_t last_time = 0;
        bool first_event = true;

        //process sorted events to calculate uptime
        for(vector<pair<uint64_t, int > >:: const_iterator event_it = events.begin(); event_it != events.end(); ++event_it){
            uint64_t time = event_it->first;
            int change = event_it->second;

            if(!first_event){
                if(active_chargers > 0){
                up_time +=(time - last_time);// add time to uptime if any chargers were active
            }
            total_time += time - last_time;
            }
            
            active_chargers += change; //update active chargers count
            last_time = time;
            first_event = false;
        }

        //compute uptime percentage and store in the map
        if(total_time > 0){
            station_uptime[station_id]=(up_time * 100)/ total_time;
        }
        else{
            station_uptime[station_id] = 0;
        }
    }
    //output results
    for (map<uint32_t, double>::const_iterator it = station_uptime.begin(); it != station_uptime.end(); ++it) {
        uint32_t station_id = it->first;
        double uptime = it->second;
        cout << station_id << " " << static_cast<int>(uptime) << endl;
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        cerr << "ERROR" << endl;
        return 1;
    }

    string filename = argv[1];
    map<uint32_t, set<uint32_t> > station_chargers;//map to store station and its chargers
    map<uint32_t, vector<availability> > charger_reports; // map to store charger reports

    parse_input(filename, station_chargers, charger_reports); // parse input file
    calculate_uptime(station_chargers, charger_reports); //compute uptime

    return 0;
}