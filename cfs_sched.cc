#include <fstream>
#include <iostream>
#include <memory>
#include "multimap.h"
#include <string>
#include <vector>


/*class task for a particular record from a file */
class task {
  char identifier; // name of task
  unsigned int start_time; // starting time of task
  unsigned int duration; // duration of task
  public:
    task(char id, unsigned int s_time, unsigned int dur) { // constructor to assign task 
      identifier = id;
      start_time = s_time;
      duration = dur;
    }
  char get_id() { // returns particular id of the current task
    return identifier;
  }
  unsigned int get_stime() { // returns particular start time of the task
    return start_time;
  }
  unsigned int get_duration() { // returns duration of the current task 
    return duration;
  }
  friend std::ostream & operator << (std::ostream & dout, task & b); // operator overloading to print tasks
};
/*displaying task data by overloading for debugging purposes */
std::ostream & operator << (std::ostream & dout, task & b) {
  dout << "identifier:  " << b.identifier << std::endl;
  dout << "start_time: " << b.start_time << std::endl;
  dout << "duration: " << b.duration << std::endl;
  return dout;
}

/* building a string by input arguments */
std::string build_string(std::string arg_string) {
  std::string ret_string;
  char ch;
  for (unsigned int i = 0; i < arg_string.length(); i++) { // loop for character and digit from the input string
    ch = arg_string[i];
    if (isdigit(ch)) { // checking for number 
      ret_string += ch; // building a string 
    }
  }
  return ret_string;
}
/*  building a task object from a input string */
task * build_object(std::string str) {
  std::string str_join; // to join a value  
  int count = 0;
  char id; // identifier to build object
  unsigned int st; // start time to build object
  unsigned int du; // duration to build object
  for (size_t j = 0; j < str.length(); j++) { // loop for identifying id , start time and duration from a record
    str_join += str[j];
    if (str[j] == ' ' || j == str.length() - 1) {
      if (count == 0) { // producing an id from the record
        id = str_join[0];
      } else if (count == 1) { // producing start time from the record
        st = std::stoi(build_string(str_join));
      } else if (count == 2) { // producing duration of the task from the record
        du = std::stoi(build_string(str_join));
      }
      str_join = ""; // for next task
      count++; // to differenciate id , start and duration 
    }
  }
  task * task_object = new task(id, st, du); // to build a task object from record
  return task_object;
}
/* function to sort the vector of object if input is in unsorted order */
std::vector < task * > sorting_id(std::vector < task * > temp_v) {
  task * ptr; // temporary task for swaping
  for (unsigned int i = 0; i < temp_v.size(); i++) { // loop to sort 
    for (unsigned int j = 0; j < temp_v.size() - i - 1; j++) { // loop to sort 
      if (temp_v[j] -> get_id() > temp_v[j + 1] -> get_id()) { // swapping the values on the basis of id
        ptr = temp_v[j];
        temp_v[j] = temp_v[j + 1];
        temp_v[j + 1] = ptr;
      }
    }
  }
  return temp_v; // return sorted vector from the identifier
}
/* displaying scheduling */
int print_task(int n_task, int c_fin_task, unsigned int t_num, char c_id) {
  std::string str; // displaying assigned task in a particular format 
  if (n_task > 0) { // displaying if some scheduling task exists 
    if (c_fin_task == 1) { // display task if it final task 
      str = std::to_string(t_num) + " [" + std::to_string(n_task) + "]: " + c_id + "*";
      std::cout << str << std::endl;
      n_task--;
    }
    if (c_fin_task == 0) { // display if it not the final task
      str = std::to_string(t_num) + " [" + std::to_string(n_task) + "]: " + c_id;
      std::cout << str << std::endl;
    }
  } else { // display if no task is assigned yet
    str = std::to_string(t_num) + " [" + std::to_string(n_task) + "]: " + "_";
    std::cout << str << std::endl;
  }
  return n_task;
}
/* function to schedule task using class task */
void scheduling(std::vector < task * > vect, int tot_task) {
  Multimap < int, char > multimap; // referencing from multimap.h to insert a particular schedule task
  unsigned int min_vruntime = 0; // representing minimum virtual time
  char current_task_id = ' '; // identifier to asssign task to cpu 
  int total_num_tasks = 0; // total number of task as incoming to the cpu
  unsigned int v_runtime_current_key = 0; // current assigning task for virtual runtime to cpu
  unsigned int v_runtime_next_key = 0; // next generating virtual runtime to current assigning task 
  int current_working_task = 0; // to count task in timeline
  int f_skip_multimap = 0; // checking if current task assigns to resource to again get resource, skip for multimap
  int f_check_finish_task = 0; // checking if a task is finished or not
  char finish_task_id = ' '; // identifier for finish task
  int fin_task = 0; //   number of finished task
  unsigned int tick = 0; // initializing tick with 0, as stated 
  while (tot_task != fin_task) { // loop to check if finished task is equal to total task, if true then timeline task is finished
    for (unsigned int j = 0; j < vect.size(); j++) { //loop for vectors
      task * check_ptr = vect[j];
      if (tick == check_ptr -> get_stime()) { // if current tick is starting time of a particular task
        total_num_tasks++; // number of task is increased to obtain rescources 
        multimap.Insert(min_vruntime, check_ptr -> get_id()); // insert in multimap of current task as minimum virtual time and identifier as its value
        current_working_task++; // number of scheduled task increases in timeline
      }
      check_ptr = nullptr; // freeing memory
      delete check_ptr;
    }
    /* starting task from timeline, extracting from multimap*/
    if (total_num_tasks > 0) { //check if a task is there
      if (f_skip_multimap == 0) { // checking if current task assigns to resource to again get resource, skip for multimap
        v_runtime_current_key = multimap.Min(); // extracting minimum from multimap as it the first task in timeline
        current_task_id = multimap.Get(v_runtime_current_key); // current key assigned to current task id
        multimap.Remove(v_runtime_current_key); // removing it from multimap
        current_working_task--; // decrease the scheduled task from the timeline
        if (current_working_task > 0) {
          min_vruntime = multimap.Min(); // to change minimum virtual runtime to task virtual runtime
        }
      }
      /* to check if task is finished or not */
      for (unsigned int k = 0; k < vect.size(); k++) { // loop for vector 
        v_runtime_next_key = v_runtime_current_key + 1; // assign next virtual time to current assigned task
        task * check_ptr = vect[k];
        if (current_task_id == check_ptr -> get_id() && v_runtime_next_key == check_ptr -> get_duration()) { // check id to current task id and duration if it is finished or not
          f_check_finish_task = 1; // if the assigned task is finished then flag value is 1
          fin_task++; // finished task increase by one
          finish_task_id = current_task_id; // assign current task id to finish task id     
        }
        check_ptr = nullptr; // freeing memory
        delete check_ptr;
      }
      /* after finishing the task, increase the value of assign task by one and insert in multimap */
      if (finish_task_id != current_task_id) { // check for if it not a finished task
        v_runtime_next_key = v_runtime_current_key + 1; // assign virtual runtime for current task
        if (v_runtime_next_key > min_vruntime) { // check if next virtual runtime is greater than current gloabl minimum virtual runtime
          multimap.Insert(v_runtime_next_key, current_task_id); // insert the task to the back of the timeline
          current_working_task++; // timeline task increases by one
          f_skip_multimap = 0; // inserted in multimap, so flag value is 0
          finish_task_id = ' '; // no finished task
          /* check if it is not a timeline task, so not required to insert in multimap as it gets resourced again */
        } else {
          f_skip_multimap = 1; // skip from multimap
          v_runtime_current_key = v_runtime_next_key; // next virtual time is current virtual time
        }
      }
    }
    total_num_tasks = print_task(total_num_tasks, f_check_finish_task, tick, current_task_id); // calling print function
    f_check_finish_task = 0; // now again set finish task to zero for current tick 
    tick++; // increase the tick by one
  }
  /* after completing the full task, freeing the memory from the vector */
  for (unsigned int k = 0; k < vect.size(); k++) {
    vect[k] = nullptr;
    delete vect[k];
  }
}

int main(int argc, char * argv[]) {
  char task_lne[30]; // a single record from the file
  std::string fname_check; // check the file name is correct or not
  std::ifstream task_fin; // input stream, read object
  std::vector < task * > vect; // vector of object of class task
  std::vector < task * > vect_temp; // vector of object of class task without sorting
  int tot_task = 0; // number of total task
  if (argc <= 1) { // if no file name is given
    std::cerr << "Usage: " << argv[0] <<
      " <task_file.dat>" <<
      std::endl;
    exit(1);
  }
  fname_check = argv[1]; // if wrong file name is given, give error 
  if (fname_check != "task.dat") {
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    exit(1);
  }
  task_fin.open(std::string(argv[1]));
  /* finding error if task file is not found */
  if (!task_fin.good()) {
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    exit(1);
  }
  /* reading task in vect_temp without sorting */
  do {
    task_fin.getline(task_lne, 30);
    std::string str; // variable to check if blank record found from the file 
    str = task_lne;
    if (str.length() != 0) {
      tot_task++;
      vect_temp.push_back(build_object(task_lne)); // push a task file object to temp vector
    }
  } while (!task_fin.eof());
  task_fin.close();
  vect = sorting_id(vect_temp); // sorted vector is now in vector vect
  /* scheduling task which is in vector */
  scheduling(vect, tot_task);
  return 0;
}