#include "llrec.h"

//*********************************************
// Provide your implementation of llpivot below
//*********************************************

void llpivot(Node*& list, Node*& small, Node*& large, int pivot) {
    Node* current = list;
    Node* small_tail = nullptr; //creating nodes to be used later 
    Node* large_tail = nullptr;

    small = nullptr; //small nullptr initialization
    large = nullptr; //large nullptr initialization

    while (current != nullptr) { // cont of loop
        Node* next_node = current->next; //stores next node before we modofiy list
        current->next = nullptr;

        if (current->val <= pivot) {
            if (small == nullptr) { //want to make sure small is not alr identified
                small = current; //tagged as first identified small node within list 
            } else {
                small_tail->next = current; //else will continue transversing list 
            }
            small_tail = current;
        } else {
            if (large == nullptr) { //making sure large is not alr tagged
                large = current; //tagged as first identified large node within list
            } else {
                large_tail->next = current; //else will continue transversing list
            }
            large_tail = current;
        }

        current = next_node; //this is what iterates the list and continues it until conditional is false
    }
    list = nullptr; //resetting list after its done iterating through list
}
