#include<iostream>
#include<stdlib>
#include<math.h>

#define QTD_ATTRIBUTES 11

class point{
    private:
        float attributes[QTD_ATTRIBUTES];
        int type;
    public:
    
    point(float attributes[]){
        this.attributes = attributes;
    }

    void set_type(int type){
        this.type = type;
    }

    float get_attribute(int pos){
        return this.attributes[pos];
    }

    float calculate_distance(point other_point){
        float total = 0;
        for(int i = 0; i < QTD_ATTRIBUTES; i++){
            float diference = this.attributes[i] - other_point.get_attribute(i);
            total += diference * diference;
        }
        return sqrt(total);
    }


}