#ifndef ARRAYLIST_STRUCT_H
#define ARRAYLIST_STRUCT_H

#define DEFAULT_CAP  5
#define TRUE  1
#define FALSE  0

typedef struct ArrayList{
	int cap;	// arraylist capacity 
	int size;    // number of elements, array size
	int ele_size; // size of each element in size
	char* data;
} ArrayList;

ArrayList* initialize_arraylist(int ele_size);	
void add_element(ArrayList*array,void*element);
int set_element(ArrayList*array,int index,void*element);
int insert_element(ArrayList*array, int index, void*element);
int remove_element(ArrayList*array, void*element);
int remove_ith_element(ArrayList*array, int index);
void* get_ith_element(ArrayList*array, int index);
int index_of_element(ArrayList*array, void*element);
int contains(ArrayList*array, void*element);

#endif
