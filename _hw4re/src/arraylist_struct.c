#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "arraylist_struct.h"

ArrayList* initialize_arraylist(int ele_size){
	ArrayList* newArray = malloc(sizeof(ArrayList));
	newArray->cap=DEFAULT_CAP;
	newArray->size=0;
	newArray->ele_size=ele_size;
	newArray->data=(char*)calloc(newArray->cap,ele_size);
	return newArray;
}
void add_element(ArrayList*array,void*element){
	if (array->cap==array->size){
		array->data=(char*)realloc(array->data, array->cap*2*array->ele_size);
		array->cap= array->cap*2;
	}
	memmove(array->data+array->size*array->ele_size, element, array->ele_size);
	array->size=array->size+1;
}
int set_element(ArrayList*array,int index,void*element){
	if(index<0 || index>=array->size)
		return FALSE;
	else{
		memmove(array->data+index*array->ele_size,element,array->ele_size);
		return TRUE;
	}
}
int insert_element(ArrayList*array, int index, void*element){
	if(index<0 || index>=array->size)
		return FALSE;
	else{
		if(array->size==array->cap){
			array->data=realloc(array->data,array->cap*2*array->ele_size);
			array->cap=array->cap*2;
		}
		memmove(array->data+(index+1)*array->ele_size, array->data+index*array->ele_size, (array->size-index)*array->ele_size);
		memmove(array->data+index*array->ele_size, element, array->ele_size);
		array->size=array->size+1;
		return TRUE;
	}
}

int remove_element(ArrayList*array, void*element){
	if(contains(array,element)==TRUE){
		int myindex=index_of_element(array,element);
		remove_ith_element(array,myindex);
		return TRUE;
	}
	return FALSE;
}
int remove_ith_element(ArrayList*array, int index){
	if(index<0||index>=array->size)
		return FALSE;
	else{
		memmove(array->data+index*array->ele_size, array->data+(index+1)*array->ele_size, (array->size-index-1)*array->ele_size);
		array->size=array->size-1;
		return TRUE;
	}
}
void* get_ith_element(ArrayList*array, int index){
	void*ith_element=NULL;
	if(index>=0 && index<array->size){
		ith_element=(void*)array->data+index*array->ele_size;
	}
	return ith_element;
}
int index_of_element(ArrayList*array, void*element){
	int index=-1;
	int i;
	for(i=0;i<array->size; i++){
		if(memcmp(array->data+i*array->ele_size, element, array->ele_size)==0)
			index=i;
	}
	return index;
}
int contains(ArrayList*array, void*element){
	return index_of_element(array,element)>0 ? TRUE:FALSE;
}
void freeArraylist(ArrayList*array){
	free(array);
	free(array->data);
}
