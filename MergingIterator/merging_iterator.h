#ifndef MERGING_ITERATOR
#define MERGING_ITERATOR
#include <vector>

template <class InputIterator, class OutputIterator, class Compare>
OutputIterator my_merge (std::initializer_list<InputIterator> first,
                         std::initializer_list<InputIterator> last,
                         OutputIterator result, Compare comp)
{

    if (first.size() != last.size()){
        return result;
    }

    std::vector<InputIterator> start = first;
    std::vector<InputIterator> end = last;
    InputIterator *max_pos = &(start[0]);
    for(;;){
        size_t last_idx = 0;
        size_t real_size = start.size();
        for(size_t i = 0; i < start.size(); i++){

            if(start[i] == end[i]){
                --real_size;
                continue;
            }


            if (comp(**max_pos, *start[i])){
                max_pos = &(start[i]);
            }
            last_idx = i;


        }
        if(!real_size){
            return result;
        }
        try{
            *result = **max_pos;
            ++result;
            ++(*max_pos);
            max_pos = &(start[last_idx]);
        }
        catch(...){
            return result;
        }

    }

    return result;
}



#endif // MERGING_ITERATOR
