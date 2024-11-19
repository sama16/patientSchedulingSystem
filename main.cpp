#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// checking ID validation
bool is_valid_id(const string& id) {
    if (id.size() != 14) return false;
    for (char c : id) {
        if (!isdigit(c)) return false;
    }
    return true;
}

//  generating random valid time
string generate_random_time() {
    int hour = rand() % 24;
    int minute = rand() % 60;
    return (hour < 10 ? "0" : "") + to_string(hour) + ":" + (minute < 10 ? "0" : "") + to_string(minute);
}
//generate random ID 
string generate_random_Id(){
    string ID= to_string(28500000000000 + rand() % 1000000);
    return ID;
    
}
///generate random gender
char generate_random_gender(){
    char gender = (rand() % 2 == 0) ? 'M' : 'F';
    return gender;
    
}
//generate random time
string generate_random_type(){
    string type= (rand() % 2 == 0) ? "Urgent" : "Normal";
    return type;
    
}
//function that helps to display average time
int convert_to_minutes(const string& arrival_time) {
    int hours = (arrival_time[0] - '0') * 10 + (arrival_time[1] - '0'); // Convert hours
    int minutes = (arrival_time[3] - '0') * 10 + (arrival_time[4] - '0'); // Convert minutes
    
    // Convert the time to total minutes since midnight
    return hours + minutes;
}
//patient class

class Patient {
    string id;
    char gender;
    string arrival_time;
    string service_type;
    
public:
    //constructor
    Patient(string id, char gender, string arrival_time, string service_type) {
        if (!is_valid_id(id)) {
            throw invalid_argument("Invalid ID format");
        }
        this->id = id;
        this->gender = gender;
        this->arrival_time = arrival_time;
        this->service_type = service_type;
    }
    //getters
    
    string get_id() const { return id; }
    char get_gender() const { return gender; }
    string get_service_type() const { return service_type; }
    string get_arrival_time() const { return arrival_time; }
};
//QueueingSystem class
class QueueingSystem {
    queue<Patient> urgent_queue;
    queue<Patient> normal_queue;
    vector<string> done_patients;
    int total_patients = 0; 
    int urgent_count = 0;          
    int normal_count = 0;
    int total_waiting_time = 0;
    int served_patients = 0;
    void display_patient_in_service(const Patient& patient) const;
public:
    //Dispatching Patients
    void add_new_patient(const Patient& patient) {
        if (patient.get_service_type() == "Urgent") {
            urgent_queue.push(patient);
            urgent_count++;
        } else {
            normal_queue.push(patient);
            normal_count++;
        }
        total_patients++;
    }
    //Serving Patients
    void serve_patients(int num_to_serve,int current_time_in_minutes) {
        for (int i = 0; i < num_to_serve; ++i) {
            try {
                if (!urgent_queue.empty()) {
                    Patient current = urgent_queue.front();
                    display_patient_in_service(current);
                    int arrival_in_minutes = convert_to_minutes(current.get_arrival_time());
                    int waiting_time = current_time_in_minutes + arrival_in_minutes; // Calculate waiting time
                    total_waiting_time += waiting_time;
                    served_patients++;
                    done_patients.push_back(current.get_id());
                    urgent_queue.pop();
                } else if (!normal_queue.empty()) {
                    Patient current = normal_queue.front();
                    display_patient_in_service(current);
                    int arrival_in_minutes = convert_to_minutes(current.get_arrival_time());
                    int waiting_time = current_time_in_minutes + arrival_in_minutes; // Calculate waiting time
                    total_waiting_time += waiting_time;
                    served_patients++;
                    done_patients.push_back(current.get_id());
                    normal_queue.pop();
                } else {
                    throw underflow_error("No patients to serve.");
                }
            } catch (const underflow_error& e) {
                cout << "Error: " << e.what() << endl;
                break;
            }
        }
    }
    
    void display_queues() const {
        cout << "\nWaiting Urgent Patients: ";
        if (urgent_queue.empty()) {
            cout << "None";
        } else {
            queue<Patient> temp = urgent_queue;
            while (!temp.empty()) {
                cout << temp.front().get_id() << " ";
                temp.pop();
            }
        }
        cout << "\nWaiting Normal Patients: ";
        if (normal_queue.empty()) {
            cout << "None";
        } else {
            queue<Patient> temp = normal_queue;
            while (!temp.empty()) {
                cout << temp.front().get_id() << " ";
                temp.pop();
            }
        }
        cout << endl;
    }
    void display_done_patients() const {
        cout << "\nDone Patients: ";
        if (done_patients.empty()) {
            cout << "None";
        } else {
            for (const auto& id : done_patients) {
                cout << id << " ";
            }
        }
        cout << endl;
    }
    void display_total() const {
        cout<<"Total patients"<<total_patients<<endl;
        cout << "Urgent Cases: " << urgent_count << endl;
        cout << "Normal Cases: " << normal_count << endl;
        
        cout << "Average Waiting Time: " << total_waiting_time / served_patients << " minutes" << endl;
    }
};
void QueueingSystem::display_patient_in_service(const Patient& patient) const {
    cout << "\nIn-Service Patient:\n"
         << "ID: " << patient.get_id()
         << ", Gender: " << patient.get_gender()
         << ", Arrival Time: " << patient.get_arrival_time()
         << ", Type: " << patient.get_service_type() << endl;
}
void random_patient_record(QueueingSystem& system, int num_patients) {
    for (int i = 0; i < num_patients; i++) {
        try {
            string id = generate_random_Id(); // Generate ID
            char gender = generate_random_gender();
            string arrival_time = generate_random_time();
            string type = generate_random_type();
            Patient p(id, gender, arrival_time, type);
            system.add_new_patient(p);
        } catch (const invalid_argument& e) {
            cout << "Error while adding patient: " << e.what() << endl;
        }
    }
}

int main() {
    srand(time(0));
    QueueingSystem system;
    
    cout << "Select scenario:\n"
         << "1. Simple (100 patients)\n"
         << "2. Moderate (300 patients)\n"
         << "3. Crowded (700 patients)\n"
         << "Enter choice: ";
    int choice;
    cin >> choice;
    
    int num_patients;
    if (choice == 1) {
        num_patients = 100;
    } else if (choice == 2) {
        num_patients = 300;
    } else if (choice == 3) {
        num_patients = 700;
    } else {
        cout << "Invalid choice";
        
    }
    
    
    random_patient_record(system, num_patients); 
    int current_time_in_minutes = 0;
    for (int minute = 0; minute < 10; ++minute) {
        system.display_queues();
        cout << "\nPress Enter to advance 1 minute...";
        cin.get();
        int num_to_serve = rand() % 6 + 5;
        system.serve_patients(num_to_serve,current_time_in_minutes);
        system.display_done_patients();
        current_time_in_minutes++;
    }
    system.display_total();
    
    return 0;

        }
