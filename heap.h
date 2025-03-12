#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T>>
class Heap
{
public:
    /**
     * @brief Construct a new Heap object
     * 
     * @param m ary-ness of heap tree (default to 2)
     * @param c binary predicate function/functor that takes two items
     *          as an argument and returns a bool if the first argument has
     *          priority over the second.
     */
    Heap(int m=2, PComparator c = PComparator()) : m_(m), comparator_(c) {}

    /**
     * @brief Destroy the Heap object
     * 
     */
    ~Heap() {}

    /**
     * @brief Push an item to the heap
     * 
     * @param item item to heap
     */
    void push(const T& item);

    /**
     * @brief Returns the top (priority) item
     * 
     * @return T const& top priority item
     * @throw std::underflow_error if the heap is empty
     */
    T const & top() const;

    /**
     * @brief Remove the top priority item
     * 
     * @throw std::underflow_error if the heap is empty
     */
    void pop();

    /// returns true if the heap is empty
    /**
     * @brief Returns true if the heap is empty
     * 
     */
    bool empty() const;

    /**
     * @brief Returns size of the heap
     * 
     */
    size_t size() const;

private:
    std::vector<T> heap_;       // container for heap elements
    int m_;                     // ary-ness of the heap
    PComparator comparator_;     // comparator for ordering
    
    void heapify_up(int index); //helper for pushing element in heap (helping with ordering afterwards)
    void heapify_down(int index); //helper in popping element in heap (helping with ordering afterwards)
};

template <typename T, typename PComparator>
T const & Heap<T, PComparator>::top() const //serving as "top" function in this case 
{
    if (empty()) {
        throw std::underflow_error("Empty Heap!!"); //conditional if not error shows
    }
    return heap_.front();  // top of the heap always in front 
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::pop() //serving as "pop" functions in this case
{
    if (empty()) {
        throw std::underflow_error("Empty Heap!!"); //conditional if not error shows
    }
    
    // Swap the first and last elements
    std::swap(heap_.front(), heap_.back());
    heap_.pop_back();  // Remove the last element (previous top)
    
    // Restore the heap property by heapifying down from the root
    heapify_down(0);
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::push(const T& item)
{
    heap_.push_back(item);  // adds item to the end of heap
    heapify_up(heap_.size() - 1);  // using helper function to restore heap with new information (like size of heap)
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::heapify_up(int index) //helper to help shift element up (similar to written hw problems)
{
    int parent = (index - 1) / m_;  //new parent int set 
    if (index > 0 && comparator_(heap_[index], heap_[parent])) { //comparing two indexes of heap to test vals
        std::swap(heap_[index], heap_[parent]); //then swaps them
        heapify_up(parent); //then will move parent up in end to restore balance of heap
    }
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::heapify_down(int index) //helper to help shift element down (similar to written hw problems)
{
    int spot = index;
    
    for (int i = 1; i <= m_; ++i) { //iterates over entire heap including children
        int child = m_ * index + i; 
        if (child < heap_.size() && comparator_(heap_[child], heap_[spot])) { //will then check child and current spot and compare  
            spot = child;
        }
    }
    
    if (spot != index) {
        std::swap(heap_[index], heap_[spot]); //will swap if child is better than the element and then performs heapfiy down
        heapify_down(spot); //moves elemnet down to restore balance
    }
}

template <typename T, typename PComparator>
bool Heap<T, PComparator>::empty() const //bool for mempty case
{
    return heap_.empty(); //bool
}

template <typename T, typename PComparator>
size_t Heap<T, PComparator>::size() const //checking for size 
{
    return heap_.size();
}

#endif
