#include "select.hpp"
void ObjectSelect::select(){

}

void ObjectSelect::next(){
    if(actual_object < count)
        actual_object++;
    else
        actual_object = 0;

}

void ObjectSelect::previous(){
    if(actual_object > count)
        actual_object--;
    else
        actual_object = 0;

    
}