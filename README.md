# stationchargercalculator
This project calculates the uptime percentage of charging stations based on the availability reports of individual chargers. The program reads an input file that provides two key sections: the mapping of stations to chargers, and the availability reports of those chargers. After processing this data, the program computes and outputs the uptime percentage for each charging station.

# What the Project Does
The core functionality of this project is to determine the uptime percentage of charging stations, which is based on the availability of the chargers assigned to each station. Here's a breakdown of how it works:

1. Input Parsing: The program parses an input file that contains:

Station and Charger Mapping: Each station is associated with a list of chargers.
Charger Availability Reports: These reports contain data about when each charger was up or down, with start times, end times, and status ("true" for up, "false" for down).

2. Uptime Calculation: For each station, the program:

Processes the availability reports for all chargers associated with the station.
Sorts the availability events (when chargers go up or down).
Calculates the total time and uptime for the station by evaluating when at least one charger is up.

# To compile the program, use the following g++ command:
g++ -o stationuptime stationuptime.cpp

# run the program using the following command:
./stationuptime <input_file>

# How Ambiguities are Resolved
Overlapping Charger Reports: When multiple chargers report availability at the same time, the program considers the charger to be "up" if at least one charger is available.
Missing or Incomplete Data: If charger reports are incomplete or missing, they are ignored in the calculation.

# Unit and Integration Tests
Unit Tests: To test individual components (e.g., parsing and uptime calculation), unit tests can be created using frameworks like Google Test. These tests should validate that parsing functions handle edge cases and that uptime calculations are correct.

Integration Tests: Full integration tests should use complete input files and verify that the program correctly computes the uptime for all stations.

# Known Limitations
Input File Formatting: The program assumes the input file is well-formed and follows the specified structure. Any deviation from this format may cause incorrect behavior or crashes.
Time Handling: The program uses raw timestamps for time calculations and does not account for different time zones or leap seconds.

# Conclusion
This project efficiently calculates the uptime of charging stations based on individual charger reports. It processes input data, sorts events, and computes uptime percentages in a straightforward manner.
