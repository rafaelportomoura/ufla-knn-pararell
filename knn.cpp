#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include <cstring>  
#include "readCsv.cpp"

#define QTD_ATTRIBUTES 16

struct type_info{
    int type;
    int qtd;
};

class point{
    private:
        double *attributes;
        int type;
    public: 
        point(){
            this->attributes = new double [QTD_ATTRIBUTES];
        }
        point(std::vector<double> vet){

            for(int i = 0; i < QTD_ATTRIBUTES; i++){
                this->attributes[i] = vet[i];
            }
        }

        void set_attributes(double attributes[QTD_ATTRIBUTES]){
            this->attributes = attributes;
        }

        void set_type(int type){
            this->type = type;
        }

        int get_type(){
            return this->type;
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
        std::vector<point> my_list;
        int num_neighbours;
        point pivo;

    public:
        list(int num_neighbours){
            this->num_neighbours = num_neighbours;
        }

        point get_pivo(){
            return this->pivo;
        }

        void add_pivo(point pivo){
            this->pivo = pivo;
        }

        void add(point new_neighbourd){
            double distance = this->pivo.calculate_distance(new_neighbourd);
            int msize = this->my_list.size();
            
            if(this->my_list.size() == 0 || distance > this->my_list[this->my_list.size()-1].calculate_distance(pivo)){
                if(this->my_list.size() == this->num_neighbours){
                    this->my_list.pop_back();
                }
                this->incert_ordered_in_list(new_neighbourd);
            }

        }

        void incert_ordered_in_list(point new_neighbourd){
            int i = this->my_list.size()-1;
            double new_distance = this->pivo.calculate_distance(new_neighbourd);
            while(i >= 0){
                if(new_distance > this->pivo.calculate_distance(this->my_list[i])){
                   this->my_list.insert(this->my_list.begin() + i+1 ,new_neighbourd);
                   break;
                }
                i--;
            }
            if(i == -1){
                this->my_list.insert(this->my_list.begin(), new_neighbourd);
                
            }
        }

        void define_type(){
            std::vector<type_info> types_present;

            this->count_types(types_present);

            int i = types_present.size()-1;
            int biggest_qtd = -1;
            this->show_types(types_present);
            while(i>=0){
                if(biggest_qtd < types_present[i].qtd){
                    biggest_qtd = types_present[i].qtd;
                }

                else{
                    types_present.erase(types_present.begin() + i);
                }
                i--;
            }

            if (types_present.size() == 1){
                std::cout<<types_present[0].qtd<<"-"<<types_present[0].type;
                this->pivo.set_type(types_present[0].type);
            }

            else{
                this->desempate(types_present);
            }
        }

        void show_types(std::vector<type_info> &types_present){
            std::cout<<"----------------------------------"<<std::endl;
            for(int i = 0; i < types_present.size(); i++){
                std::cout<<types_present[i].type << ":" << types_present[i].qtd << "\t";
            }
            std::cout<<std::endl;
        }

        void count_types(std::vector<type_info> &types_present){
            for (int i = 0; i < this->my_list.size(); i++){
                int pos = this->pos(this->my_list[i].get_type(), types_present);
                if( pos == -1){
                    type_info new_type_info;
                    new_type_info.type = this->my_list[i].get_type();
                    new_type_info.qtd = 1;
                    types_present.push_back(new_type_info);
                }
                else{
                    types_present[pos].qtd++;
                }
            }
        }

        void desempate(std::vector<type_info> &types_present){
            int qtd_species = types_present.size();
            double total_distance[qtd_species];
            for(int i = 0; i < qtd_species; i++){
                total_distance[i] = 0;
            }

            for(int i = 0; i < this->my_list.size(); i++){
                for(int j = 0; j < types_present.size(); j++){
                    if(this->my_list[i].get_type() == types_present[j].type){
                        total_distance[j] += this->pivo.calculate_distance(my_list[i]);
                    }
                }
            }

            int closer = 0;
            for(int i = 1; i < types_present.size(); i++){
                if(total_distance[i] < total_distance[closer]){
                    closer = i;
                }
            }
            std::cout<<total_distance[closer];
            this->pivo.set_type(types_present[closer].type);
            
        }

        int pos(int target, std::vector<type_info> types_present){
            for (int i = 0; i < types_present.size(); i++){
                if(types_present[i].type == target){
                    return i;
                }
            }
            return -1;
        }

};

void knn(int num_neighbours, int porcentage){
    std::vector<point> list_of_point;
    std::vector<std::vector<double>> lines;
    readCsv(lines);
    random_shuffle(lines.begin(), lines.end());
    int qtd_train = (lines.size()/100)*porcentage;
    int pos_type = QTD_ATTRIBUTES;

    for(int i = 0; i < qtd_train; i++){
        
        double* att = new double [QTD_ATTRIBUTES];
        for(int j = 0; j < QTD_ATTRIBUTES - 1 ; j++){
            att[j] = lines[0][j];
        }
        point new_point;
        new_point.set_attributes(att);
        int new_point_type = (int)lines[0][pos_type];
        new_point.set_type(new_point_type);
        list_of_point.push_back(new_point);
        lines.erase(lines.begin());
    }




    int diferente = 0, igual = 0;
    while(lines.size() > 0){

        std::vector<double> thos_line = lines[0];
        list lista(num_neighbours);
        point new_point(thos_line);
        lines.erase(lines.begin());
        lista.add_pivo(new_point);

        for(int i = 0; i < list_of_point.size(); i++){
            lista.add(list_of_point[i]);
        }
        lista.define_type();
        list_of_point.push_back(lista.get_pivo());

        //matriz de confusão
        if(lista.get_pivo().get_type() != thos_line[QTD_ATTRIBUTES]){
            diferente++;
            //std::cout<<"d\t";
        }
        else{
            //std::cout<<"i\t";
            igual++;
        }

        
    }

    std::cout<<"diferente: " << diferente << " igual: " << igual << std::endl;
}

int main(){
    knn(90, 95);
    //std::cout<<"vai aparecer um segmentetion fault depois daqui, e eu não sei pq" << std::endl;
    return 0;
}
