#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

/* class for calculating time in microseconds for linear and binary search */

class calc_time {
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
 public:
    double CurrentTime() {
        start = std::chrono::high_resolution_clock::now();
        end = std::chrono::high_resolution_clock::now();
        double elapsed_us = std::chrono::duration < double,
            std::micro > (end - start).count();
        return elapsed_us;
    }
};

/* class for displaying cities data via overloading
operator << and calculating safety index  */
class city {
    double inhabitants;
    double cases;
    double recovery;
    int safety_index;

 public:
    int disp(std::string str) {
    std::string str2;
    int count = 0;
    count = 0;
    /* breaking the entire line into inhabitants cases and recovery */
    for (size_t j = 0; j < str.length(); j++) {
        str2 += str[j];
        if (str[j] == ' ' || j == str.length() - 1) {
            if (count == 0) {
                inhabitants = std::stoi(str2);
            } else if (count == 1) {
                cases = std::stoi(str2);
            } else if (count == 2) {
                recovery = std::stoi(str2);
            }
            str2 = "";
            count++;
        }
    }
    /* calculating safety index */
    double first = 2.0 * (1 - (cases / inhabitants));
    double second = (1 / 2.0) * (recovery / cases);
    safety_index = 10000 * (first + second);
    return safety_index;
}
    /* declaration of freind function for operator overloading */
    friend std::ostream & operator << (std::ostream & dout, city & b);
};

/* operator overloading function to display cities data  */
std::ostream & operator << (std::ostream & dout, city & b) {
    dout << "inhabitants:  " << b.inhabitants << std::endl;
    dout << "cases: " << b.cases << std::endl;
    dout << "recovery: " << b.recovery << std::endl;
    return dout;
}

/* function for linear search to search safety indices */
int l_search(std::vector < int > & cities, std::vector < int > & safeties) {
    int count = 0;
    for (size_t i = 0; i < safeties.size(); i++) {
        for (size_t j = 0; j < cities.size(); j++) {
            if (safeties[i] == cities[j]) {
                count = count + 1;
                break;
            }
        }
    }
    return count;
}
/* function for binary search to search safety indices */
int b_search(std::vector < int > & cities, std::vector < int > & safeties) {
    int count = 0;
    int beg, end, m;
    for (size_t i = 0; i < safeties.size(); i++) {
        beg = 0;
        end = cities.size() - 1;
        while (beg <= end) {
            m = beg + (end - beg) / 2;
            if (safeties[i] == cities[m]) {
                count = count + 1;
                break;
            } else if (safeties[i] > cities[m]) {
                beg = m + 1;
            } else {
                end = m - 1;
            }
        }
    }
    return count;
}

int main(int argc, char * argv[]) {
    /* finding error for wrong number of arguments given  */
    if (argc <= 3) {
        std::cerr << "Usage: " << argv[0] <<
        " <city_file.dat> <safety_file.dat> <result_file.dat>" <<
        std::endl;
        exit(1);
    }
    /* declaring vectors for safety indices for cities and safety */
    std::vector < std::string > safety;
    std::vector < std::string > cities;
    std::vector < int > city_safety;
    std::vector < int > int_safety;
    std::ifstream fin_safety;
    std::ifstream fin_cities;
    /* creating object of class city */
    city obj_cities;
    char lne[30];
    /* finding error if file is not found */
    fin_cities.open(std::string(argv[1]));
    if (!fin_cities.good()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        exit(1);
    }
    /* reading file city in vector cities */
    do {
        fin_cities.getline(lne, 30);
        cities.push_back(lne);
    } while (fin_cities);
    fin_safety.open(std::string(argv[2]));
    /* finding error if safety file is not found */
    if (!fin_safety.good()) {
        std::cerr << "Error: cannot open file " << argv[2] << std::endl;
        exit(1);
    }
    /* reading file safety in vector safety */
    do {
        fin_safety.getline(lne, 30);
        safety.push_back(lne);
    } while (fin_safety);
    fin_safety.close();
    fin_cities.close();
    /* calculated values of city indices via class city inserted in vector */
    for (size_t i = 0; i < cities.size(); i++) {
        city_safety.push_back(obj_cities.disp(cities[i]));
    }
    for (size_t i = 0; i < safety.size()-1; i++) {
        int_safety.push_back(std::stoi(safety[i]));
    }
    int safety_count = 0;
    /* opening file to store output */
    std::ofstream nmatches;
    nmatches.open(std::string(argv[3]), std::ofstream::trunc);
    /* finding error if output file is not found */
    if (!nmatches.good()) {
        std::cerr << "Error: cannot open file(s)" << std::endl;
        exit(1);
    }
    calc_time ct;
    /* prompting the user to select between linear
    and binary search  */
    char choice;
    int chi = 0;
    do {
        std::cout << "Choice of search method ([l]inear, [b]inary)?"
            << std::endl;
        std::cin >> choice;
        switch (choice) {
        case 'l':
            safety_count = l_search(city_safety, int_safety);
            chi = 1;
            break;
        case 'b':
            std::sort(city_safety.begin(), city_safety.end());
            safety_count = b_search(city_safety, int_safety);
            chi = 1;
            break;
        default:
            std::cerr << "Incorrect choice" << std::endl;
            chi = 0;
        }
    } while (chi == 0);
    /* displaying time for linear or binary search */
    std::cout << "CPU time: " << ct.CurrentTime();
    std::cout << " microseconds " << std::endl;
    /* storing data in output file */
    nmatches << safety_count << std::endl;
    return 0;
}