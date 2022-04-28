#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include <cstring>  
#include "readCsv.cpp"
#include <omp.h>
#include <ctime>

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
            this->attributes = new double [QTD_ATTRIBUTES];
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
        int qtd_neighbours;
        point pivot;

    public:
        list(int qtd_neighbours){
            this->set_qtd_neighbours(qtd_neighbours);
        }

        list(){
        }

        void set_qtd_neighbours(int qtd_neighbours){
            this->qtd_neighbours = qtd_neighbours;
        }

        point get_pivo(){
            return this->pivot;
        }

        void add_pivot(point pivot){
            this->pivot = pivot;
        }

        point best_point(){
            point retorno = this->my_list[0];
            this->my_list.erase(this->my_list.begin());
            return retorno;
        }

        void set_list(std::vector<point> my_list){
            this->my_list = my_list;
        }

        void add(point new_neighbour){
            double distance = new_neighbour.calculate_distance(this->pivot);
            if(this->my_list.size() < this->qtd_neighbours || distance > this->my_list[this->my_list.size()-1].calculate_distance(pivot)){
                if(this->my_list.size() == this->qtd_neighbours){
                    this->my_list.pop_back();
                }
                this->insert_ordered(new_neighbour);
            }

        }

        void insert_ordered(point new_neighbour){
            int i = this->my_list.size()-1;
            double new_distance = this->pivot.calculate_distance(new_neighbour);
            while(i >= 0){
                if(new_distance > this->pivot.calculate_distance(this->my_list[i])){
                   this->my_list.insert(this->my_list.begin() + i+1 ,new_neighbour);
                   break;
                }
                i--;
            }
            if(i == -1){
                this->my_list.insert(this->my_list.begin(), new_neighbour);
                
            }
        }

        void define_type(){
            std::vector<type_info> types_present;

            this->count_types(types_present);
            this->filter_small_numbers(types_present);

            if (types_present.size() == 1){
                this->pivot.set_type(types_present[0].type);
            }
            else{
                this->tiebreaker(types_present);
            }
        }

        void filter_small_numbers(std::vector<type_info> &types_present){
            int i = types_present.size()-1;
            int biggest_qtd = -1;
            while(i>=0){
                if(biggest_qtd < types_present[i].qtd){
                    biggest_qtd = types_present[i].qtd;
                }

                else{
                    types_present.erase(types_present.begin() + i);
                }
                i--;
            }

            i = types_present.size()-1;
            while(i>=0){
                if(biggest_qtd > types_present[i].qtd){
                    types_present.erase(types_present.begin() + i);
                }
                i--;
            }

            if(types_present[types_present.size()-1].qtd < biggest_qtd){
                types_present.erase(types_present.begin() + types_present.size()-1);
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

        void tiebreaker(std::vector<type_info> &types_present){
            double* total_distances  = this->calculate_total_distances(types_present);
            int closest = 0;
            for(int i = 1; i < types_present.size(); i++){
                if(total_distances[i] < total_distances[closest]){
                    closest = i;
                }
            }
            this->pivot.set_type(types_present[closest].type);

            delete total_distances;
            
        }

        double* calculate_total_distances(std::vector<type_info> types_present){
            int qtd_types = types_present.size();
            double* total_distances = new double [qtd_types];

            for(int i = 0; i < qtd_types; i++){
                total_distances[i] = 0;
            }

            for(int i = 0; i < this->my_list.size(); i++){
                for(int j = 0; j < types_present.size(); j++){
                    if(this->my_list[i].get_type() == types_present[j].type){
                        total_distances[j] += this->pivot.calculate_distance(my_list[i]);
                    }
                }
            }

            return total_distances;
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

std::vector<point> train(std::vector<std::vector<double>> &lines, int porcentage){
    std::vector<point> list_of_point;

    int qtd_train = (lines.size()/100)*porcentage;

    for(int i = 0; i < qtd_train; i++){

        double* att = new double [QTD_ATTRIBUTES];
        for(int j = 0; j < QTD_ATTRIBUTES - 1 ; j++){
            att[j] = lines[0][j];
        }

        point new_point;
        new_point.set_attributes(att);
        int new_point_type = (int)lines[0][QTD_ATTRIBUTES];
        new_point.set_type(new_point_type);

        list_of_point.push_back(new_point);

        lines.erase(lines.begin());
    }
    return list_of_point;
}

void get_best_list(list *my_lists, int qtd_neighbours, int qtd_threads){
    std::vector<point> best_list;
    point bests_points[qtd_threads];

    for(int i = 0; i < qtd_threads; i++){
        bests_points[i] = my_lists[i].best_point();
    }

    for(int i = 0; i < qtd_neighbours; i++){
        int best_point_pos = 0;
        double best_distance = my_lists[0].get_pivo().calculate_distance(bests_points[best_point_pos]);
        for(int j = 1; j < qtd_threads; j++){
            double comparson_distance = my_lists[0].get_pivo().calculate_distance(bests_points[j]);
            if(comparson_distance < best_distance){
                best_point_pos = j;
                best_distance = comparson_distance;
            }
        }

        best_list.push_back(bests_points[best_point_pos]);
        bests_points[best_point_pos] = my_lists[best_point_pos].best_point();
    }
    my_lists[0].set_list(best_list);

}

void test(std::vector<std::vector<double>> &lines, std::vector<point> &list_of_point, int qtd_neighbours, int qtd_threads){
    int diferente = 0, igual = 0;
    while(lines.size() > 0){

        std::vector<double> this_line = lines[0];


        list* lista = new list [qtd_threads];
        point new_point(this_line);
        lines.erase(lines.begin());

        for(int i = 0; i < qtd_threads; i++){
            lista[i].set_qtd_neighbours(qtd_neighbours);
            lista[i].add_pivot(new_point);
        }

        int i;
        #pragma omp parallel for private(i) shared(list_of_point, lista) num_threads(qtd_threads) schedule(static,1)
        for(i = 0; i < list_of_point.size(); i++){
            lista[i%qtd_threads].add(list_of_point[i]);
        }

        get_best_list(lista, qtd_neighbours, qtd_threads);
        lista[0].define_type();
        list_of_point.push_back(lista[0].get_pivo());

        //matriz de confusão
        if(lista[0].get_pivo().get_type() != this_line[QTD_ATTRIBUTES]){
            diferente++;
        }
        else{
            igual++;
        }

        
    }

    std::cout<<"diferente: " << diferente << " igual: " << igual << std::endl;
}

void knn(int qtd_neighbours, int porcentage, int qtd_threads){
    std::vector<std::vector<double>> lines;
    readCsv(lines);
    random_shuffle(lines.begin(), lines.end());

    std::vector<point> list_of_point = train(lines, porcentage);

    test(lines, list_of_point, qtd_neighbours, qtd_threads);

}

int main(){
    int qtd_qtd_neighbours = 8;
    int qtd_neighbours[qtd_qtd_neighbours] = {80, 90, 100, 110, 120, 130, 140, 150};
    int qtd_qtd_threads = 8;
    for(int i=0; i < qtd_qtd_neighbours; i++){
        for(int j = 0; j < qtd_qtd_threads; j++){
            int start = time(NULL);
            knn(qtd_neighbours[i], 70, j);
            std::cout<< time(NULL) - start << std::endl;
        }
    }
    return 0;
}
