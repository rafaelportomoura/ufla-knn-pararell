#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include <cstring>  
#include "readCsv.cpp"

#define QTD_ATTRIBUTES 16
using namespace std;

struct type_info{
    int type;
    int qtd;
};

class point{
    private:
        double *attributes = new double [QTD_ATTRIBUTES];
        int type;
    public:
    
    point(double out_attributes[QTD_ATTRIBUTES]){
        attributes = out_attributes;
    }

    void set_type(int type){
        type = type;
    }

    double get_attribute(int pos){
        return attributes[pos];
    }

    double calculate_distance(point other_point){
        double total = 0;
        for(int i = 0; i < QTD_ATTRIBUTES; i++){
            double diference = attributes[i] - other_point.get_attribute(i);
            total += diference * diference;
        }
        return sqrt(total);
    }
};

class list{
    private:
        vector<point> my_list;
        int num_neighbours;
        point pivo;

    public:
        list(int outter_num_neighbours){
            num_neighbours = outter_num_neighbours;
        }

        void add_pivo(point out_pivo){
            pivo = out_pivo;
        }

        void add(point new_neighbourd){
            double distance = this.pivo.calculate_distance(new_neighbourd);

            if(my_list.size() != 0 && distance > my_list[my_list.size()].calculate_distance(pivo)){
                return void;
            }
            if(vector.size() == this.num_neighbours){
                my_list.pop_back();
            }
            my_list.push_back(new_neighbourd);
            sort(my_list.begin(), my_list.end());
        }

        void define_type(){
            vector<type_info> species_present;

            for (int i = 0; i < my_list.size(); i++){
                int pos = this.pos(species_present, my_list[i].type);
                if( pos == -1){
                    type_info new_type_info;
                    new_type_info.type = my_list[i].type;
                    new_type_info.qtd = 1;
                    species_present.push_back(my_list[i].type);
                }
                else{
                    species_present[pos].qtd++;
                }
            }

            int closest = 0;
            for (int i = 1; i < species_present.size(); i++){
                if(species_present[closest].qtd < species_present[i].qtd){
                    closest = i;
                }
            }

        }

        int pos(vector<type_info> species_present, int target){
            for (int i = 0; i < species_present.size(); i++){
                if(species_present[i].type == target){
                    return i;
                }
            }
            return -1;
        }

        int count(int target){
            int qtd = 0;
            for (int i = 0; i < my_list.size(); i++){
                if(my_list[i] == target){
                    qtd++;
                }
            }
            return qtd;
        }
};

int main(){
    vector<point> list_of_point;
    vector<vector<double>> lines;
    readCsv(lines);
    random_shuffle(lines.begin(), lines.end());
    int porcentage = 70;
    int qtd_train = (lines.size()/10)*7;



    for(int i = 0; i < qtd_train; i++){
        cout << i << " " << lines[i].size() << "\n";
        //shuffle(i_vec1.begin(), i_vec1.end(), rng);
        double* att = new double [QTD_ATTRIBUTES];
        for(int j = 0; j < QTD_ATTRIBUTES - 1 ; j++){
            att[j] = lines[i][j];
        }
        point new_point(att);
        int new_point_type = (int)lines[i][lines.size()-1];
        new_point.set_type(new_point_type);
        list_of_point.push_back(new_point);

    }
}
