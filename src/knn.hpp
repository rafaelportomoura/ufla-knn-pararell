#include <iostream>
#include <math.h>
#include <omp.h>
#include <vector>

using namespace std;

class point {
private:
    vector<float> attributes;
    int type;
public:

    point( vector<float> attributess ) {
        this->attributes = attributes;
    }

    void set_type( int type ) {
        this->type = type;
    }

    float get_attribute( int pos ) {
        return this->attributes[pos];
    }

    float calculate_distance( point other_point ) {
        float total = 0;
        int size = this->attributes.size();
        int i = 0;

        #pragma omp parallel for reduction(+:total) num_threads(4)
        for ( int i = 0; i < this->attributes.size(); i++ ) {
            float diference = this->attributes.at( i ) - other_point.get_attribute( i );
            total += diference * diference;
        }
        return sqrt( total );
    }
};