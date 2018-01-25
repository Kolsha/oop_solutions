#pragma once

#include <QQueue>
#include <QMutex>

template<typename T>
class QSafeQueue:private QQueue<T> {
public:
    QSafeQueue() : QQueue<T>() {}

    void enqueue(const T &t) {
        QMutexLocker locker(&mutex);
        QQueue<T>::enqueue(t);
    }

//    T dequeue() {
//        QMutexLocker locker(&mutex);
//        if(!QQueue<T>::isEmpty())
//            return QQueue<T>::dequeue();

//        return T();
//    }

//    bool isEmpty(){
//        QMutexLocker locker(&mutex);
//        return QQueue<T>::isEmpty();
//    }

    bool dequeue(T &t){
        QMutexLocker locker(&mutex);

        if(!QQueue<T>::isEmpty()){
            t = QQueue<T>::dequeue();
            return true;
        }

        return false;
    }

    virtual ~QSafeQueue() {}
private:
    QMutex mutex;
};
