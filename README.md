# patientSchedulingSystem
Overview
The Patient Scheduling System is a simple C++ application designed to simulate a queueing system for a healthcare setting. It manages patients based on their arrival time and service type (urgent or normal), ensuring that urgent cases are prioritized.

The program leverages C++ Standard Template Library (STL) features such as queue and vector to manage patient data efficiently and applies object-oriented principles for code organization.

Features of the Patient Scheduling System
1. Patient Management:
 Randomly generates patients with:
A unique 14-digit ID.
Gender (M or F).
Arrival Time in HH:MM format.
Service Type: Urgent or Normal.

2.Queue Prioritization:
Patients needing urgent care are prioritized over normal cases.
Patients are queued and served based on their arrival and urgency.

3. Interactive Workflow
Users can choose from three predefined scenarios:
Simple: 100 patients.
Moderate: 300 patients.
Crowded: 700 patients.
Minute-by-Minute Processing:

Simulates 10 minutes of real-time patient servicing.
Each minute, a random number of patients (5-10) are served from the queues.

4. Queue Status Display
Current Queues:
Displays the IDs of patients waiting in both the Urgent and Normal queues.
Served Patients:
Shows a list of patient IDs that have been successfully served.

5. Overall Statistics
Total Patient Count:
Tracks the number of patients served during the simulation.
Total number of urgent and normal cases served.
Average Waiting Time:

6. Error Handling
Ensures IDs must be exactly 14 digits.
Invalid patient records are gracefully rejected with an error message.
Handles empty queues gracefully, avoiding crashes when no patients are available to serve.
