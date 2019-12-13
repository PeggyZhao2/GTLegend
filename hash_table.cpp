 /*
 Student Name: Peiqi(Peggy) Zhao
 Date: 11/1/2019

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to  "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h" //same as import interface


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
  //write my code....
  //create HashTableEntry object
  HashTableEntry* htb = (HashTableEntry*)malloc(sizeof(HashTableEntry));  //allocation
  htb->key = key;
  htb->value = value;
  htb->next = NULL;
  return htb;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
  //buckets[0] points to the first row's first node in SLL
  HashTableEntry* pointer = NULL;
  int index = hashTable->hash(key);

  HashTableEntry* curr = hashTable->buckets[index];   //is the head of the linked list, is hashMapEntry
  while (curr) {//current is not null
    if (curr->key == key) {
     pointer = curr;
     break;   
    }
    //curr = hashTable->buckets[++index];    //pointer ++;
    curr = curr->next;
  }
  return pointer;

//return NULL;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) { //provided
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}


void destroyHashTable(HashTable* hashTable) {
    //free(hashTableEntry->value)
    //free(hashTableEntry)
    //free(buckets) ???????????@@@@@@@@@@@@@@@@@@
    //free(hashTable) 

    HashTableEntry* curr;
    HashTableEntry* temp;
    //HashTableEntry** tempBuckets;

    for (int i = 0; i < hashTable->num_buckets; i++) {
        curr = hashTable->buckets[i];
        // while (curr && curr->next) {    //curr.next != null
        //     pValue = curr->value;
        //     free(pValue);       //free(value)
        //     temp = curr;
        //     curr = curr->next;
        //     free(temp);         //free(entry)
        // }
        ////free(hashTable->buckets[i]);        //////////////////////////////////////


        //______________//
        while (curr) {
          temp = curr->next;
          free(curr->value);
          free(curr);
          curr = temp;
          // if (curr->next) {
          //   //free(curr->value);
          //   //temp = curr->next;
          //   curr = curr->next;
          //   free(temp);
          // }

          //curr = curr->next;
        }
    }

    free(hashTable->buckets);     //buckets?
    free(hashTable);        //free(hashTable)
}

//write the createentry to make key unique??????
void* insertItem(HashTable* hashTable, unsigned int key, void* value) {//fixed edge case
    //return old value if it is overwritten, or NULL if not replaced
    
    void* oldValue = NULL;
    int bucketsIndex = hashTable->hash(key);
    HashTableEntry* curr = hashTable->buckets[bucketsIndex];

    //0 elements in linked list
    if (!curr) { //curr == null
        //printf("!!!!!!!!!!!!!!!!!!!!!!!!!");
        curr = createHashTableEntry(key, value);
        hashTable->buckets[bucketsIndex]=curr;
        return NULL;
    }

    while (curr->next && curr->key != key) {  //while curr != null && curr.key != key
        curr = curr->next;
    }

    //insert at curr.next
    if (curr->key == key) {   //curr->next != null, then key already existed
        oldValue = curr->value;    //over-written
        curr->value = value;    //no allocation
    } else {
        curr->next = createHashTableEntry(key, value);  //at end of linked list
    }
    
    return oldValue;
}

void* getItem(HashTable* hashTable, unsigned int key) { //edge case 0 fixed
    //get the correspoinding value
    //hash key

    // int index = hashTable->hash(key);
    // HashTableEntry* curr = hashTable->buckets[index];

    // //0 elements in linked list
    // if (!curr) {
    //     //printf("####EDGE CASE");        //curr is NULL
    //     return NULL;
    // }

    // while (curr->next && curr->key != key) {  //curr.next != null  && curr.key != key
    //     curr = curr->next;
    // }
    // if (curr->key == key) { //next is not null
    //     //printf("value found@@@@");
    //     return curr->value;
    // }
    // //printf("$$$$$$$$$$$$$$$");
    // return NULL;
    if (findItem(hashTable, key) == NULL) {
      return NULL;
    }
    return findItem(hashTable, key)->value;
}

//return the pointer of the value corresponding to the key, or NULL if the key is not present
void* removeItem(HashTable* hashTable, unsigned int key) {  //edge case solved
    //item in the middle or at the head
    //item at the end

    int index = hashTable->hash(key);
    HashTableEntry* curr = hashTable->buckets[index];
    void* oldValue = NULL;

    //0 elements in linked list
    if (!curr) {
        //printf("^^^^^^^^^^^^^^^^");
        return oldValue;
    }
    //1 element in linked list
    if (curr->next == NULL) {
      //printf("##################");   //run into this if statement
        if (curr->key == key) {
            oldValue = curr->value;
            //free(curr);
            free(hashTable->buckets[index]);
            hashTable->buckets[index] = NULL;
            //curr = NULL;        // is this right??? don't need this?@@@@@@
        }
        return oldValue;
    }
    //edge case: 1. the LList has more than one 2. key matched with the 1st element in LList
    if (hashTable->buckets[index]->key == key) {
      oldValue = hashTable->buckets[index]->value;
      HashTableEntry* temp = curr->next;
      free(curr);
      hashTable->buckets[index] = temp;
      return oldValue;
    }
    //-----------finish edge case-----------------//
    //printf("##################");

    while (curr->next->next && curr->next->key != key) {    //curr.next.next != null
        curr = curr->next;
    }

    if (curr->next->key == key) { //curr.next.key == key--remove: curr.next
        oldValue = curr->next->value;
        //printf("@@@@@@@  OLd value = %p   %n@@@@@@@@@", curr->next->value);
        HashTableEntry* temp = curr->next->next;
        free(curr->next);       //free
        //if (temp != NULL) {
          curr -> next = temp;
        //}
        //curr->next = temp;
    }

    return oldValue;
}

//What's the difference between delete and remove()????@@@@@@@
//Can I just  call removeItem()???????????@@@@@@@@@@@@@@@
void deleteItem(HashTable* hashTable, unsigned int key) {
    removeItem(hashTable, key);


    //free()
}
