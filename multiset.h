#ifndef MULTISET_H_
#define MULTISET_H_

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>
#include <set>

template <typename K>
class Multiset {
 public:
  //
  // Public API
  //

  // * Capacity
  // Returns number of items in multiset --O(1)
  size_t Size();
  // Returns true if multiset is empty --O(1)
  bool Empty();

  // * Modifiers
  // Inserts an item corresponding to @key in multiset --O(log N) on average
  void Insert(const K &key);
  // Removes an item corresponding to @key from multiset --O(log N) on average
  //  Throws exception if key doesn't exist
  void Remove(const K &key);

  // * Lookup
  
  // Return whether @key is found in multiset --O(log N) on average
  bool Contains(const K& key);
  // Returns number of items matching @key in multiset --O(log N) on average
  //  Throws exception if key doesn't exist
  size_t Count(const K& key);

  // Return greatest key less than or equal to @key --O(log N) on average
  //  Throws exception if multiset is empty or no floor exists for key
  const K& Floor(const K &key);
  // Return least key greater than or equal to @key --O(log N) on average
  //  Throws exception if multiset is empty or no ceil exists for key
  const K& Ceil(const K &key);

  // Return max key in multiset --O(log N) on average
  //  Throws exception if multiset is empty
  const K& Max();
  // Return min key in multiset --O(log N) on average
  //  Throws exception if multiset is empty
  const K& Min();
  
 private:
  //
  // @@@ The class's internal members below can be modified @@@
  //

  // Private member variables
  // ...To be completed...

    std::multiset<int> mnode; // type of multiset 
    int mset_min; // for minimum value of multiset
    int mset_max; // for maximum value of multiset
    std::multiset<int> ::iterator itr; // iterator of multiset to move pointer
    /* to implement multiset as a BST */ 
    struct Node{   
    K key; 
    std::unique_ptr<Node> left;    
    std::unique_ptr<Node> right;  
}; 
    std::unique_ptr<Node> root; 


  // Private constants
  // ...To be completed (if any)...

  // Private methods
  // ...To be completed (if any)...
    void Insert(std::unique_ptr<Node> &n, const K &key); // to insert a value in multiset as BST
    void Remove(std::unique_ptr<Node> &n, const K &key); // to remove a value in multiset as BST
    void find_min_max(); // to find min and max value from a multiset 
    Node* delete_Min(Node *n); // to find minimum value in BST
};

//
// Your implementation of the class should be located below
//

// ...To be completed...

/* to find min and max value in multiset */
template <typename K>
void Multiset<K>::find_min_max(){ 
    std::multiset<int>::iterator itrb; // iterator for minimum value
    std::multiset<int>::reverse_iterator itrv; // iterator for maximum value
    itrb = mnode.begin();
    mset_min = *itrb; // to find minimum value
    itrv = mnode.rbegin();
    mset_max = *itrv; // to find maximum value
}

template <typename K>
bool Multiset<K>::Contains(const K &key) { 
    if(mnode.count(key) >= 1){ // use count function to check if key exsists , the complexity is logarithmic of count()
        return true;
    }else{
        return false;
    }
}

template <typename K>
bool Multiset<K>::Empty(){ 
    if(mnode.empty()){ // using function to check if multiset is empty or not, complexity is constant of empty()
        return true;
    }else{
        return false;
    }
}

template <typename K>
 size_t Multiset<K>::Size(){ 
     return mnode.size(); // using function size to check number of elements in multiset, complexity is constant of size()
 }


template <typename K>
const K& Multiset<K>::Max(void) {
   if(mnode.empty()){ 
       throw (std::underflow_error("Empty multiset")); // returns error if multiset is empty 
    }
    find_min_max(); // to find max value 
    itr = mnode.find(mset_max); // using find function to find maximum value in multiset so that it ensures logarithmic complexity 
    return *itr;
}

template <typename K>
const K& Multiset<K>::Min(void) { 
    if(mnode.empty()){
       throw (std::underflow_error("Empty multiset")); // returns error if multiset is empty
    }
   find_min_max();
   itr = mnode.find(mset_min); // using find function to find minimum value in multiset so that it ensures logarithmic complexity
    return *itr;
}

template <typename K>
size_t Multiset<K>::Count(const K& key){
    if(mnode.count(key) < 1){
       throw (std::invalid_argument("Invalid key")); // throws error if key is not found in multiset
    }
    return mnode.count(key); // using count function to count the frequency of key ensuring logarithmic complexity as count() has logarithmic complexity
}

template <typename K>
const K& Multiset<K>::Floor(const K &key){
    find_min_max(); 
    if(mnode.empty() || (mnode.count(key)==0 && (mset_min > key))){ // key is not found in multiset and if key is less than min value of multiset
       throw (std::underflow_error("Empty multiset")); // throws error 
    }
    itr = mnode.lower_bound(key); // set iterator on the position of lower bound value of key, which is more than key value or key value
    if(mnode.count(key) == 0){ // if key not found , then set iterator to previous value for floor value
        itr --; 
    }
    return *itr; // return floor value
}


template <typename K>
const K& Multiset<K>::Ceil(const K &key){
    find_min_max();
    if(mnode.empty() || (mnode.count(key)==0 && (mset_max < key))){ 
       throw (std::underflow_error("Empty multiset")); 
    }
    itr = mnode.lower_bound(key); // set iterator on the position of lower bound value , which is more than key value 
    return *itr; // return ceil value 
}

/* inserting a new value in multiset with implementation of BST in the form of left and right child*/
template <typename K>
void Multiset<K>::Insert(const K &key) { 
     Insert(root, key); 
}

template <typename K>
void Multiset<K>::Insert(std::unique_ptr<Node> &n, const K &key) {  
    if (!n){ 
       mnode.insert(key); // insert a value in multiset 
       n = std::unique_ptr<Node>(new Node); // create a new node with multiset value
       n->key=key; 
    }
    else if (key <= n->key) {  // we can also check for equality of values as multiset has duplicate keys 
         Insert(n->left, key); // set the left pointer of multiset value as node value is same as multiset value
    }else if (key >= n->key){  // we can check for greater and equality as right child is greater than current node and multiset has duplicate value
        Insert(n->right, key); // set the right pointer of multiset value as node value is same as multiset value, 
    } 
}

/* removing a  value in multiset with implementation of BST in the form of left and right child */
template <typename K>
void Multiset<K>::Remove(const K &key) {  
    if(mnode.empty()){ // displaying error if multiset is empty 
       throw (std::underflow_error("Empty multiset")); 
    }else if(mnode.count(key) < 1){ // displaying error if key is not in multiset
       throw (std::invalid_argument("Invalid key")); 
    }
    mnode.erase(mnode.lower_bound(key)); // remove a value from multiset
    Remove(root, key); // remove and set left and right child of BST of particular value of multiset
}

/* remove also left and right child accordingly as multiset is implemented as BST */
template <typename K>
typename Multiset<K>::Node* Multiset<K>::delete_Min(Node *n) { 
     if (n->left)    
        return delete_Min(n->left.get()); 
     else 
        return n;
}

template <typename K>
void Multiset<K>::Remove(std::unique_ptr<Node> &n, const K &key) {  
    // Key not found  
    if (!n) return;
    if (key < n->key) {    
        Remove(n->left, key);  
    } else if (key > n->key) {  
        Remove(n->right, key);  
    } else {
        // Found node    
      if (n->left && n->right){
          // Two children: replace with min node in right subtree      
         n->key = delete_Min(n->right.get())->key;
          Remove(n->right, n->key);    
      }else {     
             // Replace with only child or with nullptr     
              n = std::move((n->left) ? n->left : n->right);
           }  
    }
}





#endif  // MULTISET_H_