#ifndef MERGING_ITERATOR
#define MERGING_ITERATOR
#include <vector>
#include <algorithm>    // std::sort
#include <assert.h>

template <class RandomAccessIterator, class Compare>
void sort (std::vector<RandomAccessIterator> first, std::vector<RandomAccessIterator> last, Compare comp){

    assert(first.size() == last.size());
    for (auto &row_1 : first){
        for(auto &row_2 : last){

        }
    }


}




#endif // MERGING_ITERATOR
