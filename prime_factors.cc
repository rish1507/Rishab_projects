#include "multiset.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string.h>
#include <string>
#include <ctype.h>

/* global variables , as they are used in multiple functions throughout the program */
unsigned int main_number; // number to be prime factorised 
Multiset < int > mset_prime; // object of class multiset in "multiset.h"
int f_value; // prime factorised value
int c_value; // prime factorised count value 

/* function to check and display  multiple errors */
void std_error(int type) {
  switch (type) {
  case 1:
    std::cout << std::endl; 
    exit(1);
    break;
  case 2:
    std::cerr << "Invalid number" << std::endl;
    exit(1);
    break;
  case 3:
    std::cerr << "Command 'near' expects another argument: [+/-]prime" <<
      std::endl;
    exit(1);
    break;
  case 4:
    std::cerr << "Invalid prime" << std::endl;
    exit(1);
    break;
  case 5:
    std::cerr << "No prime factors" << std::endl;
    exit(1);
    break;
  case 6:
    std::cerr << "Possible commands are: all|min|max|near" <<
      std::endl;
    exit(1);
    break;
  case 7:
    std::cout << "No match" << std::endl;
    exit(1);
    break;
  }
}

/* to print prime factorized value and its count in specified format */
void print_all_function(int func_value, int count_value) {
  std::cout << func_value << " (x" << count_value << ")" << 
  std::endl;
}

/* to check if the main number is a number or not */
int check_number_argument(std::string arg_string) {
  char ch;
  for (unsigned int i = 0; i < arg_string.length(); i++) {
    ch = arg_string[i]; 
    if (!isdigit(ch)) { // checking for number
      std_error(2); // displaying error from above function 
    }
  }
  return std::stoi(arg_string);
}

/* building a string of near argument after '+' '-' */
std::string build_string(std::string arg_string) {
  std::string ret_string; 
  char ch;
  for (unsigned int i = 1; i < arg_string.length(); i++) { // starting the loop after '+' '-' symbol and building a string
    ch = arg_string[i];
    if (isdigit(ch)) { // checking for number 
      ret_string += ch; // building a string 
    } else {
      std_error(4); // displaying error 
    }
  }
  return ret_string;
}

/* checking "near" argument and displaying prime factorisation with count via implementing +(ceil) and -(floor) */ 
void check_near_argument(std::string arg_string) {
   char ch;
   int arg_number = 0; // argument value after near 
   int min_prime; // minimum prime value of main factorisation number
   int max_prime; //maximum prime value of main factorisation number
  int f_error=0; // to check error 
  ch = arg_string[0]; // to check + - in first position of argument 
  if(arg_string.length()==1 && !isdigit(ch)){ // to check if the argument is only + - 
      std_error(7); //displaying error 
  }
  if (ch == '+') { // to check if argument has + sign 
    arg_number = std::stoul(build_string(arg_string)); // extract number after + sign
    max_prime = mset_prime.Max(); 
      if (max_prime <= arg_number) { // checking if argument number is greater than multiset of prime factorisation 
        std_error(7); // displaying error 
      } else {
        arg_number = arg_number + 1; // if argument number exists in the range of multiset, then add one and generate ceil value of the number
        f_value = mset_prime.Ceil(arg_number); // ceil value of number
        c_value = mset_prime.Count(mset_prime.Ceil(arg_number)); 
      }
  } else if (ch == '-') { //checking for - sign 
    arg_number = std::stoi(build_string(arg_string)); // extracting number after - sign 
    min_prime = mset_prime.Min();
      if (min_prime >= arg_number) { //checking if argument is less than multiset of prime factorisation
        std_error(7); //displaying error 
      } else {
        arg_number = arg_number - 1; // if argument number exists  in range of multiset, subtract one and floor value of the number 
        f_value = mset_prime.Floor(arg_number);
        c_value = mset_prime.Count(mset_prime.Floor(arg_number)); // count value of the numebr
      }
      /*if argument number does not have + or - sign */
  } else if (isdigit(ch)) {
    arg_string = arg_string[0] + build_string(arg_string); // extract number from argument string after near 
    arg_number = std::stoi(arg_string); 
    if (mset_prime.Contains(arg_number)) { // if number is contained in multiset of prime factors
      f_value = arg_number; // pass for print function
      c_value = mset_prime.Count(arg_number); // pass its count to print function
    } else {
      f_error = -1;
    }
  } else {
    std_error(4); // displaying error 
  }
  if (f_error == -1) {
    std_error(7);
  } else {
    print_all_function(f_value, c_value); // printing the prime factor value and its count accordingly 
  }
}

/* function for calculting prime factors of main number */
int calc_prime_factorisation(unsigned int m_number) {
  int f = 0; // to check whether a number is prime or not 
  int p_fact = -1; // prime factor of main number 
  if (m_number % 2 == 0) { // if number is divisible by 2 , then prime factor is 2
    p_fact = 2; 
  } else {
    for (unsigned int i = 3; i <= main_number / 2; i++) { // loop from 3 to half of the number to check prime factor of the number
      f = 0;
      for (unsigned int j = 2; j <= i / 2; j++) { // to check each number is prime or not
        if (i % j == 0) {
          f = 1; // number not prime
          break;
        }
      }
      if (f == 0) { // if number is prime
        if (m_number % i == 0) { // and number is also factor of number
          p_fact = i; // assigning it to prime factor 
          break;
        }
      }
    }
  }
  return p_fact;
}

/* to insert prime factor in multiet of class multiset */
void ins_prime_factorisation(unsigned int main_number) {
  int prime_fact = 1;
  unsigned int r_number; // quotient of main number 
  r_number = main_number;
  while (prime_fact > 0 && r_number > 1) { // to check condition if any remaining quotient 
    prime_fact = calc_prime_factorisation(r_number); // extracting prime factor and pass quotient to calculate another prime factor 
    if (prime_fact == -1) {
      std_error(5);
    }
    mset_prime.Insert(prime_fact); // insert in multiset 
    r_number = r_number / prime_fact; // generate quotient 
  }
}

/* to print for all command from multiset class*/
void mset_print() {
    while (mset_prime.Empty() == false) {
    f_value = mset_prime.Min(); // prime factor 
    c_value = mset_prime.Count(mset_prime.Min()); // prime factor count
    std::cout << f_value << " (x" << c_value << "), "; // displaying prime factor and its count 
    while (c_value != 0) { 
      mset_prime.Remove(f_value); // remove value and its count after print from multiset so that next value is ready for print
      c_value--;
    }
  }
  std::cout << std::endl;
}

int main(int argc, char ** argv) { 
  if (argc == 1) { // check for no argument
    std::cerr << "Usage: " << argv[0] <<
      " <number> <command> [<args>]";
    std_error(1); // displaying error 
  } else if (argc == 2) { // check for one argument 
    std::cerr << "Usage: " << argv[0] << 
      " <number> <command> [<args>]";
    std_error(1); // dispalying error 
  } else if (argc == 3) { //if it has proper argument 
    main_number = check_number_argument(argv[1]); // check for main number if is digit or not 
    std::string str1 = argv[2];
    ins_prime_factorisation(main_number); // calculate and insert prime factors in multiset
    if (str1 == "all") { //check for all 
      mset_print(); //printing prime factors of main number
    } else if (str1 == "min") { // check for min argument
      f_value = mset_prime.Min();
      c_value = mset_prime.Count(mset_prime.Min());
      print_all_function(f_value, c_value); // print minimum value from multiset 
    } else if (str1 == "max") { // check for max argument 
      f_value = mset_prime.Max();
      c_value = mset_prime.Count(mset_prime.Max());
      print_all_function(f_value, c_value); // print max value from multiset
    } else if (str1 == "near") { // if "near" is without complete argument 
      std_error(3);
    } else {
      std::cerr << "Command '" << str1 << "' is invalid" << std::endl;
      std_error(6);
    }
  } else if (argc == 4) {
    main_number = check_number_argument(argv[1]);
    std::string str1 = argv[2];
    ins_prime_factorisation(main_number); // calculate prime factors of main numbers 
    if (str1 == "near") {
        
      check_near_argument(argv[3]); // check near argument and display its value 
    } else {
      std::cerr << "Command '" << str1 << "' is invalid" << std::endl;
      std_error(6);
    }
  }
  else if (argc > 4) {
    std_error(7);
  }
  return 0;
}
