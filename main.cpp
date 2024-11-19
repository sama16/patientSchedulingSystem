#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// checking the validation of a patient's ID
bool is_valid_id(const string& id) {
    if (id.size() != 14) return false; 
    for (char c : id) {
        if (!isdigit(c)) return false; // Ensure all characters are digits
    }
    return true;
}
//generating a random time in HH:MM format
string generate_random_time() {
    int hour = rand() % 24; // Random hour (0-23)
    int minute = rand() % 60; // Random minute (0-59)
    return (hour < 10 ? "0" : "") + to_string(hour) + ":" + (minute < 10 ? "0" : "") + to_string(minute);
}

// generating a random 14-digit patient ID
string generate_random_Id() {
    return to_string(28500000000000 + rand() % 1000000); 
}

// generating a random gender ('M' or 'F')
char generate_random_gender() {
    return (rand() % 2 == 0) ? 'M' : 'F'; 
}

// generating a random service type ('Urgent' or 'Normal')
string generate_random_type() {
    return (rand() % 2 == 0) ? "Urgent" : "Normal"; 
}

//  converting time in HH:MM format to total minutes 
int convert_to_minutes(const string& time) {
    int hours = (time[0] - '0') * 10 + (time[1] - '0'); 
    int minutes = (time[3] - '0') * 10 + (time[4] - '0'); 
    return hours * 60 + minutes; // Convert to total minutes
}

class Patient {
    string id;
    char gender;
    string arrival_time;
    string service_type;

public:
    // Constructor with ID validation
    Patient(string id, char gender, string arrival_time, string service_type) {
        if (!is_valid_id(id)) {
            throw invalid_argument("Invalid ID format");
        }
        this->id = id;
        this->gender = gender;
        this->arrival_time = arrival_time;
        this->service_type = service_type;
    }

    // Getter methods
    string get_id() const { return id; }
    char get_gender() const { return gender; }
    string get_service_type() const { return service_type; }
    string get_arrival_time() const { return arrival_time; }
};
class QueueingSystem {
    queue<Patient> urgent_queue; // Queue for urgent patients
    queue<Patient> normal_queue; // Queue for normal patients
    vector<string> done_patients; // List of served patient IDs
    int total_patients = 0; 
    int urgent_count = 0;
    int normal_count = 0;
    int total_waiting_time = 0;
    int served_patients = 0;
    void display_patient_in_service(const Patient& patient) const; //  to display active patient

public:
    // Add a new patient to the appropriate queue
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

    // Serve a specified number of patients
    void serve_patients(int num_to_serve, int current_time_in_minutes) {
        for (int i = 0; i < num_to_serve; ++i) {
            try {
                if (!urgent_queue.empty()) {
                    // Serve an urgent patient
                    Patient current = urgent_queue.front();
                    display_patient_in_service(current);
                    int arrival_in_minutes = convert_to_minutes(current.get_arrival_time());
                    int waiting_time = current_time_in_minutes - arrival_in_minutes; // Calculate waiting time
                    total_waiting_time += waiting_time;
                    served_patients++;
                    done_patients.push_back(current.get_id());
                    urgent_queue.pop();
                } else if (!normal_queue.empty()) {
                    // Serve a normal patient if no urgent ones
                    Patient current = normal_queue.front();
                    display_patient_in_service(current);
                    int arrival_in_minutes = convert_to_minutes(current.get_arrival_time());
                    int waiting_time = current_time_in_minutes - arrival_in_minutes; // Calculate waiting time
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

    // Display the current state of the queues
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

    // Display IDs of patients who have been served
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

    // Display overall statistics
    void display_total() const {
        cout << "Total patients: " << total_patients << endl;
        cout << "Urgent Cases: " << urgent_count << endl;
        cout << "Normal Cases: " << normal_count << endl;
        if (served_patients > 0) {
            cout << "Average Waiting Time: " << total_waiting_time / served_patients << " minutes" << endl;
        } else {
            cout << "No patients served yet." << endl;
        }
    }
};

// Helper to display the patient currently being served
void QueueingSystem::display_patient_in_service(const Patient& patient) const {
    cout << "\nIn-Service Patient:\n"
         << "ID: " << patient.get_id()
         << ", Gender: " << patient.get_gender()
         << ", Arrival Time: " << patient.get_arrival_time()
         << ", Type: " << patient.get_service_type() << endl;
}

// Generate and add random patients to the system
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
    srand(time(0)); // Seed random number generator
    QueueingSystem system;

    // Choose a scenario for the number of patients
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
        return 1;
    }

    // Add random patients to the system
    random_patient_record(system, num_patients);

    // Simulate serving patients over 10 minutes
    int current_time_in_minutes = 0;
    for (int minute = 0; minute < 10; ++minute) {
        system.display_queues();
        cout << "\nPress Enter to advance 1 minute...";
        cin.get();
        int num_to_serve = rand() % 6 + 5; // Random number of patients to serve
        system.serve_patients(num_to_serve, current_time_in_minutes);
        system.display_done_patients();
        current_time_in_minutes++;
    }

    // Display overall statistics
    system.display_total();

    return 0;
}
