#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

template <typename Key_T,
          typename Mapped_T,
          typename Compare = std::less<Key_T>,
          size_t MaxLevel = 32>
class SkipList
{
protected:
    using ValueType = std::pair<const Key_T, Mapped_T>;

    class Node
    {
    public:
        Node() : nodes(std::vector<Node*>(MaxLevel, nullptr)) { }
        Node(ValueType data, size_t sz) : dataPair(data), nodes(std::vector<Node*>(MaxLevel, nullptr)), size(sz) { }
        ValueType dataPair;
        std::vector<Node*> nodes;
        size_t size = 0;
    };

    Node *find_raw(const Key_T &key)
    {
        Node* current_node;
        Node* fallback_node = levels;

        for(int i = (MaxLevel - 1); i >= 0; i--)
        {
            current_node = fallback_node;
            while(current_node != nullptr)
            {
                fallback_node = current_node;
                if(current_node->nodes[i] == nullptr){
                    break; // Next string on this node
                }
                else if (current_node->nodes[i]->dataPair.first == key){
                    return current_node->nodes[i];
                }
                else if(!Compare()(current_node->nodes[i]->dataPair.first , key)){
                    break;
                }

                else if(Compare()(current_node->nodes[i]->dataPair.first , key)){
                    current_node = current_node->nodes[i];
                }

            }
        }
        return nullptr;
    }

    Node* levels;
    size_t sz;
    int getRandomLevel()
    {
        const int max_rand = 100000;
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0, max_rand);
        size_t retLevel = 1;
        while((uni(rng) < (max_rand / 2)) && (retLevel < MaxLevel))
        {
            ++retLevel;
        }
        return retLevel;
    }

public:

    class BaseIterator
    {
    public:
        bool operator==(const BaseIterator &a) { return this == &a;  }
        bool operator!=(const BaseIterator &a) { return !(this == &a); }
    };

    class Iterator : public BaseIterator
    {
    public:
        Node* value;
        Iterator(Node* item) : value(item) { }

        Iterator &operator++()
        {
            value = value->nodes[0];
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator it = *this;
            value = value->nodes[0];
            return it;
        }

        ValueType &operator*() const
        {
            return value->dataPair;
        }

        ValueType *operator->() const
        {
            return &value->dataPair;
        }


    };

    class ConstIterator : public BaseIterator
    {
    public:
        Node* value;
        ConstIterator(Node* item) : value(item) { }

        ConstIterator &operator++()
        {
            value = value->nodes[0];
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator it = *this;
            value = value->nodes[0];
            return it;
        }

        const ValueType &operator*() const
        {
            return value->dataPair;
        }

        const ValueType *operator->() const
        {
            return &value->dataPair;
        }
    };

    SkipList()
    {
        levels = new Node();
        sz = 0;
    }

    SkipList(const SkipList &list)
    {
        levels = new Node();
        sz = 0;

        Iterator it = list.begin();
        for(size_t i = 0; i < list.size(); i++){
            insert((it++).value);
        }

    }

    SkipList &operator=(const SkipList &list)
    {
        clear();
        Iterator it = list.begin();
        for(size_t i = 0; i < list.size(); i++){
            insert((it++).value);
        }

    }

    size_t size() const
    {
        return sz;
    }

    Iterator begin()
    {
        return Iterator(levels->nodes[0]);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator begin() const
    {
        return ConstIterator(levels->nodes[0]);
    }

    ConstIterator end() const
    {
        return ConstIterator(nullptr);
    }

    Iterator find(const Key_T &key)
    {
        Node *res = find_raw(key);
        return Iterator(res);
    }

    ConstIterator find(const Key_T &key) const
    {
        Node *res = find_raw(key);
        return ConstIterator(res);
    }

    std::pair<Iterator, bool> insert(ValueType &pair)
    {
        int top_level = getRandomLevel();
        std::stack<Node*> update_node_stack;
        Node* current_node;
        Node* fallback_node = levels;
        bool exists = false;

        for(int i = (MaxLevel - 1); i >= 0; i--)
        {
            current_node = fallback_node;
            while(current_node != nullptr)
            {
                fallback_node = current_node;
                if(current_node->nodes[i] == nullptr ||
                        !Compare()(current_node->nodes[i]->dataPair.first, pair.first)
                        )
                {
                    if(top_level > i){
                        update_node_stack.push(current_node);
                    }
                    break;
                }
                else if (current_node->nodes[i]->dataPair.first == pair.first)
                {
                    current_node->nodes[i]->dataPair.second = pair.second;
                    exists = true;
                    break;
                }
                else if(Compare()(current_node->nodes[i]->dataPair.first, pair.first))
                {
                    current_node = current_node->nodes[i];
                }
            }
            if(exists){
                break;
            }
        }

        if(!exists)
        {
            Node *new_node = new Node(pair, top_level);
            if(fallback_node == nullptr) // This is the first element -- Special Case
            {
                for(int k = 0; k < top_level; k++)
                    levels->nodes[k] = new_node;
            }
            else
            {
                int j = 0;
                Node *temp;
                while(update_node_stack.size() > 0)
                {
                    temp = update_node_stack.top()->nodes[j];
                    update_node_stack.top()->nodes[j] = new_node;
                    new_node->nodes[j] = temp;
                    update_node_stack.pop();
                    ++j;
                }
            }
            ++sz;
            return std::pair<Iterator, bool>(Iterator(new_node), exists);
        }
        else
        {
            return std::pair<Iterator, bool>(Iterator(fallback_node), exists);
        }
    }

    template <typename IT_T>
    void insert(IT_T range_beg, IT_T range_end)
    {
        for (IT_T it = range_beg; it != range_end; ++it){
            insert(*it);
        }

    }

    void erase(Iterator pos)
    {
        Node *org_val = pos.value;
        for(int i = 0; i < pos.value->size; i++)
        {
            Node* it = levels->nodes[i];
            Node* prev = levels;
            while(it != nullptr && it != org_val)
            {
                prev = it;
                it = it->nodes[i];
            }

            if(prev!=nullptr){
                prev->nodes[i] = org_val->nodes[i];
            }

        }
        delete org_val;
        --sz;
    }

    void erase(Iterator range_beg, Iterator range_end)
    {
        Iterator it = range_beg;
        Iterator to_delete;
        while(it != range_end)
        {
            to_delete = it;
            erase(it);
            it = ++to_delete;
        }
    }

    void clear()
    {
        while(sz > 0){
            erase((*this).begin());
        }

    }

    bool operator==(const SkipList &other)
    {
        if(sz != other.size()){
            return false;
        }

        Iterator other_it = other.begin();
        Node* self_it = levels->nodes[0];
        for(int i = 0; i < sz; i++)
        {
            if(!(other_it.value->dataPair.first == self_it->dataPair.first &&
                 other_it.value->dataPair.second == self_it->dataPair.second))
            {
                return false;
            }
            other_it++;
            self_it = self_it->nodes[0];
        }
        return true;
    }

    bool operator!=(const SkipList &a)
    {
        return !(*this == a);
    }

    bool operator<(const SkipList &other)
    {
        Iterator other_it = other.begin();
        Node* self_it = levels->nodes[0];
        for(int i = 0; i < sz; i++)
        {
            if(self_it != nullptr && other_it == nullptr) return false;
            if(self_it == nullptr && other_it != nullptr) return true;


            if(Compare()(self_it->dataPair.second, other_it.value->dataPair.second))
            {
                return true;
            }
            other_it++;
            self_it = self_it->nodes[0];
        }
        return true;
    }

    Mapped_T& operator[](const Key_T& key){
        Node *row = find_raw(key);
        if(row == nullptr){
            Mapped_T tmp_val;
            ValueType tmp_pair = {key, std::move(tmp_val)};
            auto it = insert(tmp_pair);
            row = it.first.value;
        }

        return row->dataPair.second;
    }

    Mapped_T& at(const Key_T& key){
        Node *row = find_raw(key);

        if(row == nullptr){
            throw std::out_of_range("index not found");
        }
        return row->dataPair.second;
    }

    ~SkipList()
    {
        Node* it = levels->nodes[0];
        while(it != nullptr)
        {
            Node* prev_it = it;
            it = it->nodes[0];
            delete prev_it;
        }
        delete levels;
    }
};


#endif
