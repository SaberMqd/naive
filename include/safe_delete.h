//
// Created by mqd on 2016/11/21.
//

#ifndef _NAIVE_SAFE_DELETE_H_
#define _NAIVE_SAFE_DELETE_H_

#include <cstdint>

template <class T>
void SafeDelete(T *&t){
    if(t) {
        delete t;
        t = nullptr;
    }
}

template <class T>
void SafeDeleteArray(T *&t){
    if(t) {
        delete[]t;
        t = nullptr;
    }
}

template <class T>
void SafeDeleteStlMap(T &t) {
	for (auto &v : t) {
		SafeDelete(v.second);
	}
	t.clear();
}

template <class T>
void SafeDeleteStlList(T &t) {
	for (auto &v : t) {
		SafeDelete(v);
	}
}

template <class T>
void SafeDeleteNDimensionalArray(T *&t,uint32_t num_ndim) {
	if(t){
		for (uint32_t i = 0; i < num_ndim; ++i) {
			SafeDeleteArray(t[i]);
		}
		SafeDeleteArray(t);
	}
}


#endif //_NAIVE_SAFE_DELETE_H_

