#ifndef __SIMPLEARRAY_H_
#define __SIMPLEARRAY_H_

#include <stddef.h>
#include <functional>



/*
template<class T>
struct SimpleArrayNode {
    T                    key;
    U                    data;
    SimpleMapNode<T, U>* next;
};
*/
template<typename T>
class SimpleArray {
    T* arr;
    public:
//HEST        int (*compare)(T & a, T & b);

//HEST        SimpleArray(int (*compare)(T & a, T & b));
        SimpleArray();
        virtual ~SimpleArray();

        virtual int size();
        virtual void clear();
        virtual void remove(int idx);
        virtual int add(T obj);
        virtual T get(int idx);
        

        virtual void lock();
        virtual void unlock();
        virtual bool isLocked();

    private:
        void _resize(int newSize);


    protected:
        int listSize;
        bool locked = false;
};

template<typename T>
SimpleArray<T>::SimpleArray() {
    listSize                 = 0;
    arr                      = NULL;
}

// Clear Nodes and free Memory
template<typename T>
SimpleArray<T>::~SimpleArray() {
    clear();
}

template<typename T>
T SimpleArray<T>::get(int i) {
    return arr[i];
}

template<typename T>
int SimpleArray<T>::add(T ptr) {
    
    _resize(listSize+1);
    arr[listSize] = ptr;
    listSize++;
    return listSize-1;
}

/*
template<typename T, typename U>
SimpleMapNode<T, U>* SimpleMap<T, U>::getNode(T key) {
    if (listSize > 0) {
        if ((compare(key, listBegin->key) < 0) || (compare(key, listEnd->key) > 0)) return NULL;

        if (isCached) {
            if (compare(key, lastNodeGot->key) == 0) return lastNodeGot;
        }

        SimpleMapNode<T, U>* h = listBegin;

        int lowerEnd = 0;
        int upperEnd = listSize - 1;
        int res;
        int mid = (lowerEnd + upperEnd) / 2;

        int hIndex = 0;

        while (lowerEnd <= upperEnd) {
            h      = lastNodeGot;
            hIndex = lastIndexGot;

            res = compare(key, getNodeIndex(mid)->key);

            if (res == 0) {
                return lastNodeGot;
            } else if (res < 0) {
                // when going left, set cached node back to previous cached node
                lastNodeGot  = h;
                lastIndexGot = hIndex;
                isCached     = true;

                upperEnd = mid - 1;
                mid      = (lowerEnd + upperEnd) / 2;
            } else if (res > 0) {
                lowerEnd = mid + 1;
                mid      = (lowerEnd + upperEnd) / 2;
            }
        }
    }
    return NULL;
}

template<typename T, typename U>
SimpleMapNode<T, U>* SimpleMap<T, U>::getNodeIndex(int index) {
    if ((index < 0) || (index >= listSize)) {
        return NULL;
    }

    SimpleMapNode<T, U>* hNode = listBegin;
    int c                      = 0;

    if (isCached && (index >= lastIndexGot)) {
        c     = lastIndexGot;
        hNode = lastNodeGot;
    }

    while (hNode != NULL && c < index) {
        hNode = hNode->next;
        c++;
    }

    if (hNode) {
        isCached     = true;
        lastIndexGot = c;
        lastNodeGot  = hNode;
    }

    return hNode;
}
*/

template<typename T>
void SimpleArray<T>::clear() {
    if (listSize==0)
        return;

    unlock();

   for (int i=0;i<listSize;i++) {
       T ptr = (T)arr[i]; 
       delete ptr;
        arr[i] = 0;
    }
    _resize(0);
    listSize  = 0;
}

template<typename T>
void SimpleArray<T>::_resize(int newSize) {
    if(arr==NULL)
    {
        arr = (T*)malloc(sizeof(T)*newSize);
    }
    else if (newSize<1) {
        free (arr);
        arr = NULL;
    }
    else {
        arr = (T*)realloc(arr, (size_t)(sizeof(T)*newSize));
    }

}

template<typename T>
void SimpleArray<T>::remove(int idx) {
//    unlock();
    if (idx>=0 && idx<listSize) {
        T pItemToRemove = arr[idx];
    
        for (int i=idx;i<listSize-1;i++) {
            arr[i] = arr[i+1];
        }
        arr[listSize-1] = 0;
        delete pItemToRemove;

        _resize(listSize-1);
        listSize--;
    }
}
template<typename T>
int SimpleArray<T>::size() {
    return listSize;
}
/*
template<typename T, typename U>
void SimpleArray<T, U>::put(T key, U obj) {
    // create new node
    SimpleMapNode<T, U>* newNode = new SimpleMapNode<T, U>();
    newNode->next = NULL;
    newNode->data = obj;
    newNode->key  = key;

    // look if already in list
    SimpleMapNode<T, U>* h = listBegin;
    SimpleMapNode<T, U>* p = NULL;
    bool found             = false;
    int  c                 = 0;

    if (listSize > 0) {
        while (h != NULL && !found) {
            if (compare(h->key, key) == 0) {
                found = true;
            } else {
                p = h;
                h = h->next;
                c++;
            }
        }
    }

    // replace old node with new one
    if (found) {
        if (h == listBegin) listBegin = newNode;

        if (h == listEnd) listEnd = newNode;

        if (p) p->next = newNode;
        newNode->next = h->next;
        delete h;

        lastIndexGot = c;
        lastNodeGot  = newNode;
        isCached     = true;
    }

    // create new node
    else if (!locked) {
        if (listSize == 0) {
            // add at start (first node)
            listBegin = newNode;
            listEnd   = newNode;

            lastIndexGot = 0;
        } else {
            if (compare(key, listEnd->key) >= 0) {
                // add at end
                listEnd->next = newNode;
                listEnd       = newNode;

                lastIndexGot = listSize;
            } else if (compare(key, listBegin->key) <= 0) {
                // add at start
                newNode->next = listBegin;
                listBegin     = newNode;

                lastIndexGot = 0;
            } else {
                // insertion sort
                h     = listBegin;
                p     = NULL;
                found = false;
                c     = 0;

                while (h != NULL && !found) {
                    if (compare(h->key, key) > 0) {
                        found = true;
                    } else {
                        p = h;
                        h = h->next;
                        c++;
                    }
                }
                newNode->next = h;

                if (p) p->next = newNode;

                lastIndexGot = c;
            }
        }

        listSize++;

        isCached    = true;
        lastNodeGot = newNode;
    }
}
*/

/*
template<typename T, typename U>
void SimpleMap<T, U>::remove(T key) {
    if ((listSize > 0) && !locked) {
        if ((compare(key, listBegin->key) < 0) || (compare(key, listEnd->key) > 0)) return;

        SimpleMapNode<T, U>* h = listBegin;
        SimpleMapNode<T, U>* p = NULL;
        bool found             = false;

        while (h != NULL && !found) {
            if (h->key == key) {
                found = true;
            } else {
                p = h;
                h = h->next;
            }
        }

        if (found) {
            if (p) p->next = h->next;
            else listBegin = h->next;

            if (listEnd == h) listEnd = p;
            listSize--;
            delete h;
        }
    }
}

template<typename T, typename U>
void SimpleMap<T, U>::remove(int i) {
    if (listSize > 0) {
        SimpleMapNode<T, U>* h = getNodeIndex(i);

        if (h != NULL) {
            SimpleMapNode<T, U>* p = getNodeIndex(i - 1);

            if (p != NULL) p->next = h->next;

            if (h == listBegin) listBegin = h->next;

            if (h == listEnd) listEnd = p;

            listSize--;
            delete h;
        }
    }
}

template<typename T, typename U>
bool SimpleMap<T, U>::has(T key) {
    return getNode(key) != NULL;
}

template<typename T, typename U>
T SimpleMap<T, U>::getKey(int i) {
    SimpleMapNode<T, U>* h = getNodeIndex(i);
    return h ? h->key : T();
}

template<typename T, typename U>
U SimpleMap<T, U>::getData(int i) {
    SimpleMapNode<T, U>* h = getNodeIndex(i);
    return h ? h->data : U();
}

template<typename T, typename U>
int SimpleMap<T, U>::getIndex(T key) {
    return getNode(key) ? lastIndexGot : -1;
}
*/
template<typename T>
void SimpleArray<T>::lock() {
    locked = true;
}

template<typename T>
void SimpleArray<T>::unlock() {
    locked = false;
}

template<typename T>
bool SimpleArray<T>::isLocked() {
    return locked;
}

#endif // ifndef __SIMPLEMAP_H_
